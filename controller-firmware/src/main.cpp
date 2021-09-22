
#include <Arduino.h>
#include "main.h"
#include "callbacks.h"

// Send command to flash-switcher-PCB
void setPedal(uint8_t pedal, bool reset) {
    if(reset) {
        Serial.write(pedal);
        lastPedal = pedal;
    }
}

void setProgram(uint8_t program) {
    digitalWrite(switchPins[curProgram],HIGH);
    delay(10);
    digitalWrite(switchPins[program],LOW);
    delay(10);
    curProgram = program;
}

void loadPreset(uint8_t preset){
    if(preset >= MAX_PRESETS) {
        return;
    }
    // Load program from memory into menu structure and set effect
    //menuPreset.setCurrentValue(preset);
    menuName.setTextValue( strlen(presetParams[preset].name)  >   0 ? presetParams[preset].name    : "Unnamed");
    menuPedal.setCurrentValue( presetParams[preset].pedal     <   6 ? presetParams[preset].pedal   :   0);
    menuProgram.setCurrentValue( presetParams[preset].program <   9 ? presetParams[preset].program :   0);
    menuDry.setCurrentValue( presetParams[preset].dry         < 101 ? presetParams[preset].dry     :   0);
    menuEffect.setCurrentValue( presetParams[preset].effect   < 101 ? presetParams[preset].effect  : 100);
    menuCtrl1.setCurrentValue( presetParams[preset].ctrl1     < 101 ? presetParams[preset].ctrl1   :  40);
    menuCtrl2.setCurrentValue( presetParams[preset].ctrl2     < 101 ? presetParams[preset].ctrl2   :  60);

    setPedal(menuPedal.getCurrentValue(), lastPedal != menuPedal.getCurrentValue());
    lastPedal = menuPedal.getCurrentValue();
    setProgram(menuProgram.getCurrentValue()); 
}

void printNumber(uint16_t number) {
    char buffer1[16];
    char buffer2[16];
    menuPedal.copyEnumStrToBuffer(buffer1, 16, menuPedal.getCurrentValue());
    strcpy_P(buffer2, programNames[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()]); 

    spr.fillSprite(0);
    spr.setTextColor(7);
    spr.setTextFont(8);
    spr.setTextDatum(TC_DATUM);
    spr.drawNumber(number + 1, 120, 0); // MIDI PC offset

    spr.setFreeFont(&FreeSans18pt7b);
    spr.setTextColor(14);
    spr.drawString(menuName.getTextValue(), 120, 89);

    spr.setFreeFont(&FreeSans12pt7b);
    uint16_t pedalWidth   = tft.textWidth(buffer1);
    uint16_t programWidth = tft.textWidth(buffer2);
    if(setPedalActive) {
        spr.fillRoundRect(120 - (programWidth - pedalWidth)/2 - 6 - pedalWidth - 2, 135 - 4, pedalWidth + 6, 18 + 8, 2, 11);
        spr.setTextColor(0);
    } else {
        spr.setTextColor(11);
    }
    spr.setTextDatum(TR_DATUM);
    spr.drawString(buffer1, 120 - (programWidth - pedalWidth)/2 - 6, 135); 

    if(setProgramActive) {
        spr.fillRoundRect(120 - (programWidth - pedalWidth)/2 + 6 - 2, 135 - 4, programWidth + 2, 18 + 8, 2, 10);
        spr.setTextColor(0);
    } else {
        spr.setTextColor(10);
    }
    spr.setTextDatum(TL_DATUM);
    spr.drawString(buffer2, 120 - (programWidth - pedalWidth)/2 + 6, 135); 

    spr.setTextDatum(TC_DATUM);

#ifdef UI_KNOBS    
    spr.drawCircle(30 + 0 * 60, 240 - 50, 28, 7);
    spr.drawCircle(30 + 1 * 60, 240 - 50, 28, 7);
    spr.drawCircle(30 + 2 * 60, 240 - 50, 28, 7);
    spr.drawCircle(30 + 3 * 60, 240 - 50, 28, 7);
    spr.setTextDatum(MC_DATUM);
    spr.setTextColor(7);
    spr.drawNumber(menuDry.getCurrentValue(),    30 + 0 * 60, 240 - 50);
    spr.drawNumber(menuEffect.getCurrentValue(), 30 + 1 * 60, 240 - 50);
    spr.drawNumber(menuCtrl1.getCurrentValue(),  30 + 2 * 60, 240 - 50);
    spr.drawNumber(menuCtrl2.getCurrentValue(),  30 + 3 * 60, 240 - 50);
    spr.setFreeFont(&FreeSans9pt7b);
    spr.drawString("Dry",     30 + 0 * 60, 240 - 10);
    spr.drawString("Effect",  30 + 1 * 60, 240 - 10);
    spr.drawString(ctrl1Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()],  30 + 2 * 60, 240 - 10);
    spr.drawString(ctrl2Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()],  30 + 3 * 60, 240 - 10);
#elseif UI_BARS
    spr.drawRect(  0, 150, 115, 20, 9);
    spr.drawRect(125, 150, 115, 20, 9);
    spr.drawRect(  0, 200, 115, 20, 9);
    spr.drawRect(125, 200, 115, 20, 9);
    spr.fillRect(  0, 150, (int32_t)(menuDry.getCurrentValue()    / 100.0 * 115), 20, 9);
    spr.fillRect(125, 150, (int32_t)(menuEffect.getCurrentValue() / 100.0 * 115), 20, 9);
    spr.fillRect(  0, 200, (int32_t)(menuCtrl1.getCurrentValue()  / 100.0 * 115), 20, 9);
    spr.fillRect(125, 200, (int32_t)(menuCtrl2.getCurrentValue()  / 100.0 * 115), 20, 9);

    spr.setTextDatum(MC_DATUM);
    spr.setTextColor(7);
    spr.drawString("Dry",     60, 180);
    spr.drawString("Effect", 180, 180);
    spr.drawString(ctrl1Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()],  60, 240 - 10);
    spr.drawString(ctrl2Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()], 180, 240 - 10);

    spr.setTextColor(14);
    spr.drawNumber(menuDry.getCurrentValue(),     60, 158);
    spr.drawNumber(menuEffect.getCurrentValue(), 180, 158);
    spr.drawNumber(menuCtrl1.getCurrentValue(),   60, 208);
    spr.drawNumber(menuCtrl2.getCurrentValue(),  180, 208);
#else 
    spr.drawRect(  0, 170, 115, 30, 9);
    spr.drawRect(125, 170, 115, 30, 9);
    spr.drawRect(  0, 210, 115, 30, 9);
    spr.drawRect(125, 210, 115, 30, 9);
    spr.fillRect(  0, 170, (int32_t)(menuDry.getCurrentValue()    / 100.0 * 115), 30, 9);
    spr.fillRect(125, 170, (int32_t)(menuEffect.getCurrentValue() / 100.0 * 115), 30, 9);
    spr.fillRect(  0, 210, (int32_t)(menuCtrl1.getCurrentValue()  / 100.0 * 115), 30, 9);
    spr.fillRect(125, 210, (int32_t)(menuCtrl2.getCurrentValue()  / 100.0 * 115), 30, 9);

    spr.setTextDatum(MC_DATUM);
    spr.setTextColor(7);
    spr.drawString("Dry",     58, 183);
    spr.drawString("Effect", 182, 183);
    spr.drawString(ctrl1Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()],  58, 223);
    spr.drawString(ctrl2Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()], 182, 223);
#endif

    spr.pushSprite(0,0);
}

// this is the function called by the renderer every 1/5 second once the display is
// taken over, we pass this function to takeOverDisplay().
void myDisplayFunction(unsigned int encoderValue, RenderPressMode clicked) {
    // we initialise the display on the first call.
    if(counter == 0) {
        switches.changeEncoderPrecision(MAX_PRESETS, menuPreset.getCurrentValue());  
        encoderValue = menuPreset.getCurrentValue();
        doUpdate = true;
        counter++;
    }
    // We are told when the button is pressed in by the boolean parameter.
    // When the button is clicked, we give back to the menu..
    if(clicked) {
        tft.setTextSize(1);
        renderer.giveBackDisplay();
        counter = 0;
    } else { // Program Change! Let op: deze kan ook aangeroepen worden door handleProgramChange()
        if( (menuPreset.getCurrentValue() != encoderValue) || doUpdate) { 
            menuPreset.setCurrentValue(encoderValue);
            switches.getEncoder()->setCurrentReading(encoderValue);
            loadPreset(encoderValue);
            printNumber(encoderValue);
            doUpdate = false;
        } 
        counter++;
    }
}

void setup() {
    uint8_t eeprom_initialised=0;
    EEPROM.get(0, eeprom_initialised);
    if(eeprom_initialised != 0xFF) { // not a brand new ROM
      EEPROM.get(1, presetParams);
      EEPROM.get(1 + sizeof(presetParams), settings);
    } else {
        // FIXME: reset all presets!
    }

    for(uint8_t i=0; i<4; i++) {   // set up knob (pot) outputs
        pinMode(knobPins[i], OUTPUT);
        analogWrite(knobPins[i], 255);
    }

    for(uint8_t i=0; i<9; i++) {   // setup program switch
        pinMode(switchPins[i], OUTPUT);
        digitalWrite(switchPins[i], HIGH);
    }  

    encoders.addEncoder(1, 4, PC14, 2, 100, 0);
    encoders.addEncoder(2, 4, PB6,  2, 200, 0);
    encoders.addEncoder(3, 4, PB4,  1, 300, 399);
    encoders.addEncoder(4, 4, PB5,  1, 400, 499);
    encoders.setDebounce(16);

    Serial.setRx(PA3);
    Serial.setTx(PA2);
    Serial.begin(31250);

    // 4-bit (indexed colormap) sprite for buffered DMA transfers
    spr.setColorDepth(4);
    // We use a Sprite as big as the screen. Here we build the contents then transfer it 
    // in one big DMA push.
    spr.createSprite(240, 240);
    // Create a color map with known colors (16 maximum for 4 bit Sprite)
    uint16_t cmap[16];
    cmap[0]  = TFT_BLACK; // We will keep this as black
    cmap[1]  = TFT_NAVY;
    cmap[2]  = TFT_DARKGREEN;
    cmap[3]  = TFT_DARKCYAN;
    cmap[4]  = TFT_MAROON;
    cmap[5]  = TFT_PURPLE;
    cmap[6]  = TFT_PINK;
    cmap[7]  = TFT_LIGHTGREY;
    cmap[8]  = TFT_YELLOW;
    cmap[9]  = tft.color565(0,63,135); // tcMenu Dark Blue instead of TFT_BLUE
    cmap[10] = TFT_GREEN;
    cmap[11] = TFT_CYAN;
    cmap[12] = TFT_RED;
    cmap[13] = TFT_MAGENTA;
    cmap[14] = TFT_WHITE;  // Keep as white for text
    cmap[15] = TFT_BLUE;   // Keep as blue for sprite border
    // Pass the palette to the Sprite class
    spr.createPalette(cmap);

    //renderer.setResetIntervalTimeSeconds(-1);
    renderer.setResetCallback([] {
        counter = 0;
        renderer.takeOverDisplay(myDisplayFunction);
    });

/* // pas vanaf 2.2 proper
    setTitlePressedCallback([](int id) { 
        withMenuDialogIfAvailable([](MenuBasedDialog* dlg) {
            dlg->setButtons(BTNTYPE_CLOSE, BTNTYPE_NONE);
            dlg->showRam("ARM Example", false);
            char menuVer[10];
            tccore::copyTcMenuVersion(menuVer, sizeof menuVer);
            dlg->copyIntoBuffer(menuVer);
            // FIXME:use TimestampedVersion from build_number_defines.h
        }); 
    }); */

    setupMenu();

    // Start your engines...
    loadPreset(menuPreset.getCurrentValue());
    menuSettingsMIDIChannel.setCurrentValue(settings.midichannel);
    tft.fillScreen(TFT_BLACK);

    // Lift-off!
    renderer.takeOverDisplay(myDisplayFunction);
}

void loop() {
    taskManager.runLoop();

    // SPCP: Simplistische Program Change Parser
    // Deze houdt geen rekening met SysEx messages of Realtime messages. 
    // Keep it civil. No bladed weapons, no surprise buttsex.

    // (1) is er een byte beschikbaar in de buffert?
    while(Serial.available()) {
        // (2) lees byte
        c = Serial.read();
        // (3) als we toe waren aan de data van de PC, neem deze over
        if(next_is_pc) {
            menuPreset.setCurrentValue(c);
            if(counter > 0) {
                switches.getEncoder()->setCurrentReading(c);
                loadPreset(c);
                printNumber(c);
            }
            next_is_pc = false;
        }
        // (4) check of de volgende byte een PC data byte is
        if( (settings.midichannel == 0 || settings.midichannel == (c & 0b1111) ) && ( (c >> 4) == 0b1100) ) {
            next_is_pc = true;
        }
    }

    int code = encoders.readAll();

    // Keep the code explicit to keep it readable...
    if(counter > 0) {
        if(encoders.getMode(1) == 1) { 
            if(!setPedalActive) {
                setPedalActive = true; 
                doUpdate = true; 
            }
        } else {
            if(setPedalActive) {
                setPedalActive = false;
                doUpdate = true;
            }
        } 
        if(encoders.getMode(2) == 1) { 
            if(!setProgramActive) {
                setProgramActive = true; 
                doUpdate = true; 
            }
        } else {
            if(setProgramActive) {
                setProgramActive = false;
                doUpdate = true;
            }
        } 
    }

    if(code != 0) {
        int tmp; 
        switch(code) { 
            case 101: 
                tmp = presetParams[menuPreset.getCurrentValue()].dry - 5;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].dry = tmp;
                menuDry.setCurrentValue(tmp);
                break;
            case 100: 
                tmp = presetParams[menuPreset.getCurrentValue()].dry + 5;
                if(tmp > 100 ) tmp = 100;   
                presetParams[menuPreset.getCurrentValue()].dry = tmp;
                menuDry.setCurrentValue(tmp);
                break;
            case 103: 
                tmp = presetParams[menuPreset.getCurrentValue()].pedal - 1;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].pedal = tmp;
                menuPedal.setCurrentValue(tmp);
                break;
            case 102: 
                tmp = presetParams[menuPreset.getCurrentValue()].pedal + 1;
                if(tmp > 5 ) tmp = 5;   
                presetParams[menuPreset.getCurrentValue()].pedal = tmp;
                menuPedal.setCurrentValue(tmp);
                break;
            case 201: 
                tmp = presetParams[menuPreset.getCurrentValue()].effect - 5;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].effect = tmp;
                menuEffect.setCurrentValue(tmp);
                break;
            case 200: 
                tmp = presetParams[menuPreset.getCurrentValue()].effect + 5;
                if(tmp > 100 ) tmp = 100;   
                presetParams[menuPreset.getCurrentValue()].effect = tmp;
                menuEffect.setCurrentValue(tmp);
                break;
            case 203: 
                tmp = presetParams[menuPreset.getCurrentValue()].program - 1;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].program = tmp;
                menuProgram.setCurrentValue(tmp);
                break;
            case 202: 
                tmp = presetParams[menuPreset.getCurrentValue()].program + 1;
                if(tmp > 8 ) tmp = 8;   
                presetParams[menuPreset.getCurrentValue()].program = tmp;
                menuProgram.setCurrentValue(tmp);
                break;
            case 301: 
                tmp = presetParams[menuPreset.getCurrentValue()].ctrl1 - 5;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].ctrl1 = tmp;
                menuCtrl1.setCurrentValue(tmp);
                break;
            case 300: 
                tmp = presetParams[menuPreset.getCurrentValue()].ctrl1 + 5;
                if(tmp > 100 ) tmp = 100;   
                presetParams[menuPreset.getCurrentValue()].ctrl1 = tmp;
                menuCtrl1.setCurrentValue(tmp);
                break;
            case 399: 
                tmp = menuPreset.getCurrentValue() - 1;
                if(tmp < 0) tmp = 0;   
                menuPreset.setCurrentValue(tmp);
                if(counter > 0) {
                    switches.getEncoder()->setCurrentReading(tmp);
                    loadPreset(tmp);
                    printNumber(tmp);
                }
                break;
            case 401: 
                tmp = presetParams[menuPreset.getCurrentValue()].ctrl2 - 5;
                if(tmp < 0 ) tmp = 0;   
                presetParams[menuPreset.getCurrentValue()].ctrl2 = tmp;
                menuCtrl2.setCurrentValue(tmp);
                break;
            case 400: 
                tmp = presetParams[menuPreset.getCurrentValue()].ctrl2 + 5;
                if(tmp > 100 ) tmp = 100;   
                presetParams[menuPreset.getCurrentValue()].ctrl2 = tmp;
                menuCtrl2.setCurrentValue(tmp);
                break;  
            case 499: 
                tmp = menuPreset.getCurrentValue() + 1;
                if(tmp > MAX_PRESETS) tmp = MAX_PRESETS;   
                menuPreset.setCurrentValue(tmp);
                if(counter > 0) {
                    switches.getEncoder()->setCurrentReading(tmp);
                    loadPreset(tmp);
                    printNumber(tmp);
                }
                break;     
            default:
                // Unknown or unhandled code, show in topleft corner
                tft.fillRect(0,0,100,20, TFT_BLACK);
                tft.setTextColor(TFT_WHITE);
                tft.setTextDatum(TL_DATUM);
                tft.drawNumber(code, 0, 0);
                delay(500);
                break;
        }
        doUpdate = true;
    }
}
