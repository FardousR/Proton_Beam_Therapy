
#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

#include "TMatrixDfwd.h"
#include "TDecompChol.h"
#include "TRandom.h"

class G4ParticleGun;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
      PrimaryGeneratorAction(G4double   beam_energy_input,
                             G4double   beam_size_input,
                             G4double   beam_alpha_x_input,
                             G4double   beam_alpha_y_input,
                             G4double   beam_beta_x_input,
                             G4double   beam_beta_y_input);

    virtual ~PrimaryGeneratorAction();

    // method from the base class
    virtual void GeneratePrimaries(G4Event*);

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun*  fParticleGun;       // pointer a to G4 gun class
    G4double beam_energy;               // Beam kinetic energy [MeV]
    G4double beam_energy_spread;        // Energy Spread  [%]

    G4ParticleDefinition* particle;     // Particle type

    G4double epsN_x;                    // Normalized emittance  (x) [um]
    G4double epsG_x;                    // Geometrical emittance (x) [um]
    G4double beta_x;                    // Beta function         (x) [m]
    G4double alpha_x;                   // Alpha function        (x) [-]

    G4double epsN_y;                    // Normalized emittance  (y) [um]
    G4double epsG_y;                    // Geometrical emittance (y) [um]
    G4double beta_y;                    // Beta function         (y) [m]
    G4double alpha_y;                   // Alpha function        (y) [-]

    // Beam covariance matrices [m,rad]
    TMatrixD covarX;
    TMatrixD covarY;

    TMatrixD covarX_L;
    TMatrixD covarY_L;

    //Setup for circular uniform distribution / Rcut
    G4double Rcut; // [mm]

    TRandom* RNG;
    // Seed to use when random-generating particles within Twiss distribution
    G4int rngSeed;

    //Setup for uniform energy distribution between min/max
    G4double beam_energy_min; // [MeV]
    G4double beam_energy_max; // [MeV]

    G4double   beam_size;
    G4double   beam_alpha_x;
    G4double   beam_alpha_y;
    G4double   beam_beta_x;
    G4double   beam_beta_y;

public:
    G4double x,xp, y,yp, E;
};

#endif
