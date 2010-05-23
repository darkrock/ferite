#ifndef APHEX_AS_COMPONENT
#include "../../../config.h"
#endif 

#include <errno.h>

#include "poll.h"
#include "aphex.h"

int aphex_notify_can_read_with_timeout(int fd, int wait, int ce)
{
#ifndef WIN32
    struct pollfd pfd;
    int i;
    pfd.fd = fd;
    pfd.events = POLLIN;
    pfd.revents = 0;
    i = poll(&pfd,1,wait);
    if(ce)
    {
        while((i = poll(&pfd,1,wait)) == -1)
        {
            if(errno != EAGAIN && errno != EINTR)
            {
                return i;
            }
        }
        return i;
    }
#endif
	return 0;
}

int aphex_notify_can_read(int fd, int ce)
{
    return aphex_notify_can_read_with_timeout(fd,DEFAULT_TIMEOUT,ce);
}
