#include <catch2/catch_test_macros.hpp>
#include "state_chart.hpp"
#include <sm_defs.h>
#include <iostream>

enum Events {EV_A, EV_B, EV_C};

TEST_CASE("Two states", "[statecharts]")
{
    int x = 0;
    int c = 0;
    
    SimpleState s1("On", [&x](){ x = 0; }, empty_action);
    SimpleState s2("Off", [&x](){ x = 1;}, empty_action);

    Transition onOff(s1, s2, EV_A, [&c](){c++;});
    Transition offOn(s2, s1, EV_B, [&c](){c++;});

    CompositeState comp("System");
    comp.add_child(s1);
    comp.add_child(s2);
    comp.set_initial(s1);

    REQUIRE(comp.get_configuration() == "System.On");
    REQUIRE(x == 0);
    comp.handler(EV_A);
    REQUIRE(comp.get_configuration() == "System.Off");
    REQUIRE(x == 1);

    comp.handler(EV_B);
    REQUIRE(x == 0);
    for (int i=0; i<10; i++) {
        comp.handler(EV_A);
        comp.handler(EV_B);
    }
    REQUIRE(c == 22);
}


TEST_CASE("out to in", "[statecharts]")
{
    int x = 0;
    int c = 0;

    CompositeState comp("System");
    CompositeState sub1("SubSys_1");
    comp.add_child(sub1);
    
    SimpleState s1("On", [&x](){ x = 0; }, empty_action);
    comp.add_child(s1);
    comp.set_initial(s1);
        
    SimpleState s2("Off", [&x](){ x = 1;}, empty_action);
    sub1.add_child(s2);
    sub1.set_initial(s2);
    
    Transition onOff(s1, s2, EV_A, [&c](){c++;});
    Transition offOn(s2, s1, EV_B, [&c](){c++;});

    REQUIRE(comp.get_configuration() == "System.On");
    REQUIRE(x == 0);
    comp.handler(EV_A);
    CHECK(comp.get_configuration() == "System.SubSys_1.Off");
    CHECK(x == 1);

    comp.handler(EV_B);
    CHECK(x == 0);
    CHECK(comp.get_configuration() == "System.On");
    
    for (int i=0; i<10; i++) {
        comp.handler(EV_A);
        comp.handler(EV_B);
    }
    REQUIRE(c == 22); 
}

TEST_CASE("history", "[statecharts]")
{
    CompositeState sys("System");
    
    CompositeState sub1("SUB_1");
    CompositeState sub2("SUB_2");
    sys.add_child(sub1);
    sys.add_child(sub2);
    sys.set_initial(sub1);
    
    SimpleState sa("SA");
    SimpleState sb("SB");
    sub1.add_child(sa);
    sub1.add_child(sb);
    sub1.set_initial(sa);

    SimpleState sc("SC");
    SimpleState sd("SD");
    sub2.add_child(sc);
    sub2.add_child(sd);
    sub2.set_initial(sc);
    sub2.set_history(true);
    
    Transition AB(sa, sb, EV_A);
    Transition CD(sc, sd, EV_A);

    Transition S12(sub1, sub2, EV_B);
    Transition S21(sub2, sub1, EV_B);
    
    REQUIRE(sys.get_configuration() == "System.SUB_1.SA");
    sys.handler(EV_A);
    REQUIRE(sys.get_configuration() == "System.SUB_1.SB");

    sys.handler(EV_B);
    REQUIRE(sys.get_configuration() == "System.SUB_2.SC");
    sys.handler(EV_A);
    REQUIRE(sys.get_configuration() == "System.SUB_2.SD");

    sys.handler(EV_B);
    REQUIRE(sys.get_configuration() == "System.SUB_1.SA");
    sys.handler(EV_B);
    REQUIRE(sys.get_configuration() == "System.SUB_2.SD");
}


