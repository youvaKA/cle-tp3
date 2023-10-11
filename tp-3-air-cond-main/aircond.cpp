#include <sm_defs.h>
#include <timeout.h>
#include <state_chart.hpp>
#include <iostream>


using namespace std;

class MainSM : public StateMachine {
    SimpleState *root_state;
public:
    MainSM(SimpleState &root) : root_state(&root) {}
    void handler(Evt_t evt) {
        root_state->handler(evt);
    }
};

void ON()
{
    cout << "Air cond is ON" << endl; 
}
void OFF()
{
    cout << "Air cond is OFF" << endl; 
}

enum Events {PRESS_ON_OFF, PRESS_MODE};

int main()
{
 // the root state
    CompositeState AirCond("AirCond");
    // the state machine structure (thread)
    MainSM sm(AirCond);

    SimpleState On_off("ON_OFF");
    CompositeState Manual("MANUAL");
    //OrthogonalState AUTO("AUTO");


    //add child
    AirCond.add_child(On_off);
    AirCond.add_child(Manual);  
    AirCond.set_initial(On_off);

    // MANUAL
    SimpleState Manual_pause("MANUAL_PAUSE");
    SimpleState Manual_heat("MANUAL_HEAT");
    SimpleState Manual_cool("MANUAL_COOL");

    //add child
    Manual.add_child(Manual_pause);
    Manual.add_child(Manual_heat);
    Manual.add_child(Manual_cool);
    Manual.set_initial(Manual_pause);

    //add transition
    Transition On_off_Manual(On_off, Manual,PRESS_ON_OFF,
                            []() {ON(); }
                            );

    Transition Manual_On_off(Manual, On_off,PRESS_ON_OFF,
                            []() {OFF(); }
                            );
/* 
    while (true)
    {
        // switch case
        string c;
        cin >> c;
        switch (c)
        {
        case "pressON/OFF":
            sm.send(PRESS_ON_OFF);
            break;
        case "pressMode":
            sm.send(PRESS_MODE);
            break;
        default:
            break;
        }

    }
     */

        char c;

        while ((c = getchar()) != 'q') {
            switch (c) {
                case 'm':
                    sm.send(PRESS_MODE);
                    break;
                case 'o':
                    sm.send(PRESS_ON_OFF);
                    break;
                default:
                    break;
            }
        }
        
        return 0;

}
