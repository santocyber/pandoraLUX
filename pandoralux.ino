/*
  SantoCyber 
  Complete project details at https://github.com/santocyber/pandoraLUX
  
*/


#ifdef ESP32

//Led config 
#include <Adafruit_NeoPixel.h>
#define PIN        6
#define NUMPIXELS 60

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels

//Ldr config

int ldr = A0; //Atribui A0 a variável ldr
int valorldr = 0;//Declara a variável valorldr como inteiro

//client pegar previsao da web
#include <HTTPClient.h>
String town="Paris";              //EDDIT
String Country="FR";                //EDDIT
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q="+town+","+Country+"&units=metric&APPID=";
const String key = "d0d0bf1bb7xxxx2e5dce67c95f4fd0800"; /*EDDITTTTTTTTTTTTTTTTTTTTTTTT                      */

String payload=""; //whole json 
 String tmp="" ; //temperatur
  String hum="" ; //humidity
  
StaticJsonDocument<1000> doc;

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;



#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;
//Configurando tela
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#define TFT_GREY 0x5AEB
#define lightblue 0x01E9
#define darkred 0xA041
#define blue 0x5D9B
#define TFT_BLACK 0x0000 // black


const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 0;


#include <SPI.h>
#include <WiFi.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;    //UTC Offset!
const int   daylightOffset_sec = 0;


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


#else
#endif
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid = "InternetSA";
const char* password = "cadebabaca";

// Initialize Telegram BOT
#define BOTtoken "5636503763:AAGVb0EGHIM3NbQICjdL0bRB8nbWdoAiKqc"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-878096558"

#ifdef ESP8266
  X509List cert(TELEGRAM_CERTIFICATE_ROOT);
#endif

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


// Inicia variáveis de tempo
unsigned long millisTarefa1 = millis();
unsigned long millisTarefa2 = millis();
unsigned long millisTarefa3 = millis();
unsigned long millisTarefa4 = millis();


//Botao pin on esp32 GPIO 0 boot button
int buttonPin;
buttonPin = 0; 

//Led
const int ledPin = 6;
bool ledState = LOW;


// Config do que acontece quando recebe msg

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Use os comandos a seguir para controlar a Pandora .\n\n";
      welcome += "/ledon para ligar a luminaria \n";
      welcome += "/led_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");

bot.sendMessage(chat_id,&timeinfo,"%H:%M:%S %B %d %Y \n %A");

    }

    if (text == "/ledon") {
      bot.sendMessage(chat_id, "Ligando LED RGB", "");
//Inicia ciclo
bot.sendMessage(chat_id,&timeinfo,"%H:%M:%S %B %d %Y \n %A");
      ledState = HIGH;
digitalWrite(ledPin, ledState);

    }
    
    if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      ledState = LOW;
      digitalWrite(ledPin, ledState);
    }
    
    if (text == "/state") {
      if (digitalRead(ledPin)){
        bot.sendMessage(chat_id, "LED is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "LED is OFF", "");
      }
    }
  }
}







void setup() {
  
  
  
  
//Inicia display
    tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE,TFT_BLACK);  tft.setTextSize(1);

  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, backlight[b]);
  
//inicia LDR

pinMode(ldr, INPUT); //Define ldr (pino analógico A0) como saída

//Strip led início
pixels.begin();


    static const i2s_config_t i2s_config = {
        .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
        .sample_rate = 44100, // corrected by info from bluetooth
        .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
        .intr_alloc_flags = 0, // default interrupt priority
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    a2dp_sink.set_i2s_config(i2s_config);
    a2dp_sink.start("Pandora LUX");
  
    Serial.begin(115200);
  tft.print("Connecting to ");
  tft.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    tft.print(".");
    
    // Initialize a NTPClient to get time
  timeClient.begin(); 
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);   /*EDDITTTTTTTTTTTTTTTTTTTTTTTT                      */
  getData();
  delay(500);
    
    

}

void loop() {
valorldr = analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.println(valorldr);//Imprime na serial os dados de valorldr

  if ((valorldr) < 500) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
ledgreen();
bot.sendMessage(chat_id,&timeinfo,"%H:%M:%S %B %d %Y \n %A");

bot.sendMessage(chat_id,"Alguém colocou algo sobre mim");


  }

  else { //Senão:
    digitalWrite(ledPin, LOW);//Coloca led em baixo para que o mesmo desligue ou permaneça desligado
  }




ledrandom();
ledgreen();
testeled();
NeoFade();
NeoBlink();

}



//Pisca o led randomico
void ledrandom() {
pixels.clear();
pixels.setPixelColor(random(0, 7), random(0, 255), random(0, 255), random(0, 255));
pixels.show();
delay(500);
}


void ledgreen (){

pixels.setPixelColor(2, pixels.Color(0, 255, 0));
pixels.show();


}

void testeled(){

pixels.clear();
pixels.setBrightness(10);
pixels.setPixelColor(0, pixels.Color(255, 255, 255));
pixels.setPixelColor(1, pixels.Color(255, 0, 0));
pixels.setPixelColor(2, pixels.Color(0, 255, 0));
pixels.setPixelColor(3, pixels.Color(0, 0, 255));
pixels.setPixelColor(4, pixels.Color(255, 0, 255));
pixels.setPixelColor(5, pixels.Color(255, 255, 0));
pixels.setPixelColor(6, pixels.Color(0, 255, 255));
pixels.show();
}

void NeoFade(int FadeSpeed)
{
int fspeed;
for (int i = 0; i < NUMPIXELS; i++) { pixels.setPixelColor(i, 165, 242, 243); } for (int j = 255; j > 0; j=j-2)
{
pixels.setBrightness(j);
pixels.show();
delay(FadeSpeed);
}
}


void NeoBlink(int num, int wait)
{
for (int i = 0; i < num; i++)
{
pixels.setPixelColor(i, 35, 35, 35);
}
pixels.show();
delay(wait);
for (int j = 0; j < num; j++)
{
pixels.setPixelColor(j, 0, 255, 0);
}
pixels.show();
delay(wait);
}

void tarefa1(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa1) < 200){
    // Acende o led do pino 7
    digitalWrite(7, HIGH);
  }else{
    // Apaga o led do pino 7
    digitalWrite(7, LOW);
  }
  // Verifica se já passou 400 milisegundos reinicia funcao
  if((millis() - millisTarefa1) > 400){
    millisTarefa1 = millis();
  }
  
  void getData()
{
    tft.fillRect(1,170,64,20,TFT_BLACK);
    tft.fillRect(1,210,64,20,TFT_BLACK);
   if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); //Specify the URL
    int httpCode = http.GET();  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
         payload = http.getString();
       // Serial.println(httpCode);
        Serial.println(payload);
        
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 char inp[1000];
 payload.toCharArray(inp,1000);
 deserializeJson(doc,inp);
  
  String tmp2 = doc["main"]["temp"];
  String hum2 = doc["main"]["humidity"];
  String town2 = doc["name"];
  tmp=tmp2;
  hum=hum2;
  
   Serial.println("Temperature"+String(tmp));
   Serial.println("Humidity"+hum);
   Serial.println(town);
   
 }
  
  
  
  
}
 
