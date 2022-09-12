#include "cce.h"
#include "cce_lvds.h"
#include "bas_debug.h"
#include "eapi_serialize.h"

extern lvds lvds_handle[];

Result lvds_presence_recv(connection_state * state)
{
    unsigned int group = 0;
    
    debug("lvds_presence_recv\n",0);
    
    group = uint_recv(state);
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
    
    return lvds_presence(&lvds_handle[group]);
}

Result lvds_getmode_recv(connection_state * state)
{
    unsigned int group = 0;
    
    debug("lvds_getmode_recv\n",0);
    
    group = uint_recv(state);
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
        
	return lvds_getmode(&lvds_handle[group]);
}

Result lvds_setoutputenable_recv(connection_state * state)
{
    unsigned int group = 0;
    unsigned int outputenable = 0;
    
    debug("lvds_setoutputenable_recv\n",0);
    
    group = uint_recv(state);
    outputenable = uint_recv(state);
    
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
        
    return lvds_setoutputenable(&lvds_handle[group], outputenable);
}

Result lvds_setvalue_recv(connection_state * state)
{
    unsigned int group = 0;
    unsigned int value = 0;
    
    debug("lvds_setvalue_recv\n",0);
    
    group = uint_recv(state);
    value = uint_recv(state);
    
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
        
    return lvds_setvalue(&lvds_handle[group], value);
}

Result lvds_getvalue_recv(connection_state * state)
{
    Result ret;
    unsigned int group = 0;
    unsigned int value = 0;
    
    debug("lvds_getvalue_recv\n",0);
    
    group = uint_recv(state);
    if (group >= LVDS_GROUP_MAX)
    {
        uint_send(state, value);
        return LVDSABSENT;
    }
    
    ret = lvds_getvalue(&lvds_handle[group], &value);
    
    uint_send(state, value);
    return ret;
}

Result lvds_reset_recv(connection_state * state)
{
    unsigned int group = 0;
    
    debug("lvds_reset_recv\n",0);
    
    group = uint_recv(state);
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
    
    return lvds_reset(&lvds_handle[group]);
}

Result lvds_reset_fifo_recv(connection_state * state)
{
    unsigned int group = 0;
    
    debug("lvds_reset_fifo_recv\n",0);
    
    group = uint_recv(state);
    if (group >= LVDS_GROUP_MAX)
        return LVDSABSENT;
    
    return lvds_reset_fifo(&lvds_handle[group]);
}
