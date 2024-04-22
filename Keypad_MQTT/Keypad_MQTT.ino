#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
//#include <ESP8266WiFi.h> // Para el ESP8266
#include <WiFi.h> // Para el ESP32
WiFiClient WIFI_CLIENT;
#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

// Nombre y contraseña de tu red WiFi.
const char* ssid = "Samsung Ezequiel";
const char* password = "ssrb1167";

// Dirección I2C de la pantalla LCD 1602
const int I2C_ADDR = 0x27;

// Número de columnas y filas del LCD
const int LCD_COLUMNS = 16;
const int LCD_ROWS = 2;

// Definición del teclado
const byte FILAS = 4;
const byte COLUMNAS = 4;
char keys[FILAS][COLUMNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte pinesFilas[FILAS] = {27, 32, 33, 25};
byte pinesColumnas[COLUMNAS] = {19, 18, 5, 17};
Keypad teclado = Keypad(makeKeymap(keys), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

char TECLA;
char CLAVE[7];
char CLAVE_MAESTRA[7] = "123456";
byte INDICE = 0;


void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Conectando con ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado.");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  
  // Inicializa la pantalla LCD
  lcd.init();
  // Enciende el backlight
  lcd.backlight();

  Serial.begin(9600);
}

void loop()
{
  // Continuamente comprueba si tiene conexión con MQTT,
  // en caso de que no tenga, reconecta.
  if (!MQTT_CLIENT.connected()) {
  reconnect();
  }  
  TECLA = teclado.getKey();
  if (TECLA != NO_KEY && TECLA != '#')
  {
    lcd.clear(); // Limpiar la pantalla antes de escribir la nueva tecla
    lcd.setCursor(0, 0);
    lcd.print("Tecla: ");
    lcd.print(TECLA);
    delay(10); // Retardo para evitar rebotes
    CLAVE[INDICE] = TECLA;
    INDICE++;
    Serial.println(TECLA);
  }

  if (TECLA == '#'){
    INDICE = 0;
    lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Contraseña cancelada!");
      delay(2000);
      lcd.clear();
  }

  if (INDICE == 6)
  {
    if (!strcmp(CLAVE, CLAVE_MAESTRA))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Correcto!");
      delay(2000);
      lcd.clear();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incorrecto!");
      delay(2000);
      lcd.clear();
    }
    MQTT_CLIENT.publish("XJXT06/aleatorio", CLAVE);
    INDICE = 0;
  }
}

// Reconecta con MQTT broker
void reconnect() {
MQTT_CLIENT.setServer("35.175.147.215", 1883); // si uso un servidor local <ver IP correcta>
//MQTT_CLIENT.setServer("broker.hivemq.com", 1883);  // servidor gratuito

MQTT_CLIENT.setClient(WIFI_CLIENT);

// Intentando conectar con el broker.
while (!MQTT_CLIENT.connected()) {
Serial.println("Intentando conectar con MQTT.");
MQTT_CLIENT.connect("XJXT06"); // Escribe cualquier nombre.

// Espera antes de volver a intentarlo.
delay(3000);
}

Serial.println("Conectado a MQTT.");
}