#include <Servo.h>
#include <EEPROM.h>

#define EEPROM_ADDR 0   // Dirección base de la EEPROM
#define SERVO_PIN 9     // Pin de control del servomotor

Servo myservo;          // Objeto de la biblioteca Servo
int pos = 0;            // Variable para almacenar la posición actual del servomotor

void setup() {
  myservo.attach(SERVO_PIN);   // Inicializar el servomotor en el pin 9
  myservo.write(0);            // Establecer la posición inicial a 0 grados
  delay(1000);                 // Esperar un segundo

  int storedPos = EEPROM.read(EEPROM_ADDR);   // Leer la posición almacenada en la EEPROM

  if (storedPos >= 0 && storedPos <= 180) {    // Verificar si la posición almacenada es válida
    myservo.write(storedPos);                  // Mover el servomotor a la posición almacenada
    delay(1000);                               // Esperar un segundo
  }
}

void loop() {
  for (pos = 0; pos <= 180; pos += 45) {        // Mover el servomotor en incrementos de 45 grados
    myservo.write(pos);                         // Establecer la nueva posición
    delay(1000);                                // Esperar un segundo

    EEPROM.write(EEPROM_ADDR, pos);             // Guardar la posición actual en la EEPROM
    delay(10);                                  // Esperar para asegurar que se escriba en la EEPROM correctamente
  }
}
