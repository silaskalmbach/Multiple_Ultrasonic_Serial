// ---------------------------------------------------------
// NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
// ---------------------------------------------------------
#include <NewPing.h>
#include <ArduinoJson.h>
 
#define SONAR_NUM     2 // Number or sensors.
#define MAX_DISTANCE 500 // Max distance in cm.
#define PING_INTERVAL 33 // Milliseconds between pings.
 
unsigned long pingTimer[SONAR_NUM]; // When each pings.
unsigned int cm[SONAR_NUM]; // Store ping distances.
uint8_t currentSensor = 0; // Which sensor is active.

DynamicJsonDocument doc(512);

NewPing sonar[SONAR_NUM] = { // Sensor object array.
  NewPing(2, 3, MAX_DISTANCE),
  NewPing(4, 5, MAX_DISTANCE)
};
 
void setup() {
  Serial.begin(9600);
  pingTimer[0] = millis() + 75; // First ping start in ms.
  for (uint8_t i = 1; i < SONAR_NUM; i++)
    pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
}
 
void loop() {
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    if (millis() >= pingTimer[i]) {
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;
      if (i == 0 && currentSensor == SONAR_NUM - 1)
        //oneSensorCycle(); // Do something with results.
      sonar[currentSensor].timer_stop();
      currentSensor = i;
      cm[currentSensor] = 0;
      sonar[currentSensor].ping_timer(echoCheck);
    }
  }
  // readSerial();
  writeSerial();
  // The rest of your code would go here.
}
 
void echoCheck() { // If ping echo, set distance to array.
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

void writeSerial(){
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    doc["Sonar"][i] = cm[i];
  }
serializeJson(doc, Serial);
Serial.println();
}
