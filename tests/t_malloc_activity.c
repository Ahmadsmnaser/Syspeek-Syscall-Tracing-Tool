//test malloc and free syscalls
#include <stdio.h>
#include <stdlib.h>
// Expects to see malloc and free syscalls (mmap , brk) in tracing.
int main()
{
    // Allocate memory using malloc
    size_t size = 1024 * 1024; // 1 MB --> to have a noticeable syscall
    void* ptr = malloc(size);
    if (ptr == NULL)
    {
        perror("malloc failed");
        return 1;
    }
    printf("Allocated %zu bytes of memory at %p\n", size, ptr);

    // Use the allocated memory (optional)
    for (size_t i = 0; i < size; i++)
    {
        ((char*)ptr)[i] = (char)(i % 256);
    }

    // Free the allocated memory
    free(ptr);
    printf("Freed memory at %p\n", ptr);

    return 0;
}