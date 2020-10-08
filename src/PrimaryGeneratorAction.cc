#include "PrimaryGeneratorAction.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "RooArgList.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <ctime>
#include <random>
#include <chrono>
#include <cmath>
#include "TRandom.h"


using namespace std;


PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(nullptr)
{
  G4int n_particle = 1;
   RooArgList xVec;
  fParticleGun  = new G4ParticleGun(n_particle);
  alpha_x = 0;
  alpha_y = 0;
  beta_x  = 0.8;
  beta_y  = 0.8;
  epsG_x  = 0.05;
  epsG_y  = 0.05;

  //Create covariance matrices
  covarX.ResizeTo(2,2);
  covarX[0][0] =  beta_x;
  covarX[0][1] = -alpha_x;
  covarX[1][0] = -alpha_x;

  covarX[1][1] = (1+alpha_x*alpha_x)/beta_x;
  //covarX.Print(); // Raw matrix

  covarX *= (epsG_x*1e-6);

  G4cout << "Covariance matrix (X) [m^2, m * rad, rad^2]:" << G4endl;
  covarX.Print();

  covarY.ResizeTo(2,2);
  covarY[0][0] = beta_y;
  covarY[0][1] = -alpha_y;
  covarY[1][0] = -alpha_y;
  covarY[1][1] = (1+alpha_y*alpha_y)/beta_y;
  //covarY.Print(); // Raw matrix

  covarY *= (epsG_y*1e-6);

  G4cout << "Covariance matrix (Y) [m^2, m * rad, rad^2]:" << G4endl;
  covarY.Print();

  // Get the cholesky decomposition
  TDecompChol covarX_Utmp(covarX,1e-9);
  covarX_Utmp.Decompose();
  covarX_Utmp.Print();
  covarX_L.ResizeTo(covarX);
  covarX_L = covarX_Utmp.GetU();
  // Get the lower-tridiagonal that is needed for the generation
  covarX_L.Transpose(covarX_L);
  //covarX_L.Print();

  TDecompChol covarY_Utmp(covarY,1e-9);
  covarY_Utmp.Decompose();
  covarY_Utmp.Print();
  covarY_L.ResizeTo(covarY);
  covarY_L = covarY_Utmp.GetU();
  covarY_L.Transpose(covarY_L);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
    = particleTable->FindParticle(particleName="proton");
  fParticleGun->SetParticleDefinition(particle);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of ecah event
  G4double energy = 150;                    // Beam energy in MeV

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator (seed);
  //std::normal_distribution<double> distributionZ (-390.0, ini_rms_size_z*1000);
  //G4double z = distributionZ(generator);
  G4double z = -800*mm;
  std::normal_distribution<double> distributionEnergy (energy, .01*1*energy);
  G4double e = distributionEnergy(generator);

  G4double xn  = RNG->Gaus(0,1);
  G4double xpn = RNG->Gaus(0,1);;
  x  = (xn*covarX_L[0][0] + xpn*covarX_L[0][1])*m;
  xp = (xn*covarX_L[1][0] + xpn*covarX_L[1][1])*rad;


  G4double yn  = RNG->Gaus(0,1);
  G4double ypn = RNG->Gaus(0,1);
  y  = (yn*covarY_L[0][0] + ypn*covarY_L[0][1])*m;
  yp = (yn*covarY_L[1][0] + ypn*covarY_L[1][1])*rad;

  fParticleGun->SetParticlePosition(G4ThreeVector( x, y, z*mm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xp ,yp, 1));
  fParticleGun->SetParticleEnergy(e*MeV);
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
