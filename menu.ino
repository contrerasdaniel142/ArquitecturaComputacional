//Daniel Contreras
//Cristian Collazos

#include <LiquidCrystal.h>
#include <LiquidMenu.h>
#include <Keypad.h>
#include "DHT.h" 

#include <EEPROM.h>

/*Bibliografia
  *https://www.luisllamas.es/reproducir-sonidos-arduino-buzzer-pasivo-altavoz/
  *https://naylampmechatronics.com/blog/40_tutorial-sensor-de-temperatura-y-humedad-dht11-y-dht22.html
  *https://controlautomaticoeducacion.com/arduino/teclado-matricial-keypad/
  *https://docs.arduino.cc/learn/electronics/lcd-displays
  *https://docs.arduino.cc/built-in-examples/digital/Button
*/

// Variables a modificar
int THTempHigh = 29;
int THTemLow = 26;
int THLuzHigh = 500;
int THLuzLow = 100;

int nuevoValor = 0;

const int rs = 23, en = 33, d4 = 25, d5 = 27, d6 = 29, d7 = 31;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const byte analogPin = A1;
unsigned short analogReading = 0;
unsigned short lastAnalogReading = 0;

unsigned int period_check = 1000;
unsigned long lastMs_check = 0;

unsigned int period_nextScreen = 5000;
unsigned long lastMs_nextScreen = 0;
unsigned long lastMs_previousScreen = 0;

int posicion = 0;

/*
MENU principal

1. THTempHigh
2. THTemLow
3. THLuzHihg
4. THLuzHiLow
5. Reset
*/

LiquidLine welcome_line1(0, 0, "Menu");
LiquidScreen screen(welcome_line1);

LiquidLine menu_linea1(0, 0, "1. THTempHigh");
LiquidLine menu_linea2(0, 1, "2. THTemLow");
LiquidScreen screen2(menu_linea1, menu_linea2);

LiquidLine menu_linea3(0, 0, "3. THLuzHihg");
LiquidLine menu_linea4(0, 1, "4. THLuzHiLow");
LiquidScreen screen3(menu_linea3, menu_linea4);

LiquidLine menu_linea5(0, 0, "4. THLuzHiLow");
LiquidLine menu_linea6(0, 1, "5. Reset");
LiquidScreen screen4(menu_linea5, menu_linea6);

// LiquidLine analogReading_line(0, 0, "Analog: ", analogReading);
// LiquidScreen secondary_screen(analogReading_line);

/*
MENU secundario

int THTempHigh = 29;
int THTemLow = 26;
int THLuzHigh = 500;
int THLuzLow = 100;
*/

LiquidLine submenu_linea1(0, 0, "THTempHigh");
LiquidLine submenu_linea2(0, 1, THTempHigh);
LiquidScreen subscreen(submenu_linea1, submenu_linea2);

LiquidLine submenu_linea3(0, 0, "THTemLow");
LiquidLine submenu_linea4(0, 1, THTemLow);
LiquidScreen subscreen2(submenu_linea3, submenu_linea4);

LiquidLine submenu_linea5(0, 0, "THLuzHigh");
LiquidLine submenu_linea6(0, 1, THLuzHigh);
LiquidScreen subscreen3(submenu_linea5, submenu_linea6);

LiquidLine submenu_linea7(0, 0, "THLuzLow");
LiquidLine submenu_linea8(0, 1, THLuzLow);
LiquidScreen subscreen4(submenu_linea7, submenu_linea8);

LiquidMenu menu(lcd);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'N', '0', 'N', 'N'}
};
byte rowPins[ROWS] = {3, 2, 1, 0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5, 4}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad teclado1 = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


#define DHTPIN 10     // Pin donde está conectado el sensor
//#define DHTTYPE DHT11   // Descomentar si se usa el DHT 11
#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);

#define pinBuzzer = 35; // the number of the buzzer pin

#define buttonPin = 2;  // the number of the pushbutton pin
int buttonState = 0;  // variable for reading the pushbutton status

#define LED_RED 14
#define LED_GREEN 15
#define LED_BLUE 16

void setup() {
  Serial.begin(250000);

  pinMode(analogPin, INPUT);

  lcd.begin(16, 2);

  // Menu principal
  menu.add_screen(screen);
  menu.add_screen(screen2);
  menu.add_screen(screen3);
  menu.add_screen(screen4);

  // Menu secundario
  menu.add_screen(subscreen);
  menu.add_screen(subscreen2);
  menu.add_screen(subscreen3);
  menu.add_screen(subscreen4);
}


void loop() {
  char tecla_presionada = teclado1.getKey();
  int numeroObtenido=0;

  // Verifica si alguna tecla fue presionada
  if (millis() - lastMs_check > period_check) {
    lastMs_check = millis();
    analogReading = analogRead(analogPin);

    if (analogReading != lastAnalogReading) {
      lastAnalogReading = analogReading;
      menu.update();
    }
  }

  // Menu principal
  if (tecla_presionada == 'A') {
    if (posicion > 0 && posicion < 4) {
      lastMs_nextScreen = millis();
      menu.previous_screen();
      posicion--;
    }
  }

  if (tecla_presionada == 'B') {
    if (posicion < 3) {
      lastMs_previousScreen = millis();
      menu.next_screen();
      posicion++;
    }
  }

  // Menu secundario
  if (tecla_presionada == '1') {
    lastMs_previousScreen = millis();
    menu.change_screen(&subscreen);
    THTempHigh = obtenerNuevoValorTecleado();
    posicion = 4;
  }

  if (tecla_presionada == '2') {
    lastMs_previousScreen = millis();
    menu.change_screen(&subscreen2);
    THTemLow = obtenerNuevoValorTecleado();
    posicion = 5;
  }

  if (tecla_presionada == '3') {
    lastMs_previousScreen = millis();
    menu.change_screen(&subscreen3);
    THLuzHigh = obtenerNuevoValorTecleado();
    posicion = 6;
  }

  if (tecla_presionada == '4') {
    lastMs_previousScreen = millis();
    menu.change_screen(&subscreen4);
    THLuzLow = obtenerNuevoValorTecleado();
    posicion = 7;
  }

  if (tecla_presionada == '5') {
    lastMs_previousScreen = millis();
    menu.change_screen(&screen);
    THTempHigh = 29;
    THTemLow = 26;
    THLuzHigh = 500;
    THLuzLow = 100;
    posicion = 0;
  }
}

int obtenerNuevoValorTecleado(){
  Serial.println("Funcion 1");
  nt nuevoValor = 0;
  bool salir = false;
  do{
    char tecla_presionada = teclado1.getKey();
    if (tecla_presionada >= '0' && tecla_presionada <= '9')
    {
      nuevoValor *= 10;
      nuevoValor += tecla_presionada - '0';
      Serial.println(nuevoValor);
    }
    if(tecla_presionada == 'C'){
      salir = true;
    }
  }while(salir != true);
  Serial.println("Salir Funcion 1");
  return nuevoValor;
}