//test execve syscall
#include <stdio.h>
#include <unistd.h>
// Expect to see execve syscall in tracing.
int main()
{
    char *argv[] = {"/bin/ls", "-l", NULL};
    char *envp[] = {NULL};

    execve("/bin/ls", argv, envp);
    perror("execve failed");
    return 1;
}