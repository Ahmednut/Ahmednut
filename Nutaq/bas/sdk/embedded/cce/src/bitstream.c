#include "bitstream.h"
#include <stdio.h>
#include <mtd/mtd-user.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include "bas_debug.h"

enum {devicename_size = 32};
static char devicename[devicename_size];
static char *devicebasename = "/dev/";

// name = "\"fpga\"";
static int find_fpga_device_name(const char * name)
{
	FILE *f;
	char s[256];
    int ret = -1;

	devicename[0] = '\0';

	f = fopen("/proc/mtd", "r");
	if (!f)
	{
		return -1;
	}

	while (fgets(s, 256, f))
	{
		if (strstr(s, name))
		{
			debug("using %s\n", s);
			int i = 0;
			int l;
			strcpy(devicename, devicebasename);
			l = strlen(devicebasename);
			while(s[i] && s[i] != ':' && i < devicename_size - l)
			{
				devicename[i+l] = s[i];
				i++;
			}
			devicename[i+l] = '\0';
            ret = 0;
            break;
		}
	}

    debug("devicename is now %s\n", devicename);
    fclose(f);

    return ret;
}

/* xxx - bitstream magic number need to be 16-bit aligned */
/* data is stored byte-reverse, 16-bit are bit-reversed */

int flash_prog_init(flash_prog_t * st, unsigned bitstreamsize, const char * name)
{
    //erase flash
    int ret;
    mtd_info_t meminfo;
    erase_info_t erase;

    ret = find_fpga_device_name(name);
    if(ret < 0)
    {
        printf("Flash device %s was not found.\n", name);
        return -1;
    }

    printf("Opening flash device : %s \n", devicename);
    st->fd = open( devicename , O_RDWR);
    if(st->fd < 0)
    {
        return -1;
    }

    if(ioctl( st->fd, MEMGETINFO, &meminfo ) != 0)
    {
        fprintf(stderr, "can't get meminfo...\n");
        ret = -2;
        goto Bad1;
    }

    erase.length = meminfo.erasesize;
    printf("Erasing 0x%x Bytes\n", meminfo.size);
    for (erase.start = 0; erase.start < meminfo.size;
         erase.start += meminfo.erasesize)
    {
        printf("\r%3d%%", (erase.start*100)/meminfo.size);
        fflush(stdout);

        if(ioctl(st->fd, MEMUNLOCK, &erase) != 0)
        {
            ret = -3;
            goto Bad1;
        }

        if(ioctl(st->fd, MEMERASE, &erase) != 0)
        {
            ret = -4;
            goto Bad1;
        }
    }
    printf("\r%3d%%\n", 100);

    if(lseek(st->fd, 0, SEEK_END) == (off_t) -1)
    {
        ret = -5;
        goto Bad1;
    }

    st->size = bitstreamsize;
    st->uneven = bitstreamsize & 0x1;

    return 0;

Bad1:
    close(st->fd);
    st->fd = 0;
    return ret;
}

static void reverse_array(unsigned char * buf, unsigned size)
{
    int j;
    unsigned char ctmp;
    for (j=0; j<size/2; j++)
    {
        ctmp = buf[j];
        buf[j] = buf[size-1-j];
        buf[size-1-j] = ctmp;
    }
}

static void reverse_bit(unsigned char * buf, unsigned size)
{
    int j;
    unsigned char * cbuf;
    unsigned short tmp;

    cbuf = (unsigned char*)buf;
    for (j=0; j<size; j++)
    {
        tmp = cbuf[j];
        tmp = ((tmp >> 1) & 0x55) | ((tmp << 1) & 0xaa);
        tmp = ((tmp >> 2) & 0x33) | ((tmp << 2) & 0xcc);
        tmp = ((tmp >> 4) & 0x0f) | ((tmp << 4) & 0xf0);
        cbuf[j] = tmp;
	}
}

int flash_prog_putdata(flash_prog_t * st, char * buf)
{
    int ret = 0;
    char * tmp_buf=NULL;
    unsigned timeout;
    unsigned pos = 0;

    if(st->uneven > 0)
    {
        // find the first ';'
        timeout = 1000;
        while(--timeout)
        {
            pos++;
            if( *(buf + pos) == ';' )
                break;
        }
        if(!timeout)
        {
            debug("Cannot find ';' string.\n", 0);
            pos = 16;
        }

        //add 1 space after the bitstream name and ';' to align on 16-bit boundaries
        pos++;

        tmp_buf = malloc(st->size+1);
        memcpy(tmp_buf, buf, pos);
        tmp_buf[pos] = ' ';
        memcpy(tmp_buf+pos+1, buf+pos, st->size-pos);
        buf = tmp_buf;
        st->size++;
    }

    reverse_array((unsigned char *)buf, st->size);
    reverse_bit((unsigned char *)buf, st->size);

    lseek(st->fd, -(st->size), SEEK_END);

    if(write(st->fd, buf, st->size) != st->size)
    {
        ret = -4;
        goto Error;
    }

    if(lseek(st->fd, (off_t) -(st->size), SEEK_CUR) == (off_t) -1)
    {
        ret = -5;
        goto Error;
    }

    debug("CUR at end 0x%x\n", lseek(st->fd, 0, SEEK_CUR));


Error:
    if(tmp_buf) free(tmp_buf);
    return ret;
}

int flash_prog_end(flash_prog_t * st)
{
    fsync(st->fd);
    close(st->fd);
    st->fd = 0;
    return 0;
}

int flash_read_last_sector(const char * name, unsigned char ** data)
{
    int ret;
    mtd_info_t meminfo;
    flash_prog_t st;

    ret = find_fpga_device_name(name);
    if(ret < 0)
    {
        printf("Flash device %s was not found.\n", name);
        return -1;
    }

    debug("Opening flash device : %s \n", devicename);
    st.fd = open( devicename , O_RDONLY);
    if(st.fd < 0)
    {
        return -1;
    }

    if(ioctl( st.fd, MEMGETINFO, &meminfo ) != 0)
    {
        fprintf(stderr, "can't get meminfo...\n");
        ret = -1;
        goto End;
    }

    *data = malloc(meminfo.erasesize);
    if(!*data)
    {
        fprintf(stderr, "can't allocate data...\n");
        ret = -1;
        goto End;
    }

    lseek(st.fd, -(meminfo.erasesize), SEEK_END);

    if(read(st.fd, *data, meminfo.erasesize) != meminfo.erasesize)
    {
        fprintf(stderr, "Can't read flash memory...\n");
        ret = -1;
        free(*data);
        *data = 0;
        goto End;
    }

    ret = meminfo.erasesize;

    reverse_bit(*data, meminfo.erasesize);
    reverse_array(*data, meminfo.erasesize);

End:
    close(st.fd);
    return ret;
}

int flash_write_last_sector(const char * name, unsigned char * data)
{
    int ret;
    mtd_info_t meminfo;
    erase_info_t erase;
    flash_prog_t st;

    ret = find_fpga_device_name(name);
    if(ret < 0)
    {
        printf("Flash device %s was not found.\n", name);
        return -1;
    }

    debug("Opening flash device : %s \n", devicename);
    st.fd = open( devicename , O_RDWR);
    if(st.fd < 0)
    {
        return -1;
    }

    if(ioctl( st.fd, MEMGETINFO, &meminfo ) != 0)
    {
        fprintf(stderr, "can't get meminfo...\n");
        ret = -1;
        goto End;
    }

    erase.length = meminfo.erasesize;
    erase.start = meminfo.size - meminfo.erasesize;

    if(ioctl(st.fd, MEMUNLOCK, &erase) != 0)
    {
        ret = -3;
        goto End;
    }

    if(ioctl(st.fd, MEMERASE, &erase) != 0)
    {
        ret = -4;
        goto End;
    }

    reverse_bit(data, meminfo.erasesize);
    reverse_array(data, meminfo.erasesize);

    lseek(st.fd, -(meminfo.erasesize), SEEK_END);

    if(write(st.fd, data, meminfo.erasesize) != meminfo.erasesize)
    {
        fprintf(stderr, "Can't write flash memory...\n");
        ret = -1;
        goto End;
    }

End:
    close(st.fd);
    return ret;
}

int bitstream_parse(unsigned char * data,
    char ** name, unsigned int * name_length,
    char ** type, unsigned int * type_length,
    char ** date, unsigned int * date_length,
    char ** hour, unsigned int * hour_length,
    char ** custom, unsigned int * custom_length,
    unsigned int * jump_address)
{
    unsigned char header[13] = {0x00, 0x09, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x0f, 0xf0, 0x00, 0x00, 0x01};
    int ret = 0;
    unsigned pos = 0;
    unsigned timeout;
    unsigned section_start;
    unsigned char section_length;

    if(memcmp(header, data, 13))
    {
        debug("No valid bitstream header\n", 0);
		ret  = -1;
		goto end;
	}

    // find the first "a." string
    timeout = 1000;
    while(--timeout)
	{
        pos++;
        if( *(data + pos) == 0x61 && *(data + pos + 1) == 0x00 )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find 'a.' string.\n", 0);
		ret  = -1;
		goto end;
	}
    pos+=2;

    // Skip the length byte
    pos++;
    section_start = pos;

    // find the first ';'
    timeout = 1000;
    while(--timeout)
	{
        pos++;
		if( *(data + pos) == ';' )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find ';' string.\n", 0);
		ret  = -1;
		goto end;
	}

    section_length = pos - section_start;

    if(name)
    {
        *name_length = section_length + 1;
        *name = malloc(*name_length);

        memcpy(*name, data + section_start, section_length);
        (*name)[section_length] = '\0';
        debug("Bitstream name %s\n", *name);
    }

    // find the first "b." string
    timeout = 1000;
    while(--timeout)
	{
        pos++;
        if( *(data + pos) == 0x62 && *(data + pos + 1) == 0x00 )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find 'b.' string.\n", 0);
		ret  = -1;
		goto end;
	}
    pos+=2;

    // section length byte
    section_length = *(data + pos);
    pos++;
    section_start = pos;

    if(type)
    {
        *type_length = section_length;
        *type = malloc(*type_length);

        memcpy(*type, data + section_start, section_length-1);
        (*type)[section_length-1] = '\0';
        debug("Bitstream type %s\n", *type);
    }

    pos += (section_length + 2); // skip "c."

    // section length byte
    section_length = *(data + pos);
    pos++;
    section_start = pos;

    if(date)
    {
        *date_length = section_length;
        *date = malloc(*date_length);

        memcpy(*date, data + section_start, section_length-1);
        (*date)[section_length-1] = '\0';
        debug("Bitstream date %s\n", *date);
    }

    pos += (section_length + 2); // skip "d."

    // section length byte
    section_length = *(data + pos);
    pos++;
    section_start = pos;

    if(hour)
    {
        *hour_length = section_length;
        *hour = malloc(*hour_length);

        memcpy(*hour, data + section_start, section_length-1);
        (*hour)[section_length-1] = '\0';
        debug("Bitstream hour %s\n", *hour);
    }

    pos += (section_length + 2); // skip "e."

    if(custom)
    {
        // section length byte
        section_length = *(data + pos);
        pos++;
        section_start = pos;
        
        debug("Custom length : 0x%X\n", section_length);
        debug("Verify Custom : 0x%X 0x%X 0x%X 0x%X\n", *(data + pos + 2), *(data + pos + 3), *(data + pos + 4), *(data + pos + 5));

        if( *(data + pos) > 2 &&
            *(data + pos + 2) == 0xFF && *(data + pos + 3) == 0xFF &&
            *(data + pos + 4) == 0xFF && *(data + pos + 5) == 0xFF )
        {
            *custom_length = 1;
            *custom = malloc(1);
            (*custom)[0] = '\0'; // Null string
            debug("Bitstream custom NULL string\n", 0);
        }
        else
        {
            *custom_length = section_length + 1;
            *custom = malloc(*custom_length);

            memcpy(*custom, data + section_start, section_length);
            (*custom)[section_length] = '\0';
            debug("Bitstream custom %s\n", *custom);
        }
    }

    // Search for synchronization word
    timeout = 1000;
    while(--timeout)
	{
        pos++;
		if( *(data + pos) == 0xAA && *(data + pos + 1) == 0x99 &&
            *(data + pos + 2) == 0x55 && *(data + pos + 3) == 0x66 )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find synchronization word.\n", 0);
		ret  = -1;
		goto end;
	}

    // Search for WBSTAR command
    timeout = 1000;
    while(--timeout)
	{
        pos++;
		if( *(data + pos) == 0x30 && *(data + pos + 1) == 0x02 &&
            *(data + pos + 2) == 0x00 && *(data + pos + 3) == 0x01 )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find WBSTAR command.\n", 0);
		ret  = -1;
		goto end;
	}

    if(jump_address)
    {
        if( *(data + pos + 15) == 0x0F )
        {
            *jump_address =
                (*(data + pos + 4) << 24) +
                (*(data + pos + 5) << 16) +
                (*(data + pos + 6) << 8) +
                (*(data + pos + 7) << 0);
            *jump_address &= 0x3FFFFFF;
        }
        else
        {
            *jump_address = 0x0;
        }
        debug("Jump address is 0x%X.\n", *jump_address);
    }

end:
    return ret;
}

int bitstream_set_jump_address(unsigned char * data, unsigned int jump_address)
{
    int ret = 0;
    unsigned pos = 0;
    unsigned timeout;

    // Search for WBSTAR command
    timeout = 1000;
    while(--timeout)
	{
        pos++;
		if( *(data + pos) == 0x30 && *(data + pos + 1) == 0x02 &&
            *(data + pos + 2) == 0x00 && *(data + pos + 3) == 0x01 )
            break;
	}
    if(!timeout)
	{
        debug("Cannot find WBSTAR command.\n", 0);
		ret  = -1;
		goto end;
	}

    *(data + pos + 4) = (jump_address >> 24) & 0xFF;
    *(data + pos + 5) = (jump_address >> 16) & 0xFF;
    *(data + pos + 6) = (jump_address >> 8)  & 0xFF;
    *(data + pos + 7) = (jump_address >> 0)  & 0xFF;

    // Set RS[1:0] active high, need to be done because connected to the flash reset#
    *(data + pos + 4) |= 0x18;

    *(data + pos + 12) = 0x00;
    *(data + pos + 13) = 0x00;
    *(data + pos + 14) = 0x00;

    if(jump_address == 0x0)
        *(data + pos + 15) = 0x00;
    else
        *(data + pos + 15) = 0x0F;

end:
    return ret;
}

int bitstream_set_custom(unsigned char ** data, unsigned int * size, char * custom, unsigned int custom_length)
{
    int ret = 0;
    unsigned pos = 0;
    unsigned timeout;
    unsigned uneven = 0;
    unsigned char section_length;
    
    int size_difference;
    unsigned char * tmp_buf=NULL;
    
    // search for 0x00 0x65 0x00 or 0x00 0x65 0x01
    timeout = 1000;
    while(--timeout)
	{
        pos++;
		if( *(*data + pos) == 0x00 && *(*data + pos + 1) == 0x65 &&
            (*(*data + pos + 2) == 0x00 || *(*data + pos + 2) == 0x01))
            break;
	}
    if(!timeout)
	{
        debug("Cannot find .e. command.\n", 0);
		ret  = -1;
		goto end;
	}
    pos+=3;
    
    debug("Custom start at : 0x%X\n", pos);
    
    // clean current custom field
    if( *(*data + pos) > 2 &&
        *(*data + pos + 3) == 0xFF && *(*data + pos + 4) == 0xFF &&
        *(*data + pos + 5) == 0xFF && *(*data + pos + 6) == 0xFF )
    {
        debug("Custom field is empty\n", 0);
    
        section_length = 0;
        size_difference = custom_length + 1; // size + custom field
    }
    else
    {
        section_length = *(*data + pos);
        debug("section_length : 0x%X\n", section_length);
        size_difference = custom_length - section_length;
    }
    
    debug("size_difference: %d\n", size_difference);
    
    // Make sure the bitstream length is even for the data to be align on
    // the flash data bus (2 byte wide)
    if((*size + size_difference)%2 == 1)
        uneven = 1;
        
    debug("uneven: %d\n", uneven);
    
    tmp_buf = malloc(*size + size_difference + uneven);
    
    debug("malloc: %d\n", *size + size_difference + uneven);
    
    // Copy header before custom size byte
    memcpy(tmp_buf, *data, pos);
    
    debug("data 0 and pos-1: 0x%X  0x%X\n", (*data)[0], (*data)[pos-1]);

    debug("tmp_buf[pos] before: 0x%X\n", tmp_buf[pos]);
    
    // Copy custom size byte
    tmp_buf[pos] = custom_length;
    
    debug("tmp_buf[pos] after: 0x%X\n", tmp_buf[pos]);
    
    // Copy custom field
    memcpy(tmp_buf+pos+1, custom, custom_length);
    
    if(uneven)
    {
        tmp_buf[pos+1+custom_length] = ' ';
    }
    
    memcpy(tmp_buf+pos+1+custom_length+uneven, *data+pos+section_length, *size-pos-section_length);
    
    // Use the new bitstream size
    if(*data) free(*data);
    *data = tmp_buf;
    
    // Update size value
    *size += size_difference + uneven;
    
    debug("NEW BITSTREAM DUMP:  size: %d\n", *size);
    for(section_length=0 ;section_length<16; section_length++)
    {
        for(timeout=0 ;timeout<16; timeout++)
        {
            debug("%2X ", (*data)[16*section_length + timeout]);
        }
        debug("\n", 0);
    }
        
    
end:
    return ret;
}




