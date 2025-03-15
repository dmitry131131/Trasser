#include "utils.h"
#include "custom_delay.h"

volatile int timer_counter = 0;
// Delay in 0,1s
static void custom_delay_0_1_sec() {
    Disable_ANA_Comp_Interrupt();        
    Disable_PCINT_Interrupt();           
                                
    Enable_Timer_A_Interrupt();   
    Clean_Timer_A_Flag();
    Set_Timer_A_Limit(97);
    Cleanup_Timer();
    timer_counter = 0;
    sei();      
                                
    while (!timer_counter) {}
                                
    cli();                               
    Disable_Timer_A_Interrupt();  
                                
    Enable_ANA_Comp_Interrupt();         
    Enable_PCINT_Interrupt();            
}    

void delay_x_0_1_sec(int delay) {
    for (int i = 0; i < delay; i++) custom_delay_0_1_sec();
}

ISR(TIM0_COMPA_vect) {
    timer_counter++;
}