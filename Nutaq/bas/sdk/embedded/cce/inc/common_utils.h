#ifndef __COMMON_UTILS__
#define __COMMON_UTILS__

#include "connection_state.h"
#include <setjmp.h>

extern sigjmp_buf sj_env;
void sigbus_hdl (int sig, siginfo_t *siginfo, void *ptr);

Result memory_read_recv(connection_state * state);
Result memory_read_recv_with_id(connection_state * state);
Result custom_register_read_recv(connection_state * state);
Result memory_write_recv(connection_state * state);
Result memory_write_with_mask_recv(connection_state * state);
Result memory_write_recv_with_id(connection_state * state);
Result custom_register_write_recv(connection_state * state);
Result i2c_scan_devices_recv(connection_state * state);

#endif /*__COMMON_UTILS__*/
