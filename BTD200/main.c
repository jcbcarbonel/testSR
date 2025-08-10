//Author: Glenn Vincent Lopez
//May 11, 2022

#include <18F87K22.h>
#device adc=12
#FUSES WDT_SW, WDT32768, INTRC_IO, SOSC_HIGH, NOPROTECT, NOIESO, BROWNOUT, PUT, NOCPD, STVREN, NODEBUG, NOWRT, NOWRTD, NOEBTR, NOCPB, NOEBTRB, NOWRTC, NOWRTB, FCMEN, NOXINST, MCLR, RTCOSC_T1
#use delay(clock=16000000)

#use spi(MASTER, FORCE_HW, SPI1, MODE=1, BITS=8)
#use i2c(MASTER, sda=PIN_D5, scl=PIN_D6)

//#use rs232 (baud=9600, parity=e, bits=8, stop=1, xmit=PIN_E4, rcv=PIN_E7, stream=RS485_GDAT2)
#use rs232 (baud=9600, parity=e, bits=8, stop=1, xmit=PIN_E4, rcv=PIN_E7, stream=RS485_GDAT1)

#use rs232(baud=57600, xmit=PIN_G4, rcv=PIN_G3, stream=RS232_BTD200)
#use rs232 (baud=19200, xmit=PIN_C6, rcv=PIN_C7, stream=GSM)
//#use rs232(baud=57600, xmit=TX_B, rcv=RX_B, stream=RS232_BTD200)

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <arQ1010/globalVariables.h>

#include <arQ1010/pinDefinitions.c>
#include <arQ1010/interruptSerial2_USB.c>
#include <arQ1010/InterruptSerial1_GSM.c>
#include <arQ1010/interruptExt1_GSM.c>
#include <arQ1010/interruptExt2_IN2.c>
#include <arQ1010/dualColorLED.c>
#include <arQ1010/gsm.c>
#include <arQ1010/LTE.c>
#include <arQ1010/interruptTimer1.c>

#include <arQ1010/rtc.c>
#include <arQ1010/dateTimeDetails.c>
#include <arQ1010/debugmode.c>
#include <arQ1010/crc_generate.c>
//#include <arQ1010/voltageDetails.c>
#include <arQ1010/rs485.c>

#include <sensors/AS3935.c>
#include <sensors/BTD200.c>


void mcuStatusOnStartUp(){
   char strx[20] = {};
   //char *msgP1=NULL;
   char message[40];
   int cause;
   
   cause=restart_cause();
   switch (cause){
      case WDT_TIMEOUT:{ 
          strcpy(strx,"WATCHDOG TIMEOUT");
          break;
      }
      
     
      case NORMAL_POWER_UP:{  //12 after upload of firmware
         strcpy(strx,"FRESH POWER UP!");
         break;
      }
      
    
      case RESET_INSTRUCTION:{   //0
         //readDataEEprom(EEPROM_RESET_NUMBER);
         strcpy(strx, "SOFTWARE RESET-");
         //strcat(strx, funcReturnVar);
         break;
      }
      
      case MCLR_FROM_RUN:{    //15
         strcpy(strx,"HARDWARE RESET");
         break;
      }
      
      default:{
         sprintf(strx, "OTHER RESET CAUSE (%i)", cause );
      }
   }
   
   fprintf(USB,"%s\n",strx);
   //strcpy(SMS_TOBE_SENT,strx);
   getMcuDateTime();
   //sprintf(SMS_TOBE_SENT, "%s - %s", strx, getMcuDateTime()); //getDateTime()
   //sprintf(message, "%s - %s", strx, globalDT); //getDateTime()  
   sprintf(message, "%s - %s", strx, strDateTime); //getDateTime() 
   //AttemptToSend(message,MYNUMBER2);
}



void main(){
   char dbg[6]="debug";
   char reset[6]="reset";

   unsigned int8 tmp1;
   
   //int byteCount;
   char message[255]={};
   delay_ms(500);
   
   fprintf(USB, "\n*************************************************\n");
   fprintf(USB, "ULAT Lightning Monitoring Station (Beta version)\n");
  
   enable_interrupts(INT_TIMER1); // enable timer1 interrupt, through the use of the 32 khz oscillator, it will be triggered every second   
   enable_interrupts(INT_RDA);    // hardware UART interrupt, used by Radio/GSM module
   enable_interrupts(INT_RDA2);   // hardware UART interrupt, used by USB through RS232 interface via the USB port
   enable_interrupts(INT_EXT);    // B0 used by rain gauge
   enable_interrupts(INT_EXT1);   // RING OUT - will be triggered once a call or text is received by the gsm module
   //enable_interrupts(INT_EXT2);   // Interrupt 2 is triggered when there is interrupt signal from the AS3935
   enable_interrupts(GLOBAL);     // main switch for all the interrupts
 
   ext_int_edge(2,H_TO_L);         // configure INT2 state change to trigger interrupt 2  
   
   setup_wdt(WDT_ON);// enable watch dog
   //setup_wdt(WDT_128S);  
   
   setup_timer_1(T1_EXTERNAL|T1_DIV_BY_1|T1_ENABLE_SOSC); //no T1_CLK_OUT
   
   setup_spi(SPI_MASTER |SPI_L_TO_H | SPI_XMIT_H_TO_L | SPI_CLK_DIV_64);  // Working with AS3935, set as default
    
   delay_ms(500);
   //Thunder_Init();
    
   
   //igniteGSM();
   //InitGSM();                      // initialize GSM
   
   
   
   // igtLTE();
   delay_ms(2000);
  
   
   
   
   //fprintf(GSM, "init\r");
   rtc_acquire_sync();             // sync datetime from rtc chip to both GSM and MCU
   

   InterruptInput2_Flag=0;
      
   clear_InterruptUSB();

  
   //Send to server through message format

   strcpy(SERVERNUMBER,"639564334913");
   //strcpy(MYNUMBER1,"639564334913");
   strcpy(MYNUMBER2,"639564334913");
   //strcpy(MyNUMBER,"");
   SENDING_TIME = 60; //60
   
   mcuStatusOnStartUp(); // function determines cause of MCU restart
   
   //output_high(pin_BOOST1);  //5 volts
   //output_high(pin_BOOST2); //18 volts
   
   fprintf(USB, "\nInitializing BTD200\n");
   igtLTE();
   //char gsLTE;
  
   delay_ms(200);
   //clear_inputs();
   
   
   delay_ms(200);
 
   
            
   while(1){
      restart_wdt();
      readSerial();
      
         

      if (InterruptUSB_Flag==1){              // if there is a character from USB, the interrupt will trigger            
         ledConfig(LED_GREEN);
         
         InterruptUSB_Rxd[InterruptUSB_Counter-1]='\0';       
         if (!stricmp(InterruptUSB_Rxd,dbg)){ // if data from USB = debug, it will go into debug mode.
            ledConfig(LED_GREEN);
            fprintf(USB,"DEBUG MODE");
            debugDetails();
            //whilex=1;
            //debugMenu();
            //while(whilex==1){
            //if (debugDetails()){
               //whilex=0;
            //}   
         }
         
         else if (!stricmp(InterruptUSB_Rxd,reset )){ // if data from USB = debug, it will go into debug mode.
            ledConfig(LED_GREEN);
            fprintf(USB,"Reset MCU\n");
            delay_ms(50);
            reset_cpu();
         }
         clear_InterruptUSB();
      }
        
      else{ //nothing happens in otherwords no interrupts  
      
      if (SAMPLINGFLAG == TRUE){        
         ledConfig(LED_GREEN);
               
         //readRtc(0);
         //getMcuDateTime();
         //SAMPLINGFLAG = FALSE;
      }
            
      if (SYNCFLAG==TRUE){                      
         ledConfig(LED_GREEN);
         fprintf(USB,"Time Sync\n");
   
         rtc_acquire_sync();           // sync rtc datetime to both mcu and gsm 
         SYNCFLAG = FALSE;             // two minutes before reaching the next hour, this will be activated      
      }
               
      if (RESETFLAG==TRUE){                      
         ledConfig(LED_GREEN);
         //Thunder_Init();
         RESETFLAG=FALSE;
      }   
               
            
      ledToggle(LED_GREEN);
      delay_ms(500);                   
      }  
   }
}  


