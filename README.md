# SpresenseNeoPixel

NeoPixel library for SPRESENSE (Arduino).
Major NeoPixel libraries does NOT support SPRESENSE.

## Usage

``` C++
#include <SpresenseNeoPixel.h>

const uint16_t PIN = 6;
const uint16_t NUM_PIXELS = 24;
SpresenseNeoPixel<PIN, NUM_PIXELS> neopixel;

uint32_t prev_ms = 0;
uint8_t state = 0;

void setup()
{
    neopixel.clear(); // set NeoPixels to black
}

void loop()
{
    if (millis() > prev_ms + 1000)
    {
        switch (state)
            case 0: neopixel.set(128, 0, 0); break;
            case 1: neopixel.set(0, 128, 0); break;
            case 2: neopixel.set(0, 0, 128); break;
            case 3: neopixel.set(128);       break;
            case 4: neopixel.set(0, 0, 0);   break;
        }
        state = (state + 1) % 5;
        prev_ms = millis();
    }
    neopixel.show();
}
```

## License

MIT
