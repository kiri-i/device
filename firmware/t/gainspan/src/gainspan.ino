#include "Arduino.h"
#include "pins.h"
#include "pgmStrToRAM.h"
#include "MemoryFree.h"
#include "GSwifi.h"

GSwifi gs(&Serial1);

void setup() {
    // enable 3.3V LDO
    pinMode( LDO33_ENABLE, OUTPUT );
    digitalWrite( LDO33_ENABLE, HIGH );

    // USB serial
    Serial.begin(115200);

    // wait for connection
    while ( ! Serial );

    reset3V3();

    // wait til gs wakes up
    delay( 100 );

    gs.setup();

    printGuide();
}

void printGuide() {
    Serial.println(P("Menu:"));
    Serial.println(P("b) change baud rate to 9600"));
    Serial.println(P("B) change baud rate to 115200"));
    Serial.println(P("c) connect to wifi"));
    Serial.println(P("h) help (this)"));
    Serial.println(P("R) hardware reset"));
    Serial.println(P("s) setup"));
    Serial.println(P("Command?"));
}

void loop() {
    // usb
    if (Serial.available()) {
        static uint8_t last_character = '0';

        last_character = Serial.read();

        Serial.print(P("> 0x"));
        Serial.print(last_character, HEX);
        if (last_character > 0x0D) {
            Serial.print(P(" "));
            Serial.write(last_character);
        }
        Serial.println();
        Serial.print(P("free memory: 0x")); Serial.println( freeMemory(), HEX );

        uint8_t status;
        if (last_character == 'c') {
            // gs.connect( GSwifi::GSSEC_WPA2_PSK, PB("Rhodos"), P("aaaaaaaaaaaaa") );
        }
        else if (last_character == 'b') {
            gs.setBaud(9600);
        }
        else if (last_character == 'B') {
            gs.setBaud(115200);
        }
        else if (last_character == 'h') {
            printGuide();
        }
        else if (last_character == 'R') {
            reset3V3();
        }
        else if (last_character == 's') {
            gs.setup();
        }
        else {
            Serial1.write(last_character);
        }
    }

    // gainspan
    if (Serial1.available()) {
        static uint8_t last_character_gainspan = '0';
        last_character_gainspan = Serial1.read();
        Serial.print(P("< 0x"));
        Serial.print(last_character_gainspan, HEX);
        if (last_character_gainspan > 0x0D) {
            Serial.print(P(" "));
            Serial.write(last_character_gainspan);
        }
        Serial.println();
    }
}

void reset3V3 () {
    Serial.println(P("hardware reset"));
    digitalWrite( LDO33_ENABLE, LOW );
    delay( 100 );
    digitalWrite( LDO33_ENABLE, HIGH );
}