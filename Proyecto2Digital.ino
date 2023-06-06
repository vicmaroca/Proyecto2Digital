#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servomotor;
Servo servomotor2;
Servo servomotor3;
Servo servomotor4;

int direccionX = A0;
int direccionY = A1;
int direccionX2 = A2;
int direccionY2 = A3;
int pulsador_guardar = 8;
int pulsador_realizar = 7;

int lecturaX;
int lecturaY;
int lectura_pulsador_guardar;
int lectura_pulsador_realizar;
int lecturaX2;
int lecturaY2;
int lectura_pulsador_guardar2;
int lectura_pulsador_realizar2;

int posicion_servo = 90;
int posicion_servo2 = 90;
int posicion_servo3 = 0;
int posicion_servo4 = 0;

const int EEPROM_ADDR_SERVO1 = 0;
const int EEPROM_ADDR_SERVO2 = 1;
const int EEPROM_ADDR_SERVO3 = 2;
const int EEPROM_ADDR_SERVO4 = 3;

volatile bool guardarPosicion = false;
volatile bool realizarAccion = false;
volatile bool guardarPosicion2 = false;
volatile bool realizarAccion2 = false;

void guardarPosiciones() {
  EEPROM.write(EEPROM_ADDR_SERVO1, posicion_servo);
  EEPROM.write(EEPROM_ADDR_SERVO2, posicion_servo2);
  EEPROM.write(EEPROM_ADDR_SERVO3, posicion_servo3);
  EEPROM.write(EEPROM_ADDR_SERVO4, posicion_servo4);
}

void handleGuardarPosicion() {
  guardarPosicion = true;
}

void handleRealizarAccion() {
  realizarAccion = true;
}

void setup() {
  // Configurar comunicación serial
  Serial.begin(9600);

  servomotor.attach(2);
  servomotor2.attach(3);
  servomotor3.attach(4);
  servomotor4.attach(5);

  pinMode(direccionX, INPUT);
  pinMode(direccionY, INPUT);
  pinMode(direccionX2, INPUT);
  pinMode(direccionY2, INPUT);

  pinMode(pulsador_guardar, INPUT_PULLUP);
  pinMode(pulsador_realizar, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pulsador_guardar), handleGuardarPosicion, FALLING);
  attachInterrupt(digitalPinToInterrupt(pulsador_realizar), handleRealizarAccion, FALLING);

  restaurarPosiciones();
  servomotor.write(posicion_servo);
  servomotor2.write(posicion_servo2);
  servomotor3.write(posicion_servo3);
  servomotor4.write(posicion_servo4);
}

void loop() {
  lecturaX = analogRead(direccionX);
  lecturaY = analogRead(direccionY);
  lecturaX2 = analogRead(direccionX2);
  lecturaY2 = analogRead(direccionY2);

  lectura_pulsador_guardar = digitalRead(pulsador_guardar);
  lectura_pulsador_realizar = digitalRead(pulsador_realizar);

  if (lecturaX >= 550) {
    posicion_servo++;

    if (posicion_servo > 180) {
      posicion_servo = 180;
    }
  }
  if (lecturaX2 >= 550) {
    posicion_servo3++;

    if (posicion_servo3 > 180) {
      posicion_servo3 = 180;
    }
  }

  if (lecturaX <= 450) {
    posicion_servo--;

    if (posicion_servo < 0) {
      posicion_servo = 0;
    }
  }
  if (lecturaX2 <= 450) {
    posicion_servo3--;

    if (posicion_servo3 < 0) {
      posicion_servo3 = 0;
    }
  }

  if (lectura_pulsador_guardar == LOW) {
    guardarPosiciones();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("guardando         ");
    lcd.setCursor(0, 1);
    lcd.print("           ");
    delay(1000);  // Espera un segundo para evitar guardar múltiples veces
  }

  servomotor.write(posicion_servo);
  servomotor3.write(posicion_servo3);

  if (lecturaY >= 550) {
    posicion_servo2++;

    if (posicion_servo2 > 180) {
      posicion_servo2 = 180;
    }
  }
  if (lecturaY2 >= 550) {
    posicion_servo4++;

    if (posicion_servo4 > 180) {
      posicion_servo4 = 180;
    }
  }

  if (lecturaY <= 450) {
    posicion_servo2--;

    if (posicion_servo2 < 0) {
      posicion_servo2 = 0;
    }
  }
  if (lecturaY2 <= 450) {
    posicion_servo4--;

    if (posicion_servo4 < 0) {
      posicion_servo4 = 0;
    }
  }

  if (lectura_pulsador_realizar == LOW) {
    restaurarPosiciones();
    servomotor.write(posicion_servo);
    servomotor2.write(posicion_servo2);
    servomotor3.write(posicion_servo3);
    servomotor4.write(posicion_servo4);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Reproduciendo         ");
    lcd.setCursor(0, 1);
    lcd.print("           ");
    delay(1000);  // Espera un segundo para evitar guardar múltiples veces
  }

  servomotor2.write(posicion_servo2);
  servomotor4.write(posicion_servo4);

  // Enviar datos por comunicación serial
  Serial.print("S1:");
  Serial.print(posicion_servo);
  Serial.print(",");
  Serial.print("S2:");
  Serial.print(posicion_servo2);
  Serial.print(",");
  Serial.print("S3:");
  Serial.print(posicion_servo3);
  Serial.print(",");
  Serial.print("S4:");
  Serial.println(posicion_servo4);

  delay(10);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Brazo robotico ");
  lcd.setCursor(0, 1);
  lcd.print("           ");
}
