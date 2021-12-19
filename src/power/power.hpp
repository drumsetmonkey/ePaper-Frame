#pragma once

namespace power
{

void setup(void)
{
    pinMode(config::board::power::DONE, OUTPUT); 
    digitalWrite(config::board::power::DONE, LOW);

    pinMode(config::board::power::BAT, INPUT);
    pinMode(config::board::power::LIGHT, INPUT);

    randomSeed(analogRead(config::board::power::RANDPIN));
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
}

} // namespace power
