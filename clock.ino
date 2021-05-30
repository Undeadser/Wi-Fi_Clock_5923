#include <Adafruit_GFX.h> // библиотека подстветки светодидов
#include <Adafruit_NeoMatrix.h>// библиотека светодидной матрицы
#include <Adafruit_NeoPixel.h>// библиотека светодидов
#include <ESP8266WiFi.h> // библиотека модуля ESP8266
#include <NTPClient.h> // библиотека NTP клиетна 
#include <WiFiUdp.h> // библиотека Wi-Fi подключения

#ifndef STASSID 
#define STASSID "Undeadser" // название SSID - сети Wi-Fi
#define STAPSK  "Civilization81" // пароль SSID - сети Wi-Fi
#endif



#define PIN 12
#define Pin_button1 13
#define Pin_button2 15

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16, 16, PIN,
                            NEO_MATRIX_BOTTOM + NEO_MATRIX_RIGHT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
                            NEO_GRB            + NEO_KHZ800);

const char* ssid     = STASSID;
const char* password = STAPSK;
const long utc = 10800; // смещение часового пояса

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utc); // инициализация NTP - клиент

void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(5);
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color(0, 255, 0));
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);//начало подключение к Wi-Fi
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    timeClient.begin();
    }
                // конец подключение к Wi-Fi
  pinMode(Pin_button1, INPUT);
}

int hour = 12;
int minutes = 0;
int seconds = 0;
bool dot = false;
int second_loop;


void loop() {
    matrix.setTextColor(matrix.Color(0, 255, 0));
    if (seconds == 60) {
      seconds = 0;
      minutes++;
    }
    if (minutes == 60) {
      minutes = 0;
      hour++;
    }
    if (hour == 24) {
      hour = 0;
    }
    matrix.fillScreen(0);
    matrix.setCursor(3, 0);// написание цифр
    if (hour < 10) {
      matrix.print("0");
    }
    matrix.print(hour);
    matrix.setCursor(3, 9);
    if (minutes < 10) {
      matrix.print("0");
    }
    matrix.print(minutes);
    if (dot) { //мигание точек между цифрами
      matrix.setTextColor(matrix.Color(0, 0, 255));
      matrix.setCursor(2, 2);
      matrix.print(".");
      matrix.setCursor(9, 2);
      matrix.print(".");
      dot = !dot;
    }
    else {
      dot = !dot;
    }
    if (digitalRead(Pin_button1) == HIGH) // удерживание кнопки
    {
      second_loop++;
    } else {
      second_loop = 0;
    }
    if (second_loop >= 3) {// после 3 секундного удержания кнопи будет установлено текущее время
      second_loop = 0;
      timeClient.update();
      hour = timeClient.getHours();
      minutes = timeClient.getMinutes();
      seconds = timeClient.getSeconds();
    }
    
    matrix.show();
    seconds++;
    delay(1000);
  
}
