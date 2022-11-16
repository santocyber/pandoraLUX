/*
  SantoCyber 
  Complete project details at https://github.com/santocyber/pandoraLUX
  
*/
#include <Adafruit_NeoPixel.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID "InternetSA"
#define WIFI_PASSWORD "cadebabaca"
// Telegram BOT Token (Get from Botfather)
#define BOTtoken "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String id, text, welcome, from_name;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;



//config BMP

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; // I2C



//Config LED Strip
//Lista cores http://www.cdme.im-uff.mat.br/matrix/matrix-html/matrix_color_cube/matrix_color_cube_br.html
//https://celke.com.br/artigo/tabela-de-cores-html-nome-hexadecimal-rgb
//rainbow cycle https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#rainbow_cycle.
#define PIN        4
#define NUMPIXELS 60
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 200 // Time (in milliseconds) to pause between pixels


const int buttonPin = 0;
int valorbutton = 0;


//Ldr config
int ahtPin = 14;
int ldr = 34; //Atribui A0 a variável ldr
int valorldr = 0;//Declara a variável valorldr como inteiro

int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro


//definir mic
#define threshold 500  //change this according to your clap sound
const int mic = 35;
int sound_value;
int clap_counter = 0;
int color_counter = 0;
boolean led_state = true; 




unsigned long millisTarefa1 = millis();
unsigned long tempo2 = millis();
unsigned long tempo3 = millis();
unsigned long tempo4 = millis();
unsigned long tempo5 = millis();
unsigned long tempo6 = millis();
unsigned long tempo7 = millis();
unsigned long tempo8 = millis();



//config time

uint32_t targetTime = 0;                    // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;






//####################################################


// Sound sensor code
void readSoundSensor(){


  sound_value = analogRead(mic);

  if (sound_value < threshold) {  
    // trigger threshold
    // toggle LED     
     if (led_state) {
      led_state = false;
      color_counter++;// LED was on, now off
      if(color_counter > 7) color_counter = 0;
      changeColor();
Serial.println(color_counter);
Serial.println("Clap on");
delay(5000);

      
   }
    else {
         led_state = true;
         setAll(0,0,0);
         pixels.show();
      Serial.println("Clap off");
      delay(5000);
    }  
     
    }}
 

void changeColor(){
   
  bot.sendMessage(id, "Alguem bateu palmas, acendendo a luz, mudando de cor", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
  Serial.println("Luz acionada com som");
//muda de cor
if (color_counter == 0)
  {
    //display red
      setAll(255,255,255);
      pixels.setBrightness(255);
    pixels.show();
  }
  if (color_counter == 1)
  {
    //display red
      setAll(255,255,255);
      pixels.setBrightness(255);
    pixels.show();
  }
  if (color_counter == 2)
  { setAll(255,255,255);
  pixels.setBrightness(125);
    pixels.show();
         bot.sendMessage(id, "Dimmer", "");//Envia uma Mensagem para a pessoa que enviou o Comando.

    }
   if (color_counter == 3)
  { 
       setAll(255,255,255);
       pixels.setBrightness(55);
    pixels.show();
    }
   if (color_counter == 4)
  { 
      setAll(255,255,0);
    pixels.show();
    }
   if (color_counter == 5)
  { 
       setAll(0,255,255);
    pixels.show();
    }
  if (color_counter == 6)
  { 
       setAll(0,0,255);
    pixels.show(); 
    }  
 if (color_counter == 7)
  { 
   strobe(0, 0xff, 0xff, 10, 50, 500);
}
}



void ledgreen (){
 if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
   pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}

void ledazul(){
 if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
   pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}



void ledlaranja(){
 if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
   pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(210, 100, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}


void ledamarelo(){
 if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
   pixels.clear(); // Set all pixel colors to 'off'
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(255, 250, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}

void ledvermelho(){


    if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));

    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}

void ledvioleta(){


    if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(238, 130, 238));

    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}



void ledciano(){


    if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(0, 255, 255));

    pixels.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
}}
tempo2 = millis();
}



void branco(){


    if (millis() - tempo > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
  pixels.clear(); // Set all pixel colors to 'off'

  // The first NeoPixel in a strand is #0, second is 1, all the way up
  // to the count of pixels minus one.
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...

    // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
    // Here we're using a moderately bright green color:
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
    pixels.setBrightness(255);
    pixels.show();   // Send the updated pixel colors to the hardware.

 //   delay(DELAYVAL); // Pause before next pass through loop
}}}

void ledoff(){


    if (millis() - tempo2 > 500)//Faz a verificaçao das funçoes a cada 2 Segundos
   {    setAll(0,0,0);
         pixels.show();
         
         }
         tempo2 = millis();
         }



void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause){
  for(int j = 0; j < StrobeCount; j++) {
    setAll(red,green,blue);
    pixels.show();
    delay(FlashDelay);
    setAll(0,0,0);
    pixels.show();
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


void rainbow(int wait) {
  // Hue of first pixel runs 3 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 3*65536. Adding 256 to firstPixelHue each time
  // means we'll make 3*65536/256 = 768 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in pixels...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // pixels.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through pixels.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   pixels.setPixelColor(Pixel, pixels.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUMPIXELS; i++ ) {
    setPixel(i, red, green, blue);
  }
  pixels.show();
}



void runninglights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<NUMPIXELS*5; j++)
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
     
      pixels.show();
      delay(WaveDelay);
  }
}


void rainbowcycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUMPIXELS; i++) {
      c=Wheel(((i * 256 / NUMPIXELS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
pixels.show();
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





void setup()
{

pinMode(ldr, INPUT); 
pinMode(mic, INPUT); 
analogReadResolution(9);

pixels.begin();
aht.begin();
bmp.begin();


bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
                  

 
   WiFi.mode(WIFI_STA);//Define o WiFi como Estaçao
   connect();//Funçao para Conectar ao WiFi

   Serial.begin(115200);
  Serial.println();
   // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());


  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
//  while (now < 24 * 3600){
    Serial.print(".");
    delay(100);
    now = time(nullptr);
//  }
  Serial.println(now);
           readTel();
           bot.sendMessage(id, "Pandora Conectada...", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
           Serial.println("Pandora Conectada...");
           strobe(0xff, 0xff, 0xff, 10, 50, 500);


}

void loop()
{

  if (millis() - tempo8 > 100)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
         readSoundSensor();
         tempo8 = millis();
  
   }
   
   if (millis() - tempo > 2000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
      connect();//Funçao para verificar se ainda há conexao
      readTel();//Funçao para ler o telegram
      tempo = millis();//Reseta o tempo
     
   }

  if (millis() - tempo5 > 36000000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
      verifica();
      tempo5 = millis();
     
   }

  if (millis() - tempo6 > 36000000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
       verifica2();
       tempo6 = millis();
     
   }
   
  if (millis() - tempo7 > 36000000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
       verifica3();
       tempo7 = millis();
     
   }}

void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(500);
   }
}



void readTel()//Funçao que faz a leitura do Telegram.
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
   
   int newmsg = bot.getUpdates(bot.last_message_received + 1);

   for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
   {
      id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
      text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
      //text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
      from_name = bot.messages[i].from_name;
      //bot.messages[i].type == "channel_post";


      if (text.indexOf("LEDON") > -1)//Caso o texto recebido contenha "ON"
      {
         branco();
         bot.sendMessage(id, "LED ON", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

      else if (text.indexOf("LEDOFF") > -1)//Caso o texto recebido contenha "OFF"
      {
        NeoFade(100);
        ledoff();
        bot.sendMessage(id, "LED OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
       else if (text.indexOf("verde") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "ta tudo green!", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledgreen();
      }  
         
     else if (text.indexOf("vermelho") > -1)//Caso o texto recebido contenha "OFF"
      {       
         bot.sendMessage(id, "vermelhuuuuuuu", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledvermelho();
      }    
      else if (text.indexOf("amarelo") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "amareluuuuuuuuuu", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledamarelo();
      }
            else if (text.indexOf("violeta") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "Violet", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledvioleta();
      }
            else if (text.indexOf("laranja") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "Cor de mixirica", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledlaranja();
      }
            else if (text.indexOf("ciano") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "verde azulado ciano a mistura de verde e azul", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledciano();
      }
      else if (text.indexOf("fade") > -1)//Caso o texto recebido contenha "OFF"
      {
         NeoFade(1000);
         bot.sendMessage(id, "smothieeeee", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }    
      else if (text.indexOf("runninglights") > -1)//Caso o texto recebido contenha "OFF"
      {
         runninglights(0,0,255,100);
         bot.sendMessage(id, "corre gira", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }   
      else if (text.indexOf("arcoiris") > -1)//Caso o texto recebido contenha "OFF"
      {
         rainbowcycle(10);
         bot.sendMessage(id, "LGBTQI+-", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
      else if (text.indexOf("strobe") > -1)//Caso o texto recebido contenha "OFF"
      {
        bot.sendMessage(id, "piscanduuuu", "");//Envia uma Mensagem para a pessoa que enviou o Comando.

         strobe(0xff, 0xff, 0xff, 60, 50, 10);
         strobe(0, 0, 0xff, 60, 50, 10);
         strobe(0, 250, 154, 60, 50, 10);
         strobe(154, 0, 0xff, 60, 50, 10);

         strobe(150, 0, 0, 60, 50, 10);
         strobe(250, 255, 0, 60, 50, 10);
         strobe(0, 150, 0, 60, 50, 10);
         strobe(154, 154, 0xff, 60, 50, 10);
     

      }

      else if (text.indexOf("piscapisca") > -1)//Caso o texto recebido contenha "OFF"
      {
      bot.sendMessage(id, "piscanduuuu verde 400x", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      strobe(0, 250, 154, 400, 50, 10);
      }    
       
       else if (text.indexOf("rainbow") > -1)//Caso o texto recebido contenha "OFF"
      {
         rainbow(10);
         bot.sendMessage(id, "cada celulinha do meu corpo esta alegre", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
           else if (text.indexOf("azul") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "ta tudo bluee", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         ledazul();     
      }
      else if (text.indexOf("branco") > -1)//Caso o texto recebido contenha "START"
      {
         branco();
         bot.sendMessage(id, "Luz ligada", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
         else if (text.indexOf("laranja") > -1)//Caso o texto recebido contenha "START"
      {
         ledlaranja();
         bot.sendMessage(id, "a cor do suco...", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
      else if (text.indexOf("clima") > -1)//Caso o texto recebido contenha "START"
      {
          strobe(0, 0, 0xff, 20, 30, 10);

          String msg = "Temperatura eh ";
          msg += msg.concat(temp.temperature);
          msg += "Graus Celsius";
          msg += "\n\n";
          msg += "Humidade eh ";
          msg += msg.concat(humidity.relative_humidity);
          msg += "% de H.R."; 
          msg += "\n\n";
          msg += "Pressao eh ";
          msg += msg.concat(bmp.readPressure());
          msg += " Pa"; 
          bot.sendMessage(id, msg, "");     
     }
      else if(text.indexOf("start") > -1)//Caso o texto recebido contenha "START"
      {
      if (from_name == "")
      from_name = "Guest";

      welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Essa eh a PandoraBOT\n\n";
      welcome += "/branco : Para ligar o LED branco\n";
      welcome += "/azul : Para ligar o LED \n";
      welcome += "/amarelo : Para ligar o LED \n";
      welcome += "/laranja : Para ligar o LED \n";
      welcome += "/violeta : Para ligar o LED \n";
      welcome += "/ciano : Para ligar o LED \n";
      welcome += "/vermelho : Para ligar o LED \n";
      welcome += "/verde : Para ligar o LED verde\n";
      welcome += "/runninglights : Para ligar o LED\n";
      welcome += "/rainbow : Para ligar o LED \n";
      welcome += "/arcoiris : Para ligar o LED \n";
      welcome += "/piscapisca : Para ligar o STROBE\n";
      welcome += "/strobe : Para ligar o STROBE\n";
      welcome += "/fade : Para Dimmerizar \n";
      welcome += "/clima : Para verificar temperatura, humidade e pressao\n";
      welcome += "/LEDON: Liga o LED \n";
      welcome += "/LEDOFF: Para desligar o LED\n";
      welcome += "/start : Abre esse menu\n";
      welcome += "codigo fonte em https://github.com/santocyber/pandoraLUX\n";

      bot.sendMessage(id, welcome, "Markdown");      
      }

      else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
      {
       //  bot.sendSimpleMessage(id, "Comando Invalido", "");
      }
   }

}


void verifica(){
//funcao acende led quando valor de ldr cair

//Lê o valor do sensor ldr e armazena na variável valorldr
valorldr = analogRead(ldr);

  Serial.println(valorldr);//Imprime na serial os dados de valorldr

  if ((valorldr) > 2500) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
Serial.println("Quem apagou a luz");
bot.sendMessage(id, "Quem apagou a luz? acende? /branco ou /verde");
}else{
  Serial.println("luz acesa");
bot.sendMessage(id, "luz acesa? ");
}
}



void verifica2(){


 sensors_event_t humidity, temp;
 aht.getEvent(&humidity, &temp);
  
//funcao pressao atm
  valoratm = bmp.readPressure();


Serial.println(valoratm);

if ((valoratm) < 92298) {
//Coloca led em alto para acioná-lo
ledazul();
Serial.println("Vem chuva por ai");
bot.sendMessage(id,"Vem chuva por ai");
}

if ((valoratm) > 92300) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
ledamarelo();
Serial.println("Olha o sol , ceu claro!");
bot.sendMessage(id,"Olha o sol ou lua , ceu claro!");
}


//funcao temperatura
valortemp = temp.temperature;
 
Serial.println(valortemp);

if ((valortemp) < 15) {    
ledciano();
Serial.println("Cade o casaco de neve?");
bot.sendMessage(id,"Cade o casaco de neve?");
}

  if ((valortemp) > 38){ 
ledvermelho();
Serial.println("Hora de dar um tibum na cachu!");
bot.sendMessage(id,"Hora de dar um tibum na cachu!");
}


  
//funcao humidade
valorhumi = humidity.relative_humidity;//Lê o valor do sensor ldr e armazena na variável valorldr

Serial.println(valorhumi);//Imprime na serial os dados de valorldr

if ((valorhumi) < 40) { 

ledamarelo();
Serial.println("Tempo seco, beba agua");
bot.sendMessage(id,"Tempo seco beba agua");
}

if ((valorhumi) > 92){ 

ledvioleta();
Serial.println("Tempo umido, ta chovendo?");
bot.sendMessage(id,"Tempo umido ta chovendo?");
}
}

void verifica3(){

  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
          String msg = "Temperatura eh ";
          msg += msg.concat(temp.temperature);
          msg += "Graus Celsius";
          msg += "\n\n";
          msg += "Humidade eh ";
          msg += msg.concat(humidity.relative_humidity);
          msg += "% de H.R."; 
          msg += "\n\n";
          msg += "Pressao eh ";
          msg += msg.concat(bmp.readPressure());
          msg += " Pa"; 
          bot.sendMessage(id, msg, ""); 

          
}




// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
