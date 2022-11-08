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
#define BOTtoken "5636503763:AAGVb0EGHIM3NbQICjdL0bRB8nbWdoAiKqc"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String id, text, welcome, from_name;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;



//config BMP

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);




//Config LED Strip
//Lista cores http://www.cdme.im-uff.mat.br/matrix/matrix-html/matrix_color_cube/matrix_color_cube_br.html
//rainbow cycle https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#rainbow_cycle.
#define PIN        6
#define NUMPIXELS 60
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


int buzzerPin = 26;
int ledPin = 27;
int solenoidePin = 32;
int drenagemPin = 33;
int bldcPin = 25;
const int buttonPin = 0;
const int button2Pin = 35;
int valorbutton = 0;


//Ldr config

int ldr = 36; //Atribui A0 a variável ldr
int valorldr = 0;//Declara a variável valorldr como inteiro

int valoratm = 0;//Declara a variável valorldr como inteiro
int valortemp = 0;//Declara a variável valorldr como inteiro
int valorhumi = 0;//Declara a variável valorldr como inteiro

unsigned long millisTarefa1 = millis();

//config time

uint32_t targetTime = 0;                    // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;





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











void setup()
{


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

  Serial.println(now);
}

void loop()
{
   if (millis() - tempo > 2000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
      connect();//Funçao para verificar se ainda há conexao
      readTel();//Funçao para ler o telegram
      tempo = millis();//Reseta o tempo
   }



verifica();




   
}

void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(2000);
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
         //digitalWrite(D4, 0);//Liga o LED
         bot.sendMessage(id, "LED ON", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }

      else if (text.indexOf("LEDOFF") > -1)//Caso o texto recebido contenha "OFF"
      {
         //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "LED OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
  else if (text.indexOf("verde") > -1)//Caso o texto recebido contenha "OFF"
      {
         //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "verde", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }  
         
     else if (text.indexOf("vermelho") > -1)//Caso o texto recebido contenha "OFF"
      {
         //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "vermelho", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }    
      else if (text.indexOf("amarelo") > -1)//Caso o texto recebido contenha "OFF"
      {
         //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "amarelo", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
           else if (text.indexOf("azul") > -1)//Caso o texto recebido contenha "OFF"
      {
         //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "azul", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
      else if (text.indexOf("branco") > -1)//Caso o texto recebido contenha "START"
      {
       //digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "branco", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
      }
      else if (text.indexOf("clima") > -1)//Caso o texto recebido contenha "START"
      {
          bot.sendMessage(id, "Clima agora, temperatura, humidade e pressao", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
          
          String msg = "Temperature is ";
          msg += msg.concat(temp.temperature);
          msg += "C";
          msg += "Humidity is ";
          msg += msg.concat(humidity.relative_humidity);
          msg += "%"; 
          msg += "Pressao is ";
          msg += msg.concat(bmp.readPressure());
          msg += "Pa"; 
          bot.sendMessage(id, msg, "");     
     }
      else if (text.indexOf("Start") > -1)//Caso o texto recebido contenha "START"
      {
      if (from_name == "")
      from_name = "Guest";

      welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Essa eh a PandoraBOT\n\n";
      welcome += "/branco : Para ligar o LED branco\n";
      welcome += "/azul : Para ligar o LED branco\n";
      welcome += "/amarelo : Para ligar o LED branco\n";
      welcome += "/vermelho : Para ligar o LED branco\n";
      welcome += "/verde : Para ligar o LED branco\n";
      welcome += "/branco : Para ligar o LED branco\n";
      welcome += "/rainbow : Para ligar o LED branco\n";
      welcome += "/strobe : Para ligar o LED branco\n";
      welcome += "/temperatura : Para ligar o LED branco\n";
      welcome += "/humidade : Para ligar o LED branco\n";
      welcome += "/pressao : Para ligar o LED branco\n";
      welcome += "/clima : Para ligar o LED branco\n";
      welcome += "/LEDON: Liga o LED em modo randomico\n";
      welcome += "/LEDOFF: Para desligar o LED\n";
      welcome += "/Start : Abre esse menu\n";
      bot.sendMessage(id, welcome, "Markdown");      
      }

      else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
      {
       //  bot.sendSimpleMessage(id, "Comando Invalido", "");
      }
   }

}


void verifica(){

  if((millis() - millisTarefa1) < 300000){

//funcao acende led quando valor de ldr cair

 // valorldr = analogRead(ldr);//Lê o valor do sensor ldr e armazena na variável valorldr

valorldr = 800;//Lê o valor do sensor ldr e armazena na variável valorldr
  Serial.println(valorldr);//Imprime na serial os dados de valorldr

  if ((valorldr) < 500) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
//ledgreen();
Serial.println("Ha algo sobre a mesa");
bot.sendMessage(id,"Alguém colocou algo sobre a mesa");


  }
delay(2000);



//funcao pressao atm
//  valoratm = bmp.readPressure();

valoratm = 1000;
Serial.println(valoratm);

if ((valoratm) < 800) {
//Coloca led em alto para acioná-lo
//ledazul();
Serial.println("Vem chuva por ai");
bot.sendMessage(id,"Vem chuva por ai");
}
delay(2000);
if ((valoratm) > 1100) { //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
//ledamarelo();
Serial.println("Olha o sol , ceu claro!");
bot.sendMessage(id,"Olha o sol , ceu claro!");
}


//funcao temperatura
  
//valortemp = bmp.readTemperature();
valortemp = 30;

Serial.println(valortemp);

if ((valortemp) < 15) { //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
//ledazul();
Serial.println("Cade o casaco de neve?");
bot.sendMessage(id,"Cade o casaco de neve?");
}
delay(2000);

  if ((valortemp) > 38){ //Se o valor de valorldr for menor que 500:
    //Coloca led em alto para acioná-lo
//ledvermelho();
Serial.println("Hora de dar um tibum na cachu!");
bot.sendMessage(id,"Hora de dar um tibum na cachu!");
}
delay(2000);

  
//funcao humidade
sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");
  



//valorhumi = (humidity.relative_humidity);//Lê o valor do sensor ldr e armazena na variável valorldr
valorhumi = 50;//Lê o valor do sensor ldr e armazena na variável valorldr

Serial.println(valorhumi);//Imprime na serial os dados de valorldr

if ((valorhumi) < 30) { //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
//ledazul();
Serial.println("Tempo seco, beba agua");
bot.sendMessage(id,"Tempo seco beba agua");
}
delay(2000);
if ((valorhumi) > 95){ //Se o valor de valorldr for menor que 500:
//Coloca led em alto para acioná-lo
//ledazul();
Serial.println("Tempo umido, ta chovendo?");
bot.sendMessage(id,"Tempo umido ta chovendo?");
}
delay(2000);


}}

// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
