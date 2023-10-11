#include <sm_defs.h>
#include <timeout.h>
#include <iostream>
#include <state_chart.hpp>

using namespace std;

class MainSM : public StateMachine {
    SimpleState *root_state;
public:
    MainSM(SimpleState &root) : root_state(&root) {}
    void handler(Evt_t evt) {
        root_state->handler(evt);
    }
};


enum Events {TIMEOUT, COFFEE, MONEY, TAKEN};

void start_coffee()
{
    cout << "Making coffee ... please wait" << endl; 
}

void release_coffee()
{
    cout << "Coffee is ready ! You can take it" << endl; 
}

void show_display(int m)
{
    cout << "--------------------------------------------------" << endl;
    cout << " Money : " << m << endl;
    cout << " Press : m for inserting a coin " << endl;
    cout << "       : c for ordering a coffee (2 coins) " << endl;
    cout << "       : t for taking the coffee when it is ready" << endl;
    cout << "       : q to quit simulation" << endl;
    cout << "--------------------------------------------------" << endl;    
}


int main()
{
    // initialize the timer
    timer_list_init();

    // the root state
    OrthogonalState CoffeeMachine("CoffeMachine");
    // the state machine structure (thread)
    MainSM sm(CoffeeMachine);

    // global variable for this machine
    int money = 0;
    
    CompositeState Region1("Region1");
    CompositeState Region2("Region2");

    CoffeeMachine.add_region(Region1);
    CoffeeMachine.add_region(Region2);

    // FIRST REGION
    SimpleState Idle("Idle");
    SimpleState MakingCoffee("Making", [&sm](){ set_timeout(10, SEC, TIMEOUT, &sm); }, // entry
                             empty_action);                                  // exit
    SimpleState CoffeeReady("Ready");
    Region1.add_child(Idle);
    Region1.add_child(MakingCoffee);
    Region1.add_child(CoffeeReady);
    Region1.set_initial(Idle);

    Transition Idle_Making(Idle, MakingCoffee, // source and destination
                           [&money](Evt_t evt) { return (evt == COFFEE) and money >=2; },
                           [](){ start_coffee(); }
        );

    Transition Idle_Idle(Idle, Idle,
                         [&money](Evt_t evt) {
                             return (evt == COFFEE) and (money <2); },
                         []() { cout << "Not enough money for a coffee!" << endl; });
                         
    Transition Making_Ready(MakingCoffee, CoffeeReady,
                            TIMEOUT, // Event, no boolean guard 
                            [&money](){ release_coffee();}
        );
    Transition Ready_Idle(CoffeeReady, Idle,
                          TAKEN,
                          [&money](){ show_display(money); });
    

    // Second region
    SimpleState Accept("Accept");
    SimpleState NoAccept("NoAccept");
    Region2.add_child(Accept);
    Region2.add_child(NoAccept);
    Region2.set_initial(Accept);
    
    Transition Accept_Accept(Accept, Accept,
                             MONEY,
                             [&money](){ money++; show_display(money);}
        );
    Transition Accept_NoAccept(Accept, NoAccept,
                               [&money](Evt_t evt) {
                                   return (evt == COFFEE) and (money >= 2);
                               },
                               [&money]() { money-=2; }
        );
    Transition NoAccept_Accept(NoAccept, Accept,
                               TAKEN,
                               empty_action
        );
    Transition NoAccept_NoAccept(NoAccept, NoAccept,
                                 MONEY,
                                 [](){ cout << "Money not accepted" << endl;}
        );
    
    Transition NoAccept_NoAccept2(NoAccept, NoAccept,
                                  COFFEE,
                                  [](){ cout << "Cannot order coffee now" << endl;}
        );
    
    // Main loop

    int c;

    show_display(money);

    while ((c = getchar()) != 'q') {
        if (c == 'm') sm.send(MONEY);
        else if (c == 'c') sm.send(COFFEE);
        else if (c == 't') sm.send(TAKEN);
    }
    
    return 0;
}
