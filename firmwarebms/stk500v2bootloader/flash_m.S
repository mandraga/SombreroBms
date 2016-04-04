;*************************************************************************
; Title	:    flash spm functions for the atmega168pa
; Author:    Patrick Areny
; File:      
; Software:  AVR-GCC 3.3 or higher
; Target:    Atmega168pa
;
; DESCRIPTION
; 	Just a routine scraped from other source code to erase and write a flash
;       page on a bootloader with the STK500v2 protocol. 
;
; USAGE
;	These routines can be called from C, refere to file flash_m168p.h
;
;*************************************************************************

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 303
#error "This library requires AVR-GCC 3.3 or later, update to newer AVR-GCC compiler !"
#endif


#include <avr/io.h>

;; Substract 0x20 from the port addresses such as they can be used with in ou sbiw adiw 
;; #define __SFR_OFFSET 0

#ifdef  __AVR_ATmega128__
#define SELFPRGEN SPMEN
#endif
	
#ifndef __tmp_reg__
#define __tmp_reg__ 0
#endif

#define temp1    18
#define spmcrval 19

;; Inputs
#define sizeLo   20
#define sizeHi   21
#define bufferLo 22
#define bufferHi 23
#define addrLo   24
#define addrHi   25

#define looplo   26
#define loophi   27

#define low(A) (A & 0xFF)
	
	.section .text

;*************************************************************************
;
; Wait for spm complete and then call spm
;
;*************************************************************************
	.stabs "",100,0,0,Do_spm
	.stabs "flash_m168p.S",100,0,0,Do_spm
	.func  Do_spm
	Do_spm:
		; check for previous SPM complete
	Wait_spm:
		lds  temp1, SPMCSR
		sbrc temp1, SELFPRGEN
	rjmp Wait_spm

		; check that no EEPROM write access is present
	Wait_ee:
#ifdef  __AVR_ATmega128__
	        sbic _SFR_IO_ADDR(EECR), EEWE
#else	
		sbic _SFR_IO_ADDR(EECR), EEPE
#endif
	rjmp Wait_ee

		; SPM timed sequence
		sts SPMCSR, spmcrval
		spm

	Wait_spm2:
		lds  temp1, SPMCSR
		sbrc temp1, SELFPRGEN
	rjmp Wait_spm2

	ret
	.endfunc

;*************************************************************************	
;
; extern void wr_flash_page_m168pa(unsigned int flash_addr, unsigned char *ram_buffer, unsigned int size);
; for parameter passing, see doc42055.pdf
; fash_addr: r25, r24     ram_buffer: r23, r22     size: r21, r20
;*************************************************************************

	.global wr_flash_page_m168pa
	.func   wr_flash_page_m168pa
wr_flash_page_m168pa:
; Taken from the Atmega168pa datasheet and adapted
;
;-the routine writes one page of data from RAM to Flash
; the first data location in RAM is pointed to by the Y pointer
; the first data location in Flash is pointed to by the Z-pointer
;-error handling is not included
;-the routine must be placed inside the Boot space
; (at least the Do_spm sub routine). Only code inside NRWW section can
; be read during Self-Programming (Page Erase and Page Write).
;-registers used: r0, r1, temp1 (r18), looplo (r26),
; loophi (r27), spmcrval (r19)
; storing and restoring of registers is not included in the routine
; register usage can be optimized at the expense of code size
;-It is assumed that either the interrupt table is moved to the Boot
; loader section or that the interrupts are disabled.
;
		push r0
		push r28	;
		push r29

		;
		movw YL, r22   ; Y = ram_buffer (29:28) = (23:22)
		movw ZL, r24   ; Z = flash_addr (31:30) = (25:24)
	Write_page:
		; Page Erase
		ldi spmcrval, (1 << PGERS) | (1 << SELFPRGEN)
		rcall Do_spm

		; re-enable the RWW section
		ldi spmcrval, (1 << RWWSRE) | (1 << SELFPRGEN)
		rcall Do_spm

		; transfer data from RAM to Flash page buffer
		movw looplo, sizeLo ;
	Wrloop:
		ld r0, Y+
		ld r1, Y+
		ldi spmcrval, (1 << SELFPRGEN)
		rcall Do_spm
		adiw ZL, 2
		sbiw looplo, 2
		; use subi for PAGESIZEB<=256
	brne Wrloop

		; execute Page Write
		movw ZL, r24   ; Z = flash_addr (31:30) = (25:24)
		ldi spmcrval, (1 << PGWRT) | (1 << SELFPRGEN)
		rcall Do_spm

		; re-enable the RWW section
		ldi spmcrval, (1 << RWWSRE) | (1 << SELFPRGEN)
		rcall Do_spm

	;; check removed from here

		; return to RWW section
		; verify that RWW section is safe to read
	Return:
		lds  temp1, SPMCSR 
		sbrs temp1, RWWSB
		; If RWWSB is set, the RWW section is not ready yet
	rjmp End_flash_WR
		; re-enable the RWW section
		ldi  spmcrval, (1 << RWWSRE) | (1 << SELFPRGEN)
		rcall Do_spm
	rjmp Return
	End_flash_WR:
		; restore registers
		clr r1
		pop r29
		pop r28
		pop r0
	ret
	.endfunc

