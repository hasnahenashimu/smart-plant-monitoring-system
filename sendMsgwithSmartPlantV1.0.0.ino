#include <UniversalTelegramBot.h>

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- WiFi Settings -------
char ssid[] = "";       // your network SSID (name)
char password[] = "";  // your network key

// ------- Telegram config --------
#define BOT_TOKEN ""  // your Bot Token (Get from Botfather)
#define CHAT_ID "" // first user
#define CHAT_ID_2 "" // second user
#define CHAT_ID_3 "" // third user

// SSL client needed for both libraries
WiFiClientSecure client;


UniversalTelegramBot bot(BOT_TOKEN, client);

String ipAddress = "";

volatile bool telegramButtonPressedFlag = false;
volatile bool iftttButtonPressedFlag = false;

int digitalPin = D0;
int analogPin = A0;
int inputValue = 0;


void setup() {
  // pin for esp8266
  pinMode(analogPin, INPUT);
  pinMode(digitalPin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // wifi config


  Serial.begin(115200);

  client.setInsecure();

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  ipAddress = ip.toString();

}


// send message
void sendTelegramMessage() {
  // soil sensor and motor actions

  inputValue = analogRead(analogPin);
  Serial.print(inputValue);

  // send msg to telegram
  String message = "SSID:  ";
  message.concat(ssid);
  message.concat("\n");
  message.concat("IP: ");
  message.concat(ipAddress);
 

  if (inputValue >= 700)
  {
    digitalWrite(digitalPin, HIGH);
    Serial.println("-Dry");
     message.concat("\nThe soil is dry!\n");
  }
  else if (inputValue <= 500)
  {
    digitalWrite(digitalPin, LOW);
    Serial.println("-Wet");
    message.concat("\nThe soil is wet!\n");
  }
  else
  {
    Serial.println("Error");
  }

  // send message from esp to telegram
  
  if (bot.sendMessage(CHAT_ID, message, "Markdown")) {
    Serial.println("TELEGRAM Successfully sent");
  }
  if (bot.sendMessage(CHAT_ID_2, message, "Markdown")) {
    Serial.println("TELEGRAM Successfully sent on 2");
  }
   if (bot.sendMessage(CHAT_ID_3, message, "Markdown")) {
    Serial.println("TELEGRAM Successfully sent on 3");
  }
  Serial.println(message);
}

void loop() {

  sendTelegramMessage();
  delay(5000);

}
