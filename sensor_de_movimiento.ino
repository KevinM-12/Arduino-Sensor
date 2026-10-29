#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIR = 2;
const int LED_VERDE = 3;
const int LED_ROJO = 4;
const int BUZZER = 5;

const byte FILAS = 4;
const byte COLUMNAS = 4;

char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte pinesFilas[FILAS] = {6, 7, 8, 9};
byte pinesColumnas[COLUMNAS] = {10, 11, 12, 13};

Keypad teclado = Keypad(
  makeKeymap(teclas),
  pinesFilas,
  pinesColumnas,
  FILAS,
  COLUMNAS
);

String contrasena = "1234";
String entrada = "";

bool sistemaArmado = false;
bool alarmaActiva = false;

void setup() {
  pinMode(PIR, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  lcd.init();
  lcd.backlight();

  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW);

  mostrarDesarmado();
}

void loop() {
  char tecla = teclado.getKey();

  if (tecla != NO_KEY) {
    procesarTecla(tecla);
  }

  if (sistemaArmado == true && alarmaActiva == false) {
    if (digitalRead(PIR) == HIGH) {
      activarAlarma();
    }
  }
}

void procesarTecla(char tecla) {

  if (tecla >= '0' && tecla <= '9') {

    if (entrada.length() < 4) {
      entrada += tecla;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ingrese clave:");

      lcd.setCursor(0, 1);

      for (int i = 0; i < entrada.length(); i++) {
        lcd.print("*");
      }
    }

    return;
  }

  if (tecla == '*') {

    entrada = "";

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ingrese clave:");

    return;
  }

  if (tecla == 'A') {

    if (entrada == contrasena) {

      sistemaArmado = true;
      alarmaActiva = false;
      entrada = "";

      digitalWrite(LED_VERDE, LOW);
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(BUZZER, LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sistema armado");

      lcd.setCursor(0, 1);
      lcd.print("Vigilando");

      delay(1500);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sistema armado");

      lcd.setCursor(0, 1);
      lcd.print("Vigilando");

    } else {

      entrada = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clave incorrecta");

      delay(1500);

      mostrarDesarmado();
    }

    return;
  }

  if (tecla == 'D') {

    if (entrada == contrasena) {

      sistemaArmado = false;
      alarmaActiva = false;
      entrada = "";

      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_ROJO, LOW);
      digitalWrite(BUZZER, LOW);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Acceso permitido");

      lcd.setCursor(0, 1);
      lcd.print("Desarmado");

      delay(1500);

      mostrarDesarmado();

    } else {

      entrada = "";

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clave incorrecta");

      delay(1500);

      if (sistemaArmado) {

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Sistema armado");

        lcd.setCursor(0, 1);
        lcd.print("Vigilando");

      } else {

        mostrarDesarmado();
      }
    }

    return;
  }
}

void activarAlarma() {

  alarmaActiva = true;

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, HIGH);
  digitalWrite(BUZZER, HIGH);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MOVIMIENTO");

  lcd.setCursor(0, 1);
  lcd.print("DETECTADO!");
}

void mostrarDesarmado() {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Sistema");

  lcd.setCursor(0, 1);
  lcd.print("Desarmado");
}
