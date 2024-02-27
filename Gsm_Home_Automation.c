#include<Gsm_Home_Automation.h>

void main()
{
   sys_init();
   while(TRUE)
   {
      if(flag == 1) {
         delay_ms(7000);
         flag = 0;
         check_command();
      }
   }
}

