#include <main.h>
#include <lcd.h>

void main()
{
   setup_timer_2(T2_DIV_BY_16,255,1);      //819 us overflow, 819 us interrupt
   
   lcd_init();
   lcd_clear();

   while(TRUE)
   {
      lcd_gotoxy(1,1);
      printf(lcd_putc, "Hello");
   }

}
