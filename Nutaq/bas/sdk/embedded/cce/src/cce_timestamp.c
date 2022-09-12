#include "cce.h"
#include "cce_timestamp.h"
#include "timestamp.h"
#include "eapi_serialize.h"
#include "bas_types.h"

extern Timestamp_Handle_t hTimestamp;

Result Timestamp_Presence_recv(connection_state * state)
{
    return Timestamp_Presence(hTimestamp);
}

Result Timestamp_SetTime_recv(connection_state * state)
{
    uint64_t time;
    
    // Receive the timestamp value
    time = uint64_recv(state);
    
    // Set timestamp
    return Timestamp_SetTime(hTimestamp, time);
}

Result Timestamp_GetTime_recv(connection_state * state)
{
    Result ret;
    uint64_t time = 0;
    
    // Get timestamp
    ret = Timestamp_GetTime(hTimestamp, &time);
    
    // Send the timestamp value
    uint64_send(state, time);
    
    return ret;
}

Result Timestamp_Reset_recv(connection_state * state)
{    
    return Timestamp_Reset(hTimestamp);
}
