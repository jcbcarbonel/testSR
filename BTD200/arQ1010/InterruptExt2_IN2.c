int1 InterruptInput2_Flag = 0;

#int_ext2 //when interrupt is enables on the main code the code is redirected on this include file
void ext_isr2(void)
{//if (input(PIN_B0))
//fprintf(USB, "INTEEERRRRRUPPTTT"); 
   {
     InterruptInput2_Flag=1;
     //fprintf(USB, "Interrupt!\n");
   }
}


