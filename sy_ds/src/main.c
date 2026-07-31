#include "SY_Allocator.h"
#include "SY_Result.h"

#include <stdio.h>
#include <stdlib.h>

int
main ()
{
        FILE *pFile = fopen ("assets/main.sy", "r+");
        if (pFile == nullptr)
                {
                        fprintf (stderr, "ERROR: Failed to open file!\n");
                        return EXIT_FAILURE;
                }

        fseek (pFile, 0, SEEK_END);
        long fileSize = ftell (pFile);
        fseek (pFile, 0, SEEK_SET);

        char *buffer = (char *)malloc (fileSize + 1);
        if (buffer == nullptr)
                {
                        fclose (pFile);
                        return EXIT_FAILURE;
                }

        size_t bytesRead = fread (buffer, 1, fileSize, pFile);
        if (bytesRead != fileSize)
                {
                        free (buffer);
                        fclose (pFile);
                        return EXIT_FAILURE;
                }

        buffer[fileSize] = '\0';
        printf ("File Contents:\n%s\n", buffer);

        free (buffer);
        fclose (pFile);
        return EXIT_SUCCESS;
}
