#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <ctime>
#include <random>
#include <cmath>
#include "TRandom1.h"

using namespace std;

PrimaryGeneratorAction::PrimaryGeneratorAction(G4double   beam_energy_input,
                                               G4double   beam_size_input,
                                               G4double   beam_alpha_x_input,
                                               G4double   beam_alpha_y_input,
                                               G4double   beam_beta_x_input,
                                               G4double   beam_beta_y_input):G4VUserPrimaryGeneratorAction(),
                                               fParticleGun(nullptr),
                                               beam_energy    (beam_energy_input),
                                               beam_size      (beam_size_input),
                                               beam_alpha_x   (beam_alpha_x_input),
                                               beam_alpha_y   (beam_alpha_y_input),
                                               beam_beta_x    (beam_beta_x_input),
                                               beam_beta_y    (beam_beta_y_input)
{
  G4int n_particle_gun = 1;
  rngSeed = 0;
  RNG = new TRandom1((UInt_t) rngSeed);
  fParticleGun  = new G4ParticleGun(n_particle_gun);

  // Energy spread in %
  beam_energy_spread = 1.0;


  alpha_x = 0;
  alpha_y = 0;
  beta_x  = 0.8;
  beta_y  = 0.8;
  epsG_x  = 0.05;
  epsG_y  = 0.05;

  //Create covariance matrices for x-xp [m^2, m * rad, rad^2]
  covarX.ResizeTo(2,2);
  covarX[0][0] =  beta_x;
  covarX[0][1] = -alpha_x;
  covarX[1][0] = -alpha_x;
  covarX[1][1] = (1+alpha_x*alpha_x)/beta_x;
  covarX *= (epsG_x*1e-6);

  //Create covariance matrices for y-yp [m^2, m * rad, rad^2]
  covarY.ResizeTo(2,2);
  covarY[0][0] = beta_y;
  covarY[0][1] = -alpha_y;
  covarY[1][0] = -alpha_y;
  covarY[1][1] = (1+alpha_y*alpha_y)/beta_y;
  covarY *= (epsG_y*1e-6);

  // Get the cholesky decomposition
  TDecompChol covarX_Utmp(covarX,1e-9);
  covarX_Utmp.Decompose();
  covarX_Utmp.Print();
  covarX_L.ResizeTo(covarX);
  covarX_L = covarX_Utmp.GetU();
  // Get the lower-tridiagonal that is needed for the generation
  covarX_L.Transpose(covarX_L);

  TDecompChol covarY_Utmp(covarY,1e-9);
  covarY_Utmp.Decompose();
  covarY_Utmp.Print();
  covarY_L.ResizeTo(covarY);
  covarY_L = covarY_Utmp.GetU();
  covarY_L.Transpose(covarY_L);

  // Beam Properties
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  particle = particleTable->FindParticle(particleName="proton");
  fParticleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  // std::default_random_engine generator (seed);

  G4double z = -800*mm;
  // std::normal_distribution<double> distributionEnergy (energy, .01*1*energy);
  // G4double particle_energy = distributionEnergy(generator); // Use RNG

  G4double xn              = RNG->Gaus(0,1);
  G4double xpn             = RNG->Gaus(0,1);
  G4double particle_energy = RNG->Gaus(beam_energy, (beam_energy_spread/100)*beam_energy);

  x  = (xn*covarX_L[0][0] + xpn*covarX_L[0][1])*m;
  xp = (xn*covarX_L[1][0] + xpn*covarX_L[1][1])*rad;

  G4double yn  = RNG->Gaus(0,1);
  G4double ypn = RNG->Gaus(0,1);
  y  = (yn*covarY_L[0][0] + ypn*covarY_L[0][1])*m;
  yp = (yn*covarY_L[1][0] + ypn*covarY_L[1][1])*rad;
  fParticleGun->SetParticlePosition(G4ThreeVector( x, y, z*mm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xp ,yp, 1));
  fParticleGun->SetParticleEnergy(particle_energy*MeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
