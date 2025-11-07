

#include "ActionInitialization.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_RunAction.hh"
#include "BM_EventAction.hh"
#include "BM_SteppingAction.hh"
#include <chrono>
#include <thread>
// #include <mutex>
// #include "BM_TrackingAction.hh"


ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}


ActionInitialization::~ActionInitialization()
{
}


void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new BM_RunAction());
}


void ActionInitialization::Build() const
{
  using namespace std::this_thread; // sleep_for, sleep_until
  using namespace std::chrono;      // nanoseconds, system_clock, seconds
  // sleep_for(nanoseconds(10000));
  SetUserAction(new BM_PrimaryGenerator());
  //  static bool initialized;
  //  if (!initialized) {
  //  SetUserAction(new BM_RunAction());
  //  initialized = true;
  //  }
  SetUserAction(new BM_EventAction());
  SetUserAction(new BM_SteppingAction());

  // SetUserAction(new BM_TrackingAction());
}

