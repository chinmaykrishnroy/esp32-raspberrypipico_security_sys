/********************************** ESP32 Dev Module ************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // D21 is for SDA (data) Pin & D22 is for SCL (clock).
const int ldr = 34;                                                       // LDR pin is D4.
const int led = 18;                                                       // Builtin LED D2, later should be changed and set another pin for buzzer.
const int elemag = 5;                                                     // For electromagnet, a transistor must be used to drive electromagnet as current drain from electromagnet is high and it can damage esp32.
const int touchpin = 4;                                                   // For Touch Pin
const int threshold = 50;
int sval = 0;
int tval;
void setup()
{
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    pinMode(led, OUTPUT);
    pinMode(elemag, OUTPUT);
    digitalWrite(elemag, HIGH);
    ;
}

void loop()
{
    sval = analogRead(ldr);
    tval = touchRead(touchpin);
    Serial.println("LDR value: ");
    Serial.print(sval);
    Serial.println(" ");
    Serial.println("Touch value: ");
    Serial.print(tval);
    Serial.println(" ");
    display.setFont(&FreeMono9pt7b);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Sensor Data:");
    display.setFont();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    display.print("Sensor Value:");
    display.println(sval);
    display.setCursor(0, 30);
    display.print("Fence:");
    display.display();

    if (tval < threshold)
    {
        digitalWrite(led, HIGH);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(36, 30);
        display.print("BREACH");
        display.display();
        delay(5000);
    }
    else
    {
        digitalWrite(led, LOW);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(36, 30);
        display.print("Secure");
        display.display();
    }

    while ((sval < 4000))
    {
        digitalWrite(led, HIGH);
        delay(200);
        digitalWrite(led, LOW);
        delay(200);
        digitalWrite(elemag, LOW);
        display.clearDisplay();
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print("Sensor Value: ");
        display.println(sval);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 17);
        display.println("SECURITY");
        display.print("BREACH");
        display.display();
        Serial.println("SECURITY BREACH");
    }
}
