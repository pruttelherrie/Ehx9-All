//#define UI_KNOBS


/* === CONNECTIONS ===============================================
 * PA0 = Pot 1
 * PA1 = Pot 2
 * PA2 = TX2, goes to Flash switcher PCB
 * PA3 = RX2, MIDI in
 * PA4 = TFT DC
 * PA5 = TFT CLK (SCLK)
 * PA6 = NC (MISO)
 * PA7 = TFT DATA (MOSI)
 * PA8 = Switch 5
 * PA9 = Switch 4
 * PA10 = Enc menu A
 * PA11 = Switch 2
 * PA12 = Switch 3
 * PA15 = NC
 * PB0 = Pot 3
 * PB1 = Pot 4
 * PB2 = TFT RST
 * PB3 = Switch 1
 * PB4 = Enc 1
 * PB5 = Enc 2
 * PB6 = Enc 3
 * PB7 = Enc BUS B
 * PB8 = Enc BUS A
 * PB9 = Enc BUS S
 * PB10 = ??? -> to Pot header
 * PB11 = NC!
 * PB12 = Switch 9
 * PB13 = Switch 8
 * PB14 = Switch 7
 * PB15 = Switch 6
 * PC13 = Enc Menu B
 * PC14 = Enc 4
 * PC15 = Enc Menu S 
 * 
 * =============================================================== */

#include <Arduino.h>
#include <IoAbstractionWire.h>
#include <EepromAbstractionWire.h>
#include <AnalogDeviceAbstraction.h>
#include <SPI.h>
#include "Arduino_menu.h"
#include <EEPROM.h>
#include "CommonBusEncoders.h"

#include "build_number_defines.h"
#include "lovecraft30pt7b.h"
#include "neon8.h"
#include "Amiga4Ever8pt7b.h"

TFT_eSprite spr = TFT_eSprite(&tft);
uint16_t* sprPtr;

uint8_t knobPins[]  = {PA0, PA1, PB0, PB1};
uint8_t switchPins[]= {PB14, PB13, PB12, PB15 , PB3, PA11, PA12, PA9, PA8};

CommonBusEncoders encoders(PB8,PB7,PB9,4);

struct Settings {
  uint8_t midichannel = 0;
} settings;

#define MAX_PRESETS 32

struct presetParams {
    char name[16] = "Unnamed";
    uint8_t pedal   =   0;
    uint8_t program =   0;
    uint8_t dry     =   0;
    uint8_t effect  = 100;
    uint8_t ctrl1   =  40;
    uint8_t ctrl2   =  60;
} presetParams[MAX_PRESETS];

uint8_t curProgram = 0;
int counter;
bool startup = true;
float p = 3.1415926;
bool playMode = false;
bool doUpdate = false;
uint8_t lastPedal = 255; // Force setting pedal

bool setPedalActive = false;
bool setProgramActive = false;

// voor MIDI
uint8_t c;
bool next_is_pc = false;

// All programs of the pedals.
const char programNames[9 * 6][13] PROGMEM = 
{ "OBX",        "Profit V",  "Vibe Synth", "Mini Mood",    "EHX Mini",     "Solo Synth",   "Mood Bass",  "String Synth", "Poly VI",
  "Orchestra",  "Cello",     "Strings",    "Flute",        "Clarinet",     "Saxophone",    "Brass",      "Low Choir",    "High Choir",
  "Precision",  "Longhorn",  "Fretless",   "Synth",        "Virtual",      "Bowed",        "Split Bass", "3:03",         "Flip-Flop",
  "Tone Wheel", "Prog",      "Compact",    "Shimmer",      "Lord Purple",  "MelloFlutes",  "Blimp",      "Press Tone",   "Telstar",
  "Fat & Full", "Jazz",      "Gospel",     "ClassicRock",  "Bottom End",   "Octaves",      "Cathedral",  "Continental",  "Bell Organ",
  "Dynamo",     "Wurli",     "Suitcase",   "Mallets",      "Eightyeight",  "Triglorious",  "Vibes",      "Organ",        "Steel Drums" 
};
// Some pedals have distinctive functions on the CTRL 1 and CTRL 2 pots. This is the place to define them.
const char ctrl1Names[9 * 6][13] PROGMEM = 
{ "Tone",       "Sweeptime",  "Harmonics", "Port.level",   "Sweepattck",   "Tone",         "Sweep time",   "Tone",       "Tone",
  "Attack",     "Attack",     "Attack",    "Attack",       "Attack",       "Attack",       "Fltr speed",   "Attack",     "Attack",
  "Octavemix",  "Pitch",      "Growl",     "Note range",   "Bodydens.",    "Suboctave",    "Harmonics",    "Env.depth",  "Suboctave",
  "Chorus",     "Chorus",     "Vibrato",   "Attack",       "Chorus",       "Vibrato",      "Chorus",       "Chorus",     "Vibrato",
  "Chorus",     "Chorus",     "Chorus",    "Chorus",       "Chorus",       "Chorus",       "Tremolo",      "Vibrato",    "Tremolo",
  "Bass",       "Tremdepth",  "Bass/tine", "Chorusdpt",    "Tremdepth",    "Chorusdpt",    "Treb.attack",  "Treble",     "Chorusdpt",
};  
const char ctrl2Names[9 * 6][13] PROGMEM = 
{ "Octaves",    "Interval",   "Vibrato",    "Port.time",    "Octave",      "Octave",       "Octave",     "Sweepattck",   "Mod.depth",
  "Sustain",    "Sustain",    "Sustain",    "Sustain",      "Sustain",     "Sustain",      "Lip buzz",   "Sustain",      "Sustain",
  "Tone",       "Tremolo",    "Chorus",     "Env.range",    "Necklen",     "Inv.Attack",   "Autowah",    "Env.speed",    "Lowpass",
  "Click",      "Harmonics",  "Harmonics",  "Sustain",      "Drawbars",    "Vibr.depth",   "Harmonics",  "Click",        "Harmonics",
  "Click",      "Click",      "Click",      "Click",        "Click",       "Click/Harm.",  "Tremdepth",  "Vibr.depth",   "Bell/chime",
  "Tine",       "Tremspeed",  "Phasespd",   "Chorusspd",    "Tremspeed",   "Chorusspd",    "Tremspeed",  "Rotatespd",    "Chorusspd",
};     