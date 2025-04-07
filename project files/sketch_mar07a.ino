#include <Wire.h>
#include <U8g2lib.h>

#define TRIG_PIN 14  // D5
#define ECHO_PIN 12  // D6
#define MQ_PIN A0
#define BUZZER_PIN D0
#define LED_PIN D3  // Added LED pin

// Initialize U8g2 for I2C OLED
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);  // Set LED as output

    // Initialize OLED display
    u8g2.begin();
}

void loop() {
    int GasValue = analogRead(MQ_PIN);
    Serial.print("GAS Value:");
    Serial.println(GasValue);
  
    long duration;
    float distance;

    // Trigger the sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read echo pulse duration
    duration = pulseIn(ECHO_PIN, HIGH);

    // Convert duration to distance (in cm)
    distance = (duration * 0.0343) / 2;

    String status;
    if (distance <= 5) {
        status = "\xe2\x9a\xa8 Heavy Load";  // 🚨
        digitalWrite(BUZZER_PIN, HIGH);
        digitalWrite(LED_PIN, HIGH);  // Turn LED ON
    } else if (distance > 5 && distance <= 15) {
        status = "\xe2\x9a\xa0 Loaded";  // ⚠️
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);  // Turn LED OFF
    } else if (distance > 15 && distance <= 25) {
        status = "\xe2\x9c\x85 Normal";  // ✅
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
    } else {
        status = "\xf0\x9f\x9f\xa2 Very Normal";  // 🟢
        digitalWrite(BUZZER_PIN, LOW);
        digitalWrite(LED_PIN, LOW);
    }

    Serial.println(status);
    
    // Display on OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB08_tr);
    u8g2.setCursor(0, 20);
    u8g2.print("Dustbin Level:");
    u8g2.setCursor(0, 35);
    u8g2.print("Distance: ");
    u8g2.print(distance);
    u8g2.print(" cm");
    u8g2.setCursor(0, 55);
    u8g2.print(status);
    u8g2.sendBuffer();
    
    delay(1000);  // Wait 1 second before next reading
}
