#include <stdio.h>
#include <inttypes.h>

#include "command.h"

/*
 * Uncomment the following lines to save code space 
 */
#define REMOVE_PROGRAM_LOCK_BIT_SUPPORT  // disable program lock bits
//#define REMOVE_BOOTLOADER_LED            // no LED to show active bootloader
#define REMOVE_PROG_PIN_PULLUP           // disable internal pullup, use external 
#define REMOVE_CMD_SPI_MULTI             // disable processing of SPI_MULTI commands

/*
 *  Uncomment to leave bootloader and jump to application after programming.
 */
#define ENABLE_LEAVE_BOOTLADER

/*
 * define CPU frequency in Mhz here if not defined in Makefile 
 */
#ifndef F_CPU
#define F_CPU 3686400UL
#endif

/*
 * UART Baudrate, AVRStudio AVRISP only accepts 115200 bps
 */
#define BAUDRATE 115200


/*
 * HW and SW version, reported to AVRISP, must match version of AVRStudio
 */
#define CONFIG_PARAM_BUILD_NUMBER_LOW	0
#define CONFIG_PARAM_BUILD_NUMBER_HIGH	0
#define CONFIG_PARAM_HW_VER	       	0x0F
#define CONFIG_PARAM_SW_MAJOR	       	2
#define CONFIG_PARAM_SW_MINOR	       	0x0A




/*
 * States used in the receive state machine
 */
#define	ST_START	0
#define	ST_GET_SEQ_NUM	1
#define ST_MSG_SIZE_1	2
#define ST_MSG_SIZE_2	3
#define ST_GET_TOKEN	4
#define ST_GET_DATA	5
#define	ST_GET_CHECK	6
#define	ST_PROCESS	7

// Signature for the ATMEGA168PA
#define SIGNATURE_BYTES 0x1E940B

#define APP_END (16 * 1024)

/*
 * send single byte to USART, wait until transmission is completed
 */
static void sendchar(char c)
{
  printf("0x%02x\n", c);
}

#define TIMEOUTLIMIT 256000
/*
 * Read single byte from USART, block if no data available
 */
unsigned char recchar(char *ptimeout, unsigned char *msg, int size, int j)
{
  if (j >= size)
    {
      *ptimeout = 1;
      return 0;
    }
  *ptimeout = 0;
  return msg[j];
}

typedef uint32_t address_t;
 
int stk_receive(unsigned char *msg, int size)
{
    address_t       address = 0;
    address_t       eraseAddress = 0;	
    unsigned char   msgParseState;
    unsigned int    i = 0;
    unsigned char   checksum = 0;
    unsigned char   seqNum = 0;
    unsigned int    msgLength = 0;
    unsigned char   msgBuffer[285];
    unsigned char   c, *p;
    unsigned char   isLeave = 0;
    char            timeout;
    int             j = 0;

    /* main loop */
    while(!isLeave)                             
      {
	/*
	 * Collect received bytes to a complete message
	 */
	msgParseState = ST_START;
	while ( msgParseState != ST_PROCESS )
	  {
	    c = recchar(&timeout, msg, size, j++);
	    if (timeout)
	      goto skip_to_main_program;
	    //sendchar(c);	  
	    switch (msgParseState)
	      {
	      case ST_START:
		if ( c == MESSAGE_START )
		  {
		    msgParseState = ST_GET_SEQ_NUM;
		    checksum = MESSAGE_START^0;
		  }
		break;
		
	      case ST_GET_SEQ_NUM:
#define _FIX_ISSUE_505_
#ifdef _FIX_ISSUE_505_
		seqNum = c;
		msgParseState = ST_MSG_SIZE_1;
		checksum ^= c;
#else
		if ( (c == 1) || (c == seqNum) )
		  {
		    seqNum = c;
		    msgParseState = ST_MSG_SIZE_1;
		    checksum ^= c;
		  }
		else
		  {
		    msgParseState = ST_START;
		  }
#endif
		break;
		
	      case ST_MSG_SIZE_1:
		msgLength = c<<8;
		msgParseState = ST_MSG_SIZE_2;
		checksum ^= c;
		break;
		
	      case ST_MSG_SIZE_2:
		msgLength |= c;
		msgParseState = ST_GET_TOKEN;
		checksum ^= c;
		break;
		
	      case ST_GET_TOKEN:
		if ( c == TOKEN )
		  {
		    msgParseState = ST_GET_DATA;
		    checksum ^= c;
		    i = 0;
		  }
		else
		  {
		    msgParseState = ST_START;
		  }
		break;
		
	      case ST_GET_DATA:
		msgBuffer[i++] = c;
		checksum ^= c;
		if (i == msgLength )
		  {
		    msgParseState = ST_GET_CHECK;
		  }
		break;
		
	      case ST_GET_CHECK:
		if ( c == checksum )
		  {
		    msgParseState = ST_PROCESS;
		  }
		else
		  {
		    msgParseState = ST_START;
		  }
		break;
	      }//switch
	  }//while(msgParseState)
	
	/*
	 * Now process the STK500 commands, see Atmel Appnote AVR068
	 */
	
	switch (msgBuffer[0])
	  {
#ifndef REMOVE_CMD_SPI_MULTI
	  case CMD_SPI_MULTI:
	    {
	      unsigned char answerByte = 0;
	      
	      // only Read Signature Bytes implemented, return dummy value for other instructions
	      if ( msgBuffer[4]== 0x30 )
		{						
		  unsigned char signatureIndex = msgBuffer[6];						
		  
		  if ( signatureIndex == 0 )
		    answerByte = (SIGNATURE_BYTES >>16) & 0x000000FF;
		  else if ( signatureIndex == 1 )
		    answerByte = (SIGNATURE_BYTES >> 8) & 0x000000FF;
		  else
		    answerByte = SIGNATURE_BYTES & 0x000000FF;
		}					
	      msgLength = 7;
	      msgBuffer[1] = STATUS_CMD_OK;
	      msgBuffer[2] = 0;					
	      msgBuffer[3] = msgBuffer[4];  // Instruction Byte 1
	      msgBuffer[4] = msgBuffer[5];  // Instruction Byte 2
	      msgBuffer[5] = answerByte;	                
	      msgBuffer[6] = STATUS_CMD_OK;
	    }
	    break;
#endif
	  case CMD_SIGN_ON:
	    msgLength = 11;		        
	    msgBuffer[1]  = STATUS_CMD_OK;
	    msgBuffer[2]  = 8;
	    msgBuffer[3]  = 'A';
	    msgBuffer[4]  = 'V';
	    msgBuffer[5]  = 'R';
	    msgBuffer[6]  = 'I';
	    msgBuffer[7]  = 'S';
	    msgBuffer[8]  = 'P';
	    msgBuffer[9]  = '_';
	    msgBuffer[10] = '2';
	    break;
	    
	  case CMD_GET_PARAMETER:
	    {
	      unsigned char value;
	      
	      switch(msgBuffer[1])
		{
		case PARAM_BUILD_NUMBER_LOW:
		  value = CONFIG_PARAM_BUILD_NUMBER_LOW;
		  break;				    
		case PARAM_BUILD_NUMBER_HIGH:
		  value = CONFIG_PARAM_BUILD_NUMBER_HIGH;
		  break;				    
		case PARAM_HW_VER:
		  value = CONFIG_PARAM_HW_VER;
		  break;				    
		case PARAM_SW_MAJOR:
		  value = CONFIG_PARAM_SW_MAJOR;
		  break;
		case PARAM_SW_MINOR:
		  value = CONFIG_PARAM_SW_MINOR;
		  break;				
		default:
		  value = 0;
		  break;
		}
	      msgLength = 3;		        
	      msgBuffer[1] = STATUS_CMD_OK;
	      msgBuffer[2] = value;
	    }
	    break;
	    
	  case CMD_LEAVE_PROGMODE_ISP:
#ifdef ENABLE_LEAVE_BOOTLADER
	    isLeave = 1;
#endif
	  case CMD_ENTER_PROGMODE_ISP:    	            
	  case CMD_SET_PARAMETER:	        
	    msgLength = 2;		        
	    msgBuffer[1] = STATUS_CMD_OK;
	    break;
	    
	  case CMD_READ_SIGNATURE_ISP:
	    {
	      unsigned char signatureIndex = msgBuffer[4];
	      unsigned char signature;
	      
	      if ( signatureIndex == 0 )
		signature = (SIGNATURE_BYTES >>16) & 0x000000FF;
	      else if ( signatureIndex == 1 )
		signature = (SIGNATURE_BYTES >> 8) & 0x000000FF;
	      else
		signature = SIGNATURE_BYTES & 0x000000FF;
	      
	      msgLength = 4;
	      msgBuffer[1] = STATUS_CMD_OK;
	      msgBuffer[2] = signature;
	      msgBuffer[3] = STATUS_CMD_OK;	                
	    }
	    break;
	    
	  case CMD_READ_LOCK_ISP:            
	    msgLength = 4;
	    msgBuffer[1] = STATUS_CMD_OK;
	    msgBuffer[2] = 0x11; //boot_lock_fuse_bits_get( GET_LOCK_BITS );
	    msgBuffer[3] = STATUS_CMD_OK;	                                                
	    break;
	    
	  case CMD_READ_FUSE_ISP:
	    {                    
	      unsigned char fuseBits;                    
	      
	      if ( msgBuffer[2] == 0x50 )
		{
		  if ( msgBuffer[3] == 0x08 )
		    fuseBits = 0x11; //boot_lock_fuse_bits_get( GET_EXTENDED_FUSE_BITS );                            
		  else
		    fuseBits = 0x11; //boot_lock_fuse_bits_get( GET_LOW_FUSE_BITS );                            
		}
	      else 
		{
		  fuseBits = 0x11; //boot_lock_fuse_bits_get( GET_HIGH_FUSE_BITS );
		}                    
	      msgLength = 4;    
	      msgBuffer[1] = STATUS_CMD_OK;
	      msgBuffer[2] = fuseBits;	                
	      msgBuffer[3] = STATUS_CMD_OK;	                                    
	    }
	    break;
	    
#ifndef REMOVE_PROGRAM_LOCK_BIT_SUPPORT
	  case CMD_PROGRAM_LOCK_ISP:
	    {
	      unsigned char lockBits = msgBuffer[4];
	      
	      lockBits = (~lockBits) & 0x3C;  // mask BLBxx bits
	      boot_lock_bits_set(lockBits);	// and program it
	      boot_spm_busy_wait();
	      
	      msgLength = 3;
	      msgBuffer[1] = STATUS_CMD_OK;	                
	      msgBuffer[2] = STATUS_CMD_OK;	                                                        
	    }
	    break;
#endif
	  case CMD_CHIP_ERASE_ISP:
	    eraseAddress = 0;
	    msgLength = 2;
	    msgBuffer[1] = STATUS_CMD_OK;
	    break;
	    
	  case CMD_LOAD_ADDRESS:
#if defined(RAMPZ)
	    address = ( ((address_t)(msgBuffer[1])<<24)|((address_t)(msgBuffer[2])<<16)|((address_t)(msgBuffer[3])<<8)|(msgBuffer[4]) )<<1;
#else
	    address = ( ((msgBuffer[3])<<8)|(msgBuffer[4]) )<<1;  //convert word to byte address
#endif
	    msgLength = 2;
	    msgBuffer[1] = STATUS_CMD_OK;
	    break;
	    
	  case CMD_PROGRAM_FLASH_ISP:
	  case CMD_PROGRAM_EEPROM_ISP:                
	    {
	      unsigned int  size = (((unsigned int)msgBuffer[1])<<8) | msgBuffer[2];
	      //unsigned char *p = msgBuffer+10;
	      //unsigned int  data;
	      //unsigned char highByte, lowByte;                    
	      //address_t     tempaddress = address;
	      
	      
	      if ( msgBuffer[0] == CMD_PROGRAM_FLASH_ISP )
		{
		  // erase only main section (bootloader protection)
		  if  (  eraseAddress < APP_END )
		    {
		      //boot_page_erase(eraseAddress);	// Perform page erase
		      //boot_spm_busy_wait();		// Wait until the memory is erased.
		      //eraseAddress += SPM_PAGESIZE;    // point to next page to be erase
		    }
		  
		  /* Write FLASH */
		  do {
		    //lowByte   = *p++;
		    //highByte  = *p++;
		    
		    //data =  (highByte << 8) | lowByte;
		    //boot_page_fill(address,data);
		    
		    address = address + 2;  	// Select next word in memory
		    size -= 2;			// Reduce number of bytes to write by two    
		  } while(size);			// Loop until all bytes written
		  
		  //boot_page_write(tempaddress);
		  //boot_spm_busy_wait();	
		  //boot_rww_enable();				// Re-enable the RWW section                    
		}
	      else
		{
		  /* write EEPROM */
		}
	      msgLength = 2;
	      msgBuffer[1] = STATUS_CMD_OK;    		        
	    }
	    break;
	    
	  case CMD_READ_FLASH_ISP:
	  case CMD_READ_EEPROM_ISP:                                                
	    {
	      unsigned int  size = (((unsigned int)msgBuffer[1])<<8) | msgBuffer[2];
	      unsigned char *p = msgBuffer+1;
	      msgLength = size+3;
	      
	      *p++ = STATUS_CMD_OK;                    
	      if (msgBuffer[0] == CMD_READ_FLASH_ISP )
		{
		  //unsigned int data;
		  
		  // Read FLASH
		  do {                            
#if defined(RAMPZ)
		    //data = pgm_read_word_far(address);
#else
		    //data = pgm_read_word_near(address);
#endif
		    //*p++ = (unsigned char)data;         //LSB
		    //*p++ = (unsigned char)(data >> 8);	//MSB  
		    address    += 2;  	 // Select next word in memory
		    size -= 2;
		  }while (size);
		}
	      else
		{
		  /* Read EEPROM */
		}
	      *p++ = STATUS_CMD_OK;
	    }
	    break;
	    
	  default:
	    msgLength = 2;   
	    msgBuffer[1] = STATUS_CMD_FAILED;
	    break;
	  }                             
	
	/*
	 * Now send answer message back
	 */
	sendchar(MESSAGE_START);     
	checksum = MESSAGE_START^0;
	
	sendchar(seqNum);
	checksum ^= seqNum;
	
	c = ((msgLength>>8)&0xFF);
	sendchar(c);
	checksum ^= c;
	
	c = msgLength&0x00FF;
	sendchar(c);
	checksum ^= c;
	
	sendchar(TOKEN);
	checksum ^= TOKEN;
	
	p = msgBuffer;
	while ( msgLength )
	  {                
	    c = *p++;
	    sendchar(c);
	    checksum ^=c;
	    msgLength--;               
	  }                   
	sendchar(checksum);	        
	seqNum++;
	
      }
 skip_to_main_program:
    sendchar('T');
    sendchar('E');
    sendchar('S');
    sendchar('T');
    return 0;
}
