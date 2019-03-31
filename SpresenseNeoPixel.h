#pragma once
#ifndef NEOPIXELSPRESENSE_H
#define NEOPIXELSPRESENSE_H

#include <Arduino.h>
#include "wiring_private.h"

template <uint8_t PIN, uint16_t N_PIXELS>
class SpresenseNeoPixel
{
    uint32_t reg;
    uint32_t reg_val;

    uint8_t pixels[N_PIXELS * 3];

    const uint16_t n_wait_cycles_t0h_t1l;
    const uint16_t n_wait_cycles_t1h_t0l;
    const uint16_t n_wait_cycles_offset;
    const uint16_t n_wait_us_reset;

public:

    SpresenseNeoPixel()
    : n_wait_cycles_t0h_t1l(4)
    , n_wait_cycles_t1h_t0l(20)
    , n_wait_cycles_offset(2)
    , n_wait_us_reset(50)
    {
        pinMode(PIN, OUTPUT);
        digitalWrite(PIN, LOW);
        analog_stop(PIN);

        reg = get_gpio_regaddr(pin_convert(PIN));
        reg_val = *(volatile uint32_t*)reg;

        clear();
    }

    inline void show()
    {
        write(LOW);
        delayMicroseconds(n_wait_us_reset);
        noInterrupts();
        for (uint8_t i = 0; i < N_PIXELS * 3; ++i)
        {
            (pixels[i] & 0x80) ? one() : zero();
            (pixels[i] & 0x40) ? one() : zero();
            (pixels[i] & 0x20) ? one() : zero();
            (pixels[i] & 0x10) ? one() : zero();
            (pixels[i] & 0x08) ? one() : zero();
            (pixels[i] & 0x04) ? one() : zero();
            (pixels[i] & 0x02) ? one() : zero();
            (pixels[i] & 0x01) ? one() : zero();
        }
        interrupts();
    }

    inline void clear()
    {
        memset(pixels, 0, N_PIXELS * 3);
    }

    inline void set(uint8_t n, uint8_t r, uint8_t g, uint8_t b)
    {
        if (n < N_PIXELS)
        {
            pixels[n * 3 + 0] = g;
            pixels[n * 3 + 1] = r;
            pixels[n * 3 + 2] = b;
        }
    }

    inline void set(uint8_t r, uint8_t g, uint8_t b)
    {
        for(size_t i = 0; i < N_PIXELS; ++i) set(i, r, g, b);
    }

    inline void set(uint8_t brightness)
    {
        set(brightness, brightness, brightness);
    }

private:

    inline void zero()
    {
        write(HIGH);
        wait_cycles(n_wait_cycles_t0h_t1l);
        write(LOW);
        wait_cycles(n_wait_cycles_t1h_t0l);
        wait_cycles(n_wait_cycles_offset); // buffer
    }

    inline void one()
    {
        write(HIGH);
        wait_cycles(n_wait_cycles_t1h_t0l);
        write(LOW);
        wait_cycles(n_wait_cycles_t0h_t1l);
        wait_cycles(n_wait_cycles_offset); // buffer
    }

    inline void write(uint8_t value)
    {
        bitWrite(reg_val, GPIO_OUTPUT_SHIFT, value);
        *(volatile uint32_t*)reg = reg_val;
    }

    inline void wait_cycles(uint32_t n) // 4 clocks per cycle
    {
        asm volatile
        (
            "0:" "SUBS %[count], 1;"
            "BNE 0b;"
            : [count] "+r" (n)
        );
    }
};

#endif // NEOPIXELSPRESENSE_H
