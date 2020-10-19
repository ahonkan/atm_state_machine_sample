#include "gtest/gtest.h" 

#include <stdio.h>

#include "ATMStateObjectBase.h"
#include "ATMEvents.h"
#include "ATMStateMachine.cc"


#ifndef LOG_PRINT
#define LOG_PRINT(...)  do { printf("%s:%s:%d ", __FILE__, __func__, __LINE__); printf(__VA_ARGS__); } while(0) 
#endif

// Test States for FSM Testing
//
class ATMState_StateA: public ATMStateObjectBase {
public:
  ATMState_StateA() : 
    enter_event(ATMEvents::kInvalid), run_loop_event(ATMEvents::kInvalid), exit_event(ATMEvents::kInvalid), 
    enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateA() {}

  virtual bool enter(ATMEvents event) { LOG_PRINT("StateA\n"); enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { LOG_PRINT("StateA\n"); run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { LOG_PRINT("StateA\n"); exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

class ATMState_StateB: public ATMStateObjectBase {
public:
  ATMState_StateB() : 
    enter_event(ATMEvents::kInvalid), run_loop_event(ATMEvents::kInvalid), exit_event(ATMEvents::kInvalid), 
    enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateB() {}

  virtual bool enter(ATMEvents event) { LOG_PRINT("StateB\n"); enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { LOG_PRINT("StateB\n"); run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { LOG_PRINT("StateB\n"); exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

class ATMState_StateC: public ATMStateObjectBase {
public:
  ATMState_StateC() : 
    enter_event(ATMEvents::kInvalid), run_loop_event(ATMEvents::kInvalid), exit_event(ATMEvents::kInvalid), 
    enter_rc(false), run_loop_rc(false), exit_rc(false) {}
  ~ATMState_StateC() {}

  virtual bool enter(ATMEvents event) { LOG_PRINT("StateC\n"); enter_event = event; return enter_rc; }
  virtual bool run_loop(ATMEvents event) { LOG_PRINT("StateC\n"); run_loop_event = event; return run_loop_rc; }
  virtual bool exit(ATMEvents event) { LOG_PRINT("StateC\n"); exit_event = event; return exit_rc; }

  ATMEvents enter_event;
  ATMEvents run_loop_event;
  ATMEvents exit_event;

  bool enter_rc;
  bool run_loop_rc;
  bool exit_rc;
};

static ATMState_StateA gStateA;
static ATMState_StateB gStateB;
static ATMState_StateC gStateC;


class ATMStateMachine_Exposed : public ATMStateMachine {
public:
  ATMStateMachine_Exposed(ATMStateObjectBase& start) : ATMStateMachine(start) { LOG_PRINT("enter\n"); }

  ATMStateObjectBase *exposed_current_state_(void) { return current_state_; }
  void exposed_current_state_(ATMStateObjectBase *obj) { current_state_ = obj; }

  TransitionMap& exposed_transition_map_(void) { return transition_map_; }
  std::queue<ATMEvents>& exposed_event_queue_(void) { return event_queue_; }

  typedef EventAction Exposed_EventAction;
  typedef TransitionMap Exposed_TransitionMap;

};

class ATMStateMachine_test_sm : public ATMStateMachine_Exposed {
public:
  ATMStateMachine_test_sm(ATMStateObjectBase& start) : ATMStateMachine_Exposed(start) { LOG_PRINT("enter\n");}
};

void ATMStateMachine::setup_transition_map() {

  transition_map_[&gStateA][kPowerUp] = &gStateB;
  transition_map_[&gStateB][kSystemReady] = &gStateC;
  transition_map_[&gStateC][kCardInserted] = &gStateA;

  transition_map_[&gStateA][kCancel] = &gStateC;
  transition_map_[&gStateC][kPinEntryTimeout] = &gStateB;
  transition_map_[&gStateB][kAuthOk] = &gStateA;
}


/*
 * Test successful transition of state from State A to State B when
 * the power up event has occured
 */
TEST(StateMachine, TestSMOperations_Success_Transition) {
  ATMStateMachine_test_sm test_sm(gStateA);
  ATMStateMachine_test_sm::Exposed_TransitionMap::iterator tmap_it; 
  ATMStateMachine_test_sm::Exposed_EventAction::iterator event_it; 

  // Validate the transition map is setup correctly
  //
  tmap_it = test_sm.exposed_transition_map_().find(&gStateA);
  ASSERT_EQ(false, tmap_it == test_sm.exposed_transition_map_().end());
  
  event_it = tmap_it->second.find(kPowerUp); 
  ASSERT_EQ(false, event_it == tmap_it->second.end());
  ASSERT_EQ(&gStateB, event_it->second);

  tmap_it = test_sm.exposed_transition_map_().find(&gStateB);
  ASSERT_EQ(false, tmap_it == test_sm.exposed_transition_map_().end());

  tmap_it = test_sm.exposed_transition_map_().find(&gStateC);
  ASSERT_EQ(false, tmap_it == test_sm.exposed_transition_map_().end());

  /*
   * When SM object starts, it sends the PowerUp event to the passed object.
   * which is StateA and the sm transitions to state b
   */
  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kInvalid);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);

  gStateA.exit_rc = true;
  gStateB.enter_rc = true;
  test_sm.execute();

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateB);
}

/*
 * Test failed transition of state from State A to State B when
 * the power up event has occured but the exit from State A fails.
 */
TEST(StateMachine, TestSMOperations_Fail_Transition_Exit) {
  ATMStateMachine_test_sm test_sm(gStateA);
  ATMStateMachine_test_sm::Exposed_TransitionMap::iterator tmap_it; 
  ATMStateMachine_test_sm::Exposed_EventAction::iterator event_it; 

  // Reset state machine back to uninitialized
  // 
  gStateA.enter_event = ATMEvents::kInvalid;
  gStateA.run_loop_event = ATMEvents::kInvalid;
  gStateA.exit_event = ATMEvents::kInvalid;

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kInvalid);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);

  gStateA.exit_rc = false;
  gStateB.enter_rc = false;
  test_sm.execute();

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);
}

/*
 * Test failed transition of state from State A to State B when
 * the power up event has occured, the exit from State A succeeds, but the 
 * entry to State B fails.
 */
TEST(StateMachine, TestSMOperations_Fail_Transition_Enter) {
  ATMStateMachine_test_sm test_sm(gStateA);
  ATMStateMachine_test_sm::Exposed_TransitionMap::iterator tmap_it; 
  ATMStateMachine_test_sm::Exposed_EventAction::iterator event_it; 

  // Reset state machine back to uninitialized
  // 
  gStateA.enter_event = ATMEvents::kInvalid;
  gStateA.run_loop_event = ATMEvents::kInvalid;
  gStateA.exit_event = ATMEvents::kInvalid;

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kInvalid);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);

  gStateA.exit_rc = true;
  gStateB.enter_rc = false;
  test_sm.execute();

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);
}

/*
 * Test sucessive transitions from A->B->C->A
 */
TEST(StateMachine, TestSMOperations_Success_Transition_ABCA) {
  ATMStateMachine_test_sm test_sm(gStateA);
  
  /*
   * When SM object starts, it sends the PowerUp event to the passed object.
   * which is StateA and the sm transitions to state b
   */
  gStateA.enter_event = ATMEvents::kInvalid;
  gStateA.run_loop_event = ATMEvents::kInvalid;
  gStateA.exit_event = ATMEvents::kInvalid;

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kInvalid);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);

  gStateA.exit_rc = true;
  gStateB.enter_rc = true;
  test_sm.execute();

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(gStateA.exit_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateB);
  ASSERT_EQ(gStateB.enter_event, ATMEvents::kPowerUp);
  ASSERT_EQ(gStateB.run_loop_event, ATMEvents::kPowerUp);

  gStateB.exit_rc = true;
  gStateC.enter_rc = true;
  test_sm.raise_event(kSystemReady);
  test_sm.execute();

  ASSERT_EQ(gStateB.run_loop_event, ATMEvents::kSystemReady);
  ASSERT_EQ(gStateB.exit_event, ATMEvents::kSystemReady);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateC);
  ASSERT_EQ(gStateC.enter_event, ATMEvents::kSystemReady);
  ASSERT_EQ(gStateC.run_loop_event, ATMEvents::kSystemReady);

  gStateC.exit_rc = true;
  gStateA.enter_rc = true;
  test_sm.raise_event(kCardInserted);
  test_sm.execute();

  ASSERT_EQ(gStateC.run_loop_event, ATMEvents::kCardInserted);
  ASSERT_EQ(gStateC.exit_event, ATMEvents::kCardInserted);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);
  ASSERT_EQ(gStateA.enter_event, ATMEvents::kCardInserted);
  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kCardInserted);

}




