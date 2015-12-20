// TODO interaction with the site/pc/mobile/etc

#include <ESP8266WiFi.h>
#include <SPI.h>
#include "MFRC522.h"

/* wiring the MFRC522 to ESP8266 (ESP-12)
  RST     = GPIO0
  SDA(SS) = GPIO2
  MOSI    = GPIO13
  MISO    = GPIO12
  SCK     = GPIO14
  GND     = GND
  3.3V    = 3.3V
*/

#define RST_PIN 0
#define SS_PIN 2

#define LED_R 4
#define LED_G 15
#define LED_B 5

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void led_set(int r, int g, int b)
{
  digitalWrite(LED_R, r);
  digitalWrite(LED_G, g);
  digitalWrite(LED_B, b);
}

void led_set_white()
{
  led_set(HIGH, HIGH, HIGH); // white
}
void led_set_cyan()
{
  led_set(LOW, HIGH, HIGH); // cyan
}
void led_set_puprle()
{
  led_set(HIGH, LOW, HIGH); // purple
}
void led_set_yellow()
{
  led_set(HIGH, HIGH, LOW); // yellow
}
void led_set_red()
{
  led_set(HIGH, LOW, LOW); // red
}
void led_set_green()
{
  led_set(LOW, HIGH, LOW); // green
}
void led_set_blue()
{
  led_set(LOW, LOW, HIGH); // blue
}
void led_off()
{
  led_set(0, 0, 0);
}

void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
  for (byte j = 0; j < bufferSize; j++) {
    // String(13, HEX);
    //Serial.print(buffer[j]);
    Serial.print(String(buffer[j], HEX));
    Serial.print(",");
  }
  Serial.println();
}

String get_uid(byte *buffer, byte bufferSize)
{
  String uid = "";
  for (byte j = 0; j < bufferSize; j++) {
    uid += String(buffer[j], HEX) + ",";
  }
  return uid;
}

void setup()
{
  Serial.begin(9600);
  //  delay(250);

  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
}

void loop()
{
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    //    delay(50);
    return;
  }

  String uid = get_uid(mfrc522.uid.uidByte, mfrc522.uid.size);

  led_off();
  delay(100);
  // FIXME use settings arrays
  if (uid == "4,20,ad,2a,85,32,80,")
    led_set_red();
  else if (uid == "4,b0,4,aa,41,2d,84,")
    led_set_green();
  else if (uid == "4,1d,b0,2a,85,32,80,")
    led_set_blue();
  else if (uid == "4,65,be,2a,85,32,80,")
    led_set_yellow();
  else if (uid == "7e,47,69,23,")
    led_set_white();
  else if (uid == "a9,15,98,1a,")
    led_off();

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

