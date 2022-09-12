#ifndef __ADP_BITSTREAM_PROG__
#define __ADP_BITSTREAM_PROG__

typedef struct flash_prog {
    unsigned size;
    int fd;
    int uneven;
} flash_prog_t;

int flash_prog_init(flash_prog_t * st, unsigned bitstreamsize, const char * name);
int flash_prog_putdata(flash_prog_t * st, char * buf);
int flash_prog_end(flash_prog_t * st);

int flash_read_last_sector(const char * name, unsigned char ** data);
int flash_write_last_sector(const char * name, unsigned char * data);

int bitstream_parse(unsigned char * data,
    char ** name, unsigned int * name_length,
    char ** type, unsigned int * type_length,
    char ** date, unsigned int * date_length,
    char ** hour, unsigned int * hour_length,
    char ** custom, unsigned int * custom_length,
    unsigned int * jump_address);
int bitstream_set_jump_address(unsigned char * data, unsigned int jump_address);
int bitstream_set_custom(unsigned char ** data, unsigned int * size, char * custom, unsigned int custom_length);

#endif
