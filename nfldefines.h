#ifndef nfldefines_h
#define nfldefines_h

#define LEDMATRIX
//#define NEOMATRIX
#define DISABLE_MATRIX_TEST
#include "neomatrix_config.h"

// All my backends (ESP8266, ESP32, and ArduinoOnPC) have some kind of FS
// Backend. Technically this code could run on some chip that has no FS
// (like a teensy with sdcard), and all demos outside of GIFS will work,
// but you will have to make read_config_index work somehow, probably by
// hardcoding the file back into the code (where it used to be, but I moved
// it out so that it can be updated by a web server at runtime).
#define HAS_FS
#ifdef ESP8266
    #define mheight 32
#elif defined(ESP32)
    #ifdef M64BY64
	#define mheight 64
    #else
	#define mheight 96
    #endif
#elif defined(ARDUINOONPC)
    #if GFXDISPLAY_M64BY96
	#define mheight 96
    #else
	#define mheight 192
    #endif
#else
#error "Matrix config undefined, please set height"
#endif

// How many ms used for each matrix update
// This is now used to schedule Matrix_Handler every 20ms
// if possible (50 frames generated per second). 
// Note that actual framerate generation will be slowed
// down by how long things take to run.
#define MX_UPD_TIME 20

// Separate Neopixel Strip, if used. Not the NeoMatrix
uint8_t led_brightness = 64;
// matrix_brightness is defined on neomatrix_config.h

uint8_t dfl_matrix_brightness_level = 5;

#ifdef ESP32
// Use https://github.com/lbernstone/IR32.git instead of IRRemote
// 0x4008fd64: invoke_abort at /Users/ficeto/Desktop/ESP32/ESP32/esp-idf-public/components/esp32/panic.c line 155
// 0x4008ff95: abort at /Users/ficeto/Desktop/ESP32/ESP32/esp-idf-public/components/esp32/panic.c line 170
// 0x4008b6f7: xRingbufferReceive at /Users/ficeto/Desktop/ESP32/ESP32/esp-idf-public/components/esp_ringbuf/ringbuf.c line 845
// 0x400f323c: IRRecv::read(char*&, bool) at /home/merlin/Arduino/libraries/IR32/src/IRRecv.cpp line 128
// 0x400e7689: check_startup_IR_serial() at /home/merlin/arduino/emulation/ArduinoOnPc-FastLED-GFX-LEDMatrix/examples/NeoMatrix-FastLED-IR/NeoMatrix-FastLED-IR.ino line 3287
// 0x400e805a: setup() at /home/merlin/arduino/emulation/ArduinoOnPc-FastLED-GFX-LEDMatrix/examples/NeoMatrix-FastLED-IR/NeoMatrix-FastLED-IR.ino line 4715
// 0x400f7e6b: loopTask(void*) at /home/merlin/Arduino/hardware/espressif/esp32/cores/esp32/main.cpp line 14
// 0x4008bb7d: vPortTaskWrapper at /Users/ficeto/Desktop/ESP32/ESP32/esp-idf-public/components/freertos/port.c line 143
//#define ESP32RMTIR
#endif

// On ESP32, I have a 64x64 direct matrix (not tiled) with 2 options of drivers.
#if mheight == 96 || mheight == 192
    // Using RGBPanel via SmartMatrix

    // Memory After GIF init:
    // before SmartMatrix::GFX bufferless change
    // Heap/32-bit Memory Available: 114048 bytes total,  85748 bytes largest free block
    // 8-bit/DMA Memory Available  :  28300 bytes total,  15456 bytes largest free block

    // after SmartMatrix::GFX zero copy change.
    // Heap/32-bit Memory Available: 132476 bytes total,  85748 bytes largest free block
    // 8-bit/DMA Memory Available  :  46728 bytes total,  40976 bytes largest free block
    #ifndef ARDUINOONPC
        #define IR_RECV_PIN 34
    
        #define STRIP_NUM_LEDS 48
        CRGB leds[STRIP_NUM_LEDS];
        #define NEOPIXEL_PIN 13
	#ifdef BOARD_HAS_PSRAM
	    // There isn't enough RAM to do Wifi with all the demos I have
	    #pragma message "PSRAM and WIFI enabled"
	    #define WIFI
	#else
	    #pragma message "PSRAM disabled, so WIFI disabled too"
	#endif
    #endif

#elif mheight == 64
    // Make the burning man 64x64 brighter by default, we have a big power supply :)
    dfl_matrix_brightness_level = 6;
    #define IR_RECV_PIN 34

#elif mheight == 32
    #define STRIP_NUM_LEDS 48
    CRGB leds[STRIP_NUM_LEDS];
    #define NEOPIXEL_PIN D1 // GPIO5

    #define IR_RECV_PIN D4
#else
    #error "unknown matrix height, no idea what demos to run"
#endif

// Disable fonts in many sizes
// Sketch uses 283784 bytes (27%) of program storage space. Maximum is 1044464 bytes.
// Global variables use 32880 bytes (40%) of dynamic memory, leaving 49040 bytes for local variables. Maximum is 81920 bytes.
// Uploading 287936 bytes from /tmp/arduino_build_498793/NeoMatrix-FastLED-IR.ino.bin to flash at 0x00000000
//#define NOFONTS 1

// show all demos by default,
bool show_best_demos = false;


bool matrix_reset_demo = 1;

void matrix_show();
void aurora_setup();
bool sav_newgif(const char *pathname);
void sav_setup();
bool sav_loop();

#endif // nfldefines_h
