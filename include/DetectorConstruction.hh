
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4UserLimits.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4bool       collimator_input,
                         G4double     collimator_radius_input,
                         G4double     max_step_lenght_input);
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:
    G4UserLimits*      StepLimit;
    G4bool             collimator;
    G4double           collimator_radius;
    G4double           max_step_lenght;

  protected:
    G4LogicalVolume*  fScoringVolume;
};
#endif
