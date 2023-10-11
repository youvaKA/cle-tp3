#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>

#include <sm_defs.h>
#include <timeout.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include "fan.hpp"

/* ----------------------------------------*/       

void turn_on()
{
    printf("Action: Turn on FAN at SLOW speed\n");
    //show_display();
}

void turn_off()
{
    printf("Action: Turn off FAN\n");
    //show_display();
}

void change_speed()
{
    printf("Action: Changing speed\n");
    //show_display();
}

void FanMachine::print_state()
{
    std::stringstream ss;
    ss << "Status : " << state_names[currState];        
    //print_at(MSG_AREA_X-2, MSG_AREA_Y-1, "+------------ FAN DISPLAY -------------+");
    //print_at(MSG_AREA_X-2, MSG_AREA_Y,   ss.str().c_str());  
    //print_at(MSG_AREA_X-2, MSG_AREA_Y+1, "| (*) : on/off    (+) : increase speed |");
    //print_at(MSG_AREA_X-2, MSG_AREA_Y+2, "| (q) : exit simulation                |");
    //print_at(MSG_AREA_X-2, MSG_AREA_Y+3, "+--------------------------------------+");
    //refresh();
    printf("%s\n", ss.str().c_str());
    //show_display();
}
    
void FanMachine::off_handler(Evt_t evt) {
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

void FanMachine::slow_handler(Evt_t evt) {
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

void FanMachine::med_handler(Evt_t evt) {
    switch(evt) {
    case ONOFF:
        currState = OFF;
        turn_off();
        break;
    case PLUS:
        currState = FAST;
        timeout_id = set_timeout(5, SEC, TIMEOUT, this);
        printf("Setting 5 seconds timeout\n");
        //show_display();
        change_speed();
        break;
    case TIMEOUT:
        break;
    }
}
    
void FanMachine::fast_handler(Evt_t evt) {
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

void FanMachine::handler(Evt_t evt) {
    printf("%s\n",evt_names[evt]);
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


class MyWindow : public QWidget
{
    Q_OBJECT
public:
    MyWindow(QWidget *parent = nullptr);
    virtual ~MyWindow() {}                    
private slots:
    void OnPlus();
    void OnMinus();
private:
    QLabel *lbl;
    QPushButton *plusBtn;
    QPushButton *minusBtn;
    QPushButton *quitBtn;
    QGridLayout *grid;
};

MyWindow::MyWindow(QWidget *parent) : QWidget(parent)
{
    quitBtn = new QPushButton("Quit", this);

    plusBtn = new QPushButton("+", this);
    minusBtn = new QPushButton("-", this);
    grid = new QGridLayout(this);
    lbl = new QLabel("0", this);
    grid->addWidget(plusBtn, 0, 0);
    grid->addWidget(minusBtn, 0, 1);
    grid->addWidget(lbl, 1, 1);

    connect(quitBtn, &QPushButton::clicked, qApp, &QApplication::quit);
    connect(plusBtn, &QPushButton::clicked, this, &MyWindow::OnPlus);
    connect(minusBtn, &QPushButton::clicked, this, &MyWindow::OnMinus);
}


void MyWindow::OnPlus()
{
    int val = lbl->text().toInt();
    val++;
    lbl->setText(QString::number(val));
}


void MyWindow::OnMinus()
{
    int val = lbl->text().toInt();
    val--;
    lbl->setText(QString::number(val));
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    MyWindow window;
    
    window.resize(500, 400);
    window.setWindowTitle("Fan Demo");
    window.show();
    return app.exec();
}


/*
int main()
{
    //init_display(2,2,40,6);
    printf("(*) : on/off    (+) : increase speed\n");
    printf("(q) : quit simulation\n");
    timer_list_init();

    FanMachine sm;

    sm.print_state();
    while (1) {
        int c = getchar();
        if (c == '*') 
            sm.send(FanMachine::ONOFF);
        else if (c == '+')
            sm.send(FanMachine::PLUS);
        else if (c == 'q') {
            //print_at(LOG_AREA_X, LOG_AREA_Y, "Bye");
            //show_display();
            break;
        }
    }
    //end_display();
    }
*/
