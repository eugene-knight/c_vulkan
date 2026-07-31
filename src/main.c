#include "SY_Allocator.h"
#include "SY_Result.h"
#include "SY_Vector.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

typedef struct SY_Window
{
        u32 height;
        u32 width;
} SY_Window;
const SY_Window SY_WINDOW = {
        .height = 800,
        .width = 600,
};

typedef struct SY_Vulkan_Functions
{
        PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
        PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
} SY_Vulkan_Functions;
typedef struct SY_Vulkan_Context
{
        VkInstance instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkPhysicalDevice physicalDevice;
} SY_Vulkan_Context;

const char* SY_VALIDATION_LAYERS[] = { "VK_LAYER_KHRONOS_validation" };
const u32 SY_VALIDATION_LAYERS_COUNT = SY_ARRAY_SIZE(SY_VALIDATION_LAYERS);
#ifdef NDEBUG
const bool SY_ENABLE_VALIDATION_LAYERS = false;
#else
const bool SY_ENABLE_VALIDATION_LAYERS = true;
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL
SY_Vulkan_DebugCallback(
  VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
  VkDebugUtilsMessageTypeFlagsEXT messageType,
  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
  void* pUserData)
{
        fprintf(stderr, "Validation Layers:%s\n", pCallbackData->pMessage);
        return VK_FALSE;
}

void
SY_Vulkan_PopulateDebugUtilsMessenger(
  VkDebugUtilsMessengerCreateInfoEXT* createInfo)
{
        createInfo->sType =
          VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo->messageSeverity =
          // VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo->messageType =
          VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
          VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo->pfnUserCallback = SY_Vulkan_DebugCallback;
        createInfo->pNext = nullptr;
}

bool
SY_Vulkan_CheckValidationLayerSupport()
{
        u32 layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        VkLayerProperties* layerProperties =
          SY_TALLOCATE_(SY_DEFAULT_ALLOCATOR_PTR, VkLayerProperties, layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, &layerProperties[0]);

        for (u32 i = 0; i < SY_VALIDATION_LAYERS_COUNT; i++) {
                bool layerFound = false;
                for (u32 j = 0; j < layerCount; j++) {
                        const char* lhs = SY_VALIDATION_LAYERS[i];
                        const char* rhs = layerProperties[j].layerName;
                        if (strcmp(lhs, rhs) == 0) {
                                layerFound = true;
                                break;
                        }
                }
                if (!layerFound) {
                        SY_DEALLOCATE_(SY_DEFAULT_ALLOCATOR_PTR,
                                      &layerProperties[0]);
                        return false;
                }
        }
        SY_DEALLOCATE_(SY_DEFAULT_ALLOCATOR_PTR, &layerProperties[0]);
        return true;
}
SY_Result
SY_Vulkan_InstanceInitialize(SY_Vulkan_Context* pContext)
{
        SY_Result result = { .domain = SY_DOMAIN_SUCCESS, .code = 0 };

        VkApplicationInfo applicationInfo = {
                .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                .pApplicationName = "Hello Triangle",
                .applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                .pEngineName = "No Engine",
                .engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0),
                .apiVersion = VK_API_VERSION_1_4,
        };

        VkInstanceCreateInfo createInfo = { 0 };
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &applicationInfo;

        u32 sdlExtensionCount = 0;
        const char* const* sdlExtensionNames =
          SDL_Vulkan_GetInstanceExtensions(&sdlExtensionCount);

        SY_Vector_ConstCharPtr extensionNames = { 0 };
        u32 extensionCount = sdlExtensionCount;
        if (SY_ENABLE_VALIDATION_LAYERS) {
                extensionCount += 1;
                SY_IGNORE SY_Vector_ConstCharPtr_Initialize(
                  &extensionNames, extensionCount, SY_DEFAULT_ALLOCATOR_PTR);
                for (u32 i = 0; i < sdlExtensionCount; i++) {
                        SY_Vector_ConstCharPtr_PushbackUnsafe(
                          &extensionNames, sdlExtensionNames[i]);
                }
                SY_Vector_ConstCharPtr_PushbackUnsafe(
                  &extensionNames, VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
                createInfo.enabledExtensionCount = extensionNames.size;
        } else {
                SY_IGNORE SY_Vector_ConstCharPtr_Initialize(
                  &extensionNames, extensionCount, SY_DEFAULT_ALLOCATOR_PTR);
                for (u32 i = 0; i < sdlExtensionCount; i++) {
                        SY_Vector_ConstCharPtr_PushbackUnsafe(
                          &extensionNames, sdlExtensionNames[i]);
                }
                createInfo.enabledExtensionCount = extensionNames.size;
        }
        createInfo.ppEnabledExtensionNames = &extensionNames.values[0];

        if (SY_ENABLE_VALIDATION_LAYERS) {
                createInfo.enabledLayerCount = SY_VALIDATION_LAYERS_COUNT;
                createInfo.ppEnabledLayerNames = &SY_VALIDATION_LAYERS[0];

                VkDebugUtilsMessengerCreateInfoEXT debugInfo;
                SY_Vulkan_PopulateDebugUtilsMessenger(&debugInfo);
                createInfo.pNext = &debugInfo;
        } else {
                createInfo.enabledLayerCount = 0;
                createInfo.ppEnabledLayerNames = nullptr;
                createInfo.pNext = nullptr;
        }

        VkResult vkResult =
          vkCreateInstance(&createInfo, nullptr, &pContext->instance);
        if (vkResult != VK_SUCCESS) {
                result.domain = SY_DOMAIN_VULKAN;
                result.code = vkResult;
        }

        SY_Vector_ConstCharPtr_Release(&extensionNames, SY_DEFAULT_ALLOCATOR_PTR);
        return result;
}

void
SY_Vulkan_InstanceRelease(SY_Vulkan_Context* pContext)
{
        vkDestroyInstance(pContext->instance, nullptr);
}

SY_Result
SY_Vulkan_DebugUtilsMessengerEXTInitialize(SY_Vulkan_Context* pContext,
                                           SY_Vulkan_Functions* pFunctions)
{
        SY_Result errorCode = { .domain = SY_DOMAIN_SUCCESS, .code = 0 };
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        SY_Vulkan_PopulateDebugUtilsMessenger(&createInfo);
        VkResult result = pFunctions->vkCreateDebugUtilsMessengerEXT(
          pContext->instance, &createInfo, nullptr, &pContext->debugMessenger);
        if (result != VK_SUCCESS) {
                errorCode.domain = SY_DOMAIN_VULKAN;
                errorCode.code = result;
        }
        return errorCode;
}

void
SY_Vulkan_DebugUtilsMessengerEXTRelease(SY_Vulkan_Context* pContext,
                                        SY_Vulkan_Functions* pFunctions)
{
        pFunctions->vkDestroyDebugUtilsMessengerEXT(
          pContext->instance, pContext->debugMessenger, nullptr);
}

bool
SY_Vulkan_FunctionPointersInitialize(SY_Vulkan_Context* pContext,
                                     SY_Vulkan_Functions* pFunctions)
{
        pFunctions->vkCreateDebugUtilsMessengerEXT =
          (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            pContext->instance, "vkCreateDebugUtilsMessengerEXT");
        if (!pFunctions->vkCreateDebugUtilsMessengerEXT) {
                return false;
        }
        pFunctions->vkDestroyDebugUtilsMessengerEXT =
          (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            pContext->instance, "vkDestroyDebugUtilsMessengerEXT");
        if (!pFunctions->vkDestroyDebugUtilsMessengerEXT) {
                return false;
        }
        return true;
}

bool SY_Vulkan_IsDeviceSuitable(VkPhysicalDevice physicalDevice) {
        return true;
}

SY_Result SY_Vulkan_PickPhysicalDevice(SY_Vulkan_Context *pContext)
{
        SY_Result result = { .domain = SY_DOMAIN_SUCCESS, .code = 0 }; 
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(pContext->instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
                fprintf(stderr, "ERROR: Failed to find GPUs with vulkan support!");
        }
        VkPhysicalDevice* physicalDevices = SY_TALLOCATE_(
                SY_DEFAULT_ALLOCATOR_PTR, VkPhysicalDevice, deviceCount
        );
        vkEnumeratePhysicalDevices(pContext->instance, &deviceCount, &physicalDevices[0]);
        
        for (u32 i = 0; i < deviceCount; i++) {
                if(SY_Vulkan_IsDeviceSuitable(physicalDevices[i])) {
                        pContext->physicalDevice = physicalDevices[i];
                        break;
                }
        }

        if (pContext->physicalDevice == nullptr) {
                fprintf(stderr, "ERROR: Failed to find suitable GPU!");
                result.domain = SY_DOMAIN_VULKAN;
                result.code = SY_PLACEHOLDER_RESULT;
                return result;
        }

        return result;
}

int
main()
{
        SY_Vulkan_Context context = { 0 };
        SY_Vulkan_Functions functions = { 0 };
        SDL_Init(SDL_INIT_VIDEO);

        SDL_Window* pWindow = SDL_CreateWindow(
          "Window", SY_WINDOW.height, SY_WINDOW.width, SDL_WINDOW_VULKAN);

        SY_Vulkan_InstanceInitialize(&context);
        SY_Vulkan_FunctionPointersInitialize(&context, &functions);
        SY_Vulkan_DebugUtilsMessengerEXTInitialize(&context, &functions);

        bool isRunning = true;
        while (isRunning) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                                case SDL_EVENT_QUIT:
                                        isRunning = false;
                                        break;
                                case SDL_EVENT_KEY_DOWN:
                                        if (event.key.key == SDLK_ESCAPE) {
                                                isRunning = false;
                                        }
                                        break;
                        }
                }
        }

        if (SY_ENABLE_VALIDATION_LAYERS) {
                SY_Vulkan_DebugUtilsMessengerEXTRelease(&context, &functions);
        }
        SY_Vulkan_InstanceRelease(&context);

        SDL_DestroyWindow(pWindow);
        SDL_Quit();

        return EXIT_SUCCESS;
}
