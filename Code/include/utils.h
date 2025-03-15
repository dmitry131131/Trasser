// File with common functions and macro for project
#pragma once

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

void small_outside_blink();

// ANA_COMP
#define Enable_ANA_Comp_Interrupt()  ACSR |= (1 << ACIE);
#define Disable_ANA_Comp_Interrupt() ACSR &= ~(1 << ACIE);
#define Clean_ANA_Comp_Flag()        ACSR |= (1 << ACI);

//Timer 0
#define Enable_Timer_A_Interrupt()         TIMSK0 |= (1 << OCIE0A);
#define Disable_Timer_A_Interrupt()        TIMSK0 &= ~(1 << OCIE0A);
#define Set_Timer_A_Limit(limit)           OCR0A = limit;
#define Enable_Timer_B_Interrupt()         TIMSK0 |= (1 << OCIE0B);
#define Disable_Timer_B_Interrupt()        TIMSK0 &= ~(1 << OCIE0B);
#define Set_Timer_B_Limit(limit)           OCR0B = limit;
#define Enable_Timer_Overflow_Interrupt()  TIMSK0 |= (1 << TOIE0);
#define Disable_Timer_Overflow_Interrupt() TIMSK0 &= ~(1 << TOIE0);
#define Clean_Timer_A_Flag()               TIFR0 |= (1 << OCF0A);
#define Clean_Timer_B_Flag()               TIFR0 |= (1 << OCF0B);
#define Clean_Timer_Overflow_Flag()        TIFR0 |= (1 << TOV0);
#define Cleanup_Timer()                    TCNT0 = 0x0;

// PCINT

#define Enable_PCINT_Interrupt()  GIMSK |= (1 << PCIE);   // Разрешение прерываний PCINT0
#define Disable_PCINT_Interrupt() GIMSK &= ~(1 << PCIE);

// LED
#define Inside_LED_On()   PORTB |= (1 << PB4);
#define Inside_LED_Off()  PORTB &= ~(1 << PB4);
#define Outside_LED_On()  PORTB |= (1 << PB3);
#define Outside_LED_Off() PORTB &= ~(1 << PB3);