/*
  SantoCyber 
  Complete project details at https://github.com/santocyber/pandoraLUX
  
*/

#include <Adafruit_NeoPixel.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "DHT.h"//Carrega a biblioteca DHT
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>


#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);



// Pino analógico em que o sensor DHT11 está conectado
#define DHTPIN 4 
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
DHT dht(DHTPIN, DHTTYPE);



//Configura fuso horario em segundos
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0

#define NTP_OFFSET  19800 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

//define saidas do display
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



// Replace with your network credentials
const char* ssid = "InternetSA";
const char* password = "cadebabaca";


//Config LED Strip
//Lista cores http://www.cdme.im-uff.mat.br/matrix/matrix-html/matrix_color_cube/matrix_color_cube_br.html

//rainbow cycle https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#rainbow_cycle.

int ledPin = 6;
#define PIN        6
#define NUMPIXELS 60

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


//Ldr config

int ldr = A0; //Atribui A0 a variável ldr
int valorldr = 0;//Declara a variável valorldr como inteiro

int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro


// Initialize Telegram BOT
#define BOTtoken "5636503763:AAGVb0EGHIM3NbQICjdL0bRB8nbWdoAiKqc"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-878096558"

WiFiClientSecure secured_client;
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
const int buttonPin = 0;





//Configura os voids

void showStrip() {
   // NeoPixel
   pixels.show();
   }

   
void setPixel(int Pixel, byte red, byte green, byte blue) {
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
}
   
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}




void ledgreen (){

   pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
}}


 void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    showStrip();
    delay(FlashDelay);
    setAll(0,0,0);
    showStrip();
    delay(FlashDelay);
  }
 
 delay(EndPause);
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



void ledazul(){
   pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
}

}

void ledamarelo(){

    pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(255, 255, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
}

}

void ledvermelho(){
   pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.

    delay(DELAYVAL); // Pause before next pass through loop
}

}


void Sparkle(byte red, byte green, byte blue, int SpeedDelay) {
  int Pixel = random(NUMPIXELS);
  setPixel(Pixel,red,green,blue);
  showStrip();
  delay(SpeedDelay);
  setPixel(Pixel,0,0,0);
}




void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUMPIXELS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUMPIXELS; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}



void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      c=Wheel(((i * 256 / NUMPIXELS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
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
  }}
  
  



// Config do que acontece quando recebe msg

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
                    sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  

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

    }

    if (text == "/ledon") {
      bot.sendMessage(chat_id, "Ligando LED RGB", "");
NeoFade(100);
Sparkle(random(255), random(255), random(255), 0);
 RunningLights(0xff,0xff,0x00, 50);
  rainbowCycle(20);

    }
    if (text == "azul") {
      bot.sendMessage(chat_id, "Esta tudo azul", "");
ledazul();
}
    if (text == "verde") {
      bot.sendMessage(chat_id, "Ta tudo green", "");
      ledgreen();
}
           if (text == "amarelo") {
      bot.sendMessage(chat_id, "Amarelouuuu", "");
      ledamarelo();
}
           if (text == "rainbow") {
      bot.sendMessage(chat_id, "Cada celulinha do meu corpo esta alegre", "");
      rainbowCycle(20);
    }
               if (text == "pisca") {
      bot.sendMessage(chat_id, "Pisca pisca", "");
      Strobe(0xff, 0xff, 0xff, 10, 50, 1000);
    }


               if (text == "temperatura") {

               
String msg = "Temperature is ";
          msg += msg.concat(temp.temperature);
          msg += "C";
        bot.sendMessage(chat_id,msg, "");

    }
    
       if (text == "umidade") {
          String msg = "Humidity is ";
          msg += msg.concat(humidity.relative_humidity);
          msg += "%"; 
          bot.sendMessage(chat_id,msg, ""); 

    }
   
    
          if (text == "/led_off") {
      bot.sendMessage(chat_id, "LED state set to OFF", "");
      digitalWrite(ledPin, LOW);
      pixels.clear(); 
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
  Serial.begin(9600);

  
  
//Inicia sensor temp e umi
dht.begin();
aht.begin();
bmp.begin();


bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  

  
//inicia LDR

pinMode(ldr, INPUT); //Define ldr (pino analógico A0) como saída


Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
timeClient.begin();
display.begin(SSD1306_SWITCHCAPVCC);

//inicia strip
 pixels.begin();
 pixels.show(); // Initialize all pixels to 'off'


}


void loop() {
   sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");
  
//Mostra hora na tela
timeClient.update();
String formattedTime = timeClient.getFormattedTime();
display.clearDisplay();
display.setTextSize(3);                         // set these parameters according to your need..
display.setCursor(0, 0);
display.println(formattedTime);

delay(2000); // Aguarda 2 segundos


//mostra temperatura e umidade na tela


// Exibindo valor da leitura do sensor de temperatura no display 
display.clearDisplay(); // Limpa o display do LCD
display.setTextSize(3);
display.print("Temperatura:"); // Imprime a string no display do LCD
display.print(temp.temperature);
display.write(B11011111); // Símbolo de graus Celsius
display.print("C");
display.setCursor(0,1); // Coloca o cursor na linha 0 e coluna 1
display.print("Umidade:");
display.print(humidity.relative_humidity);
display.print("%");
delay(2000); // Aguarda 2 segundos



//verifica mensagem no telgram
 if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  
//funcao acende led quando valor de ldr cair
  
valorldr = analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.println(valorldr);//Imprime na serial os dados de valorldr

  if ((valorldr) < 500) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
ledgreen();
Serial.println("Ha algo sobre a mesa");
bot.sendMessage(CHAT_ID,"Alguém colocou algo sobre a mesa");


  }

  else { //Senão:

    NeoFade(100);
    delay(1000);
    ledazul();
    delay(1000);
    rainbowCycle(2000);
    delay(1000);
 Strobe(0xff, 0xff, 0xff, 10, 50, 1000);
    delay(1000);
   
  }



//funcao pressao atm
  
valoratm = bmp.readPressure();
Serial.println(valoratm);

if ((valoratm) < 800) {
//Coloca led em alto para acioná-lo
ledazul();
Serial.println("Vem chuva por ai");
bot.sendMessage(CHAT_ID,"Vem chuva por ai");
}
if ((valoratm) > 900) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
ledamarelo();
Serial.println("Olha o sol , ceu claro!");
bot.sendMessage(CHAT_ID,"Olha o sol , ceu claro!");
}
//funcao temperatura
  
valortemp = bmp.readTemperature();
Serial.println(valortemp);

if ((valortemp) < 20) { //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
ledazul();
Serial.println("Cade o casaco de neve?");
bot.sendMessage(CHAT_ID,"Cade o casaco de neve?");
}
  if ((valortemp) > 35) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
ledvermelho();
Serial.println("Hora de dar um tibum na cachu!");
bot.sendMessage(CHAT_ID,"Hora de dar um tibum na cachu!");
}

  
//funcao humidade
valorhumi = (humidity.relative_humidity);//Lê o valor do sensor ldr e armazena na variável valorldr
Serial.println(valorhumi);//Imprime na serial os dados de valorldr

if ((valorhumi) < 30) { //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
ledazul();
Serial.println("Tempo seco, beba agua");
bot.sendMessage(CHAT_ID,"Tempo seco beba agua");
}

if ((valorhumi) > 90) { //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
ledazul();
Serial.println("Tempo umido, ta chovendo?");
bot.sendMessage(CHAT_ID,"Tempo umido ta chovendo?");
}







}
