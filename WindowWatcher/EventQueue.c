#include "EventQueue.h"
#include <errno.h>
#include <string.h>
mqd_t mq;

void init_queue(const char* name, int create)
{
    int flags = create ? (O_WRONLY | O_CREAT) : O_RDONLY;
    
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = (sizeof(uEvent) < 128) ? 128 : sizeof(uEvent);  // ← Add this
    attr.mq_curmsgs = 0;
    
    mq = mq_open(name, flags, 0644, create ? &attr : NULL);
    if(mq == -1)
        perror("mq_open failed");
}


uEvent receive_event(void)
{
    uEvent event = {0};
    int ret = mq_receive(mq, (char*)&event, sizeof(uEvent), NULL);
    if (ret == -1) {
        if (errno != EAGAIN) {
            fflush(stdout);
        }
        return event;  
    }

    fflush(stdout);
    return event;
}


void send_event(uEvent event)
{
    int ret = mq_send(mq, (const char*)&event, sizeof(uEvent), 0);
    if (ret == -1) {
        perror("[ERROR] mq_send failed");
        return;
    }
    fflush(stdout);
}


void close_queue(const char* name)
{
    mq_close(mq);
    mq_unlink(name);
}