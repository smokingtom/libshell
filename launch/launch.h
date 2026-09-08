#ifndef LAUNCH_H
#define LAUNCH_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

pid_t launch(const char* cmd, ...);

#ifdef __cplusplus
}
#endif

#endif
