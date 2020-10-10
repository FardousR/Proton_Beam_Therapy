
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "G4SystemOfUnits.hh"


ActionInitialization::ActionInitialization(G4double   beam_energy_input,
                                           G4double   beam_sigma_x_input,
                                           G4double   beam_sigma_y_input,
                                           G4double   beam_alpha_x_input,
                                           G4double   beam_alpha_y_input,
                                           G4double   beam_beta_x_input,
                                           G4double   beam_beta_y_input,
                                           G4bool     collimator_input,
                                           G4double   collimator_radius_input,
                                           G4double   max_step_lenght__input,
                                           G4double   gun_z_position_input): G4VUserActionInitialization(),
                                           beam_energy          (beam_energy_input),
                                           beam_sigma_x         (beam_sigma_x_input),
                                           beam_sigma_y         (beam_sigma_y_input),
                                           beam_alpha_x         (beam_alpha_x_input),
                                           beam_alpha_y         (beam_alpha_y_input),
                                           beam_beta_x          (beam_beta_x_input),
                                           beam_beta_y          (beam_beta_y_input),
                                           collimator           (collimator_input),
                                           collimator_radius    (collimator_radius_input),
                                           max_step_lenght      (max_step_lenght__input),
                                           gun_z_position       (gun_z_position_input)
{
  output_location = "/home/fardous/Desktop/Proton_Beam/OutputFile/";
  output_filename = "Energy_"+std::to_string(beam_energy)+"Alpha:_x"+std::to_string(beam_alpha_x)+"_y_"+std::to_string(beam_alpha_y)+
                             "Beta:_x"+std::to_string(beam_beta_x)+"_y_"+std::to_string(beam_beta_y)+".root";
  Output_File      = output_location + output_filename;
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{

    RunAction* runAction = new RunAction(Output_File);
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{

  PrimaryGeneratorAction* primaryGeneratorAction = new PrimaryGeneratorAction(beam_energy,
                                                                              beam_alpha_x,
                                                                              beam_alpha_y,
                                                                              beam_beta_x,
                                                                              beam_beta_y,
                                                                              beam_sigma_x,
                                                                              beam_sigma_y,
                                                                              gun_z_position);
  SetUserAction(primaryGeneratorAction);

  RunAction* runAction = new RunAction(Output_File);
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction(runAction);
  SetUserAction(eventAction);

  SteppingAction* steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);
}
