#include <fstream>
#include <string>
// #define SEG_DBG 
#ifdef SEG_DBG
  #include <stdio.h>
  #include <execinfo.h>
  #include <signal.h>
  #include <stdlib.h>
  #include <unistd.h>
#endif

#include "g4root.hh"
#include "G4Types.hh"
#include "G4UImanager.hh"

#define G4VIS_USE
#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
  #include "G4UIExecutive.hh"
#endif
// #define G4MULTITHREADED
#ifdef G4MULTITHREADED
   #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif
#include "BM_RunAction.hh"
#include "BM_EventAction.hh"
#include "BM_SteppingAction.hh"
//#include "BM_TrackingAction.hh" 
#include "ActionInitialization.hh"
//#include "BM_PhysicsList.hh" // removed
#include "QBBC.hh"

#include "BM_Detector.hh"
#include "BM_PrimaryGenerator.hh"
#include "BM_Output.hh"

class TFile;
#include <TFile.h>
#ifdef SEG_DBG
void handler(int sig) // for segfault
{
   void* array[10];
   size_t size;
   size = backtrace(array, 10);
   fprintf(stderr, "Error: signal-%d:\n", sig);
   backtrace_symbols_fd(array, size, STDERR_FILENO);
   exit(1);
}
#endif

int main(int argc, char** argv)
{ 
  G4cout << "Hello!  I'm in the main function." << G4endl;
  
  #ifdef SEG_DBG
    std::cout << "Using our segfault debugger...\n";
    signal(SIGSEGV, handler); // using the segfault handler
  #endif

  //Change to G4MTRunManager if we want multithreading
  // #define G4MULTITHREADED
  // #ifdef G4MULTITHREADED
  // G4MTRunManager* runManager = new G4MTRunManager;
  // runManager->SetNumberOfThreads((G4Threading::G4GetNumberOfCores())-2);
  // G4cout << "Multithreaded, you crazy kid" << G4endl;
  // #else
  G4RunManager* runManager = new G4RunManager;
  G4cout << "Single threaded" << G4endl;
  // #endif
  runManager->SetVerboseLevel(1);

  // initialize detector
  runManager->SetUserInitialization(new BM_Detector());
  
  // initialize physics list - use a standard one for now
  //runManager->SetUserInitialization(new BM_PhysicsList());
  //physlist->AddPhysicsList("local");
  
  G4VModularPhysicsList* physicsList = new QBBC; 
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);
  runManager->SetUserInitialization(new ActionInitialization());
  
  // // initialize work manager - for multithreading
  // G4WorkerRunManager* WorkManager = new G4WorkerRunManager;
  // WorkManager->SetUserAction(new BM_PrimaryGenerator());
  // runManager->SetUserAction(new BM_SteppingAction());
  // runManager->SetUserAction(new BM_EventAction());
  // runManager->SetUserAction(new BM_RunAction());
  // runManager->SetUserAction(new BM_TrackingAction());

  // runManager->Initialize();

  // initialize ROOT output
  // BM_Output::Instance()->SetFilename();
  
  // initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  #ifdef G4VIS_USE
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
  #endif

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  if (argc != 1)
  {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else
  {
    // interactive mode : define UI session
    G4UIExecutive* ui = 0;
    ui = new G4UIExecutive(argc, argv);
    // #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    //#else
    //  UImanager->ApplyCommand("/control/execute init.mac");
    //#endif
    ui->SessionStart();
    delete ui;
    // #endif
  }

  // Job termination
  // Free the store. user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  #ifdef G4VIS_USE
    delete visManager;
  #endif
  
  delete runManager;

  return 0;
}
