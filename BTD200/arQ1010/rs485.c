#define RS485_DE PIN_E5
#define RS485_RE PIN_E6

void disable485(){
   output_high(RS485_RE);
   output_low(RS485_DE);
}

void writeToRS485(){
   output_high(RS485_RE);
   output_high(RS485_DE);
}
   
void readFromRS485(){
   output_low(RS485_RE);
   output_low(RS485_DE);
}

