#include <stdio.h>    // sprintf.
#include <cstdlib>    // Exit codes.
#include <fcntl.h>    // Function codes.
#include <unistd.h>   // ftruncate and sleep.
#include <sys/mman.h> // Shared memory API.

int main(int argc, char** argv)
{
    // Create the shared memory object.
    int sharedMemFd = shm_open("TestSharedMemory", O_CREAT | O_RDWR, 0666);

    // Re-size to 4096 bytes.
    ftruncate(sharedMemFd, 4096);

    // Create a memory map of the shared memory object.
    void* sharedMemPtr = mmap(0, 4096, PROT_WRITE, MAP_SHARED, sharedMemFd, 0);

    // Write to shared memory block.
    sprintf((char*)sharedMemPtr, "%s", "Hello world!");

    // Wait for 10 seconds.
    sleep(10);

    // Unlink shared memory.
    shm_unlink("TestSharedMemory");

    return EXIT_SUCCESS;
}
