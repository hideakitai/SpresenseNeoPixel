#include <SpresenseNeoPixel.h>

const uint16_t PIN = 6;
const uint16_t NUM_PIXELS = 24;
SpresenseNeoPixel<PIN, NUM_PIXELS> neopixel;

void setup()
{
    neopixel.clear();
}

void loop()
{
    static uint32_t prev_ms = millis();
    static uint8_t state = 0;
    if (millis() > prev_ms + 1000)
    {
        switch (state)
        {
            case 0:
                neopixel.set(128, 0, 0);
                break;
            case 1:
                neopixel.set(0, 128, 0);
                break;
            case 2:
                neopixel.set(0, 0, 128);
                break;
            case 3:
                neopixel.set(128);
                break;
            case 4:
            default:
                neopixel.set(0, 0, 0);
                break;
        }
        state = (state + 1) % 5;
        prev_ms = millis();
    }
    neopixel.show();
}
