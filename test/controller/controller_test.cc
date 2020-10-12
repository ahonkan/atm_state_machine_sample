#include "gtest/gtest.h" 



#include "ATMStateObjectBase.h"
#include "ATMEvents.h"
#include "ATMStateMachine.cc"


// Test States for FSM Testing
//
class ATMState_StateA: public ATMStateObjectBase {
public:
  ATMState_StateA() : enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateA() {}

  virtual bool enter(ATMEvents event) { enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

class ATMState_StateB: public ATMStateObjectBase {
public:
  ATMState_StateB() : enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateB() {}

  virtual bool enter(ATMEvents event) { enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

class ATMState_StateC: public ATMStateObjectBase {
public:
  ATMState_StateC() : enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateC() {}

  virtual bool enter(ATMEvents event) { enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

ATMState_StateA gStateA;
ATMState_StateB gStateB;
ATMState_StateC gStateC;


class ATMStateMachine_Exposed : public ATMStateMachine {
public:
  ATMStateMachine_Exposed(ATMStateObjectBase& start) : ATMStateMachine(start) {}

  ATMStateObjectBase *exposed_current_state_(void) { return current_state_; }
  void exposed_current_state_(ATMStateObjectBase *obj) { current_state_ = obj; }

  TransitionMap& exposed_transition_map_(void) { return transition_map_; }
  std::queue<ATMEvents>& exposed_event_queue_(void) { return event_queue_; }

};

class ATMStateMachine_test_sm : public ATMStateMachine_Exposed {
public:
  ATMStateMachine_test_sm(ATMStateObjectBase& start) : ATMStateMachine_Exposed(start) {}
};

void ATMStateMachine::setup_transition_map() {

  transition_map_[&gStateA][kPowerUp] = &gStateB;
  transition_map_[&gStateB][kSystemReady] = &gStateC;
  transition_map_[&gStateC][kCardInserted] = &gStateA;

  transition_map_[&gStateA][kCancel] = &gStateC;
  transition_map_[&gStateC][kPinEntryTimeout] = &gStateB;
  transition_map_[&gStateB][kAuthOk] = &gStateA;
}



TEST(StateMachine, TestSMOperations) {
  ATMStateMachine_test_sm test_sm(gStateA);
//  test_sm.setup_transition_map();

  /*
   * When SM object starts, it sends the PowerUp event to the passed object.
   * which is StateA and the sm transitions to state b
   */
  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateB);
  
}
