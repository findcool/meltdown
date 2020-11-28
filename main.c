#include <stdio.h>
#include <stdlib.h>

#define SHELLCODE_SIZE 32

usigned char *shellcode =
    "";

int main(int argc, char *argv[]) 
{
    pid_t target;
    struct user_regs_struct regs;
    int syscall;
    long dst;
  
    if (argc < 2) {
        fprintf(stderr, "Usage: meltdown <pid>\n");
        exit(1);
    }
    target = atoi(argv[1]);
    return 0;
}
