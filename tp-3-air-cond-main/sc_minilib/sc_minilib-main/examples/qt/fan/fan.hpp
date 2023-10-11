#include <sm_defs.h>


class FanMachine : public StateMachine {
public:
    enum STATE {OFF = 1, SLOW = 2, MED = 3, FAST = 4};
    enum EVENT {PLUS = 1, ONOFF = 2, TIMEOUT = 3};
    const char* state_names[FAST+1] = {
        "", "Off", "Slow speed", "Medium speed", "Fast speed"
    };
    const char *evt_names[TIMEOUT+1] = {
        "", "Button plus pressed", "Button on/off pressed", "Timeout event"
    };

    int currState = OFF;
    int timeout_id = 0;

    void print_state();
    
    void off_handler(Evt_t evt);

    void slow_handler(Evt_t evt);

    void med_handler(Evt_t evt);
    
    void fast_handler(Evt_t evt);

    void handler(Evt_t evt);
};


void turn_on();
void turn_off();
void change_speed();

