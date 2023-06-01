#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#define EEPROM_ADDR 0x00     // Dirección base de la EEPROM
#define SERVO_PIN PB1        // Pin de control del servomotor
#define POT_PIN PC5        // Pin de lectura del potenciómetro
#define BUTTON_A_PIN PB2     // Pin del botón A
#define BUTTON_B_PIN PB3     // Pin del botón B

void servoWrite(uint8_t angle) {
    OCR1A = (angle * 19) / 2 + 600;    // Cálculo del valor del registro de comparación para generar el pulso PWM
    _delay_ms(15);                     // Esperar para asegurar el movimiento del servomotor
}

void servoSavePosition(uint8_t position) {
    eeprom_update_byte((uint8_t*)EEPROM_ADDR, position);   // Guardar la posición actual en la EEPROM
    _delay_ms(10);                                         // Esperar para asegurar que se escriba en la EEPROM correctamente
}

uint8_t servoLoadPosition() {
    return eeprom_read_byte((uint8_t*)EEPROM_ADDR);   // Leer la posición almacenada en la EEPROM
}

void adcInit() {
    ADMUX |= (1 << REFS0);                   // Establecer la referencia de voltaje en AVCC
    ADCSRA |= (1 << ADEN) | (1 << ADPS2);    // Habilitar el ADC y establecer el prescaler en 16
}

uint16_t adcRead(uint8_t channel) {
    ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);      // Seleccionar el canal de entrada del ADC
    ADCSRA |= (1 << ADSC);                           // Iniciar la conversión ADC
    while (ADCSRA & (1 << ADSC));                     // Esperar a que la conversión ADC se complete
    return ADC;                                      // Devolver el valor convertido
}

void buttonInit() {
    DDRB &= ~((1 << BUTTON_A_PIN) | (1 << BUTTON_B_PIN));    // Configurar los pines de los botones como entradas
    PORTB |= (1 << BUTTON_A_PIN) | (1 << BUTTON_B_PIN);      // Activar las resistencias pull-up internas de los botones
}

int buttonAPressed() {
    if (!(PINB & (1 << BUTTON_A_PIN))) {    // Verificar si el botón A está presionado
        _delay_ms(10);                         // Debounce: esperar un corto tiempo para evitar rebotes
        if (!(PINB & (1 << BUTTON_A_PIN))) {  // Verificar nuevamente si el botón A sigue presionado
            return 1;                           // Indicar que el botón A ha sido presionado
        }
    }
    return 0;                             // Indicar que el botón A no ha sido presionado
}

int buttonBPressed() {
    if (!(PINB & (1 << BUTTON_B_PIN))) {    // Verificar si el botón B está presionado
        _delay_ms(10);                         // Debounce: esperar un corto tiempo para evitar rebotes
        if (!(PINB & (1 << BUTTON_B_PIN))) {  // Verificar nuevamente si el botón B sigue presionado
            return 1;                           // Indicar que el botón B ha sido presionado
        }
    }
    return 0;                             // Indicar que el botón B no ha sido presionado
}

int main(void) {
    DDRB |= (1 << SERVO_PIN);                // Configurar el pin del servomotor como salida
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);   // Configurar el modo de operación del temporizador en modo Fast PWM de 10 bits
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);   // Configurar el prescaler del temporizador en 8
    ICR1 = 19999;                             // Establecer el valor máximo del temporizador para obtener una frecuencia de PWM de 50 Hz

    adcInit();                                // Inicializar el ADC
    buttonInit();                             // Inicializar los botones

    uint8_t storedPos = servoLoadPosition();   // Leer la posición almacenada en la EEPROM

    if (storedPos >= 0 && storedPos <= 180) {   // Verificar si la posición almacenada es válida
        servoWrite(storedPos);                  // Mover el servomotor a la posición almacenada
        _delay_ms(1000);                         // Esperar un segundo
    }

    uint8_t angle = 0;
    uint8_t previousAngle = 0;

    while (1) {
        if (buttonAPressed()) {                   // Verificar si el botón A está presionado
            servoWrite(previousAngle);                // Mover el servomotor a la posición anterior
        } else if (buttonBPressed()) {            // Verificar si el botón B está presionado
            angle = adcRead(POT_PIN) / 4;             // Leer el valor del potenciómetro y convertirlo a un ángulo entre 0 y 180
            servoSavePosition(angle);                  // Guardar la posición actual en la EEPROM
            previousAngle = angle;                      // Actualizar la posición anterior con la posición actual
            servoWrite(angle);                           // Mover el servomotor al ángulo calculado
            _delay_ms(10);                                 // Esperar para asegurar que se escriba en la EEPROM correctamente
        } else {
            angle = adcRead(POT_PIN) / 4;             // Leer el valor del potenciómetro y convertirlo a un ángulo entre 0 y 180
            servoWrite(angle);                           // Mover el servomotor al ángulo calculado
        }
    }
}
