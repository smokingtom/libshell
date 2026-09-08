#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include "launch.h"

static void sigchld(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

pid_t launch(const char* cmd, ...) {
    static volatile sig_atomic_t init = 0;
    
    if (!init) {
        signal(SIGCHLD, sigchld);
        init = 1;
    }
    
    va_list args;
    va_start(args, cmd);
    int argc = 1; 
    while (va_arg(args, const char *) != NULL) {
        argc++;
    }
    va_end(args);
        char **argv = (char **)malloc((argc + 1) * sizeof(char *));
    if (!argv) return -1;
    
    argv[0] = (char *)cmd;
    va_start(args, cmd);
    for (int i = 1; i < argc; i++) {
        argv[i] = (char *)va_arg(args, const char *);
    }
    va_end(args);
    argv[argc] = NULL;
    
    pid_t pid = fork();
    if (pid < 0) {
        free(argv);
        return -1;
    }
    
    if (pid == 0) {
        execvp(cmd, argv);
        _exit(127);
    }
    free(argv);
    return pid;
}
