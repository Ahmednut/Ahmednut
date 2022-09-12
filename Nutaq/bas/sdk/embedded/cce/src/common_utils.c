/****************************************************************************  
 *
 *    ****                              I                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       common_utils.c
 * @brief      Utility functions common to all carriers
 *
 * $Date: 2015/11/13 19:22:55 $
 * $Revision: 1.3 $
 *
 * (c) 2013, Nutaq Inc. All rights reserved.              
 *
 * @ingroup CCE
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include "common_utils.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_error.h"
#include "bitstream.h"
#include "bas_debug.h"
#include "xiic.h"
#include "xio.h"

extern Carrier_handle_t hCarrier;

extern volatile unsigned * register_space;
extern volatile unsigned * fmc_space;
extern volatile unsigned * fmc_i2c_space;
extern volatile unsigned * dpram_space;

extern uint32_t register_begin,    register_size,  register_end;
extern uint32_t fmc_begin,         fmc_size,       fmc_end;
extern uint32_t fmc_i2c_begin,     fmc_i2c_size,   fmc_i2c_end;
extern uint32_t dpram_begin,       dpram_size,     dpram_end;

#ifdef DEBUG
// Uncomment this line to add some trace debug output strings
#define COMMON_UTILS_MODDEBUG 1
#endif

// Debug related definitions
#ifdef COMMON_UTILS_MODDEBUG
    // These macros are used to get printout info while debugging
    #define PDEBUG(f, ...) printf(f, __VA_ARGS__)

#else
    #define PDEBUG(f, ...)

#endif

/* Environment and blocked signal state for restoration in a signal handler. */
sigjmp_buf sj_env;

void sigbus_hdl (int sig, siginfo_t *siginfo, void *ptr)
{
	/* Jump (goto) to the saved program state where we don't use mmapped()
	 * memory. */
	siglongjmp (sj_env, 1);
}

Result memory_read_recv(connection_state * state)
{
    Result ret = OK;
    unsigned addr, data;
    int i;    
    debug("memory read\r\n", addr);
    i = recv(state->stream, &addr, sizeof(addr), 0);
    addr = ntohl(addr);
    if(i!=sizeof(addr)) state->has_ended=1;
    debug("memory addr %x\r\n", addr);
    
    /* Remember this point as a state that we can return to using
	 * siglongjmp(). If this function returns a value other than 0
	 * it means that we are here because of siglongjmp().
	 */
	if (sigsetjmp(sj_env, 1)) {
        uint_send(state, 0);
        return CCEINVALIDREG;
    }
    
    if (addr & 0x3) //check for unaligned access
    {
        data = 0;
        ret = CCEINVALIDVALUE;
    }else if (addr >= register_begin && addr <= register_end) {
        addr %= register_size;
        data = register_space[addr>>2];
        ret = OK;
    }else if ( addr >= fmc_begin && addr <= fmc_end) {
        addr %= fmc_size;
        data = fmc_space[addr>>2];
        ret = OK;
    }else if ( addr >= dpram_begin && addr <= dpram_end) {
    	 debug("dpram addr %x\r\n", addr);
        addr %= dpram_size;
        data = dpram_space[addr>>2];
    	 debug("dpram data %x\r\n", data );
        ret = OK;
    }else{
#ifdef DEBUG
    	data = *((volatile unsigned int*)(addr));
#else
        data = 0;
        ret = CCEINVALIDVALUE;
#endif        
    }
    debug("data %x\r\n", data);
    uint_send(state, data);
    return ret;
}

Result memory_read_recv_with_id(connection_state * state)
{
    Result ret=OK;
    unsigned id = 0;
    unsigned long long addr, data=0;
    debug("memory read with id\r\n", id);
    id = (uint_recv(state));
    debug("fpga id %x\r\n", id);
    addr = uint64_recv(state);
    debug("memory addr %x\r\n", addr);
    
    /* Remember this point as a state that we can return to using
	 * siglongjmp(). If this function returns a value other than 0
	 * it means that we are here because of siglongjmp().
	 */
	if (sigsetjmp(sj_env, 1)) {
        uint_send(state, 0);
        return CCEINVALIDREG;
    }
	
    if(id == 0)
    {
        if (addr & 0x3) //check for unaligned access
        {
            data = 0;
            ret = CCEINVALIDVALUE;
        }
        else if 
            (addr >= register_begin && addr <= register_end) {
            addr %= register_size;
            data = register_space[addr>>2];
            ret = OK;
        }
        else if 
            ( addr >= fmc_begin && addr <= fmc_end) {
            addr %= fmc_size;
            data = fmc_space[addr>>2];
            ret = OK;
        }       
        else if 
        (
	     addr >= dpram_begin && addr <= dpram_end) {
    	     debug("dpram addr %x\r\n", addr);
            addr %= dpram_size;
            data = dpram_space[addr>>2];
            debug("dpram data %x\r\n", data );
            ret = OK;
        }
        else
        {
            data = 0;
            ret = CCEINVALIDVALUE;
        }
    }
    else
    {
	
    }
    debug("data %x\r\n", data);
    uint_send(state, data);
    return ret;
}

Result custom_register_read_recv(connection_state * state)
{
	Result ret=OK;
	unsigned id = 0, data = 0;
	
	debug("memory read with id\r\n", id);
	id = (uint_recv(state));

    ret = Carrier_CustomRegisterRead(hCarrier, id, &data);
	
	debug("data %x\r\n", data);
	uint_send(state, data);
	return ret;	
}

Result memory_write_recv(connection_state * state)
{
    Result ret = OK;
    unsigned addr, data;
    int i;
    i = recv(state->stream, &addr, sizeof(addr), 0);
    if(i!=sizeof(addr))
    {
        state->has_ended=1;
        return CCETCPERROR;
    }
    i = recv(state->stream, &data, sizeof(data), 0);
    if(i!=sizeof(data))
    {
        state->has_ended=1;
        return CCETCPERROR;
    }
    addr = ntohl(addr);
    // fprintf(stderr, "memory addr %x", addr);
    data = ntohl(data);
    // fprintf(stderr, "\tdata: %x\n", data);
    
    /* Remember this point as a state that we can return to using
	 * siglongjmp(). If this function returns a value other than 0
	 * it means that we are here because of siglongjmp().
	 */
	if (sigsetjmp(sj_env, 1)) {
        return CCEINVALIDREG;
    }
    
    if (addr & 0x3) //check for unaligned access
    {
        ret = CCEINVALIDVALUE;
    }else if (addr >= register_begin && addr <= register_end) {
        addr %= register_size;
        register_space[addr>>2] = data;
    }else if ( addr >= fmc_begin && addr <= fmc_end) {
        addr %= fmc_size;
        fmc_space[addr>>2] = data;
    }else if ( addr >= dpram_begin && addr <= dpram_end) {
        addr %= dpram_size;
        dpram_space[addr>>2] = data;
    }else{
        ret = CCEINVALIDVALUE;
    }
    return ret;
}

Result memory_write_with_mask_recv(connection_state * state)
{
    Result ret = OK;
    unsigned addr, data, mask;
    unsigned tmp_data;
    int i;
    i = recv(state->stream, &addr, sizeof(addr), 0);
    if(i!=sizeof(addr))
    {
        state->has_ended=1;
        return CCETCPERROR;
    }
    i = recv(state->stream, &data, sizeof(data), 0);
    if(i!=sizeof(data))
    {
        state->has_ended=1;
        return CCETCPERROR;
    }
    i = recv(state->stream, &mask, sizeof(mask), 0);
    if(i!=sizeof(mask))
    {
        state->has_ended=1;
        return CCETCPERROR;
    }
    addr = ntohl(addr);
    // fprintf(stderr, "memory addr %x", addr);
    data = ntohl(data);
    // fprintf(stderr, "\tdata: %x\n", data);
    mask = ntohl(mask);
    
    /* Remember this point as a state that we can return to using
	 * siglongjmp(). If this function returns a value other than 0
	 * it means that we are here because of siglongjmp().
	 */
	if (sigsetjmp(sj_env, 1)) {
        return CCEINVALIDREG;
    }
    
    if (addr & 0x3) //check for unaligned access
    {
        ret = CCEINVALIDVALUE;
    }else if (addr >= register_begin && addr <= register_end) {
        addr %= register_size;
        tmp_data = register_space[addr>>2];
        tmp_data &= ~(mask);
        tmp_data |= (data & mask);
        register_space[addr>>2] = tmp_data;
    }else if ( addr >= fmc_begin && addr <= fmc_end) {
        addr %= fmc_size;
        tmp_data = fmc_space[addr>>2];
        tmp_data &= ~(mask);
        tmp_data |= (data & mask);
        fmc_space[addr>>2] = tmp_data;
    }else if ( addr >= dpram_begin && addr <= dpram_end) {
        addr %= dpram_size;
        tmp_data = dpram_space[addr>>2];
        tmp_data &= ~(mask);
        tmp_data |= (data & mask);
        dpram_space[addr>>2] = tmp_data;
    }else{
        ret = CCEINVALIDVALUE;
    }
    return ret;
}

Result memory_write_recv_with_id(connection_state * state)
{
    Result ret = OK;
    unsigned id;
    unsigned long long addr, data;
	
    id = (uint_recv(state));
    // fprintf(stderr, "fpga id %x", id);
    addr = uint64_recv(state);
    // fprintf(stderr, "memory addr %x", addr);
    data = uint_recv(state);
    // fprintf(stderr, "\tdata: %x\n", data);
    
    /* Remember this point as a state that we can return to using
	 * siglongjmp(). If this function returns a value other than 0
	 * it means that we are here because of siglongjmp().
	 */
	if (sigsetjmp(sj_env, 1)) {
        return CCEINVALIDREG;
    }
    
    if(id == 0)
    {
        if (addr & 0x3) //check for unaligned access
        {
            ret = CCEINVALIDVALUE;
        }
        else if (addr >= register_begin && addr <= register_end) {
            addr %= register_size;
            register_space[addr>>2] = data;
        }
        else if ( addr >= fmc_begin && addr <= fmc_end) {
            addr %= fmc_size;
            fmc_space[addr>>2] = data;
        }         
        else if (addr >= dpram_begin && addr <= dpram_end) {
            addr %= dpram_size;
            dpram_space[addr>>2] = data;
        }
        else
        {
            ret = CCEINVALIDVALUE;
        }
    }
    else
    {
        //
    }
    return ret;
}

Result custom_register_write_recv(connection_state * state)
{
	Result ret=OK;
	unsigned id = 0, data = 0;
	
	id = uint_recv(state);
	data = uint_recv(state);
	
	debug("id %u\r\n", id);
	debug("data %x\r\n", data);
	
    ret = Carrier_CustomRegisterWrite(hCarrier, id, data);
	
	return ret;	
}


Result i2c_scan_devices_recv(connection_state * state)
{
    Result ret = OK; 
    I2C_stI2cScan scanraw;
    unsigned char ndevices=0;
    unsigned int I2cZone;

    memset(&scanraw,0,sizeof(I2C_stI2cScan));
    
    // Get current bus number to be scanned (dummy for the moment)
    I2cZone = uint_recv(state);

	PDEBUG("i2c_scan_devices\n",0); 
    
    // Scan the i2c bus
    ret = Carrier_I2cScanDevices(hCarrier, I2cZone, &scanraw, &ndevices);
    
	PDEBUG("i2c_scan_devices ndevices: %d\n",ndevices); 
    uchar_send(state, ndevices);
    buf_send(state, (char*)&scanraw, sizeof(I2C_stI2cScan));
    return ret;
}


