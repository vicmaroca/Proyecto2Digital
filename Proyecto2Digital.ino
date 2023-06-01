#include <Servo.h>
#include <EEPROM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo servomotor;
Servo servomotor2;
Servo servomotor3;
Servo servomotor4;

int direccionX = A0;  // Pin analógico A0
int direccionY = A1;  // Pin analógico A1
int direccionX2 = A2; // Pin analógico A2
int direccionY2 = A3; // Pin analógico A3

int pulsador_guardar = 8;
int pulsador_realizar = PD7;
int pulsador_nuevo = PD6;
int LadoaOtrob = 9;


int lecturaX;
int lecturaY;
int lectura_pulsador_guardar;
int lectura_pulsador_realizar;
int lectura_pulsador_nuevo;
int lectura_lado_a_otros;
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

const int EEPROM_ADDR_SERVO5 = 4;
const int EEPROM_ADDR_SERVO6 = 5;
const int EEPROM_ADDR_SERVO7 = 6;
const int EEPROM_ADDR_SERVO8 = 7;
const int EEPROM_ADDR_SERVO9 = 8;

const int EEPROM_ADDR_SERVO10 = 9;
const int EEPROM_ADDR_SERVO11 = 10;
const int EEPROM_ADDR_SERVO12 = 11;
const int EEPROM_ADDR_SERVO13 = 12;
const int EEPROM_ADDR_SERVO14 = 13;
const int EEPROM_ADDR_SERVO15 = 14;
const int EEPROM_ADDR_SERVO16 = 15;
const int EEPROM_ADDR_SERVO17 = 16;



volatile bool guardarPosicion = false;
volatile bool realizarAccion = false;
volatile bool realizarAccionNuevo = false;
volatile bool guardarPosicion2 = false;
volatile bool realizarAccion2 = false;
volatile bool LadoaOtro = false;


int posicion__servo = 90;
int posicion__servo2 = 90;
int posicion__servo3 = 0;
int posicion__servo4 = 0;
void guardarPosiciones()
{
  EEPROM.write(EEPROM_ADDR_SERVO1, posicion_servo);
  EEPROM.write(EEPROM_ADDR_SERVO2, posicion_servo2);
  EEPROM.write(EEPROM_ADDR_SERVO3, posicion_servo3);
  EEPROM.write(EEPROM_ADDR_SERVO4, posicion_servo4);
  
}
void guardarPosicionesCerrada()
{
  EEPROM.write(EEPROM_ADDR_SERVO5,posicion_servo= 0);
  EEPROM.write(EEPROM_ADDR_SERVO6, posicion_servo2=90);
  EEPROM.write(EEPROM_ADDR_SERVO7, posicion_servo3=90);
  EEPROM.write(EEPROM_ADDR_SERVO8, posicion_servo4=90);
  posicion_servo = EEPROM.read(EEPROM_ADDR_SERVO5);
  posicion_servo2 = EEPROM.read(EEPROM_ADDR_SERVO6);
  posicion_servo3 = EEPROM.read(EEPROM_ADDR_SERVO7);
  posicion_servo4 = EEPROM.read(EEPROM_ADDR_SERVO8);
  
}
void restaurarPosiciones()
{
  posicion_servo = EEPROM.read(EEPROM_ADDR_SERVO1);
  posicion_servo2 = EEPROM.read(EEPROM_ADDR_SERVO2);
  posicion_servo3 = EEPROM.read(EEPROM_ADDR_SERVO3);
  posicion_servo4 = EEPROM.read(EEPROM_ADDR_SERVO4);

  lcd.init();
  lcd.init();
}
void Lado_a_Otro1()
{
  EEPROM.write(EEPROM_ADDR_SERVO10,posicion_servo= 0);
  EEPROM.write(EEPROM_ADDR_SERVO11, posicion_servo2=90);
  EEPROM.write(EEPROM_ADDR_SERVO12, posicion_servo3=90);
  EEPROM.write(EEPROM_ADDR_SERVO13, posicion_servo4=0);

  posicion_servo = EEPROM.read(EEPROM_ADDR_SERVO10);
  posicion_servo2 = EEPROM.read(EEPROM_ADDR_SERVO11);
  posicion_servo3 = EEPROM.read(EEPROM_ADDR_SERVO12);
  posicion_servo4 = EEPROM.read(EEPROM_ADDR_SERVO13);

  delay(1000);
}
void Lado_a_Otro2()
{

  EEPROM.write(EEPROM_ADDR_SERVO14,posicion_servo= 90);
  EEPROM.write(EEPROM_ADDR_SERVO15, posicion_servo2=0);
  EEPROM.write(EEPROM_ADDR_SERVO16, posicion_servo3=0);
  EEPROM.write(EEPROM_ADDR_SERVO17, posicion_servo4=90);
 
  posicion_servo = EEPROM.read(EEPROM_ADDR_SERVO14);
  posicion_servo2 = EEPROM.read(EEPROM_ADDR_SERVO15);
  posicion_servo3 = EEPROM.read(EEPROM_ADDR_SERVO16);
  posicion_servo4 = EEPROM.read(EEPROM_ADDR_SERVO17);
  delay(1000);
}
void handleGuardarPosicion()
{
  guardarPosicion = true;
}

void handleRealizarAccion()
{
  realizarAccion = true;
}

void handleRealizarAccionNuevo()
{
  realizarAccionNuevo = true;
}

void setup()
{
  servomotor.attach(PD2);
  servomotor2.attach(PD3);
  servomotor3.attach(PD4);
  servomotor4.attach(PD5);

  pinMode(direccionX, INPUT_PULLUP);
  pinMode(direccionY, INPUT_PULLUP);
  pinMode(direccionX2, INPUT_PULLUP);
  pinMode(direccionY2, INPUT_PULLUP);

  pinMode(pulsador_guardar, INPUT_PULLUP);
  pinMode(pulsador_realizar, INPUT_PULLUP);
  pinMode(pulsador_nuevo, INPUT_PULLUP);
  pinMode(LadoaOtrob, INPUT_PULLUP);\

  attachInterrupt(digitalPinToInterrupt(pulsador_guardar), handleGuardarPosicion, FALLING);
  attachInterrupt(digitalPinToInterrupt(pulsador_realizar), handleRealizarAccion, FALLING);
  attachInterrupt(digitalPinToInterrupt(pulsador_nuevo), handleRealizarAccionNuevo, FALLING);
  attachInterrupt(digitalPinToInterrupt(LadoaOtrob), handleRealizarAccionNuevo, FALLING);


  restaurarPosiciones();
  servomotor.write(posicion_servo);
  servomotor2.write(posicion_servo2);
  servomotor3.write(posicion_servo3);
  servomotor4.write(posicion_servo4);
}

void loop()
{
  lecturaX = analogRead(direccionX);
  lecturaY = analogRead(direccionY);
  lecturaX2 = analogRead(direccionX2);
  lecturaY2 = analogRead(direccionY2);

  lectura_pulsador_guardar = !digitalRead(pulsador_guardar);
  lectura_pulsador_realizar = !digitalRead(pulsador_realizar);
  lectura_pulsador_nuevo = !digitalRead(pulsador_nuevo);
  lectura_lado_a_otros = !digitalRead(LadoaOtrob);
  if (lecturaX >= 550)
  {
    posicion_servo++;

    if (posicion_servo > 180)
    {
      posicion_servo = 180;
    }
  }
  if (lecturaX2 >= 550)
  {
    posicion_servo3++;

    if (posicion_servo3 > 180)
    {
      posicion_servo3 = 180;
    }
  }

  if (lecturaX <= 450)
  {
    posicion_servo--;

    if (posicion_servo < 0)
    {
      posicion_servo = 0;
    }
  }
  if (lecturaX2 <= 450)
  {
    posicion_servo3--;

    if (posicion_servo3 < 0)
    {
      posicion_servo3 = 0;
    }
  }

  if (lectura_pulsador_guardar)
  {
    guardarPosiciones();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("guardando         ");
    lcd.setCursor(0, 1);
    lcd.print("           ");
    delay(1000); // Espera un segundo para evitar guardar múltiples veces
  }

  servomotor.write(posicion_servo);
  servomotor3.write(posicion_servo3);

  if (lecturaY >= 550)
  {
    posicion_servo2++;

    if (posicion_servo2 > 180)
    {
      posicion_servo2 = 180;
    }
  }
  if (lecturaY2 >= 550)
  {
    posicion_servo4++;

    if (posicion_servo4 > 180)
    {
      posicion_servo4 = 180;
    }
  }

  if (lecturaY <= 450)
  {
    posicion_servo2--;

    if (posicion_servo2 < 0)
    {
      posicion_servo2 = 0;
    }
  }
  if (lecturaY2 <= 450)
  {
    posicion_servo4--;

    if (posicion_servo4 < 0)
    {
      posicion_servo4 = 0;
    }
  }

  if (lectura_pulsador_realizar)
  {
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
    delay(1000); // Espera un segundo para evitar guardar múltiples veces
  }

  if (lectura_pulsador_nuevo)
  {
    for (int i = 0; i < 2; i++)
    {
    guardarPosicionesCerrada();
    servomotor.write(posicion_servo);
    servomotor2.write(posicion_servo2);
    servomotor3.write(posicion_servo3);
    servomotor4.write(posicion_servo4);

    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Cerrar garra         ");
    lcd.setCursor(0, 1);
    lcd.print("           ");
    delay(1000); // Espera un segundo para evitar guardar múltiples veces
    }
    delay(1000); // Espera un segundo para evitar ejecutar múltiples veces
  }

  if (lectura_lado_a_otros)
  {
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Lado A Lado         ");
    lcd.setCursor(0, 1);
    lcd.print("           ");
    Lado_a_Otro1();
    servomotor.write(posicion_servo);
    servomotor2.write(posicion_servo2);
    servomotor3.write(posicion_servo3);
    servomotor4.write(posicion_servo4);
    delay(1000);
    Lado_a_Otro2();
    servomotor.write(posicion_servo);
    servomotor2.write(posicion_servo2);
    servomotor3.write(posicion_servo3);
    servomotor4.write(posicion_servo4);

    delay(2000); // Espera un segundo para evitar guardar múltiples veces
  }

  servomotor2.write(posicion_servo2);
  servomotor4.write(posicion_servo4);
  delay(10);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Brazo robotico ");
  lcd.setCursor(0, 1);
  lcd.print("           ");
}

