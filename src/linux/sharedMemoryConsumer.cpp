#include <stdio.h>    // printf.
#include <cstdlib>    // Exit code.
#include <fcntl.h>    // File access modifiers.
#include <sys/mman.h> // Shared memory API.

int
main(int argc, char** argv)
{
    // Create the shared memory object.
    int sharedMemFd = shm_open("TestSharedMemory", O_RDONLY, 0666);

    // Create a memory map of the shared memory object.
    void* sharedMemPtr = mmap(0, 4096, PROT_READ, MAP_SHARED, sharedMemFd, 0);

    // Print contents.
    printf("%s\n", (char*)sharedMemPtr);

    return EXIT_SUCCESS;
}
