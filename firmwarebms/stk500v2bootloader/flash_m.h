#ifndef _FLASH_M168P_H
#define _FLASH_M168P_H

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif

extern void wr_flash_page_m168pa(unsigned int flash_addr, unsigned char *ram_buffer, unsigned int size);

#endif
