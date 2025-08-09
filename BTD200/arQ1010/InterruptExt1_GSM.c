
#int_ext1
void ext_isr1(void)
{
   EXT_FLAG=1;
   strcpy(SMS_RCV,InterruptGSM_Rxd);
}
