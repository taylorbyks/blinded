// Author: Taylor Byk Schinvelksi

#include "Arduino.h"

// Libs MP3
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

// Libs RFID
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 // PINO SDA
#define RST_PIN 9 // PINO DE RESET
#define TX_PIN 5 // PINO TX
#define RX_PIN 3 // PINO RX

//PASSAGEM DE PARÂMETROS REFERENTE AOS PINOS
SoftwareSerial mySoftwareSerial(TX_PIN, RX_PIN);
MFRC522 rfid(SS_PIN, RST_PIN);

DFRobotDFPlayerMini MP3Player;

String Tag_RFID = "";

void encode(){ // Gera tag RFID
  Tag_RFID = "";
  for (byte i = 0; i < 4; i++) {
    Tag_RFID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  Tag_RFID.toUpperCase();
}

void setup()
{
  mySoftwareSerial.begin(9600);//Comunicacao serial com o modulo
  Serial.begin(115200);//Inicializa a serial do Arduino
  SPI.begin(); //inicializa barramento SPI
  rfid.PCD_Init(); //Inicializa leitor RFID

  Serial.println(F("Inicializando modulo MP3... (3~5 segundos)"));
  if (!MP3Player.begin(mySoftwareSerial))
  {
    Serial.println(F("Nao inicializado:"));
    Serial.println(F("1.Cheque as conexoes do MP3"));
    Serial.println(F("2.Insira um cartao SD"));
    while (true);
  }
  Serial.println(F("Modulo MP3 inicializado!"));

  //Definicoes iniciais
  MP3Player.setTimeOut(500); //Timeout serial 500ms
  MP3Player.volume(5); //Volume 5
  MP3Player.EQ(0); //Equalizacao normal

  //Mostra os MP3 no cartao SD
  Serial.print("Numero de arquivos no cartao SD: ");
  Serial.println(MP3Player.readFileCounts(DFPLAYER_DEVICE_SD));
}

void loop()
{
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) // Le o RFID
    return; // Retorna para ler novamente
    
  encode();
  
  if(Tag_RFID == "51:70:23:D9"){
    Serial.println("Local 1");
    MP3Player.play(1);  
  }
  if(Tag_RFID == "AA:68:8F:C9"){
    Serial.println("Local 2");
    MP3Player.play(2);  
  }

  rfid.PICC_HaltA(); //Para a leitura
  rfid.PCD_StopCrypto1(); //Para a criptografia
}
