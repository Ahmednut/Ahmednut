#ifndef __CCE_OSCONFIG__
#define __CCE_OSCONFIG__

#include "bas_error.h"
#include "connection_state.h"

Result os_config_get_parameter(connection_state * state);
Result os_config_get_first_parameter(connection_state * state);
Result os_config_get_next_parameter(connection_state * state);
Result os_config_set_parameter(connection_state * state);
Result os_config_unset_parameter(connection_state * state);
Result os_config_clear_file(connection_state * state);
Result os_config_load_file(connection_state * state);
Result os_config_save_file(connection_state * state);
Result os_config_apply_file(connection_state * state);
Result os_config_select_file(connection_state * state);
Result os_config_get_first_file(connection_state * state);
Result os_config_get_next_file(connection_state * state);

enum osconfig_error{
    OSCONFIG_BADPARAMETER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 0x10),
    OSCONFIG_BADVALUE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 0x20),
    OSCONFIG_BADFILE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 0x30),
    OSCONFIG_NOTLOADED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 0x31),
};

#endif
