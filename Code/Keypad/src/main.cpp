#include <Arduino.h>
#include <extEEPROM.hpp>
#include <AnimationDriver.hpp>
#include <DefaultAnimations.hpp>
#include <Adafruit_NeoPixel.h>
#include <GIFController.h>
#include <utils/kakashi1/kakashi1.h>
#include <utils/kakashi2/kakashi2.h>
#include <Adafruit_TinyUSB.h>
#include <ButtonFSM.h>

// Conditional Compilation
// #define DEV // In development

/*
 Hardware Defines
*/
// GIF Controller
#define FRAME_TIME 100
#define NUM_GIFS 1

// EEPROM
#define ADDR_ANIMS 0x000  // Start address for animations in EEPROM
#define ADDR_MACROS 0x5FF // Start address for macros in EEPROM

// LED Strip
#define NUM_PIXELS 12      // Number of pixels in the strip
#define PIN_STRIP 6        // Pin LED is connected to
#define LED_BRIGHTNESS 220 // Brightness for LEDs

// Keys
#define KEY_ROWS 4
#define KEY_COLS 3
#define NUM_KEYS (KEY_ROWS * KEY_COLS)
#define DEBOUNCE_MS 100
#define MACRO_LENGTH 21             // Length of Macros in key presses
#define MACRO_SIZE MACRO_LENGTH * 2 // Length of Macros in size of bytes
/**
 * @brief Return 1D index of button, given col and row (starting from 0)
 *
 */
#define KEY_INDEX (row, col)(row * NUM_COLS + col)
/**
 * 0  1  2
 * 3  4  5
 * 6  7  8
 * 9  10 11
 */

extEEPROM extEE(0b1010000, extEEPROM::deviceIDs::ID_24AA16H);

Adafruit_NeoPixel keyLED(NUM_PIXELS, PIN_STRIP, NEO_KHZ800 + NEO_GRB);

Adafruit_USBD_HID usb_hid;

GIF *gifs[] = {&kakashi1, &kakashi2};
GIFController gifCtrl(gifs[0]->frames, gifs[0]->frame_count, &Wire, FRAME_TIME);

uint8_t PINS_ROWS[] = {10, 9, 8, 7}; // Pin mapping for rows
uint8_t PINS_COLS[] = {2, 1, 0};     // Pin mapping for columns
ButtonFSM *keys = new ButtonFSM[NUM_KEYS];
uint16_t macros[NUM_KEYS][21];

AnimationDriver::AnimationDriver animator(millis);
const AnimationDriver::animation Solid_White PROGMEM = SOLID_COLOR(255, 255, 255);
const AnimationDriver::animation Solid_Red PROGMEM = SOLID_COLOR(255, 0, 0);
const AnimationDriver::animation Breathe_White PROGMEM = BREATHE_COLOR(255, 255, 255, 3000UL);
const AnimationDriver::animation Solid_Green PROGMEM = SOLID_COLOR(0, 255, 0);
const AnimationDriver::animation Rainbow PROGMEM = RAINBOW(4000UL);
const AnimationDriver::animation Solid_Blue PROGMEM = SOLID_COLOR(0, 0, 255);

const AnimationDriver::animation defaults[] PROGMEM = {
    Solid_White,
    Solid_Red,
    Breathe_White,
    Solid_Green,
    Rainbow,
    Solid_Blue};

AnimationDriver::animation currentAnim;

// EEPROM FUNCTIONS
/**
 * @brief Write a macro from memory to EEPROM
 * @param index key index to write the macro to
 * @param data pointer to byte array of the macro
 * @return whether or not write was a success
 */
bool writeMacro(uint8_t index, uint8_t *data)
{
  // For each macro
  for (uint8_t macro = 0; macro < NUM_KEYS; macro++)
  {
    // For each keypress in the macro
    for (uint8_t index = 0; index < MACRO_LENGTH; index++)
    {
      if (extEE.put(ADDR_MACROS + macro * MACRO_SIZE + index * 2, macros[index], 2) != 0)
        return false;
    }
  }
  return true;
}

/**
 * @brief Load macros from EEPROM into memory
 * @return whether or not read was a success
 */
bool loadMacros()
{
  // For each macro
  for (uint8_t macro = 0; macro < NUM_KEYS; macro++)
  {
    // For each keypress in the macro
    for (uint8_t index = 0; index < MACRO_LENGTH; index++)
    {
      if (extEE.get(ADDR_MACROS + macro * MACRO_SIZE + index * 2, macros[index], 2) != 0)
        return false;
    }
  }
  return true;
}

// KEYBOARD FUNCTIONS
/**
 * @brief Setup pins & memory for keyboard keys
 *
 */
void initKeys()
{
  // Allocate memory for each button object
  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    keys[i] = ButtonFSM(millis, DEBOUNCE_MS);
  }
  // Set pinmode for column and row pins
  for (uint8_t row = 0; row < KEY_ROWS; row++)
  {
    pinMode(PINS_ROWS[row], INPUT_PULLDOWN);
  }
  for (uint8_t col = 0; col < KEY_COLS; col++)
  {
    pinMode(PINS_COLS[col], OUTPUT);
  }
}
void scanButtons()
{
  for (uint8_t i = 0; i < KEY_COLS; i++)
  {
    // Set row pin high and others low
    for (uint8_t j = 0; j < KEY_ROWS; j++)
    {
      // Read pin row state & update each FSM
    }
  }
}

// LED FUNCTIONS

// GIF ANIMATION FUNCTIONS

void setup()
{
#ifdef DEV
  Serial.begin(9600);
#endif
  // Start OLED display
  if (!gifCtrl.init())
  {
#ifdef DEV
    Serial.println(F("SSD1306 allocation failed"));
#endif
    digitalWrite(LED_BUILTIN, LOW); // Active Low LED
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Setup keys
  initKeys();

  // Setup LEDs
  keyLED.begin();
  keyLED.setBrightness(LED_BRIGHTNESS);
  keyLED.clear();
  keyLED.show();

  // Load macros from EEPROM
  loadMacros();
}

void loop()
{
}
