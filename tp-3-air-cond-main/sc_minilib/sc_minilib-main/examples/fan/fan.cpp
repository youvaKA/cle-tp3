#include <sm_defs.h>
#include <timeout.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#include <display.h>

/* ----------------------------------------*/       

#define LOG_AREA_X 5
#define LOG_AREA_Y 10

void turn_on()
{
    print_line(2, "Action: Turn on FAN at SLOW speed");
    show_display();
}

void turn_off()
{
    print_line(2, "Action: Turn off FAN");
    show_display();
}

void change_speed()
{
    print_line(2, "Action: Changing speed");
    show_display();
}

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

    void print_state() {
        std::stringstream ss;
        ss << "Status : " << state_names[currState];        
        //print_at(MSG_AREA_X-2, MSG_AREA_Y-1, "+------------ FAN DISPLAY -------------+");
        //print_at(MSG_AREA_X-2, MSG_AREA_Y,   ss.str().c_str());  
        //print_at(MSG_AREA_X-2, MSG_AREA_Y+1, "| (*) : on/off    (+) : increase speed |");
        //print_at(MSG_AREA_X-2, MSG_AREA_Y+2, "| (q) : exit simulation                |");
        //print_at(MSG_AREA_X-2, MSG_AREA_Y+3, "+--------------------------------------+");
        //refresh();
        print_line(1, ss.str().c_str());
        show_display();
    }
    
    void off_handler(Evt_t evt) {
        switch(evt) {
        case ONOFF:
            currState = SLOW;
            turn_on();
            break;
        case PLUS:
            break;
        case TIMEOUT:
            break;
        }
    }

    void slow_handler(Evt_t evt) {
        switch(evt) {
        case ONOFF:
            currState = OFF;
            turn_off();
            break;
        case PLUS:
            currState = MED;
            change_speed();
            break;
        case TIMEOUT:
            break;
        }
    }

    void med_handler(Evt_t evt) {
        switch(evt) {
        case ONOFF:
            currState = OFF;
            turn_off();
            break;
        case PLUS:
            currState = FAST;
            timeout_id = set_timeout(5, SEC, TIMEOUT, this);
            print_at(LOG_AREA_X, LOG_AREA_Y,  "Setting 5 seconds timeout");
            show_display();
            change_speed();
            break;
        case TIMEOUT:
            break;
        }
    }
    
    void fast_handler(Evt_t evt) {
        switch(evt) {
        case ONOFF:
            currState = OFF;
            remove_timeout(timeout_id);
            turn_off();
            break;
        case PLUS:
            currState = SLOW;
            remove_timeout(timeout_id);
            change_speed();
            break;
        case TIMEOUT:
            currState = SLOW;
            change_speed();
        }
    }

    void handler(Evt_t evt) {
        print_line(3, evt_names[evt]);
        switch (currState) {
        case OFF:
            off_handler(evt);
            break;
        case SLOW:
            slow_handler(evt);
            break;
        case MED:
            med_handler(evt);
            break;
        case FAST:
            fast_handler(evt);
            break;
        }
        print_state();
    }    
};


int main()
{

    init_display(2,2,40,6);
    print_line(5, "(*) : on/off    (+) : increase speed ");
    print_line(6, "(q) : quit simulation");
    timer_list_init();

    FanMachine sm;

    sm.print_state();
    while (1) {
        int c = get_input();
        if (c == '*') 
            sm.send(FanMachine::ONOFF);
        else if (c == '+')
            sm.send(FanMachine::PLUS);
        else if (c == 'q') {
            print_at(LOG_AREA_X, LOG_AREA_Y, "Bye");
            show_display();
            break;
        }
    }
    end_display();
}
