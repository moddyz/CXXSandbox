#include <stdio.h>
#include <cstdlib> // Exit code.
#include <fcntl.h> // File access modifiers.
#include <sys/mman.h>
#include <sys/stat.h>

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("usage: memoryMapRead <FILE>\n");
        return EXIT_FAILURE;
    }

    // Open input file.
    int inFd = open(argv[1], O_RDONLY);
    if (inFd < 0) {
        fprintf(stderr, "Failed to open file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Get file size.
    struct stat inStat;
    if (fstat(inFd, &inStat) < 0) {
        fprintf(stderr, "Failed to stat file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Memory map the input file.
    void* filePtr =
        mmap(nullptr, inStat.st_size, PROT_READ, MAP_SHARED, inFd, 0);
    if (filePtr == MAP_FAILED) {
        fprintf(stderr, "Failed to mmap file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Print the entire contents of the file.
    printf("%s\n", (char*)filePtr);

    // Deletes virtual address mappings for address range.
    if (munmap(filePtr, inStat.st_size) < 0) {
        fprintf(stderr, "Failed to munmap address: %p\n", filePtr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
