#include <heater.h>
#include <Arduino.h>

void Heater::set_current(float current){
    this->current = current;
}

void Heater::set_targetTemp(float temp){
    this->targetTemp = temp;
}

void Heater::set_enabled(bool enabled){
    this->enabled = enabled;
}

void Heater::update(){
    if (!this->enabled){
        this->heating = false;
        this->ready = false;
        return;
    }

    if (this->heating && this->current >= this->targetTemp + HYSTERESIS){
        this->heating = false;
    } 

    if (!this->heating && this->current <= this->targetTemp - HYSTERESIS){
        this->heating = true;
    }

    if (this->current >= this->targetTemp){
        this->ready = true;
    }
}

bool Heater::get_signal(){
    this->update();
    return this->heating;
}
