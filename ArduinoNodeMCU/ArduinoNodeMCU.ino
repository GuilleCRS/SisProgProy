#include <Servo.h>
#include <SoftwareSerial.h>
#define ServoPin 9
SoftwareSerial SerialNMCU(2,3); // rx tx

Servo myservo;
char data;
int autorizado=6,denegado=7 ;
int interval=15000;
int focos=4;
unsigned long seInicio=0;
void setup() {

  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialNMCU.begin(9600);
  myservo.attach(9);
  myservo.write(0);
  pinMode(autorizado,OUTPUT);
  pinMode(denegado,OUTPUT);
  pinMode(focos,OUTPUT);

}

void loop() {
  checkSerialComm();
  delay(500);

}
void checkSerialComm(){
  if(SerialNMCU.available()>0){
    while(SerialNMCU.available()>0){
      data =(char) SerialNMCU.read();
      Serial.print(data);
       if(data=='*'){
      den();
      myservo.write(0);

    }if(data=='+'){
        myservo.write(0);
       aut();
       myservo.write(180);
       digitalWrite(focos,HIGH);
       seInicio=millis();
    } 
    }
  }

  if ((unsigned long)(millis() - seInicio) >= interval) {
     digitalWrite(focos,LOW);
  }
}
void aut(){
  digitalWrite(autorizado,HIGH);
  delay(500);
  digitalWrite(autorizado,LOW);
}
void den(){
   for(int i=0;i<3;i++){
        digitalWrite(denegado,HIGH);
        delay(100);//wait for 2ms
        digitalWrite(denegado,LOW);
        delay(100);//wait for 2ms
    }
}
