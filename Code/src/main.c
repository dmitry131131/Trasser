#include "utils.h"
#include "init.h"
#include "custom_delay.h"

// Enum с режимами работы насадки
volatile enum Mode {
    ONLY_INSIDE,
    ONLY_OUTSIDE,
    INSIDE_AND_OUTSIDE,
} global_mode = ONLY_INSIDE;

volatile uint16_t wdt_count = 0;
const uint16_t target = 225;  // 450 * 8 сек = 3600 сек (1 час)
volatile uint8_t active = 1;

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

    wdt_count = 0;  // Сброс таймера на сон

    if (!active) {
        active = 1;
        Clean_ANA_Comp_Flag();
        IR_LED_On();
        Enable_ANA_Comp_Interrupt();
    }
    else {
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
    }
    small_outside_blink();
}

// Прерывание по компаратору
// Срабатывание датчика 
ISR(ANA_COMP_vect) {
    // Отключение прерывания компаратора
    Disable_ANA_Comp_Interrupt();
    wdt_count = 0;  // Сброс таймера на сон

    blink_led(global_mode);

    Clean_ANA_Comp_Flag();
    Enable_ANA_Comp_Interrupt();
}

// Прерывание по срабатыванию сторожевого таймера
ISR(WDT_vect) {
    wdt_count++;
    if (wdt_count >= target) {
        wdt_count = 0;  // Сброс таймера на сон
        active = 0;     // Отключение МК
        Disable_ANA_Comp_Interrupt();
        IR_LED_Off();
    }
    Enable_WDT_Interrupt(); // Очень важно заново активировать прерывания от WDT, иначе в следующий раз будет выполнен сброс
}

int main(void) {
    // Инициализация периферии
    PORT_B_Init();
    WDT_Init();
    ANA_Comp_Init();
    Timer_Init();
    PCINT0_Interrupt_Init();

    // Разрешение глобальных прерываний
    sei();

    // Основной цикл программы
    while (1) {}

    return 0;
}
