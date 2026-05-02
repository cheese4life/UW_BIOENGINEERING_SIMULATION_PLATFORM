/*
   The copyright to the computer program(s) herein is the property of
   Invetech Pty Ltd, Australia. This file is subject to the terms and
   conditions found in the 'IMPLicense.txt' file included with this
   source code package.
*/

#ifndef FSMLIST_HPP_INCLUDED
#define FSMLIST_HPP_INCLUDED

#include <tinyfsm.hpp>

#include "elevator.hpp"
#include "motor.hpp"

using fsm_list = tinyfsm::FsmList<Motor, Elevator>;

/** dispatch event to both "Motor" and "Elevator" */
template<typename E>
void send_event(E const & event)
{
  fsm_list::template dispatch<E>(event);
}


#endif
