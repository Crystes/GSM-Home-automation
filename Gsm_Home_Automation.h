#include<16f876a.h>
#include<string.h>

#use delay(crystal = 4M)
#use fast_io(ALL)
#use rs232(UART)

#fuses XT,NOWDT,PUT,NOBROWNOUT,NOLVP,DEBUG,PROTECT

#define pwr_led   pin_b0
#define fan       pin_b1
#define ac        pin_b2
#define fan_load  1
#define ac_load   2

unsigned int8 x,flag = 0;
char rcv,buffer[25];
char *ptr;

#INT_RDA
void isr_rda()
{
   flag = 1;
   rcv = getc();
   if(rcv != '\0') {
      buffer[x] = rcv;
      x++;
   }
   if(x>=24) x=0;
}


void sys_init() {
   output_a(0);
   output_b(0);
   output_c(0);
   set_tris_a(0b00000000);
   set_tris_b(0b00000000);
   set_tris_c(0b10000000);
   output_a(0);
   output_b(0);
   output_c(0);
   
   setup_uart(9600);
   
   enable_interrupts(int_rda);
   enable_interrupts(global);
   clear_interrupt(int_rda);
   
   output_high(pwr_led);
   
   puts("System Initialised");
}

void turn_on(unsigned int8 appliance) {
   switch (appliance) { 
      case 1: output_high(fan);
      break;
      case 2: output_high(ac);
      break;
   }
}

void turn_off(unsigned int8 appliance) {
   switch (appliance) { 
      case 1: output_low(fan);
      break;
      case 2: output_low(ac);
      break;
   }
}

void clear_buffer() {
   unsigned int8 i;
   for(i=0;i<(sizeof(buffer));i++) {
      buffer[i] = '\0';
   }
   x = 0;
}

//void check_command() {
//   disable_interrupts(int_rda);
//   char on_fan[7] = "on_fan";
//   char off_fan[8] = "off_fan";
//   ptr = strstr(buffer,on_fan);
//   if(ptr) {
//      turn_on();
//      puts("Switched on");
//      enable_interrupts(int_rda);
//      clear_buffer();
//      return;
//   }
//   else {
//      ptr = strstr(buffer,off_fan);
//      if(ptr) {
//         turn_off();
//         puts("Switched off");
//      }
//      else puts("wrong command");
//   }
//   enable_interrupts(int_rda);
//   clear_buffer();
//}

void check_command() {
   disable_interrupts(int_rda);
   char checker[10];
   strcpy(checker,"on_fan");
   ptr = strstr(buffer,checker);
   if(ptr) {
      turn_on(fan_load);
      puts("Fan Switched on");
      enable_interrupts(int_rda);
      clear_buffer();
      return;
   }
   
   strcpy(checker,"off_fan");
   ptr = strstr(buffer,checker);
   if(ptr) {
      turn_off(fan_load);
      puts("Fan Switched off");
      enable_interrupts(int_rda);
      clear_buffer();
      return;
   }
   
   strcpy(checker,"on_ac");
   ptr = strstr(buffer,checker);
   if(ptr) {
      turn_on(ac_load);
      puts("AC SWITCHED ON");
      enable_interrupts(int_rda);
      clear_buffer();
      return;
   }
   
   strcpy(checker,"off_ac");
   ptr = strstr(buffer,checker);
   if(ptr) {
      turn_off(ac_load);
      puts("AC SWITCHED OFF");
      enable_interrupts(int_rda);
      clear_buffer();
      return;
   }
   
   puts("wrong command");
   enable_interrupts(int_rda);
   clear_buffer();
}
