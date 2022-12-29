#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

//WI-FI
const char *ssid = "SMART_PJU";
const char *password = "iotworkshop2021";

//RABBIT MQ
const char *mqtt_broker = "cloudrmqserver.pptik.id";
const char *topic = "Log";
const char *mqtt_username = "/kapalselam:kapalselam";
const char *mqtt_password = "1245and4512";
const int mqtt_port = 1883;
const char *guid = "53e91d90-4333-42dc-ac03-2243575834d8";
const char *name = "Bell Lskk Pelajar Pejuang";
bool shouldSaveConfig = false;


int relay1 = D1 ;
int relay2 = D2 ;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  setup_wifi ();
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = guid;
    Serial.printf("The client %s connects to the rabbitmq \n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("RabbitMQ connected !");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe(guid);
}

void callback(char *guid, byte *payload, unsigned int length) {
  char message [7] ;
  Serial.println(topic);
  Serial.println("Pesan :");
  for (int i = 0; i < length; i++) {
    Serial.print((char) payload[i]);
    message[i] = (char)payload[i];
  }
  String convertMsg = String(message) ;
  if (message[0] == '0') {
    digitalWrite(relay1, LOW);
  } else if (message[0] == '1') {
    digitalWrite(relay1, HIGH);
  } else {
    digitalWrite(relay1, HIGH);
  }
  delay(2000);
  digitalWrite(relay1, HIGH);
}

void loop() {
  client.loop();
}

void sensor () {}

void setup_wifi () {
  WiFiManager wifiManager;
  wifiManager.setSaveConfigCallback(saveConfigCallback);
  wifiManager.setAPStaticIPConfig(IPAddress(192,168,4,1), IPAddress(192,168,4,1), IPAddress(255,255,255,0));
  if(!wifiManager.autoConnect("Bell IoT")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  } 
  Serial.println("Wi-Fi connected...)");
}

void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
