//test for the open syscall failing to open a non-existent file
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
// Expect the open syscall to fail and print the errno value.
int main()
{
    const char* filename = "non_existent_file.txt";
    // Attempt to open a non-existent file
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        // Print the error number
        printf("Failed to open file '%s': errno = %d\n", filename, errno);
        return errno;
    }
    else
    {
        // Close the file if opened successfully (unexpected)
        close(fd);
        printf("File '%s' opened successfully (unexpected)\n", filename);
        return 0;
    }
}   