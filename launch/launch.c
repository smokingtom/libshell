/*
 * libshell - A shell library
 * Copyright (C) 2026 smokingtom
 *
 * Author: smokingtom
 * Project: https://github.com/smokingtom/libshell
 *
 * This file is part of libshell.
 *
 * libshell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libshell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libshell. If not, see <https://www.gnu.org/licenses/>.
 */

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
