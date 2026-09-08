#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H
    #include <mqueue.h>
    #include "WindowWatcher.h"
    #include <stdio.h>
    extern mqd_t mq;
    void init_queue(const char* name, int create);
    void send_event(uEvent event);
    uEvent receive_event();
    void close_queue();
#endif