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
        this->prev_time = 0;
        this->prev_value = 0;
        return;
    }

    if (this->heating && this -> enabled && this->targetTemp > this->current){
        if (this->prev_time == 0){
            this->prev_time = millis();
            this-> prev_value = this->current;
        }
        if (millis() - this->prev_time > ERROR_CHECK_TIME){
            this->prev_time = millis();
            if (this->current < this->prev_value && this->current < this->targetTemp - ERROR_CHECK_DIFF){
                this->error = true;
            }
            this-> prev_value = this->current;
        }
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
    return this->heating && !this->error;
}

bool Heater::is_error(){
    return this->error;
}
bool Heater::get_ready(){
    this->update();
    return this->ready;
}
