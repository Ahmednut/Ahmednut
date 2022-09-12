#ifndef __HARDWARE__
#define __HARDWARE__

#include "connection_state.h"

Result kermode_presence(connection_state * state);
Result perseus_presence(connection_state * state);
Result zedboard_presence(connection_state * state);
Result reboot_recv(connection_state * state);


#endif
