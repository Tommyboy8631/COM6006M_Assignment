#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <U8x8lib.h>

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const int B = 4275;               //Temp coverter check to verify later
const int R0 = 100000;            




const int pinLightSensor = A0;
const int pinTempSensor = A1;


const char mqttBroker[] = "192.168.1.121";
const int mqttPort = 1883;
const char textLogTopic[] = "textLog";
const char lightSensorTopic[] = "homeassistant/lightSensor";
const char tempSensorTopic[] = "homeassistant/TempSensor";
const char lightOnTopic[] = "homeassistant/lightOn";

void setup() {
  char ssid[] = "House_TinterWeb";   
  char pass[] = "TomIsACunt";    
  int status = WL_IDLE_STATUS;     
  Serial.begin(9600);
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);
  if (status != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while(true);
  }else {
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("with the IP address: ");
    Serial.println(WiFi.localIP());
  }

  Serial.println("Attempting to connect to your MQTT broker : ");
  Serial.println(mqttBroker);

  while(!mqttClient.connect(mqttBroker, mqttPort)){
    Serial.println("Mqtt Connection failed error code = ");
    Serial.println(mqttClient.connectError());
  }

  Serial.println("Your connected to the Mqtt broker!");
}


void loop() {
  // put your main code here, to run repeatedly:
  int lightSensorValue = analogRead(pinLightSensor);
  Serial.println("Light: ");
  Serial.println(lightSensorValue);
  mqttClient.beginMessage(lightSensorTopic);
  mqttClient.print(lightSensorValue);
  mqttClient.endMessage();
  mqttClient.beginMessage(lightOnTopic);
  if (lightSensorValue < 100){
    mqttClient.print("Yes");
  }else{
    mqttClient.print("No");
  }
  mqttClient.endMessage();

  int tempSensorValue = analogRead(pinTempSensor);
  int B = 3975;

  float R = (float) (1023-tempSensorValue)*10000/tempSensorValue;
  float temperature = 1/(log(R/10000)/B+1/298.15)-276.13;

  Serial.println("Temp: ");
  Serial.println(temperature);
  mqttClient.beginMessage(tempSensorTopic);
  mqttClient.print(temperature);
  mqttClient.endMessage();
  


  delay(1000);
}
