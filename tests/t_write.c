// Tests for the write syscall
// Executes a write syscall to output a message to standard output.
#include <stdio.h>
#include <unistd.h>
int main()
{
    const char* msg = "Hello, Syspeek!\n";
    // Write message to standard output (file descriptor 1)
    write(1, msg, 16);
    return 0;
}