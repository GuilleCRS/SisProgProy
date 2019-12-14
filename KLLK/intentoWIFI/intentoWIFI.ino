#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN D4
#define RST_PIN D2
MFRC522 mfrc522(SS_PIN, RST_PIN); 


#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "INFINITUM1892";  //ENTER YOUR WIFI SETTINGS
const char *password = "ra170080128";

//Web/Server address to read/write from 
const char *host = "192.168.43.128";   //https://circuits4you.com website or IP address of server

//=======================================================================
//                    Power on setup
//=======================================================================

void setup() {
  delay(1000);
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Se previene el error de reconexion, se apaga para otro intento
  delay(1000);
  WiFi.mode(WIFI_STA);        //Previenene que el modulo sea visible como un hotspot
  
  WiFi.begin(ssid, password);     //Iniciamos Conexion con el wifi
  Serial.println("");

  Serial.print("Conectando");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Si la conexion es exitosa se muestra la ip y la conexion mediante serial
  Serial.println("");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("Direccion IP: ");
  Serial.println(WiFi.localIP()); 


  SPI.begin();    
  mfrc522.PCD_Init();   // Iniciamos la conexion rfid
  Serial.println("Aproxima tu tarjeta al lector");
  Serial.println();
}

void loop() 
{
  // Si no hay tarjetas presentes regresa
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Si npo hay tarjetas para leer regresa
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  Serial.print("UID/reg_hash :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  content.toUpperCase();
  String UID=content;
  HTTPClient http; //Iniciamos el cliente http
  String reg_hash;
  String getD = "?id=\""+UID+"\"";
  String link = "http://192.168.1.69/busca.php/" + getD;
  Serial.print("Peticion: ");
  Serial.println(link);
  http.begin(link);
  int httpCode = http.GET();            //Manda la peticion
  reg_hash = http.getString();    //Se obtiene la respuesta de la peticion
  reg_hash.replace(" ",""); //Se remueven posibles espacios durante el registro
 
  //Serial.println(httpCode);   //Se imprime el codigo de conexion php
  //Serial.println(reg_hash);    //Se imprime la respuesta de la peticion
  if(UID==reg_hash){
    Serial.println("Se encontro en el registro\nAcceso Autorizado\n");
    delay(500);
    Serial.print("+");
  }else{
    Serial.println("No se encuentra registrado\nAcceso Denegado\n");
    delay(500);
    Serial.print("*");
  }


  http.end();  //Cerramos la conexion
  
  delay(2000);  //Tiempo de espera entre escan
  /*
  if (UID == "19 45 D7 98") //change here the UID of the card/cards that you want to give access
  {
    delay(250);
    Serial.println("Acceso Autorizado");
    Serial.print("+");
    delay(1000);

  }
 
 else   {
    Serial.println("Acceso denegado");
    Serial.print("*");
    delay(1000);
  }*/
} 
