
#include <string.h>
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_error.h"
#include "bas_debug.h"

#include "cce_io.h"
#include "connection_state.h"

Result flush_data(int stream, unsigned size)
{
    int transferred=0;
    int i, getsize, remaining;
    char databuf[4096];
    while(transferred<size)
    {
        remaining = size-transferred;
        debug("remaining %i bytes\t", remaining);
        getsize = remaining > 4096 ? 4096 : remaining ;
        debug("flushing %i bytes\t", getsize);
        i = recv(stream, databuf, getsize, 0);
        debug("flushed %i bytes\n", i);
        if (i<=0) return CCETCPERROR;
        transferred+=i;
    }
    return OK;
}

Result buf_send(connection_state * state, const char * buf, unsigned size)
{
    debug("buf_send: %p  %i\n", buf, size);
    uint_send(state, size);
    return connection_write(state, buf, size);
}

Result buf_recv(connection_state * state, char * buf, unsigned maxsize)
{
    Result ret;
    int i;
    unsigned size;
    int transferred=0;
    int remaining;

    size = uint_recv(state);
    if(size>maxsize)
    {
        ret = flush_data(state->stream, size);
        if(FAILURE(ret)) return ret;
        return CCEBUFFER2BIG;
    }

    while(transferred<size)
    {
        remaining = size-transferred;
        i = recv(state->stream, buf, remaining, 0);
        if(i<=0) return CCETCPERROR;
        transferred += i;
    }
    return OK;
}

Result string_send(connection_state * state, const char * str)
{
    int size;
    size = strlen(str);
    size++; //include NULL (\0) character
    return buf_send(state, str, size);
}
