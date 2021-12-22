#pragma once

namespace power
{

void setup(void)
{
    pinMode(config::board::power::DONE, OUTPUT); 
    digitalWrite(config::board::power::DONE, LOW);

    pinMode(config::board::power::BAT, INPUT);
    pinMode(config::board::power::LIGHT, INPUT);

    int seed = 0;
    int last = analogRead(config::board::power::RANDPIN);
    for (int i = 0; i < 16; i++) {
        int value;
        if (seed & 0b10) {
            value = analogRead(config::board::power::BAT);
        } else if (seed & 0b01) {
            value = analogRead(config::board::power::LIGHT);
        } else {
            value = analogRead(config::board::power::RANDPIN);
        }

        if ((seed & 0b111) == 0b111 
          ||(value & 0b1111) == 0b1010
          ||(seed & 0b1111) == 0b1001)  {
            seed <<= 1;
            last >>= 1;
        }

        value ^= last;
        seed <<= 1;
        seed |= value & 0x1;
        if (seed & 0b100) {
            value += random(-1,2);
        }
        last = value;
        randomSeed(seed);
    }
}

float battery_voltage(void)
{
    float measure = analogRead(config::board::power::BAT);
    measure *= 2;
    measure *= 3.3;
    measure /= 1024;

    return measure;
}

bool light(void)
{
    float lightLevel = analogRead(config::board::power::LIGHT);

    if (lightLevel > 768)
        return false;
    else
        return true;
}

void shutoff(void)
{
    digitalWrite(config::board::power::DONE, HIGH);
    delay(125);
    digitalWrite(config::board::power::DONE, LOW);
    delay(500);
    //while(true) {}
}

} // namespace power
