#define LED_OFF 0
#define LED_GREEN 1
#define LED_RED 2

void ledConfig(int mode)
{
   switch (mode) {
      case LED_RED:
         output_high(LED_STAT_A);
         output_low(LED_STAT_B);
         break;
      case LED_GREEN:
         output_low(LED_STAT_A);
         output_high(LED_STAT_B);
         //output_low(LED_STAT_B);
         break;
      default:
         output_low(LED_STAT_A);
         output_low(LED_STAT_B);
         break;
   }
}

void ledToggle(int1 color)
{
   static int1 toggle=0;
   if (toggle==0)
   {
      if (color == LED_GREEN)
         ledConfig(LED_GREEN);
      else if (color == LED_RED)
         ledConfig(LED_RED);
      toggle=1;
   }
   else
   {
      ledConfig(LED_OFF);
      toggle=0;
   }
}
