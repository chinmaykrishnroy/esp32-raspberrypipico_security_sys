/**********************WORKING ON DUAL CORE OF THE ESP32 WITH BOTH SERIAL AND WIFI FEATURES***********************/
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); // D21 is for SDA (data) Pin & D22 is for SCL (clock).
const char *ssid = "Prefect X";                                           // Set your device SSID here.
const char *passssword = "bakwashai";                                     // Set your device password here.
const int ldr = 34;                                                       // Use D34 as an LDR pin.
const int led = 18;                                                       // Use D18 as Buzzer pin.
const int rp2040sw = 27;                                                  // Use D27 to switch transistor which powers Raspberry Pi Pico.
const int led2 = 2;                                                       // Use D26 as WiFi lamp pin.
const int led1 = 26;                                                      // D2 is LED_BUILTIN pin.
const int elemag = 5;                                                     // Use D5 to switch ON/OFF transistor to which electromagnet (gate) is connected.
const int touchpin = 4;                                                   // Use D4 as touch sensor input pin.
const int threshold = 72;                                                 // SET TOUCH THRESHOLD HERE.
int sval = 0;
int tval;
WiFiServer server(80);
TaskHandle_t Task1;
const int pswd = 3457;                                                    // SET SERIAL PASSWORD HERE
const int wauth = 3457;
const int unlockTime = 10;                                                // SET UNLOCK TIME HERE IN SECONDS
int password = 0000;
int wifiauth = 0000;
void codeForTask1(void *parameter)
{
    for (;;)
    {
        if (password == 0000)
        {
            password = Serial.parseInt();
        }
        while (password == pswd)
        {
            delay(unlockTime * 1000 * 2);                                  // Unlock time over serial password is twice of unlock time (of WiFi).
            password = 0000;
        }
    }
}
void setup()
{
    xTaskCreatePinnedToCore(codeForTask1, "Task_1", 1000, NULL, 1, &Task1, 0);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;)
            ;
    }
    Serial.begin(115200);
    pinMode(led, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(elemag, OUTPUT);
    pinMode(rp2040sw, OUTPUT);
    digitalWrite(rp2040sw, LOW);
    digitalWrite(elemag, HIGH);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    display.clearDisplay();
    display.setFont();
    display.setTextSize(1);
    display.fillRect(0, 0, 127, 15, WHITE);
    display.setTextSize(1);
    display.setTextColor(BLACK, WHITE);
    display.setCursor(18, 4);
    display.print("WiFi Management");
    display.setTextColor(WHITE);
    display.setCursor(0, 16 + 10);
    display.print("Connecting to ");
    display.setCursor(72, 26 + 10);
    display.println(ssid);
    display.display();
    WiFi.begin(ssid, passssword);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(i, (32 + 10));
        display.print(".");
        display.display();
        i += 4;
        delay(500);
        Serial.print(".");
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(35, 28);
    display.print("Connected!");
    display.display();
    delay(500);
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");                                        // GET IP ADDRESS ON SERIAL MONITOR TO ACCESS SERVER
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop()
{
    digitalWrite(rp2040sw, HIGH);
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("New Client.");
        String currentLine = "";
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                Serial.write(c);
                if (c == '\n')
                {
                    if (currentLine.length() == 0)
                    {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.print("<style>:root{--clr-neon:hsl(293 100% 54%);--clr-bg:hsl(323 21% 16%);}*,*::before,*::after {box-sizing: border-box;}body{min-height: 50vh;display: grid;place-items: center;background: var(--clr-bg);color: var(--clr-neon);padding-right: 10rem;}.neon-button {font-size:4rem;display: inline-block;cursor: pointer;font-weight: bold;text-decoration: none;color: var(--clr-neon);border: var(--clr-neon) 0.125em solid;padding: .25em 1em;border-radius: 0.25em;text-shadow:  0 0 0.125em hsl(0 0% 100% / 0.3),  0 0 0.125em currentColor;box-shadow: inset 0 0 0.5em 0 var(--clr-neon), 0 0 0.5em var(--clr-neon);position: relative;transition: background-color 10ms linear;}.neon-button::before {pointer-events: none;content: \'\';position: absolute;background: var(--clr-neon);top: 120%;left: 0;width: 100%;height: 100%;transform: perspective(1em) rotateX(40deg) scale(1, 0.35);filter: blur(1.5em);opacity: 0.7;}.neon-button:after {content: \'\';position: absolute;top: 0;left: 0;right: 0;bottom: 0;box-shadow: 0 0 2em 0.5em var(--clr-neon);opacity: 0;background-color: var(--clr-neon);z-index: -1;transition: opacity 100ms linear;}.neon-button:hover,.neon-button:focus {background: var(--clr-neon);color: var(--clr-bg);text-shadow: none;}.neon-button:hover::before,.neon-button:focus::before {  opacity: 1;}.neon-button:hover:after,.neon-button:focus::after {opacity: 1;}</style><a href=\"/OL\" class=\"neon-button\">UNLOCK</a><br><div><span><a href=\"/H\" class=\"neon-button\">L &nbsp ON</a></span><span><i>&nbsp &nbsp &nbsp</i></span><span><a href=\"/L\" class=\"neon-button\">L OFF</a></span>");
                        client.println();
                        break;
                    }
                    else
                    {
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {
                    currentLine += c;
                }
                if (currentLine.endsWith("GET /OL"))
                {
                    wifiauth = 3457;
                    digitalWrite(led1, HIGH);
                    delay(50);
                    digitalWrite(led1, LOW);
                }
                if (currentLine.endsWith("GET /H"))
                {
                    digitalWrite(led2, HIGH);
                }
                if (currentLine.endsWith("GET /L"))
                {
                    digitalWrite(led2, LOW);
                }
            }
        }
        client.stop();
        Serial.println("Client Disconnected.");
    }
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
    display.println(tval);
    display.setTextColor(BLACK, WHITE);
    display.drawRect(12, 49, 104, 10, WHITE);
    display.setCursor(13, 50);
    display.println("'your_text_here-'");
    display.display();
    if ((tval < threshold) && (password != pswd) && (wifiauth != wauth))
    {
        digitalWrite(led, HIGH);
        digitalWrite(led1, HIGH);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(72, 32);
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
        display.setCursor(72, 32);
        display.print("Secure");
        display.display();
    }
    while ((sval < 4000) && (password != pswd) && (wifiauth != wauth))
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

        WiFiClient client = server.available();
        if (client)
        {
            Serial.println("New Client.");
            String currentLine = "";
            while (client.connected())
            {
                if (client.available())
                {
                    char c = client.read();
                    Serial.write(c);
                    if (c == '\n')
                    {
                        if (currentLine.length() == 0)
                        {
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println();
                            client.print("<style>:root{--clr-neon:#ffffff;--clr-bg:rgb(0, 0, 0);}*,*::before,*::after {box-sizing: border-box;}body{min-height: 100vh;display: grid;place-items: center;background: var(--clr-bg);color: var(--clr-neon);padding-right: 10rem;}.neon-button {font-size:4rem;display: inline-block;cursor: pointer;font-weight: bold;text-decoration: none;color: var(--clr-neon);border: var(--clr-neon) 0.125em solid;padding: .25em 1em;text-shadow:  0 0 0.125em #000000,  0 0 0.125em currentColor;transition: background-color 10ms linear;}.neon-button::before {pointer-events: none;content: \'\';position: absolute;background: var(--clr-neon);top: 120%;left: 0;width: 100%;height: 100%;transform: perspective(1em) rotateX(40deg) scale(1, 0.35);filter: blur(1.5em);opacity: 0.7;}.neon-button:after {content: \'\';position: absolute;top: 0;left: 0;right: 0;bottom: 0;box-shadow: 0 0 2em 0.5em var(--clr-neon);opacity: 0;background-color: var(--clr-neon);z-index: -1;transition: opacity 100ms linear;}.neon-button:hover,.neon-button:focus {background: var(--clr-neon);color: var(--clr-bg);text-shadow: none;}.neon-button:hover::before,.neon-button:focus::before {  opacity: 1;}.neon-button:hover:after,.neon-button:focus::after {opacity: 1;}</style><div><span><a href=\"/RL\" class=\"neon-button\">Force Stop!</a></span>");
                            break;
                        }
                        else
                        {
                            currentLine = "";
                        }
                    }
                    else if (c != '\r')
                    {
                        currentLine += c;
                    }
                    if (currentLine.endsWith("GET /RL"))
                    {
                        sval = 4095;
                    }
                }
            }
            client.stop();
            Serial.println("Client Disconnected.");
        }
    }
    while (wifiauth == wauth)
    {
        digitalWrite(rp2040sw, LOW);
        delay(unlockTime * 1000);
        wifiauth = 0000;
    }
}
