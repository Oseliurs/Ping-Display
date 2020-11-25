#include <SPI.h>
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include <ESP8266WiFi.h>
#include <ESP8266Ping.h>

//Internet Stuff
const char* ssid     = "SSID";
const char* password = "PASS";
IPAddress ip (1,1,1,1); // adresses des serveurs
int avg_ms, x, disp_ms;
String avg_ms_str;

//Screen Stuff
Adafruit_SSD1306 display(128, 32, &Wire, -1); //(Screen Width, Screen Height, &Wire, Reset Pin)

//Display Stuff
int Stor[64];


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  delay(10);

  //Init Display
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.clearDisplay();
  display.display();

  Serial.println();
  Serial.println("Connecting to WiFi");

  WiFi.begin(ssid, password);
  display.clearDisplay();

  for (int i = 0; i < 64; i = i + 1) {
    Stor[i] = 31;
  }

  while (WiFi.status() != WL_CONNECTED) {

    display.setCursor(0, 6);
    display.print("Conn ..");
    Serial.println("Connecting ...");
    display.display();
    delay(100);
  }
  display.clearDisplay();
  display.setCursor(0, 6);
  display.print("Connd !");
  Serial.println("Connected !");
  display.display();
  delay(1000);
}


void loop() {
  Serial.println("Loop ..");

  for (int i = 63; i >= 0; i = i - 1) {
    if (i != 63){
        Stor[i+1] = Stor[i];
    }
  }
  
  if ( Ping.ping(ip, 2)) {
    avg_ms = Ping.averageTime();
    disp_ms = map(avg_ms, 0, 500, 32, 0);
    Stor[0] = disp_ms;
    if (avg_ms >= 500){
      Stor[0] = 0;
    }
    avg_ms_str = String(avg_ms);
    display.clearDisplay();

    for (int i = 0; i < 63; i = i + 1) {
      display.drawPixel(63+i, Stor[i], WHITE);
      Serial.print(String(Stor[i]));
    }
    Serial.println();

    display.setCursor(0, 6);
    display.print(avg_ms_str);
    Serial.println(avg_ms_str);
    display.display();
  }


}
