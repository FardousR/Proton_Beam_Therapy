
#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"


ActionInitialization::ActionInitialization(G4double   beam_energy_input,
                                           G4double   beam_size_input,
                                           G4double   beam_alpha_x_input,
                                           G4double   beam_alpha_y_input,
                                           G4double   beam_beta_x_input,
                                           G4double   beam_beta_y_input): G4VUserActionInitialization(),
                                           beam_energy    (beam_energy_input),
                                           beam_size      (beam_size_input),
                                           beam_alpha_x   (beam_alpha_x_input),
                                           beam_alpha_y   (beam_alpha_y_input),
                                           beam_beta_x    (beam_beta_x_input),
                                           beam_beta_y    (beam_beta_y_input)
{
  output_filename = "Energy_"+std::to_string(beam_energy)+"Alpha:_x"+std::to_string(beam_alpha_x)+"_y_"+std::to_string(beam_alpha_y)+
                             "Beta:_x"+std::to_string(beam_beta_x)+"_y_"+std::to_string(beam_beta_y);
}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    RunAction* runAction = new RunAction;
    SetUserAction(runAction);
}

void ActionInitialization::Build() const
{

  PrimaryGeneratorAction* primaryGeneratorAction = new PrimaryGeneratorAction(beam_energy,
                                                                              beam_size,
                                                                              beam_alpha_x,
                                                                              beam_alpha_y,
                                                                              beam_beta_x,
                                                                              beam_beta_y);
  SetUserAction(primaryGeneratorAction);

  RunAction* runAction = new RunAction;
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction;
  SetUserAction(eventAction);

  SteppingAction* steppingAction = new SteppingAction(eventAction);
  SetUserAction(steppingAction);


}
