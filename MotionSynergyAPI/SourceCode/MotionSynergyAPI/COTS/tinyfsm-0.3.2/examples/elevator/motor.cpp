/*
   The copyright to the computer program(s) herein is the property of
   Invetech Pty Ltd, Australia. This file is subject to the terms and
   conditions found in the 'IMPLicense.txt' file included with this
   source code package.
*/

#include <tinyfsm.hpp>
#include "motor.hpp"
#include <iostream>

using namespace std;


// ----------------------------------------------------------------------------
// Motor states
//

class Stopped
: public Motor
{
  void entry() override {
    cout << "Motor: stopped" << endl;
    direction = 0;
  };
};

class Up
: public Motor
{
  void entry() override {
    cout << "Motor: moving up" << endl;
    direction = 1;
  };
};

class Down
: public Motor
{
  void entry() override {
    cout << "Motor: moving down" << endl;
    direction = -1;
  };
};


// ----------------------------------------------------------------------------
// Base State: default implementations
//

void Motor::react(MotorStop const &) {
  transit<Stopped>();
}

void Motor::react(MotorUp const &) {
  transit<Up>();
}

void Motor::react(MotorDown const &) {
  transit<Down>();
}

int Motor::direction{0};


// ----------------------------------------------------------------------------
// Initial state definition
//
FSM_INITIAL_STATE(Motor, Stopped)
