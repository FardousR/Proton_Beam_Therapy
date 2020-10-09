

#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4UIQt.hh"
#include "G4SystemOfUnits.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4StepLimiterPhysics.hh"
#include <getopt.h>

// #ifdef G4MULTITHREADED
// #include "G4MTRunManager.hh"
// #else
#include "G4RunManager.hh"
// #endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


int main(int argc,char** argv)
{
    DetectorConstruction* detector;
    G4double beam_energy = 150;
    G4double beam_size = 1.5;
    G4double beam_alpha = 4.0;
    G4double beam_beta  = 0.8;
    G4int    num_particle = 1500;
    G4double max_step_lenght = .02;

    G4double   beam_energy_input;
    G4double   beam_size_input;
    G4double   beam_alpha_x_input;
    G4double   beam_alpha_y_input;
    G4double   beam_beta_x_input;
    G4double   beam_beta_y_input;

  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv);
  }

  // Choose the Random engine
  G4Random::setTheSeed(time(NULL));
  G4Random::setTheEngine(new CLHEP::RanecuEngine);


 //Construct the default run manager
  // #ifdef G4MULTITHREADED
  //   G4MTRunManager* runManager = new G4MTRunManager;
  // #else
    G4RunManager* runManager = new G4RunManager;
  // #endif

  /************** Set mandatory initialization classes*************/

  // Detector construction
  detector = new DetectorConstruction();
  runManager->SetUserInitialization(detector);
  // Physics list
  G4PhysListFactory physListFactory;
  const G4String phylistname = "QGSP_BIC_EMY";
  G4VModularPhysicsList* physicsList = physListFactory.GetReferencePhysList(phylistname);
  runManager->SetUserInitialization(physicsList);
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(beam_energy,
                                                             beam_size_input,
                                                             beam_alpha_x_input,
                                                             beam_alpha_y_input,
                                                             beam_beta_x_input,
                                                             beam_beta_y_input));


  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
