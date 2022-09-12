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
 * @file       perseus_utils.c
 * @brief      Utility functions for the Perseus carrier
 *
 * $Date: 2015/05/19 14:40:21 $
 * $Revision: 1.2 $
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
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>
#include "cce.h"
#include "utils.h"
#include "bas_error.h"
#include "bitstream.h"
#include "eapi_serialize.h"
#include "cce_io.h"
#include "bas_debug.h"
#include "cce_osconfig.h"
#include "cce_fmcvita.h"
#include "xiic.h"
#include "xio.h"


#define BITSTREAM1_ADDRESS (0x0)
#define BITSTREAM2_ADDRESS (0x15FFFFF)
#define BITSTREAM1_PARTITION ("\"fpga1\"")
#define BITSTREAM2_PARTITION ("\"fpga2\"")
#define UBOOT_PARTITION ("\"boot\"")
#define KERNEL_PARTITION ("\"image\"")
#define DTB_PARTITION ("\"dtb\"")
#define JFFS2_PARTITION ("\"jffs2\"")

#define MEMLOC_ADDR		0xB24  //for DTB file
#define DTB_CODE		0x8087 //code to change MAC Address with parameter

#ifdef __linux__
#define sscanf_s sscanf
#endif

extern char cce_mac[20];

Result fpgaflash_recv(connection_state * state)
{
    Result ret = OK;
    int i = 0;
    unsigned bitstream_size, index, comment_length;
    int transferred;
    unsigned char * new_bitstream;
    char * comment;
    flash_prog_t st;
    unsigned char * data = NULL;
    unsigned int jump_address;
    char * custom;
    unsigned int custom_length;
    
    //receive the index of the bitstream
    index = uint_recv(state);
    debug("index = %i \n", index);
    
    //receive size of bitstream
    bitstream_size = uint_recv(state);
    debug("bitstream_size = %i \n", bitstream_size);
    
    new_bitstream = malloc(bitstream_size);
    if(!new_bitstream) return CCEMEMORY;
    
    transferred = 0;
    while (transferred<bitstream_size)
    {
        i = recv(state->stream, &new_bitstream[transferred], bitstream_size-transferred, 0);
        if(i<0) return CCETCPERROR;
        transferred+=i;
    }
    
    //receive length of comment
    comment_length = uint_recv(state);
    debug("comment_length = %i \n", comment_length);
    
    comment = malloc(comment_length);
    if(!comment) return CCEMEMORY;
    
    transferred = 0;
    while (transferred<comment_length)
    {
        i = recv(state->stream, &comment[transferred], comment_length-transferred, 0);
        if(i<0) return CCETCPERROR;
        transferred+=i;
    }
    
    debug("comment received : %s\n", comment);
    
    // Verify the validity of the bitstream by trying to parse it
    // Verify if there is already a comment in the custom section of the header
    ret = bitstream_parse(new_bitstream, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &custom, &custom_length, NULL);
    if(FAILURE(ret))
    {
        printf("Failed to parse the new bitstream : 0x%x\n", ret);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
    if(custom_length == 1)
    {
        if(custom[0] == '\0')
        {
            debug("bitstream_set_custom\n", 0);
            ret = bitstream_set_custom(&new_bitstream, &bitstream_size, comment, comment_length);
        }
    }
    
    // If flashing the first bitstream
    // look at the current index configuration and apply it the the current bitstream to not change the
    // boot-up behaviour
    if(index==1)
    {
        ret = flash_read_last_sector(BITSTREAM1_PARTITION, &data);
        if(FAILURE(ret))
        {
            printf("Failed to read last partition : 0x%x\n", ret);
            ret = CCEINVALIDPARTITIONTABLE;
            goto end;
        }
        
        ret = bitstream_parse(data, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &jump_address);
        // if failed, the new bitstream will be unchanged since the current bitstream in the flash is not valid 
        if(SUCCESS(ret))
        {
            ret = bitstream_set_jump_address(new_bitstream, jump_address);
            debug("JUMP ADDRESS : 0x%X\n", jump_address);
            debug("New bitstream\n", 0);
        }
    }
    
    if(index==1)
        ret = flash_prog_init(&st, bitstream_size, BITSTREAM1_PARTITION);
    else if(index==2)
        ret = flash_prog_init(&st, bitstream_size, BITSTREAM2_PARTITION);
    else
        ret = CCEINVALIDINDEX;
    if(FAILURE(ret)) goto end;
    
    //We have a complete bitstream
    printf("Programming bitstream %d in the flash... \n", index);
    fflush(stdout);
    ret = flash_prog_putdata(&st, (char *)new_bitstream);
    if(FAILURE(ret)) goto end;
    
    printf("DONE\n");
    flash_prog_end(&st);

end:
    if(data) free(data);
    if(new_bitstream) free(new_bitstream);
    
    if (FAILURE(i)) state->has_ended=1; //connection has reset ?
    return ret;
}

Result updateflash_recv(connection_state * state)
{
    Result ret = OK;
    int i = 0;
    unsigned file_size, partition;
    int transferred;
    unsigned char * new_file;
    flash_prog_t st;
	
	//receive partition number
	partition = uint_recv(state);
	debug("partition = %i \n", partition);
        
    //receive size of file
    file_size = uint_recv(state);
    debug("file_size = %i \n", file_size);
    
    new_file = malloc(file_size);
    if(!new_file) return CCEMEMORY;
    
    transferred = 0;
    while (transferred<file_size)
    {
        i = recv(state->stream, &new_file[transferred], file_size-transferred, 0);
        if(i<0) return CCETCPERROR;
        transferred+=i;
    }	
	//erasing flash memory depending of partition
	switch(partition)
	{
		case 0: //Kernel
		{
			ret = flash_prog_init(&st, file_size, KERNEL_PARTITION);
			if(FAILURE(ret)) goto end;			
			printf("Programming kernel in the flash... \n");
			break;			
		}
		case 1: //CCE
		{
			printf("Programming CCE in the flash... \n");
			ret = change_cce((char *)new_file, file_size);
			if(FAILURE(ret)) goto end;			
			break;
		}
		case 2: //U-Boot
		{
			ret = flash_prog_init(&st, file_size, UBOOT_PARTITION);
			if(FAILURE(ret)) goto end;			
			printf("Programming U-Boot in the flash... \n");
			break;
		}
		case 3://JFFS2
		{
			ret = flash_prog_init(&st, file_size, JFFS2_PARTITION);
			if(FAILURE(ret)) goto end;			
			printf("Programming JFFS2 in the flash... \n");
			break;
		}
	}   
    
    fflush(stdout);
	if(partition != 1)
	{
		ret = updateflash_prog_putdata(&st, (char *)new_file);
		if(FAILURE(ret)) goto end;
	}
    
    printf("DONE\n");
    flash_prog_end(&st);

end:    
    if(new_file) free(new_file);    
    if (FAILURE(i)) state->has_ended=1; //connection has reset ?
    return ret;
}

Result dtbflash_recv(connection_state * state)
{
    Result ret = OK;
    int i = 0;
    unsigned file_size, mac_address_length, code;
    int transferred;
    unsigned char * new_file;
	char * newMacAddr;
    flash_prog_t st;
	
	//receive code number
	code = uint_recv(state);
	debug("code = 0x%X \n", code);
        
    //receive size of file
    file_size = uint_recv(state);
    debug("file_size = %i \n", file_size);
    
    new_file = malloc(file_size);
    if(!new_file) return CCEMEMORY;
    
    transferred = 0;
    while (transferred<file_size)
    {
        i = recv(state->stream, &new_file[transferred], file_size-transferred, 0);
        if(i<0) return CCETCPERROR;
        transferred+=i;
    }   
	
	//receive length of mac_address
    mac_address_length = uint_recv(state);
    debug("mac_address_length = %i \n", mac_address_length);
    
    newMacAddr = malloc(mac_address_length);
    if(!newMacAddr) return CCEMEMORY;
    
    transferred = 0;
    while (transferred<mac_address_length)
    {
        i = recv(state->stream, &newMacAddr[transferred], mac_address_length-transferred, 0);
        if(i<0) return CCETCPERROR;
        transferred+=i;
    }    
    debug("newMacAddr received : %s\n", newMacAddr);
	
	if(code == DTB_CODE)  // correct code, so we set the new mac_address received from CLI
	{
		printf("MAC Address is %s\n",newMacAddr);
		ret = set_mac_address_dtb((char *)new_file, (char *)newMacAddr);
		if(FAILURE(ret)) goto end;
	}
	else // bad code, or no code, so we set the mac_address from the default FPGA's MAC address
	{
		printf("MAC Address is %s\n",cce_mac);
		ret = set_mac_address_dtb((char *)new_file, (char *)cce_mac);
		if(FAILURE(ret)) goto end;
	}
	
	ret = flash_prog_init(&st, file_size, DTB_PARTITION);
	if(FAILURE(ret)) goto end;
	
	printf("Programming DTB in the flash... \n");		    
    fflush(stdout);
    ret = updateflash_prog_putdata(&st, (char *)new_file);
    if(FAILURE(ret)) goto end;
    
    printf("DONE\n");
    flash_prog_end(&st);

end:    
    if(new_file) free(new_file); 
	if(newMacAddr) free(newMacAddr);
    if (FAILURE(i)) state->has_ended=1; //connection has reset ?
    return ret;
}

Result fpgaflash_get_info_recv(connection_state * state)
{
    Result ret = OK;
    unsigned int index = 0;
    unsigned char * data = NULL;
    char * name = NULL;
    char * type = NULL;
    char * date = NULL;
    char * hour = NULL;
    char * custom = NULL;
    unsigned int name_length;
    unsigned int type_length;
    unsigned int date_length;
    unsigned int hour_length;
    unsigned int custom_length;
    
    index = uint_recv(state);
    if(index < 1 || index > 2)
    {
        ret = CCEINVALIDINDEX;
        goto end;
    }
    
    if(index == 1)
        ret = flash_read_last_sector(BITSTREAM1_PARTITION, &data);
    else
        ret = flash_read_last_sector(BITSTREAM2_PARTITION, &data);
    if(FAILURE(ret))
    {
        printf("Failed to read last partition : 0x%x\n", ret);
        ret = CCEINVALIDPARTITIONTABLE;
        goto end;
    }
    
    ret = bitstream_parse(data, &name, &name_length, &type, &type_length, &date, &date_length, &hour, &hour_length, &custom, &custom_length, NULL);
    if(FAILURE(ret))
    {
        if(index == 1)
            printf("Could not parse bitstream at partition : %s\n", BITSTREAM1_PARTITION);
        else
            printf("Could not parse bitstream at partition : %s\n", BITSTREAM2_PARTITION);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
end:
    
    if(SUCCESS(ret))
    {
        uint_send(state,1); // Send 1 to tell all header information will be sent
        buf_send(state, name, name_length);
        buf_send(state, type, type_length);
        buf_send(state, date, date_length);
        buf_send(state, hour, hour_length);
        buf_send(state, custom, custom_length);
        
        debug("name %d : %s\n", name_length, name);
        debug("type %d : %s\n", type_length, type);
        debug("date %d : %s\n", date_length, date);
        debug("hour %d : %s\n", hour_length, hour);
        debug("custom %d : %s\n", custom_length, custom);
    }
    else
    {
        uint_send(state,0); // Send 0 to tell no header information will be sent
    }
    
    if(data) free(data);
    if(name) free(name);
    if(type) free(type);
    if(date) free(date);
    if(hour) free(hour);
    if(custom) free(custom);
   
    return ret;
}

Result fpgaflash_print_info()
{
    Result ret = OK;
    unsigned char * data = NULL;
    char * name = NULL;
    char * type = NULL;
    char * date = NULL;
    char * hour = NULL;
    char * custom = NULL;
    unsigned int name_length;
    unsigned int type_length;
    unsigned int date_length;
    unsigned int hour_length;
    unsigned int custom_length;
    unsigned int jump_address;
    
    // Read first partition
    ret = flash_read_last_sector(BITSTREAM1_PARTITION, &data);
    if(FAILURE(ret))
    {
        printf("Failed to read partition fpga1.\n");
        printf("Verify that the system is fully updated.\n");
        ret = CCEINVALIDPARTITIONTABLE;
        goto end;
    }

    ret = bitstream_parse(data, &name, &name_length, &type, &type_length, &date, &date_length, &hour, &hour_length, &custom, &custom_length, &jump_address);
    if(FAILURE(ret))
    {
        printf("Bitstream 1 in flash is not a valid bitstream.\n");
    }
    else
    {
        printf("Current bitstream index is:\n");
        if(jump_address == BITSTREAM1_ADDRESS)
            printf("  1\n");
        else if (jump_address == BITSTREAM2_ADDRESS)
            printf("  2\n");
        else
            printf("  undefined\n");
    
        printf("Bitstream 1:\n");
        printf("  %s\n", name);
        printf("  %s\n", type);
        printf("  %s %s\n", date, hour);
        printf("  %s\n", custom);
    }
    
    // free data of bitstream partition 1
    if(data) free(data);
    data = NULL;
    
    if(name) free(name);
    name = NULL;
    if(type) free(type);
    type = NULL;
    if(date) free(date);
    date = NULL;
    if(hour) free(hour);
    hour = NULL;
    if(custom) free(custom);
    custom = NULL;
    
    // Read second partition
    ret = flash_read_last_sector(BITSTREAM2_PARTITION, &data);
    if(FAILURE(ret))
    {
        printf("Failed to read partition fpga2.\n");
        printf("Verify that the system is fully updated.\n");
        ret = CCEINVALIDPARTITIONTABLE;
        goto end;
    }

    ret = bitstream_parse(data, &name, &name_length, &type, &type_length, &date, &date_length, &hour, &hour_length, &custom, &custom_length, &jump_address);
    if(FAILURE(ret))
    {
        printf("Bitstream 2 in flash is not a valid bitstream.\n");
    }
    else
    {
        printf("Bitstream 2:\n");
        printf("  %s\n", name);
        printf("  %s\n", type);
        printf("  %s %s\n", date, hour);
        printf("  %s\n", custom);
    }
    
end:
    if(data) free(data);
    
    if(name) free(name);
    if(type) free(type);
    if(date) free(date);
    if(hour) free(hour);
    if(custom) free(custom);
   
    return ret;
}

Result fpgaflash_get_index_recv(connection_state * state)
{
    Result ret = OK;
    unsigned int index = 0;
    unsigned char * data = NULL;
    unsigned int jump_address;
    
    ret = flash_read_last_sector(BITSTREAM1_PARTITION, &data);
    if(FAILURE(ret))
    {
        printf("Failed to read last sector : 0x%x\n", ret);
        ret = CCEINVALIDPARTITIONTABLE;
        goto end;
    }
    
    ret = bitstream_parse(data, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &jump_address);
    if(FAILURE(ret))
    {
        printf("Could not parse bitstream at partition : %s\n", BITSTREAM1_PARTITION);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
    if(jump_address == BITSTREAM2_ADDRESS)
        index = 2;
    else if(jump_address == BITSTREAM1_ADDRESS)
        index = 1;
    else
        index = 0;
    
end:
    if(data) free(data);
    
    uint_send(state, index);
    return ret;
}

Result fpgaflash_set_index_recv(connection_state * state)
{
    Result ret = OK;
    unsigned int index = 0;
    unsigned char * data = NULL;
    unsigned int jump_address;
    
    index = uint_recv(state);
    if(index < 1 || index > 2)
        return CCEINVALIDINDEX;
    
    ret = flash_read_last_sector(BITSTREAM1_PARTITION, &data);
    if(FAILURE(ret))
    {
        printf("Failed to read last partition : 0x%x\n", ret);
        ret = CCEINVALIDPARTITIONTABLE;
        goto end;
    }
    
    //debug("size 0x%x\n", size);
    
    ret = bitstream_parse(data, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &jump_address);
    if(FAILURE(ret))
    {
        printf("Could not parse bitstream at partition : %s\n", BITSTREAM1_PARTITION);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
    // Verify if the current setting is already the desired one
    if(index == 2 && jump_address == BITSTREAM2_ADDRESS)
    {
        ret = OK;
        goto end;
    }
    if(index == 1 && jump_address == BITSTREAM1_ADDRESS)
    {
        ret = OK;
        goto end;
    }

    if(index == 1)
        ret = bitstream_set_jump_address(data, BITSTREAM1_ADDRESS);
    else if(index == 2)
        ret = bitstream_set_jump_address(data, BITSTREAM2_ADDRESS);
        
    if(FAILURE(ret))
    {
        printf("Could not set bitstream jump address: 0x%x\n", ret);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
    ret = flash_write_last_sector(BITSTREAM1_PARTITION, data);
    debug("flash_write_last_sector done\n",0);
    if(FAILURE(ret))
    {
        printf("Failed to write last partition: 0x%x\n", ret);
        ret = CCEINVALIDBITSTREAM;
        goto end;
    }
    
end:
    if(data)
    {
        free(data);
        data = NULL;
    }
    return ret;
}

Result memory_verify_size(unsigned int baseaddr)
{
	int i;
	unsigned char buffer[5];
	int sentByteCount, receivedByteCount, statusReg;
	const int transfer_size = 1;
	const int read_size = 1;

	const unsigned char DDR3_EEPROM_ADDR = 0x50;
	buffer[0] = 0x05;
	debug("Reading size %x\n",baseaddr);
	// Check the status register for the busy bus bit,
	// to verify if a previous write to the device could be pending and it
	// will not ack until that write is complete.
	i = 20000000;
	do {
		statusReg = XIo_In8(baseaddr + XIIC_SR_REG_OFFSET);
		if(!(statusReg & XIIC_SR_BUS_BUSY_MASK))
		{
			break;
		}
		i--;
	}while(i != 0);
	debug("Status\n",0);
	// If it failed then return
	if( i == 0 )
	{
		return(-1);
	}

	// Send the i2c message to prepare reading
	sentByteCount = XIic_Send(baseaddr, DDR3_EEPROM_ADDR, buffer, transfer_size, XIIC_STOP);
	debug("Send\n",0);
	// Wait with max timeout
	i = 20000000;
	do {
		statusReg = XIo_In8(baseaddr + XIIC_SR_REG_OFFSET);
		if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
			break;
		}
		i--;
	}while(i != 0);
	debug("Received\n",0);
	// Did the timeout exausted?
	if (i == 0)
	{
		debug("Timeout\n",0);
		return -1;
	}

	// Did the preparation message was written correctly?
	if (sentByteCount != transfer_size)
	{
		debug("Transfer size\n",0);
		return -1;
	}

	//read back the data from I2C bridge
	receivedByteCount = XIic_Recv(baseaddr, DDR3_EEPROM_ADDR, buffer, read_size, XIIC_STOP);
	debug("ReadSize = %x\n",read_size);
	debug("Byte 0 = %x\n",buffer[0]);

    	return buffer[0];
}

unsigned char read_sodimm_spd_register(unsigned int baseaddr, unsigned char addr)
{
	int i;
	unsigned char buffer[5];
	int sentByteCount, receivedByteCount, statusReg;
	const int transfer_size = 1;
	const int read_size = 1;

	const unsigned char DDR3_EEPROM_ADDR = 0x50;
	buffer[0] = addr;
	debug("Reading size %x\n",baseaddr);
	// Check the status register for the busy bus bit,
	// to verify if a previous write to the device could be pending and it
	// will not ack until that write is complete.
	i = 20000000;
	do {
		statusReg = XIo_In8(baseaddr + XIIC_SR_REG_OFFSET);
		if(!(statusReg & XIIC_SR_BUS_BUSY_MASK))
		{
			break;
		}
		i--;
	}while(i != 0);
	debug("Status\n",0);
	// If it failed then return
	if( i == 0 )
	{
		return(-1);
	}

	// Send the i2c message to prepare reading
	sentByteCount = XIic_Send(baseaddr, DDR3_EEPROM_ADDR, buffer, transfer_size, XIIC_STOP);
	debug("Send\n",0);
	// Wait with max timeout
	i = 20000000;
	do {
		statusReg = XIo_In8(baseaddr + XIIC_SR_REG_OFFSET);
		if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
			break;
		}
		i--;
	}while(i != 0);
	debug("Received\n",0);
	// Did the timeout exausted?
	if (i == 0)
	{
		debug("Timeout\n",0);
		return -1;
	}

	// Did the preparation message was written correctly?
	if (sentByteCount != transfer_size)
	{
		debug("Transfer size\n",0);
		return -1;
	}

	//read back the data from I2C bridge
	receivedByteCount = XIic_Recv(baseaddr, DDR3_EEPROM_ADDR, buffer, read_size, XIIC_STOP);
	debug("ReadSize = %x\n",read_size);
	debug("Byte 0 = %x\n",buffer[0]);

    	return buffer[0];
}

int updateflash_prog_putdata(flash_prog_t * st, char * buf)
{
	lseek(st->fd, 0, SEEK_SET);	
	debug("CUR at beginning 0x%x\n", lseek(st->fd, 0, SEEK_CUR));

    if(write(st->fd, buf, st->size) != st->size)
    {
        return -4;
    }

    if(lseek(st->fd, (off_t) -(st->size), SEEK_CUR) == (off_t) -1)
    {
        return -5;
    }
    debug("CUR at end 0x%x\n", lseek(st->fd, 0, SEEK_CUR));
	return 0;
}

Result set_mac_address_dtb(char * buf, char * macstring)
{
	Result ret = OK;
	int n, i;
	unsigned pTempMacAddr[6];
	unsigned char Addr[6];

	n = sscanf_s(macstring, "%02x:%02x:%02x:%02x:%02x:%02x",
               &pTempMacAddr[0], &pTempMacAddr[1], &pTempMacAddr[2],
               &pTempMacAddr[3], &pTempMacAddr[4], &pTempMacAddr[5]);
	if( n != 6)
	{
		return -1;
	}
	
	for (i=0; i<6; i++)
	{
		Addr[i] = (unsigned char)pTempMacAddr[i] & 0xff;
	}
	
	for (i=0; i<6; i++)
	{
		buf[MEMLOC_ADDR+i]=Addr[i];
		debug("%02x",buf[MEMLOC_ADDR+i]);
	}
	debug("\n",0);
	return ret;
}

Result change_cce(char * buf, unsigned size)
{
	Result ret = OK;
	int pFile;
	
	system("touch ccenew"); //Create empty file
	
	pFile = open("ccenew", O_RDWR); //Open empty file
	if(pFile < 0)
	{
		printf("Error opening cce file %i\n",pFile);
		return -1;
	}
	
	lseek(pFile,0,SEEK_SET);
	
	if(write(pFile, buf, size) != size) //Write in empty file
	{
		printf("Error writing in cce file\n");
		close(pFile);
		return -2;
	}
	close(pFile);
	system("cp /bin/cce /bin/cceold"); // make a safety copy of cce with the name of cceold
	system("cp ccenew cce"); // copy the ccenw to cce
	system("rm ccenew"); //remove ccenew
	system("cp cce /bin"); //copy cce to /bin
	system("chmod 777 /bin/cce");
	system("chmod 777 /bin/cceold");
	return ret;
}
