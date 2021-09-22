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

#define LED1         9 // PB0
#define LED2        10 // PB1
#define LED3        11 // PB2

#define RST_DSP     12 // PB3
#define SWITCH      13 // PB4

#define NUM_FLASH 4

uint8_t address = 0;

void setPedal(uint8_t address) {
    digitalWrite(Addr0, (address & 1) == 1);
    digitalWrite(Addr1, (address & 2) == 2);
    digitalWrite(Addr2, (address & 4) == 4);

    switch(address) {
        default:
        case 0:
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, HIGH);
            break;
        case 1:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, LOW);
            digitalWrite(LED3, HIGH);
            break;
        case 2:
            digitalWrite(LED1, HIGH);
            digitalWrite(LED2, HIGH);
            digitalWrite(LED3, LOW);
            break;
        case 3:
            digitalWrite(LED1, LOW);
            digitalWrite(LED2, HIGH);
            analogWrite(LED3, 5);
            break;
    }
    
    digitalWrite(RST_DSP, LOW);
    pinMode(RST_DSP, OUTPUT); // Pull low
    delay(10);
    pinMode(RST_DSP, INPUT);
    delay(250);
}

void setup() {
    pinMode(Addr0, OUTPUT);
    pinMode(Addr1, OUTPUT);
    pinMode(Addr2, OUTPUT);

    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    
    pinMode(RST_DSP, INPUT); // Hi-Z
    pinMode(SWITCH, INPUT_PULLUP);

    setPedal(address);
}



void loop() {
    if(digitalRead(SWITCH) == LOW) {
         if(++address >= NUM_FLASH) { 
             address = 0;
         }
         setPedal(address);
    }
}
