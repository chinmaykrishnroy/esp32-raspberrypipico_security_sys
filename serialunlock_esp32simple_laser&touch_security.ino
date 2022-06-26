/****************************WORKING ON THE DUAL CORE OF ESP32 WITH SERIAL PARSE PASSWORD INTERFACE***************************/
TaskHandle_t Task1;
const int pswd = 3457;     // SET PASSWORD HERE
const int unlockTime = 10; // SET UNLOCK TIME (in secs) HERE
int password = 0000;
void codeForTask1(void *parameter)
{
    for (;;)
    {
        // Serial.print("Core currently being used is: ");
        // Serial.println(xPortGetCoreID());
        if (password == 0000)
        {
            password = Serial.parseInt();
        }
        else
        {
        }
        while (password == pswd)
        {
            delay(unlockTime * 1000);
            password = 0000;
        }
    }
}
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // D21 is for SDA (data) Pin & D22 is for SCL (clock).
const int ldr = 15;                                                       // LDR pin is D4.
const int led = 18;                                                       // Builtin LED D2, later should be changed and set another pin for buzzer.
const int led1 = 2;
const int elemag = 5;     // For electromagnet, a transistor must be used to drive electromagnet as current drain from electromagnet is high and it can damage esp32.
const int touchpin = 4;   // For Touch Pin
const int threshold = 72; // Must be calibrated after any change in length of touch wire. (take value 73 for a wire few cm long)
int sval = 0;
int tval;
void setup()
{
    Serial.begin(115200);
    xTaskCreatePinnedToCore(codeForTask1, "Task_1", 1000, NULL, 1, &Task1, 0);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ; // Don't proceed, loop forever
    }
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(elemag, OUTPUT);
    digitalWrite(elemag, HIGH);
    // password = 1234;// ENTER PASSWORD HERE
    // password = Serial.parseInt();
}

void loop()
{
    // Serial.print("Core currently being used is: ");
    // Serial.println(xPortGetCoreID());
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
    display.drawRect(0, 18, 127, 12, WHITE);
    display.setCursor(3, 20);
    display.print("Sensor Value:");
    display.println(sval);
    display.drawRect(0, 30, 127, 12, WHITE);
    display.setCursor(3, 32);
    display.print("Fence:");
    display.setTextColor(BLACK, WHITE);
    display.drawRect(12, 49, 104, 10, WHITE);
    display.setCursor(13, 50);
    display.println("Chinmay, IBM 1432");
    display.display();
    if ((tval < threshold) & password != pswd)
    {
        digitalWrite(led, HIGH);
        digitalWrite(led1, HIGH);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(39, 32);
        display.print("BREACH");
        display.display();
        delay(5000);
    }
    else
    {
        digitalWrite(led, LOW);
        digitalWrite(led1, LOW);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(39, 32);
        display.print("Secure");
        display.display();
    }
    while ((sval < 4000) & password != pswd)
    {
        digitalWrite(led, HIGH);
        delay(200);
        digitalWrite(led, LOW);
        delay(200);
        digitalWrite(elemag, LOW);
        digitalWrite(led1, HIGH);
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
