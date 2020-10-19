#include "gtest/gtest.h" 

#include <stdio.h>

#include "ATMStateObjectBase.h"
#include "ATMEvents.h"
#include "ATMStateMachine.cc"
#include "ATMStateMachine_transitions.cc"

#include "ATMState_Authenticate.cc"
#include "ATMState_SystemError.cc"
#include "ATMState_Idle.cc"
#include "ATMState_Transaction.cc"
#include "ATMState_PowerUp.cc"
#include "ATMStateObjects.cc"

#ifndef LOG_PRINT
#define LOG_PRINT(...)  do { printf("%s:%s:%d ", __FILE__, __func__, __LINE__); printf(__VA_ARGS__); } while(0) 
#endif

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


/*
 * Test successful transition of state from State A to State B when
 * the power up event has occured
 */
TEST(StateMachine, TestSMOperations_Success_Transition) {
  ATMStateMachine_test_sm test_sm(gAtmState_PowerUp);
  ATMStateMachine_test_sm::Exposed_TransitionMap::iterator tmap_it; 
  ATMStateMachine_test_sm::Exposed_EventAction::iterator event_it; 

  // Validate the transition map is setup correctly
  //
  tmap_it = test_sm.exposed_transition_map_().find(&gAtmState_PowerUp);
  ASSERT_EQ(false, tmap_it == test_sm.exposed_transition_map_().end());
  
  event_it = tmap_it->second.find(kSystemError); 
  ASSERT_EQ(false, event_it == tmap_it->second.end());
  ASSERT_EQ(&gAtmState_SystemError, event_it->second);

  /*
   * When SM object starts, it sends the PowerUp event to the passed object.
   * which is StateA and the sm transitions to state b
   */
/*
  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kInvalid);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateA);

  gStateA.exit_rc = true;
  gStateB.enter_rc = true;
  test_sm.execute();

  ASSERT_EQ(gStateA.run_loop_event, ATMEvents::kPowerUp);
  ASSERT_EQ(test_sm.exposed_current_state_(), &gStateB);
*/
}

#if 0
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
#endif




