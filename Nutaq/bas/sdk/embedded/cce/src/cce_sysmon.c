#include "cce.h"
#include "eapi_serialize.h"
#include "bas_error.h"
#include "bas_debug.h"

#include "cce_sysmon.h"
#include "sysmon.h"

extern Sysmon_handle_t hSysmon;

Result Sysmon_Read_recv(connection_state * state)
{
    Result ret;
    Sysmon_eType_t type;
    float value = 0.0f;
    unsigned int * raw = &value;
    
    debug("lvds_presence_recv\n",0);
    
    type = (Sysmon_eType_t)uint_recv(state);
    
    ret = Sysmon_Read(hSysmon, type, &value);
    
    uint_send(state, *raw);
    
    return ret;
}
