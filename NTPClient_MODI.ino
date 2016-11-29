/*

 Udp NTP Client

 Get the time from a Network Time Protocol (NTP) time server
 Demonstrates use of UDP sendPacket and ReceivePacket
 For more on NTP time servers and the messages needed to communicate with them,
 see http://en.wikipedia.org/wiki/Network_Time_Protocol

 created 4 Sep 2010
 by Michael Margolis
 modified 9 Apr 2012
 by Tom Igoe
 updated for the ESP8266 12 Apr 2015 
 by Ivan Grokhotkov

 This code is in the public domain.

*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <MaxMatrix.h>
#include <TimeLib.h>

// Esta parte define la presentacion de caracteres en los displays
PROGMEM const char CH[] = {
  5, 8, B00000000, B00000000, B00000000, B00000000, B00000000, // space
  5, 8, B00000000, B00000000, B01011111, B00000000, B00000000, // !
  5, 8, B00000000, B00000011, B00000000, B00000011, B00000000, // "
  5, 8, B00010100, B01111111, B00010100, B01111111, B00010100, // #
  5, 8, B00100100, B00101010, B01111111, B00101010, B00010010, // $
  5, 8, B00100011, B00010011, B00001000, B01100100, B01100010, // %
  5, 8, B00110110, B01001001, B01010110, B00100000, B01010000, // &
  5, 8, B00000011, B00000000, B00000000, B00000000, B00000000, // '
  5, 8, B00011100, B00100010, B01000001, B00000000, B00000000, // (
  5, 8, B01000001, B00100010, B00011100, B00000000, B00000000, // )
  5, 8, B00101000, B00011000, B00001110, B00011000, B00101000, // *
  5, 8, B00001000, B00001000, B00111110, B00001000, B00001000, // +
  5, 8, B10110000, B01110000, B00000000, B00000000, B00000000, // ,
  5, 8, B00001000, B00001000, B00001000, B00001000, B00000000, // -
  5, 8, B01100000, B01100000, B00000000, B00000000, B00000000, // .
  5, 8, B01100000, B00011000, B00000110, B00000001, B00000000, // /
  5, 8, B00111110, B01010001, B01001001, B01000101, B00111110, // 0
  5, 8, B00000000, B01000010, B01111111, B01000000, B00000000, // 1
  5, 8, B01000010, B01100001, B01010001, B01001001, B01000110, // 2
  5, 8, B00100001, B01000001, B01000101, B01001011, B00110001, // 3
  5, 8, B00011000, B00010100, B00010010, B01111111, B00010000, // 4
  5, 8, B00100111, B01000101, B01000101, B01000101, B00111001, // 5
  5, 8, B00111100, B01001010, B01001001, B01001001, B00110000, // 6
  5, 8, B00000011, B00000001, B01110001, B00001001, B00000111, // 7
  5, 8, B00110110, B01001001, B01001001, B01001001, B00110110, // 8
  5, 8, B00000110, B01001001, B01001001, B00101001, B00011110, // 9
  4, 8, B00000000, B00110110, B00110110, B00000000, B00000000, // :
  5, 8, B10000000, B01010000, B00000000, B00000000, B00000000, // ;
  5, 8, B00010000, B00101000, B01000100, B00000000, B00000000, // <
  5, 8, B00010100, B00010100, B00010100, B00000000, B00000000, // =
  5, 8, B01000100, B00101000, B00010000, B00000000, B00000000, // >
  5, 8, B00000010, B01011001, B00001001, B00000110, B00000000, // ?
  5, 8, B00111110, B01001001, B01010101, B01011101, B00001110, // @
  5, 8, B01111110, B00010001, B00010001, B00010001, B01111110, // A
  5, 8, B01111111, B01001001, B01001001, B01001001, B00110110, // B
  5, 8, B00111110, B01000001, B01000001, B01000001, B00100010, // C
  5, 8, B01111111, B01000001, B01000001, B01000001, B00111110, // D
  5, 8, B01111111, B01001001, B01001001, B01000001, B01000001, // E
  5, 8, B01111111, B00001001, B00001001, B00000001, B00000001, // F
  5, 8, B00111110, B01000001, B01000001, B01001001, B01111010, // G
  5, 8, B01111111, B00001000, B00001000, B00001000, B01111111, // H
  5, 8, B00000000, B01000001, B01111111, B01000001, B00000000, // I
  5, 8, B00110000, B01000000, B01000001, B00111111, B00000000, // J
  5, 8, B01111111, B00001000, B00010100, B01100011, B00000000, // K
  5, 8, B01111111, B01000000, B01000000, B01000000, B00000000, // L
  5, 8, B01111111, B00000010, B00001100, B00000010, B01111111, // M
  5, 8, B01111111, B00000100, B00001000, B00010000, B01111111, // N
  5, 8, B00111110, B01000001, B01000001, B01000001, B00111110, // O
  5, 8, B01111111, B00001001, B00001001, B00000110, B00000000, // P
  5, 8, B00111110, B01000001, B01000001, B10111110, B00000000, // Q
  5, 8, B01111111, B00001001, B00001001, B01110110, B00000000, // R
  5, 8, B01000110, B01001001, B01001001, B00110010, B00000000, // S
  5, 8, B00000001, B00000001, B01111111, B00000001, B00000001, // T
  5, 8, B00111111, B01000000, B01000000, B00111111, B00000000, // U
  5, 8, B00001111, B00110000, B01000000, B00110000, B00001111, // V
  5, 8, B00111111, B01000000, B00111000, B01000000, B00111111, // W
  5, 8, B01100011, B00010100, B00001000, B00010100, B01100011, // X
  5, 8, B00000111, B00001000, B01110000, B00001000, B00000111, // Y
  5, 8, B01100001, B01010001, B01001001, B01000111, B00000000, // Z
  5, 8, B01111111, B01000001, B00000000, B00000000, B00000000, // [
  5, 8, B00000001, B00000110, B00011000, B01100000, B00000000, // \ backslash
  5, 8, B01000001, B01111111, B00000000, B00000000, B00000000, // ]
  5, 8, B00000010, B00000001, B00000010, B00000000, B00000000, // hat
  5, 8, B01000000, B01000000, B01000000, B01000000, B00000000, // _
  5, 8, B00000001, B00000010, B00000000, B00000000, B00000000, // `
  4, 8, B00100000, B01010100, B01010100, B01111000, B00000000, // a
  4, 8, B01111111, B01000100, B01000100, B00111000, B00000000, // b
  4, 8, B00111000, B01000100, B01000100, B00101000, B00000000, // c
  4, 8, B00111000, B01000100, B01000100, B01111111, B00000000, // d
  4, 8, B00111000, B01010100, B01010100, B00011000, B00000000, // e
  3, 8, B00000100, B01111110, B00000101, B00000000, B00000000, // f
  4, 8, B10011000, B10100100, B10100100, B01111000, B00000000, // g
  4, 8, B01111111, B00000100, B00000100, B01111000, B00000000, // h
  3, 8, B01000100, B01111101, B01000000, B00000000, B00000000, // i
  4, 8, B01000000, B10000000, B10000100, B01111101, B00000000, // j
  4, 8, B01111111, B00010000, B00101000, B01000100, B00000000, // k
  3, 8, B01000001, B01111111, B01000000, B00000000, B00000000, // l
  5, 8, B01111100, B00000100, B01111100, B00000100, B01111000, // m
  4, 8, B01111100, B00000100, B00000100, B01111000, B00000000, // n
  4, 8, B00111000, B01000100, B01000100, B00111000, B00000000, // o
  4, 8, B11111100, B00100100, B00100100, B00011000, B00000000, // p
  4, 8, B00011000, B00100100, B00100100, B11111100, B00000000, // q
  4, 8, B01111100, B00001000, B00000100, B00000100, B00000000, // r
  4, 8, B01001000, B01010100, B01010100, B00100100, B00000000, // s
  3, 8, B00000100, B00111111, B01000100, B00000000, B00000000, // t
  4, 8, B00111100, B01000000, B01000000, B01111100, B00000000, // u
  5, 8, B00011100, B00100000, B01000000, B00100000, B00011100, // v
  5, 8, B00111100, B01000000, B00111100, B01000000, B00111100, // w
  5, 8, B01000100, B00101000, B00010000, B00101000, B01000100, // x
  4, 8, B10011100, B10100000, B10100000, B01111100, B00000000, // y
  3, 8, B01100100, B01010100, B01001100, B00000000, B00000000, // z
  3, 8, B00001000, B00110110, B01000001, B00000000, B00000000, // {
  1, 8, B01111111, B00000000, B00000000, B00000000, B00000000, // |
  3, 8, B01000001, B00110110, B00001000, B00000000, B00000000, // }
  4, 8, B00001000, B00000100, B00001000, B00000100, B00000000, // ~
};

// A partir de este codigo, añado el control de las Matrices de LEDs

int data = D2;     // DIN pin del modulo MAX7219
int load = D1;     // CS  pin del modulo MAX7219
int clock = D0;    // CLK pin del modulo MAX7219
int maxInUse = 4;  //Cambiar esta variable para ajustar el numero de MAX7219 en uso
MaxMatrix m(data, load, clock, maxInUse); // Definimos el display con 4 modulos
                                          // DIN en D2, CS en D1 y CLK en D0
byte buffer[10];
char cadena[] = "00:00"; // Cadenas de caracteres para pruebas

// Aqui definimos los parametros de nuestra WIFI
const char ssid[] = "Arduino";                   //  tu  SSID (nombre de la WIFI)
const char pass[] = "8399-ITZ-269-0014";         //  la clave de tu WIFI
const int timeZone = 1;                          //  Zona Horaria

IPAddress timeServer(150,214,94,5); // hora.roa.es NTP server
IPAddress timeServerIP; // hora.roa.es NTP server address
const char* ntpServerName = "hora.roa.es";

const int NTP_PACKET_SIZE = 48; // la hora NTP se encuentra en los primeros 48 bytes del mensaje
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer para guardar los paquetes entrantes y salientes
const unsigned int localPort = 2390;      // puerto local para recibir paquetes UDP
// Canal UDP para enviar y recibir paquetes sobre UDP ( 2390 en nuestro caso)
WiFiUDP udp;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  m.init(); // Inicializar el modulo
  m.setIntensity(15); // Intensidad de la matriz 0-15
//  printString(cadena);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  Serial.print(".");
  }
 Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
//  setSyncProvider(getNtpTime);
}

void loop()
{
  //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
  }
  else {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:
    unsigned long secsSince1900;
    secsSince1900 =  (unsigned long)packetBuffer[40] << 24; // Convertimos los 4 bytes desde la posicion 40 en un entero largo
    secsSince1900 |= (unsigned long)packetBuffer[41] << 16; // El valor de este entero largo corresponde a los segundos
    secsSince1900 |= (unsigned long)packetBuffer[42] << 8;  // transcurridos desde el 1 de Enero de 1900
    secsSince1900 |= (unsigned long)packetBuffer[43]; 

    // Ahora oonvertimos el horario NTP en el horario de cada dia:
    // El horario Unix comienza el 1 de Enero de 1970. En segundos, son 2208988800
    unsigned long ahora = secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    
    setTime(ahora);
    int menor  =  minute();
    int mayor  =  hour();
    int resto  =  menor % 10;
    int alto   =  menor /10;
    int restom =  mayor % 10;
    int altom  =  mayor /10;
  
    cadena[4]  =  char (resto+48);
    cadena[3]  =  char (((menor - resto)/10)+48);
    cadena[1]  =  char (restom+48);
    cadena[0]  =  char (((mayor - restom)/10)+48);
    printString(cadena);
  
    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((ahora  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    Serial.print(':');
    if ( ((ahora % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((ahora  % 3600) / 60); // print the minute (3600 equals secs per minute)
    Serial.print(':');
    if ( (ahora % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(ahora % 60); // print the second
  }

  // wait ten seconds before asking for the time again
  delay(1000);
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address)
{
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();

  }
void printCharWithShift(char c, int shift_speed){
  if (c < 32) return;
    c -= 32;
    memcpy_P(buffer, CH + 7*c, 7);
    m.writeSprite(maxInUse*8, 0, buffer);
    m.setColumn(maxInUse*8 + buffer[0], 0);
    for (int i=0; i<buffer[0]+1; i++)
      {
      delay(shift_speed);
      m.shiftLeft(false, false);
      }
}  

void printStringWithShift(char* s, int shift_speed){
  while (*s != 0){
    printCharWithShift(*s, shift_speed);
    s++;
  }
}

void printString(char* s) {
  int col = 2;
  while (*s != 0)
  {
    if (*s < 32) continue;
    char c = *s - 32;
    Serial.print(*s-32);
    Serial.print("W ");
    memcpy_P(buffer, CH + 7*c, 7);
    m.writeSprite(col, 0, buffer);
    m.setColumn(col + buffer[0], 0);
    col += buffer[0] + 1;
    s++;
  }
}

