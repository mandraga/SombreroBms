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

	
;; Words
#define M168PAGESIZE  64
;; Bytes
#define PAGESIZEB     (2 * M168PAGESIZE)
	
#ifndef __tmp_reg__
#define __tmp_reg__ 0
#endif

#define temp1    18
#define temp2    19

#define addrLo   24
#define addrHi   25
#define bufferLo 22
#define bufferHi 23
#define sizeLo   20
#define sizeHi   21

#define loophi   23
#define looplo   24
#define spmcrval 25

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
	;; replaced sbric
		lds  temp1, SPMCSR
		sbrc temp1, SELFPRGEN
	rjmp Wait_spm

		; input: spmcrval determines SPM action
		; disable interrupts if enabled, store status
		lds temp2, SREG
		cli
		; check that no EEPROM write access is present
	Wait_ee:
		;sbic EECR, EEPE
		lds  temp1, EECR
		sbrc temp1, EEPE
	rjmp Wait_ee
		; SPM timed sequence
	;; replaced out
		sts SPMCSR, spmcrval
		spm
		; restore SREG (to enable interrupts if originally enabled)
		sts SREG, temp2
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
;  Taken from the Atmega168pa datasheet and adapted
; 
;-the routine writes one page of data from RAM to Flash
; the first data location in RAM is pointed to by the Y pointer
; the first data location in Flash is pointed to by the Z-pointer
;-error handling is not included
;-the routine must be placed inside the Boot space
; (at least the Do_spm sub routine). Only code inside NRWW section can
; be read during Self-Programming (Page Erase and Page Write).
;-registers used: r0, r1, temp1 (r18), temp2 (r19), looplo (r24),
; loophi (r25), spmcrval (r20)
; storing and restoring of registers is not included in the routine
; register usage can be optimized at the expense of code size
;-It is assumed that either the interrupt table is moved to the Boot
; loader section or that the interrupts are disabled.
		push r28
		push r29
		push r30
		;
		movw r28, r22 ; Y = ram_buffer
		movw r30, r24 ; Z = flash_addr
		mov looplo, sizeLo
		clr loophi
	Write_page:
		; Page Erase
		ldi spmcrval, (1 << PGERS) | (1 << SELFPRGEN)
		rcall Do_spm
	
		; re-enable the RWW section
		ldi spmcrval, (1 << RWWSRE) | (1 << SELFPRGEN)
		rcall Do_spm
	
		; transfer data from RAM to Flash page buffer
		ldi looplo, low(PAGESIZEB)	;; init loop variable
		;ldi loophi, high(PAGESIZEB)	;; not required for PAGESIZEB<=256
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
		subi ZL, low(PAGESIZEB)		; restore pointer
		;sbci ZH, high(PAGESIZEB)	; not required for PAGESIZEB<=256
		ldi spmcrval, (1 << PGWRT) | (1 << SELFPRGEN)
		rcall Do_spm

		; re-enable the RWW section
		ldi spmcrval, (1 << RWWSRE) | (1 << SELFPRGEN)
		rcall Do_spm

#ifdef CHECK	
		;read back and check, optional
		ldi looplo, low(PAGESIZEB)	;; init loop variable
		;ldi loophi, high(PAGESIZEB)	;; not required for PAGESIZEB<=256
		subi YL, low(PAGESIZEB)		;; restore pointer
		sbci YH, high(PAGESIZEB)
	
	Rdloop:
		lpm  r0, Z+
		ld   r1, Y+
		cpse r0, r1
	rjmp Error
		;sbiw loophi:looplo, 1 ;use subi for PAGESIZEB<=256
		subi  looplo, 1
	brne Rdloop
zdezed
#endif
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
		pop r30
		pop r29
		pop r28
	ret
	.endfunc

