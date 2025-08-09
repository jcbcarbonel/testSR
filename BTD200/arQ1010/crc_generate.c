
int16 crc_generator(int8 *data, int16 len){
   int16 i;
   int16 crc = 0xFFFF;
   char crc_string[6];
   
   while(len--){
      i = make8(crc,1) ^ *data++;
      i ^= i>>4;
      crc = (crc << 8) ^ (i << 12) ^ (i <<5) ^ i;
   }
   //fprintf(PC,"CRC: %LX\n",crc);
   //sprintf(convto_string, "<%lu>", crc);
   itoa(crc,16,crc_string);
   //strcpy(funcReturnVar,crc_string);
   //fprintf(PC,"string: %lu\n",crc);

   return crc;
}
