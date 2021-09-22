// ================= Prototypes for functions that are called from within callbacks
void setPedal(uint8_t, bool);
void setProgram(uint8_t); 
void myDisplayFunction(unsigned int, RenderPressMode);
void loadPreset(uint8_t);

// ================= Callbacks

/*
ResetCallbackFn onReset() {
    renderer.takeOverDisplay(myDisplayFunction);
}
*/

void CALLBACK_FUNCTION onPresetMode(int id) {
    // FIXME: bijhouden dat we in presetMode zitten
    renderer.takeOverDisplay(myDisplayFunction);
}

void CALLBACK_FUNCTION onPreset(int id) {
    loadPreset(menuPreset.getCurrentValue());
}

void CALLBACK_FUNCTION onName(int id) {
    strcpy(presetParams[menuPreset.getCurrentValue()].name, menuName.getTextValue());
}


void CALLBACK_FUNCTION onPedal(int id) {
    presetParams[menuPreset.getCurrentValue()].pedal = menuPedal.getCurrentValue();
    uint8_t i = menuProgram.getCurrentValue();
    menuProgram.setCurrentValue(10);
    menuProgram.setCurrentValue(i);
    // FIXME: alleen aanroepen als anders is!
    setPedal(menuPedal.getCurrentValue(), true);
    strcpy(minfoCtrl1.name, ctrl1Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()]);
    strcpy(minfoCtrl2.name, ctrl2Names[menuPedal.getCurrentValue() * 9 + menuProgram.getCurrentValue()]);
}

void CALLBACK_FUNCTION onDry(int id) {
    presetParams[menuPreset.getCurrentValue()].dry = menuDry.getCurrentValue();
    analogWrite(knobPins[0], 255 - (uint8_t)(menuDry.getCurrentValue() / 100.0 * 255.0)); 
}

void CALLBACK_FUNCTION onEffect(int id) {
    presetParams[menuPreset.getCurrentValue()].effect = menuEffect.getCurrentValue();
    analogWrite(knobPins[1], 255 - (uint8_t)(menuEffect.getCurrentValue() / 100.0 * 255.0)); 
}

void CALLBACK_FUNCTION onCtrl1(int id) {
    presetParams[menuPreset.getCurrentValue()].ctrl1 = menuCtrl1.getCurrentValue();
    analogWrite(knobPins[2], 255 - (uint8_t)(menuCtrl1.getCurrentValue() / 100.0 * 255.0)); 
}

void CALLBACK_FUNCTION onCtrl2(int id) {
    presetParams[menuPreset.getCurrentValue()].ctrl2 = menuCtrl2.getCurrentValue();
    analogWrite(knobPins[3], 255 - (uint8_t)(menuCtrl2.getCurrentValue() / 100.0 * 255.0)); 
}

void CALLBACK_FUNCTION onStoreAll(int id) {
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_LIGHTGREY);
    tft.setFreeFont(&FreeSans18pt7b);
    tft.drawString("Storing...", 120, 120);
    EEPROM.put(0, 0x00); // EEPROM initialised
    EEPROM.put(1, presetParams);
    EEPROM.put(1 + sizeof(presetParams), settings);
    renderer.takeOverDisplay(myDisplayFunction);
}

void CALLBACK_FUNCTION onSettingsMIDIChannel(int id) {
    settings.midichannel = menuSettingsMIDIChannel.getCurrentValue();
}

int CALLBACK_FUNCTION fnProgramRtCall(RuntimeMenuItem *item, uint8_t row, RenderFnMode mode, char *buffer, int bufferSize) {
    switch(mode) {
        case RENDERFN_EEPROM_POS:
            return false;
        case RENDERFN_INVOKE:
            // when using custom mode, this is how you detect a change in the value
            presetParams[menuPreset.getCurrentValue()].program = menuProgram.getCurrentValue();
            strcpy(minfoCtrl1.name, ctrl1Names[menuPedal.getCurrentValue() * 9 + row]);
            strcpy(minfoCtrl2.name, ctrl2Names[menuPedal.getCurrentValue() * 9 + row]);
            setProgram(menuProgram.getCurrentValue());
            return true;
        case RENDERFN_NAME:
            // this is how to provide the display name of the item 
            strcpy(buffer, "Progr.");
            return true;
        case RENDERFN_VALUE:
            strcpy(buffer, programNames[menuPedal.getCurrentValue() * 9 + row]);  
            setProgram(menuProgram.getCurrentValue());
            return true;
        default: return false;
    }
}
