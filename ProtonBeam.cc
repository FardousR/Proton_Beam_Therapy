

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

    G4double   beam_energy    = 150*MeV;
    G4double   beam_sigma_x   = 20.0*mm;
    G4double   beam_sigma_y   = 20.0*mm;
    G4double   beam_alpha_x   = 8.0;
    G4double   beam_alpha_y   = 8.0;
    G4double   beam_beta_x    = 0.4*m;
    G4double   beam_beta_y    = 0.4*m;
    G4int      num_particle   = 100;

    G4double   gun_z_position      = -1.0*mm;
    G4bool     collimator          = false;
    G4double   collimator_radius   = 5.0*mm;
    G4double   max_step_lenght     = 10.0*mm;



  if(collimator)
    {
        gun_z_position = -800.00*mm;
    }
  else
   {
       gun_z_position = - 0.1*mm;
   }



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
  detector = new DetectorConstruction(collimator,
                                      collimator_radius,
                                      max_step_lenght);
  runManager->SetUserInitialization(detector);
  // Physics list
  G4PhysListFactory physListFactory;
  const G4String phylistname = "QGSP_BIC_EMY";
  G4VModularPhysicsList* physicsList = physListFactory.GetReferencePhysList(phylistname);
  physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization(beam_energy,
                                                             beam_sigma_x,
                                                             beam_sigma_y,
                                                             beam_alpha_x,
                                                             beam_alpha_y,
                                                             beam_beta_x,
                                                             beam_beta_y,
                                                             collimator,
                                                             collimator_radius,
                                                             max_step_lenght,
                                                             gun_z_position));


  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session

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

  // UImanager->ApplyCommand(G4String("/run/beamOn ") + std::to_string(num_particle));

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
