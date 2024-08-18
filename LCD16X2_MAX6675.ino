#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <max6675.h>
#include <LiquidCrystal.h>

// Calibración del valor de pH
float calibration_value = 54.34 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;
float ph_act;

// Definiciones para el display OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pines para el sensor de temperatura MAX6675
#define CONFIG_TCSCK_PIN 10  // SPI SCK
#define CONFIG_TCCS_PIN  9  // SPI CS
#define CONFIG_TCDO_PIN  8  // SPI SO
MAX6675 thermocouple(CONFIG_TCSCK_PIN, CONFIG_TCCS_PIN, CONFIG_TCDO_PIN);

// Pines para el LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    // Inicializar OLED
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);

    // Inicializar LCD
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print(F("Sensores Iniciados"));
    delay(1000);
    lcd.clear();
}

void loop() {
    // Leer pH
    for (int i = 0; i < 10; i++) {
        buffer_arr[i] = analogRead(A0);
        delay(30);
    }
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (buffer_arr[i] > buffer_arr[j]) {
                temp = buffer_arr[i];
                buffer_arr[i] = buffer_arr[j];
                buffer_arr[j] = temp;
            }
        }
    }
    avgval = 0;
    for (int i = 2; i < 8; i++) {
        avgval += buffer_arr[i];
    }
    float volt = (float)avgval * 5.0 / 1024 / 6;
    ph_act = -5.70 * volt + calibration_value;

    // Leer temperatura
    double temperature = thermocouple.readCelsius();

    // Mostrar pH y temperatura en Serial
    Serial.print(ph_act);
    Serial.print(",");
    Serial.println(temperature);

    // Mostrar pH y temperatura en LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("pH: ");
    lcd.print(ph_act);
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature);

    // Mostrar pH en OLED
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.print("pH:");
    display.setCursor(55, 0);
    display.print(ph_act);
    display.display();

    // Esperar un segundo entre lecturas
    delay(1000);
}
