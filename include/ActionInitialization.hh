
#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization(G4double   beam_energy_input,
                         G4double   beam_size_input,
                         G4double   beam_alpha_x_input,
                         G4double   beam_alpha_y_input,
                         G4double   beam_beta_x_input,
                         G4double   beam_beta_y_input);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

   private:
       G4double   beam_energy;
       G4double   beam_size;
       G4double   beam_alpha_x;
       G4double   beam_alpha_y;
       G4double   beam_beta_x;
       G4double   beam_beta_y;
       G4String   output_filename;
       G4String   output_location;
       G4String   Output_File;    
};
#endif
