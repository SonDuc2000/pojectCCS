#include <18F4431.h>
#device ADC=10

#FUSES HS
#FUSES NOWDT                    //No Watch Dog Timer
#FUSES WDT128                   //Watch Dog Timer uses 1:128 Postscale
#FUSES NOBROWNOUT               //No brownout reset
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O

#use delay(crystal=20000000)
#use RS232(UART1,baud=9600,xmit=pin_c6,rcv=pin_c7,parity=N,bits=8,stream=DEBUG_PORT,errors)//Khai bao su dung uart cung
#use i2c(Master,Fast,sda=PIN_D2,scl=PIN_D3)

unsigned INT16 max_cb[6] = {10,10,10,10,10,10};
unsigned INT16 min_cb[6] = {1023,1023,1023,1023,1023,1023};
unsigned INT32 MAX[6] = {0,0,0,0,0,0};
unsigned INT32 KQ_MAX[6] = {0,0,0,0,0,0};
unsigned INT8 w[6] = {1,2,4,8,16,32};
unsigned INT32 ADC[6] = {0,0,0,0,0,0};
unsigned INT32 L[6] = {0,0,0,0,0,0};
unsigned CHAR sensor = 0x000000;
int8 error_sensor = 0, pre_sensor;

void doc_adc(VOID)
{
   set_adc_channel (5) ;
   delay_us (20) ;
   L[0] = read_adc ();
   
   set_adc_channel (4) ;
   delay_us (20) ;
   L[1] = read_adc ();
   
   set_adc_channel (3) ;
   delay_us (20) ;
   L[2] = read_adc ();
   
   set_adc_channel (2) ;
   delay_us (20) ;
   L[3] = read_adc ();
   
   set_adc_channel (1) ;
   delay_us (20) ;
   L[4] = read_adc ();
   
   set_adc_channel (0) ;
   delay_us (20) ;
   L[5] = read_adc ();
}

void luu_eeprom(VOID)
{
   INT i;
   FOR (i = 0; i < 6; i++)
   {
      write_eeprom (i, ADC[i]);
      delay_ms (20) ;
   }
}

void TB_ADC()
{
   UNSIGNED int8 i = 0, j = 0, n = 0;
   fprintf (DEBUG_PORT, "Bat dau lay mau\r") ;
   FOR (n = 0; n < 100; n++)
   {
      FOR (j = 0; j < 100; j++)
      {
         doc_adc () ;
         FOR (i = 0; i < 6; i++)
         {
            IF (max_cb[0] <= L[0])
               max_cb[0] = L[0];

            IF (min_cb[0] >= L[0])
               min_cb[0] = L[0];

            IF (max_cb[1] <= L[1])
               max_cb[1] = L[1];

            IF (min_cb[1] >= L[1])
               min_cb[1] = L[1];

            IF (max_cb[2] <= L[2])
               max_cb[2] = L[2];

            IF (min_cb[2] >= L[2])
               min_cb[2] = L[2];

            IF (max_cb[3] <= L[3])
               max_cb[3] = L[3];

            IF (min_cb[3] >= L[3])
               min_cb[3] = L[3];

            IF (max_cb[4] <= L[4])
               max_cb[4] = L[4];

            IF (min_cb[4] >= L[4])
               min_cb[4] = L[4];

            IF (max_cb[5] <= L[5])
               max_cb[5] = L[5];

            IF (min_cb[5] >= L[5])
               min_cb[5] = L[5];
         }
      }

      MAX[0] += max_cb[0];
      MAX[1] += max_cb[1];
      MAX[2] += max_cb[2];
      MAX[3] += max_cb[3];
      MAX[4] += max_cb[4];
      MAX[5] += max_cb[5];
   }

   FOR (i = 0; i < 6; i++)
   {
      KQ_MAX[i] = MAX[i] / 200;
      MAX[i] = 0;
   }

   FOR (i = 0; i < 6; i++)
   {
      ADC[i] = (KQ_MAX[i] + min_cb[i]) / 2;
   }

   luu_eeprom () ;
}

void so_sanh_adc()
{
   
   INT8 i;
   sensor = 0x00;
   FOR (i = 0; i < 6; i++)
   {
      doc_adc ();
      ADC[i] = read_eeprom (i);

      IF ( (ADC[i]) < L[i])
      {
         CHAR t;
         FOR (t = 0; t < 5; t++)
         {
            delay_ms (1);
            IF ( (ADC[i]) >= L[i]) goto exit_t;
         }

         sensor|= w[5 - i];
         exit_t: ;
      }ELSE

      sensor|= 0;
   }

}

void read_sensor(VOID)
{
   so_sanh_adc ();
   
   IF (sensor == 1)
   {
      error_sensor = -5;
      pre_sensor = 1;
   }else

   IF (sensor == 3)
      error_sensor = -4;
   else
   IF (sensor == 2)
      error_sensor = -3;
   else
   IF (sensor == 6)
      error_sensor = -2;
   else
   IF (sensor == 4)
      error_sensor = -1;
   else
   IF (sensor == 12)
      error_sensor = 0;
   else
   IF (sensor == 8)
      error_sensor = 1;
   else
   IF (sensor == 24)
      error_sensor = 2;
   else
   IF (sensor == 16)
      error_sensor = 3;
   else
   IF (sensor == 48)
      error_sensor = 4;
   else
   IF (sensor == 32)
   {
      error_sensor = 5;
      pre_sensor = -1;
   }
   else
   IF (sensor == 0&&pre_sensor == 1)
      error_sensor = -6;
   else
   IF (sensor == 0&&pre_sensor == - 1)
      error_sensor = 6;
}
