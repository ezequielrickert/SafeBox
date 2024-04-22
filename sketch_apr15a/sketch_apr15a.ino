#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>


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
char CLAVE[17];
char CLAVE_MAESTRA[17] = "123A456B789C*0#D";
byte INDICE = 0;


void setup()
{
    // Inicializa la pantalla LCD
    lcd.init();
    // Enciende el backlight
    lcd.backlight();

    Serial.begin(9600);
}

void loop()
{
    
    TECLA = teclado.getKey();

    if (TECLA != NO_KEY)
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

    if (INDICE == 16)
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
        INDICE = 0;

    }
}