

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

    int getopt_char;
    int getopt_idx;

    G4UIExecutive* ui = 0;
    if ( argc == 1 ) {
      ui = new G4UIExecutive(argc, argv);
    }
    G4cout<<argv[0];
    G4cout<<"\n"<<argv[1];

    //Parse command line arguments

    DetectorConstruction* detector;

    G4double   beam_energy    = 150*MeV;
    G4double   beam_sigma_x   = 10.0*mm;
    G4double   beam_sigma_y   = 10.0*mm;
    G4double   beam_alpha_x   = 4.0;
    G4double   beam_alpha_y   = 4.0;
    G4double   beam_beta_x    = 0.8*m;
    G4double   beam_beta_y    = 0.8*m;
    G4int      num_particle   = 100;

    G4double   gun_z_position      = -1.0*mm;
    G4bool     collimator          = false;
    G4double   collimator_radius   = 4.0*mm;
    G4double   max_step_lenght     = 0.2*mm;
    G4bool     useGUI              = false;



  if(collimator)
    {
        gun_z_position = -800.00*mm;
    }
  else
   {
       gun_z_position = - 0.1*mm;
   }


   // static struct option long_options[] = {
   //                                        {"energy",                    required_argument, NULL, 'e'   },
   //                                        {"alphax",                    required_argument, NULL, 'ax'  },
   //                                        {"betax",                     required_argument, NULL, 'bx'  },
   //                                        {"sizex",                     required_argument, NULL, 'sx'  },
   //                                        {"alphay",                    required_argument, NULL, 'ay'  },
   //                                        {"betay",                     required_argument, NULL, 'by'  },
   //                                        {"sizey",                     required_argument, NULL, 'sy'  },
   //                                        {"number",                    required_argument, NULL, 'n'   },
   //                                        {"step",                      required_argument, NULL, 'l'   },
   //                                        {"gui",                       no_argument,       NULL, 'g'   },
   //                                        {"col",                       no_argument,       NULL, 'c'   },
   //                                        {"colrad",                    required_argument, NULL, 'r'   },
   //                                        {0,0,0,0}
   // };
   //
   // while ( (getopt_char = getopt_long(argc,argv, "e:ax:ay:bx:by:l:n:sx:sy:r:gc", long_options, &getopt_idx)) != -1) {
   //     switch(getopt_char) {
   //
   //     case 'g': //Use GUI
   //         useGUI = true;
   //         break;
   //
   //     case 'c': //Collimator On/Off
   //         collimator = true;
   //         break;
   //
   //
   //     case 'e': //Beam Energy
   //         try
   //         {
   //           beam_energy = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading beam energy" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'ax': //Beam Alpha(x)
   //         try {
   //             beam_alpha_x  = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading Alpha(x)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'ay': //Beam Alpha(y)
   //         try {
   //             beam_alpha_y  = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading Alpha(y)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'bx': //Beam Beta(x)
   //         try {
   //             beam_beta_x = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading Beta(x)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'by': //Beam Beta(y)
   //         try {
   //             beam_beta_y = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading Beta(y)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'n': //Number of Particle
   //         try {
   //             num_particle = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading number of particles" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a integer number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'sx': //Beam Sigma(x)
   //         try {
   //             beam_sigma_x = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading beam sigma(x)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a integer number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'sy': //Beam Sigma(y)
   //         try {
   //             beam_sigma_y = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading beam sigma(y)" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a integer number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //
   //     case 'l': // Maximum step lenght
   //         try {
   //             max_step_lenght = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading beam step limit" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //
   //     case 'r': // Maximum step lenght
   //         try {
   //             collimator_radius = std::stod(std::string(optarg));
   //         }
   //         catch (const std::invalid_argument& ia) {
   //             G4cout << "Invalid argument when reading collimator radius" << G4endl
   //                    << "Got: '" << optarg << "'" << G4endl
   //                    << "Expected a floating point number! (exponential notation is accepted)" << G4endl;
   //             exit(1);
   //         }
   //         break;
   //     }
   // }
   //
   // int argc_effective = argc-optind+1;
   // char** argv_effective = new char*[argc_effective];
   // argv_effective[0] = argv[0]; //First arg is always executable name
   // for (int i = optind; i < argc;i++){
   //     argv_effective[i+1-optind] = argv[i];
   // }

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
    UImanager->ApplyCommand("/run/beamOn 100");
    ui->SessionStart();
    delete ui;
  }

  // else{
  //   // interactive mode
  //   UImanager->ApplyCommand("/control/execute init_vis.mac");
  //   UImanager->ApplyCommand("/run/beamOn 100");
  //   ui->SessionStart();
  //   delete ui;
  // }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}
