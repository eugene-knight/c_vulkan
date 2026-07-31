<!-- <!-- PROJECT LOGO OR BANNER -->
<!-- <br /> -->
<!-- <div align="center"> -->
<!--   <a href="https://github.com"> -->
<!--     <img src="https://unsplash.com" alt="Logo" width="80" height="80"> -->
<!--   </a> -->
<!---->
<!--   <h3 align="center">Project Title</h3> -->
<!---->
<!--   <p align="center"> -->
<!--     A short, punchy one-sentence description of what this project does and who it is for. -->
<!--     <br /> -->
<!--     <a href="https://github.com"><strong>Explore the docs »</strong></a> -->
<!--     <br /> -->
<!--     <br /> -->
<!--     <a href="https://github.com/issues">Report Bug</a> -->
<!--     · -->
<!--     <a href="https://github.com/issues">Request Feature</a> -->
<!--   </p> -->
<!-- </div> -->

<!-- BADGES -->
<!-- <div align="center"> -->
<!---->
<!-- [![MIT License](https://shields.io)](https://choosealicense.com) -->
<!-- [![PRs Welcome](https://shields.io)](https://makeapullrequest.com) -->
<!-- [![GitHub Stars](https://shields.io)](https://github.com/stargazers) -->
<!-- </div> -->

<!-- <details> -->
<!--   <summary>📋 Table of Contents</summary> -->
<!--   <ol> -->
<!--     <li><a href="#about-the-project">About The Project</a></li> -->
<!--     <li><a href="#built-with">Built With</a></li> -->
<!--     <li><a href="#getting-started">Getting Started</a></li> -->
<!--     <li><a href="#usage">Usage</a></li> -->
<!--     <li><a href="#roadmap">Roadmap</a></li> -->
<!--     <li><a href="#contributing">Contributing</a></li> -->
<!--     <li><a href="#license">License</a></li> -->
<!--     <li><a href="#contact">Contact</a></li> -->
<!--   </ol> -->
<!-- </details> -->

## About The Project

> "A collection fo data structures for personal use written in C23"

These repository is used a submodules for my personal projects written in the
C programming language (C23).

## Getting Started

Follow these steps to set up the project locally.

### Prerequisites
The following are required:
* ninja
* cmake

### Installation
1. Clone the repository
```sh
git clone https://github.com/eugene-knight/sy_ds.git
```
2.1 Build using `ninja`
```sh
cd build/
```
2.2 Set cmake options to build either `SHARED`, `STATIC` or `TESTS`
```sh
cmake -DSY_BUILD_SHARED_LIB
cmake -DSY_BUILD_STATIC_LIB
cmake -DSY_BUILD_TESTS
```
3.0 Use data structures as wish

## Basic Usage
All data structures are implement with an object-oriented approach (OOP) where you pass
a pointer to data structure into their respective methods. 

All methods will return a `SY_Result` to signal errors.
Methods that provide values back to the user are done via *out parameters*.

For example:
```c
#include "SY_Vector.h"
int main() {
    // A simple result record defined with { u32 domain; u32 code }
    SY_Result result = { .domain = SY_DOMAIN_SUCCESS, .code = 0 };

    SY_Vector_ConstCharPtr vec = {0};

    // Parameters are explicit named to variables for clarity
    size_t capacity = 10;
    // A default allocator pointer that uses `malloc` and `free` for allocation and deallocation respectively
    const SY_Allocator* const SY_DEFAULT_ALLOCATOR_PTR = &SY_DEFAULT_ALLOCATOR;

    result = SY_Vector_ConstCharPtr_Initialize(&vec, capacity, SY_DEFAULT_ALLOCATOR_PTR);
    if (result.domain != SY_DOMAIN_SUCCESS) {
        fprintf(stderr, "ERROR: Failed to initialize `SY_Vector_ConstCharPtr`");
        return EXIT_FAILURE;
    }
    // All methods return `SY_Result` by default unless method name is followed by `Unsafe`
    // which skips safety checks
    SY_Vector_ConstCharPtr_PushbackUnsafe(&vec, "VK_LAYER_KHRONOS_validation");

    // If the data structure is trivial as with the case with dynamic arrays,
    // you can directly access the fields without calling getters like
    // `SY_Vector_ConstCharPtr_GetSize(&vec)`. 
    printf("Names:\n");
    for (size_t i = 0; i < vec.size; i++) {
        printf("\t%s\n", vec.values[i]);
    }
    // Prints:
    /*
        Names:
            VK_LAYER_KHRONOS_validation
    */
    // Remember to match `SY_Allocator` used to allocate the data structure
    // (a simple exception is `SY_Arena` but release method is still called since 
    // `SY_Arena` would be a `noop`.
    SY_Vector_ConstCharPtr_Release(&vec, SY_DEFAULT_ALLOCATOR_PTR);
    return EXIT_SUCCESS; 
}
```
## License

Distributed under the GNU GPL V3. See `LICENSE.txt` for more information.

## Contact

Project Link: [https://github.com/eugene-knight](https://github.com/eugene-knight)

