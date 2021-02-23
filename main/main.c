
#include <motor_drive.h>
#define PWM_MAX 1023;
#define PWM_MIN 0;

#define START        PIN_B1
#define RUN          PIN_B0

void hien_thi(int1 a, int1 b, int1 c)
{
   output_bit(PIN_B2, a);
   output_bit(PIN_B3, b);
   output_bit(PIN_B4, c);
}

void run_method()
{
   while(input_state(RUN) == 0)
   {   
         putc(sensor);
         PID_control();   
         fprintf(DEBUG_PORT, "\nPID: %0.2f - %d", PWM_R, error_sensor);
   }
}

void get_adc()
{
         hien_thi(1,0,0);
         TB_ADC();
         hien_thi(1,1,1);
         delay_ms(100);  
}



void main()
{
   setup_spi (SPI_MASTER|SPI_L_TO_H|SPI_CLK_DIV_4) ;

   //Ham cai dat adc
   
   enable_interrupts (INT_EXT); //ngat ngoai 0
   enable_interrupts (INT_EXT1); //ngat ngoai 1
   enable_interrupts (GLOBAL); //Ngat toan cuc
   
   setup_timer_1 (T1_INTERNAL|T1_DIV_BY_8); //1.6us interrupts
   set_timer1 (49911); // 25ms overflow
   enable_interrupts (INT_TIMER1);
   
   setup_timer_2 (T2_DIV_BY_4, 124, 1); //819 us overflow, 819 us interrupt 1, 221Hz
   setup_adc (ADC_CLOCK_DIV_2);
   setup_adc_ports (ALL_ANALOG);
   setup_ccp1 (CCP_PWM);
   setup_ccp2 (CCP_PWM);

   set_tris_a (0x7f);
   //output_high (PIN_B2);
   //set_pwm1_duty ( (INT16) 0); //duty 50 % max 1023 min 1 // xe chay khong duoc 1 vong / p
   //set_pwm2_duty ( (INT8) 216) ;
   output_low(PIN_D5);
   output_low(PIN_D6);
   
   WHILE (true)
   {
      //set_pwm1_duty((int16)200);
      //set_pwm2_duty(0);
      hien_thi(0,0,0);
      read_sensor();
      //Lap trinh cho nut nhan reset
      
      while(input_state(RUN) == 0)
      {
         hien_thi(0,0,1);
         
         run_method();
      }
      
      if(input_state(START) == 0)
      {
         get_adc();
      }
      
   }
}
