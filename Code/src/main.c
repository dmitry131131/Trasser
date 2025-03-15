#include "utils.h"
#include "init.h"
#include "custom_delay.h"

// Enum с режимами работы насадки
volatile enum Mode {
    ONLY_INSIDE,
    ONLY_OUTSIDE,
    INSIDE_AND_OUTSIDE,
} global_mode = ONLY_INSIDE;

// Функция генерации вспышки от выстрела
void blink_led(enum Mode mode) {
    switch (mode)
    {
    case ONLY_INSIDE:
        Inside_LED_On();
        break;
    case ONLY_OUTSIDE:
        Outside_LED_On();
        break;
    case INSIDE_AND_OUTSIDE:
        Inside_LED_On();
        Outside_LED_On();
    break;
    }

    delay_x_0_1_sec(1);

    switch (mode)
    {
    case ONLY_INSIDE:
        Inside_LED_Off();
        break;
    case ONLY_OUTSIDE:
        Outside_LED_Off();
        break;
    case INSIDE_AND_OUTSIDE:
        Inside_LED_Off();
        Outside_LED_Off();
    break;
    }
}

// Прерывание по изменению уровня на PCINT2
// Переключение режима работы насадки
ISR(PCINT0_vect) {
    if (PINB & (1 << PB2)) return;

    delay_x_0_1_sec(6);

    if (PINB & (1 << PB2)) return;

    switch (global_mode)
    {
    case ONLY_INSIDE:
        global_mode = ONLY_OUTSIDE;
        break;
    case ONLY_OUTSIDE:
        global_mode = INSIDE_AND_OUTSIDE;
        break;
    case INSIDE_AND_OUTSIDE:
        global_mode = ONLY_INSIDE;
        break;
    }
    small_outside_blink();
}

// Прерывание по компаратору
// Срабатывание датчика 
ISR(ANA_COMP_vect) {
    // Отключение прерывания компаратора
    Disable_ANA_Comp_Interrupt();

    blink_led(global_mode);

    Clean_ANA_Comp_Flag();
    Enable_ANA_Comp_Interrupt();
}

int main(void) {
    // Инициализация периферии
    PORT_B_Init();
    ANA_Comp_Init();
    Timer_Init();
    PCINT0_Interrupt_Init();

    // Разрешение глобальных прерываний
    sei();

    // Основной цикл программы
    while (1) {}

    return 0;
}
