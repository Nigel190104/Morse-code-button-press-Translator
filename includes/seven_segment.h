cc LIB_PICO_INT64_OPS/* Seven-segment display (5161BS) */
// http://www.xlitx.com/datasheet/5161BS.pdf

// (Raspberry Pi Pico pinout: https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf)

#pragma once

/** Top-center horizontal bar. */
#define SEGMENT_A            7  // Pin 10 (GPIO 07)

/** Top-right vertical bar */
#define SEGMENT_B            6  // Pin 09 (GPIO 06)

/** Bottom-right vertical bar */
#define SEGMENT_C           20  // Pin 26 (GPIO 20)

/** Bottom-center horizontal bar */
#define SEGMENT_D           19  // Pin 25 (GPIO 19)

/** Bottom-left vertical bar */
#define SEGMENT_E           18  // Pin 24 (GPIO 18)

/** Top-left vertical bar */
#define SEGMENT_F            8  // Pin 11 (GPIO 08)

/** Center horizontal bar */
#define SEGMENT_G            9  // Pin 12 (GPIO 09)

/** Decimal-point */
#define SEGMENT_DP          21  // Pin 27 (GPIO 21)

/** An array containing the pins for all segments. */
static const unsigned int ALL_SEGMENTS[] = {SEGMENT_A, SEGMENT_B, SEGMENT_C, SEGMENT_D, SEGMENT_E, SEGMENT_F, SEGMENT_G, SEGMENT_DP};

#define ALL_SEGMENTS_COUNT  8

void seven_segment_init();

/** Turn off the entire seven-segment display. */
void seven_segment_off();

/** Turn on the entire seven-segment display. */
void seven_segment_on();

//You will need to extend the array to include more letters.
uint8_t values[] = {
  0b11101110,//a
    0b00111110,//b
    0b10011100,//c
    0b01111010,//d
    0b10011110,//e
    0b10001110,//f
    0b11110110,//g
    0b01101110,//h
    0b00001100,//i
    0b01111000,//j
    0b01101110,//K
    0b00011100,//l
    0b10101000,//m
//    abcdefgdp
    0b00101010,//n
    0b11111100,//o
    0b11001110,//p
    0b11100110,//q
    0b00001010,//r
    0b10110110,//s
    0b00011110,//t
    0b01111100,//u
    0b00111000,//v
    0b01010100,//w
    0b01101110,//x
    0b01110110,//y
    0b11011010,//z
    0b11111110,//8
    

};


/* Initialise the GPIO pins for the seven-segment display. */
void seven_segment_init() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_init(ALL_SEGMENTS[i]);
        gpio_set_dir(ALL_SEGMENTS[i], GPIO_OUT);
    }    
}

/* Turn off the seven-segment display. */
void seven_segment_off() {
    for (unsigned int i = 0; i < ALL_SEGMENTS_COUNT; i++) {
        gpio_put(ALL_SEGMENTS[i], true);
    }
}

void seven_segment_show(unsigned int number) {
    for (unsigned int i = 0; i < 8; i++) {
        unsigned int segmentBit = 1 << (7 - i);
        bool illuminateSegment = (segmentBit & values[number]);

        gpio_put(
            ALL_SEGMENTS[i],
            !illuminateSegment
        );
    }
}

//turns on and off horizontal middle segment after the WELCOME MESSAGE
void seven_segment_welcome(){
    gpio_put(SEGMENT_G,true);
        sleep_ms(1000);
        gpio_put(SEGMENT_G, false);
        sleep_ms(1000);
    seven_segment_off();
}

//7-segment displays "8" when user holds button for mroe than 700ms
void button_press_enter(){
    gpio_put(SEGMENT_A, true);
    gpio_put(SEGMENT_B, true);
    gpio_put(SEGMENT_C, true);
    gpio_put(SEGMENT_D, true);
    gpio_put(SEGMENT_E, true);
    gpio_put(SEGMENT_F, true);
    gpio_put(SEGMENT_G, true);
    sleep_ms(1000);
    gpio_put(SEGMENT_A, false);
    gpio_put(SEGMENT_B, false);
    gpio_put(SEGMENT_C, false);
    gpio_put(SEGMENT_D, false);
    gpio_put(SEGMENT_E, false);
    gpio_put(SEGMENT_F, false);
    gpio_put(SEGMENT_G, false);
    seven_segment_off();
}