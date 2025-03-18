#include <WiFi.h>
#include <MQTT.h>
#include <LCD_I2C.h>

#define D1 23
#define D2 5
#define D3 19
#define D4 18

#define STEPS_PER_REVOLUTION 600
#define DELAY_BETWEEN_STEPS 10

const char ssid[] = "@JumboPlusIoT";
const char pass[] = "czsdr6bn";
const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_topic[] = "group215/command";
const char mqtt_client_id[] = "arduino_group_215";
int MQTT_PORT = 1883;
int toggle = 0;  // 0: Curtain closed, 1: Curtain open
WiFiClient net;
MQTTClient client;

LCD_I2C lcd(0x27, 16, 2);

void connect() {
  Serial.print("Checking WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nConnecting to MQTT...");
  while (!client.connect(mqtt_client_id)) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected to MQTT!");

  client.subscribe(mqtt_topic);
}

void messageReceived(String &topic, String &payload) {
  Serial.println("Incoming message: " + topic + " - " + payload);

  lcd.clear(); // Clear the LCD before printing new message

  if (payload == "on") {
    lcd.print("     CURTAIN");
    lcd.setCursor(6, 1);
    lcd.print("OPEN!");
    toggle = 1;
    moveClockwise();
  }
  if (payload == "off") {
    lcd.print("     CURTAIN");
    lcd.setCursor(6, 1);
    lcd.print("CLOSED!");
    toggle = 0;
    moveCounterclockwise();
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);

  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);

  lcd.begin();
  lcd.backlight();

  connect();
}
void loop() {
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }
}

void moveClockwise() {
  int totalSteps = STEPS_PER_REVOLUTION / 2;

  for (int i = 0; i < totalSteps; i++) {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    delay(7);  // Adjust the delay based on your motor's requirements
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
    delay(7);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    delay(7);
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    delay(7);
  }
  stop();
}

void moveCounterclockwise() {
  int totalSteps = STEPS_PER_REVOLUTION / 2;

  for (int i = 0; i < totalSteps; i++) {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    delay(7);  // Adjust the delay based on your motor's requirements
    digitalWrite(D1, LOW);
    digitalWrite(D2, HIGH);
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW);
    delay(7);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
    delay(7);
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
    delay(7);
  }
  stop();
}

void stop() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW);
}