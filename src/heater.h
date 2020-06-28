#ifndef HEATER_HEATER_
#define HEATER_HEATER_
#define HYSTERESIS 2
class Heater {
    public:
        void set_targetTemp(float temp);
        void set_current(float temp);
        void set_enabled(bool enabled);
        bool get_signal();
        bool get_ready();
        void update();
    private:
        int pin;
        bool enabled = false;
        bool heating = false;
        float current = 0;
        float targetTemp = 0;
        bool ready;
        
};
#endif