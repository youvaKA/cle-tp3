#include <sm_defs.h>
#include <timeout.h>
#include <state_chart.hpp>
#include <transition.hpp>
#include <iostream>
#include <pthread.h>

using namespace std;

enum Events { TICK, START, BOOST };

pthread_mutex_t display_m = PTHREAD_MUTEX_INITIALIZER;

void start_engine(int level)
{
    pthread_mutex_lock(&display_m);
    cout << "Engine started at "
         << ((level == 0) ? "SLOW" : "FAST")
         << " speed" << endl; 
    pthread_mutex_unlock(&display_m);
}

void stop_engine()
{
    pthread_mutex_lock(&display_m);
    cout << "Engine stopped" << endl;
    pthread_mutex_unlock(&display_m);
}

void start_fan()
{
    pthread_mutex_lock(&display_m);
    cout << "Fan started" << endl;
    pthread_mutex_unlock(&display_m);
}

void stop_fan()
{
    pthread_mutex_lock(&display_m);
    cout << "Fan stopped" << endl;
    pthread_mutex_unlock(&display_m);
}

void show(int temp, const SimpleState &state)
{
    pthread_mutex_lock(&display_m);
    cout << "-----------------------------------------------" << endl;
    cout << " State : " <<  state.get_name() << endl;
    cout << " Temperature is now : " << temp << endl; 
    cout << " Press: s to start " << endl;
    cout << "        b to boost" << endl;
    cout << "        q to quit" << endl;
    cout << "-----------------------------------------------" << endl;   
    pthread_mutex_unlock(&display_m);
}

void show_temp(int temp)
{
    cout << " Temperature is now : " << temp << endl; 
}

//----------------------------

class MainSM : public StateMachine {
    SimpleState *root;
public:
    MainSM(SimpleState &s) : root(&s)
        {}
    void handler(Evt_t evt) {
        root->handler(evt);
        if (evt == TICK) 
            set_timeout(1, SEC, TICK, this);
    }
};

int main()
{
    timer_list_init();

    CompositeState Root("Root");
    MainSM sm(Root);

    int temp = 10;
    
    SimpleState Off("Off");
    CompositeState On("On");
    CompositeState Pause("Pause");
    Transition Off_On(Off, On, START);    
    
    Transition On_Pause(On, Pause,
                        [&temp](Evt_t evt) {
                            return (evt == TICK) and (temp > 30);
                        },
                        []() {
                            stop_engine();
                        });

    Transition Pause_On(Pause, On,
                        [&temp](Evt_t evt) {
                            return (evt == START) and (temp <= 20);
                        },
                        empty_action);
    

    Root.add_child(Off);
    Root.add_child(On);
    Root.add_child(Pause);
    Root.set_initial(Off);

    // On composite State
    SimpleState Working("Working", []() { start_engine(0); },   // entry
                        empty_action);                          // exit

    SimpleState Boost("Boost", []() { start_engine(1); },       // entry
                      empty_action);                            // exit

    Transition Working_Working(Working, Working,
                             [&temp](Evt_t evt) {
                                 return (evt == TICK) and (temp <= 30);
                             },
                             [&temp]() {
                                 temp++;
                                 show_temp(temp);
                             });
    Transition Boost_Boost(Boost, Boost,
                           [&temp](Evt_t evt) {
                               return (evt == TICK) and (temp <= 30);
                           },
                           [&temp]() {
                               temp += 2;
                               show_temp(temp);
                           });
    Transition Working_Boost(Working, Boost, BOOST);
    Transition Boost_Working(Boost,Working, BOOST);

    On.add_child(Working);
    On.add_child(Boost);
    On.set_initial(Working);
    On.set_history(true);

    // Pause composite state
    SimpleState Cooldown("Cooldown", []() { start_fan(); },   // entry
                         []() { stop_fan(); });   // exit

    SimpleState Ready("Ready");
    
    Transition Cooldown_Cooldown(Cooldown, Cooldown,
                                 [&temp](Evt_t evt) {
                                     return (evt == TICK) and (temp > 20);
                                 },
                                 [&temp]() {
                                     temp--;
                                     show_temp(temp);
                                 });
    
    Transition Cooldown_Ready(Cooldown, Ready,
                              [&temp](Evt_t evt) {
                                  return (evt == TICK) and (temp <= 20);
                              },
                              [&temp, &Root]() {
                              });
    Pause.add_child(Ready);
    Pause.add_child(Cooldown);
    Pause.set_initial(Cooldown); 
    
    // main cycle

    set_timeout(1, SEC, TICK, &sm);
    int c = 0;
    show(temp, Root);
    while ( (c = getchar()) != 'q') {
        if (c == 's') sm.send(START);
        else if (c == 'b') sm.send(BOOST);
        show(temp, Root);
    }  
    return 0;
}
