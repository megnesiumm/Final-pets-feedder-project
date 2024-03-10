#include <WiFi.h>
#include <PubSubClient.h>

// Replace the next variables with your SSID/Password combination
const char* ssid = "Spothot";
const char* password = "24604444";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.5.204";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// LED Pin
const int ledPin = 2;

#include <ESP32Servo.h>
Servo sg90;

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  // status = bme.begin();
  // if (!bme.begin(0x44)) {
  //   Serial.println("Could not find a valid SHT31 sensor, check wiring!");
  //   while (1)
  //     ;
  // }
  setup_wifi();
  client.setServer(mqtt_server, 18883);
  client.setCallback(callback);

  pinMode(ledPin, OUTPUT);

  sg90.attach(2);  // ระบุขาที่เชื่อมกับไซโรมอเตอร์
  sg90.write(90);
}

void setup_wifi() {
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.printf("WiFi Failed!\n");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.println(messageTemp);

    if (messageTemp == "on") {
      sg90.write(0);
      delay(300);
      sg90.write(90);
    }
    // else if (messageTemp == "off") {
    //   sg90.write(0);
    // }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", "sub1", "passsub1")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output", 1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;

    // // Temperature in Celsius
    // temperature = bme.readTemperature();
    // // Uncomment the next line to set temperature in Fahrenheit
    // // (and comment the previous temperature line)
    // //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit

    // // Convert the value to a char array
    // char tempString[8];
    // dtostrf(temperature, 1, 2, tempString);
    // Serial.print("Temperature: ");
    // Serial.println(tempString);
    // client.publish("esp32/temperature", tempString,2,true);

    // humidity = bme.readHumidity();

    // // Convert the value to a char array
    // char humString[8];
    // dtostrf(humidity, 1, 2, humString);
    // Serial.print("Humidity: ");
    // Serial.println(humString);
    // client.publish("esp32/humidity", humString ,2,false);
  }
}