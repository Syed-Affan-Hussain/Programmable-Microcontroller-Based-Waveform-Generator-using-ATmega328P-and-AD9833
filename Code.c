#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#define DATA 3
#define CLK 5
#define FSYNC 2

float _freq;

typedef enum mode {
    MODE_SINE,
    MODE_SQUARE,
    MODE_TRIANGLE,
} mode_t;

void AD_write16(uint16_t data) {
    PORTB &= ~(1 << FSYNC);
    SPDR = (data >> 8);
    while (!(SPSR & (1 << SPIF)));
    SPDR = (data & 0xFF);
    while (!(SPSR & (1 << SPIF)));
    PORTB |= (1 << FSYNC);
}

void AD_setFrequency(float freq) {
    _freq = freq;
    uint32_t _regFreq = freq * 10.73742 + 0.5;
    AD_write16(0x4000 | (uint16_t)(_regFreq & 0x3FFF));
    AD_write16(0x4000 | (uint16_t)((_regFreq >> 14) & 0x3FFF));
}

const float AD_getFrequency() {
    return _freq;
}

void AD_setPhase(uint16_t phase) {
    uint32_t _regPhase = (51.2 * phase) / 45 + 0.5;
    AD_write16(0xC000 | (uint16_t)(_regPhase & 0xFFF));
}

void AD_setMode(mode_t mode) {
    switch (mode) {
        case MODE_SINE:
            AD_write16(0x2000);
            break;
        case MODE_SQUARE:
            AD_write16(0x2028);
            break;
        case MODE_TRIANGLE:
            AD_write16(0x2002);
            break;
    }
}

void AD_init(void) {
    DDRB = (1 << DATA) | (1 << CLK) | (1 << FSYNC);
    PORTB &= ~(1 << CLK);
    PORTB |= (1 << FSYNC);
    SPCR = 0x59;
    AD_write16(0x2100);
    AD_setFrequency(1000);
    AD_setPhase(0);
    AD_write16(0x2000);
    AD_setMode(MODE_SINE);
}

void lcd_command(unsigned char cmnd) {
    PORTD = cmnd;
    PORTB &= ~(1 << PB0);
    PORTB |= (1 << PB1);
    _delay_us(1);
    PORTB &= ~(1 << PB1);
    _delay_us(100);
}

void lcd_data(unsigned char data) {
    PORTD = data;
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB1);
    _delay_us(1);
    PORTB &= ~(1 << PB1);
    _delay_us(100);
}

void lcd_init() {
    DDRD = 0xFF;
    DDRB |= 0x03;
    PORTB &= ~(1 << PB1);
    _delay_us(2000);
    lcd_command(0x38);
    lcd_command(0x0C);
    lcd_command(0x01);
    _delay_us(2000);
}

void lcd_print(char* StringPtr) {
    unsigned char i = 0;
    while (StringPtr[i] != 0) {
        lcd_data(StringPtr[i]);
        i++;
    }
}

int main(void) {
    DDRC &= ~(0x38);
    AD_init();
    lcd_init();
    _delay_ms(1000);

    int x = 0;
    float freq_val = 0;
    char str_temp[10];

    while (1) {
        freq_val = AD_getFrequency();

        if ((freq_val > 100) && (freq_val < 100000)) {
            if (PINC & (1 << PC5)) {
                AD_setFrequency(freq_val + 100);
            }
            if (PINC & (1 << PC4)) {
                AD_setFrequency(freq_val - 100);
            }
        }

        lcd_command(0x02);
        _delay_ms(500);
        lcd_print("FREQ=");
        dtostrf(freq_val, 10, 1, str_temp);
        lcd_print(str_temp);

        if (PINC & (1 << PC3)) {
            x += 1;
            switch (x) {
                case 1:
                    AD_setMode(MODE_TRIANGLE);
                    lcd_command(0xC0);
                    lcd_print("TRIANGLEWAVE");
                    break;
                case 2:
                    AD_setMode(MODE_SINE);
                    lcd_command(0xC0);
                    lcd_print("SINE        ");
                    break;
                case 3:
                    AD_setMode(MODE_SQUARE);
                    lcd_command(0xC0);
                    lcd_print("SQUARE      ");
                    x = 0;
                    break;
            }
        }
    }

    return 0;
}
