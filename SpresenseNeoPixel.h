#pragma once
#ifndef NEOPIXELSPRESENSE_H
#define NEOPIXELSPRESENSE_H

#include <Arduino.h>
#include "wiring_private.h"

template <uint8_t PIN, uint32_t N_PIXELS>
class SpresenseNeoPixel
{
    uint32_t reg;
    uint32_t reg_val;

    uint8_t pixels[N_PIXELS * 3];
    uint32_t interval_us {25000}; // 40fps
    uint32_t prev_us {0};

    const uint32_t n_wait_cycles_t0h_t1l;
    const uint32_t n_wait_cycles_t1h_t0l;
    const uint32_t n_wait_cycles_reset;

public:

    SpresenseNeoPixel()
    : n_wait_cycles_t0h_t1l(1)
    , n_wait_cycles_t1h_t0l(28)
    , n_wait_cycles_reset(1)
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
        uint32_t curr_us = micros();
        if (curr_us >= prev_us + interval_us)
        {
            noInterrupts();
            for (uint32_t pixel = 0; pixel < N_PIXELS; ++pixel)
            {
                write(LOW);
                wait_cycles(n_wait_cycles_reset);
                for (uint32_t rgb = 0; rgb < 3; ++rgb)
                {
                    uint32_t i = pixel * 3 + rgb;
                    (pixels[i] & 0x80) ? one() : zero();
                    (pixels[i] & 0x40) ? one() : zero();
                    (pixels[i] & 0x20) ? one() : zero();
                    (pixels[i] & 0x10) ? one() : zero();
                    (pixels[i] & 0x08) ? one() : zero();
                    (pixels[i] & 0x04) ? one() : zero();
                    (pixels[i] & 0x02) ? one() : zero();
                    (pixels[i] & 0x01) ? one() : zero();
                }
            }
            interrupts();
            prev_us = curr_us;
        }
    }

    inline void clear()
    {
        memset(pixels, 0, N_PIXELS * 3);
    }

    inline void set(uint32_t n, uint8_t r, uint8_t g, uint8_t b)
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

    inline void framerate(float fps)
    {
        interval_us = (uint32_t)(1000000.f / fps);
    }

    inline void interval(uint32_t us)
    {
        interval_us = us;
    }

private:

    inline void zero()
    {
        write(HIGH);
        wait_cycles(n_wait_cycles_t0h_t1l);
        write(LOW);
        wait_cycles(n_wait_cycles_t1h_t0l);
    }

    inline void one()
    {
        write(HIGH);
        wait_cycles(n_wait_cycles_t1h_t0l);
        write(LOW);
        wait_cycles(n_wait_cycles_t0h_t1l);
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
