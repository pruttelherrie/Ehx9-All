/*
 * https://forum.arduino.cc/t/attiny-25-45-85-2313-pin-numbers/81499/4
 * 
 * #define PIN_D0  ( 0)
 * #define PIN_D1  ( 1)
 * #define PIN_A1  ( 2)
 * #define PIN_A0  ( 3)
 * #define PIN_D2  ( 4)
 * #define PIN_D3  ( 5)
 * #define PIN_D4  ( 6)
 * #define PIN_D5  ( 7)
 * #define PIN_D6  ( 8)
 * #define PIN_B0  ( 9)
 * #define PIN_B1  (10)
 * #define PIN_B2  (11)
 * #define PIN_B3  (12)
 * #define PIN_B4  (13)
 * #define PIN_B5  (14)
 * #define PIN_B6  (15)
 * #define PIN_B7  (16)
 * #define PIN_A2  (17) 
 */

#define Addr0       16 // PB7
#define Addr1       15 // PB6
#define Addr2       14 // PB5

#define RST_DSP     12 // PB3

void setPedal(uint8_t address) {
    digitalWrite(Addr0, (address & 1) == 1);
    digitalWrite(Addr1, (address & 2) == 2);
    digitalWrite(Addr2, (address & 4) == 4);

    digitalWrite(RST_DSP, LOW);
    pinMode(RST_DSP, OUTPUT); // Pull low
    delay(10);
    pinMode(RST_DSP, INPUT);
    delay(50);
}

void setup() {
    Serial.begin(31250); // MIDI speed
  
    pinMode(Addr0, OUTPUT);
    pinMode(Addr1, OUTPUT);
    pinMode(Addr2, OUTPUT);
    
    pinMode(RST_DSP, INPUT); // Hi-Z

    setPedal(0);
}



void loop() {

    // Just take any byte as a pedal change. 
    // This could be made smarter with for example Program Changes.
    
    uint8_t inByte;
    
    if(Serial.available() > 0) {
        inByte = Serial.read();
        setPedal((uint8_t)inByte);
    }
}
