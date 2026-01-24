//test open, read, and close syscalls
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
// Executes open, read, and close syscalls on a test file.
int main()
{
    const char* filename = "testfile.txt";
    char buffer[100];
    ssize_t bytesRead;

    // Open the file for reading
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("Failed to open file");
        return 1;
    }       
    // Read from the file
    bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead == -1)
    {
        perror("Failed to read file");
        close(fd);
        return 1;
    }
    buffer[bytesRead] = '\0'; // Null-terminate the buffer
    printf("Read %zd bytes: %s\n", bytesRead, buffer);
    // Close the file
    if (close(fd) == -1)
    {
        perror("Failed to close file");
        return 1;
    }
    return 0;
}   