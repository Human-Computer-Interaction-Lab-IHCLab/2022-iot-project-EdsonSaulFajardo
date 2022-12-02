#include "UbidotsEsp32Mqtt.h"
//(primer ubidots con cuenta de udec)const char *UBIDOTS_TOKEN = "BBF-tCFhmihtIiJKhNvEafFTc6wYXzvHAr";  // Put here your Ubidots TOKEN
const char *UBIDOTS_TOKEN = "BBF-dM2ia3tqtxfUFjiLFVgGEoWpsHU6un";  // Put here your Ubidots TOKEN
const char *WIFI_SSID = "LabArquitectura";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "L4bArqui2022";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "niveles_iot";   // Put here your Device label to which data  will be published

const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

const int trigPin = 5;      // configura los pines con su numero correspondiente
const int echoPin = 18;     //
const int trigPin2 = 23;    //  
const int echoPin2 = 22;    //
const int bomba = 13;
#define SOUND_SPEED 0.034 //

long duracion;
float distanceCm;
long duracion2;
float distanceCm2;
Ubidots ubidots(UBIDOTS_TOKEN);


void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
  
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();


 
 //______________________________ 
Serial.begin(115200);       // Es para iniciar la lectura de datos en el puerto serial con una tasa de lectura de baudios
  pinMode(trigPin, OUTPUT); // Habilitar los pines de salida para sensor ultrasonico
  pinMode(echoPin, INPUT); // Habilitar los pines de entrada para sensor ultrasonico
  pinMode(trigPin2, OUTPUT); // Habilitar los pines de salida para sensor ultrasonico
  pinMode(echoPin2, INPUT);
  pinMode(bomba, OUTPUT);   // Habilitar los pines de salida pars
  

}
  
//_______________________________  

//______________________________
void loop() {
  if (!ubidots.connected())
  {
    ubidots.reconnect();
  }
  // Es la iniciacion del ultrasonido para tener previo un dato bajo
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
//____________________________// Es para leer el ultrasonido el dato de regreso
duracion = pulseIn(echoPin, HIGH);  // Se guarda el dato en un duration
distanceCm = duracion * SOUND_SPEED/2;

Serial.print("DistanceT (cm): ");
Serial.println(distanceCm);
 
  delay(1000);
 digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
//____________________________// Es para leer el ultrasonido el dato de regreso
duracion2 = pulseIn(echoPin2, HIGH);  // Se guarda el dato en un duration
distanceCm2 = duracion2 * SOUND_SPEED/2;

Serial.print("DistanceC (cm): ");
Serial.println(distanceCm2);
 
  delay(1000);
 
if(distanceCm >= 7.9 && distanceCm2 <=8.5)
{
  Serial.println("Bomba de llenado tinaco en edificio encendida");// la logica para encender el led  encendida HIGH, pero para RELAY SE INVIERTE
  digitalWrite(bomba,HIGH); 
}
else  {
    Serial.println("Bomba de llenado tinaco en edificio apagada");
    digitalWrite(bomba, LOW); 
  
}
 
 ubidots.add("Bomba", digitalRead (bomba)); 
 ubidots.add("NivelCisterna",distanceCm2 );
 ubidots.add("NivelTinaco", distanceCm);
    ubidots.publish(DEVICE_LABEL);

 ubidots.loop();
}
