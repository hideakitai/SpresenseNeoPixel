#include <SpresenseNeoPixel.h>

// const uint16_t PIN = 6;
const uint16_t PIN = 24;
const uint16_t NUM_PIXELS = 24;
SpresenseNeoPixel<PIN, NUM_PIXELS> neopixel;

void setup()
{
    Serial.begin(115200);
    neopixel.clear();
    neopixel.framerate(40); // default framerate is 40[fps]

    delay(1000);

    Serial.println("start");
}

void loop()
{
    static uint32_t prev_ms = millis();
    static uint8_t state = 0;
    if (millis() > prev_ms + 1000)
    {
        Serial.println("change color");

        switch (state)
        {
            case 0:
                neopixel.set(128, 0, 0);
                break;
            case 1:
                neopixel.set(255, 0, 0);
                break;
            case 2:
                neopixel.set(0, 128, 0);
                break;
            case 3:
                neopixel.set(0, 255, 0);
                break;
            case 4:
                neopixel.set(0, 0, 128);
                break;
            case 5:
                neopixel.set(0, 0, 255);
                break;
            case 6:
                neopixel.set(128);
                break;
            case 7:
                neopixel.set(255);
                break;
            case 8:
            default:
                neopixel.set(0, 0, 0);
                break;
        }
        state = (state + 1) % 9;
        prev_ms = millis();
    }

    neopixel.show(); // automatically control framerate to desired fps
}
