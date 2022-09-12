#ifndef __CCE_IO__
#define __CCE_IO__

#include "connection_state.h"

Result flush_data(int stream, unsigned size);
Result buf_send(connection_state * state, const char * buf, unsigned size);
Result buf_recv(connection_state * state, char * buf, unsigned maxsize);
Result string_send(connection_state * state, const char * str);

#endif

