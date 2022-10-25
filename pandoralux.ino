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

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
#include <WiFi.h>
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;    //UTC Offset!
const int   daylightOffset_sec = 0;


TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define TFT_BLACK 0x0000 // black


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









void setup() {

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
}
 
