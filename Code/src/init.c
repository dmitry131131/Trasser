#include "utils.h"
#include "init.h"

// Функция инициализации портов ввода/вывода
void PORT_B_Init() {
    // Настройка порта B
    DDRB |= (1 << DDB4) | (1 << DDB3); // Установка PB4 как выход
    PORTB |= (1 << PB0) | (1 << PB2);  // Установка 1 в PB0 и в PB2
}

// Функция инициализации таймера
void Timer_Init() {
    // Настройка таймера/счетчика0
    TCCR0B |= (1 << CS02) | (1 << CS00); // Предделитель 1024
    TCCR0A |= (1 << WGM01);              // CTC режим
    //OCR0A = 0xff;                        // Загрузка порогового значения
    //TIMSK0 |= (1 << OCIE0A);             // Разрешение прерывания по переполнению OCIE0A
}

// Настройка аналогового компаратора
void ANA_Comp_Init() {
    ACSR |= (1 << ACIS1);
    ACSR &= ~(1 << ACBG);
    Enable_ANA_Comp_Interrupt();
}

// Настройка прерывания PCINT (для кнопки)
void PCINT0_Interrupt_Init() {
    PCMSK |= (1 << PCINT2); // Разрешение прерывания PCINT2
    GIMSK |= (1 << PCIE);   // Разрешение прерываний PCINT0
}