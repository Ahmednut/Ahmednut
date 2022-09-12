#ifndef __UTILS__
#define __UTILS__

#include "connection_state.h"
#include "bitstream.h"

Result fpgaflash_recv(connection_state * state);
Result updateflash_recv(connection_state * state);
Result dtbflash_recv(connection_state * state);
Result fpgaflash_get_info_recv(connection_state * state);
Result fpgaflash_print_info();
Result fpgaflash_get_index_recv(connection_state * state);
Result fpgaflash_set_index_recv(connection_state * state);
Result memory_verify_size(unsigned int baseaddr);
unsigned char read_sodimm_spd_register(unsigned int baseaddr, unsigned char addr);
int updateflash_prog_putdata(flash_prog_t * st, char * buf);
Result set_mac_address_dtb(char * buf, char * macstring);
Result change_cce(char * buf, unsigned size);

#endif /*__UTILS__*/
