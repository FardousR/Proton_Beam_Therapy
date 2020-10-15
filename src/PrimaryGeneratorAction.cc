
#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <fstream>
#include <iostream>
#include <ctime>
#include <random>
#include <cmath>
#include "TRandom1.h"

using namespace std;
using std::fstream;


PrimaryGeneratorAction::PrimaryGeneratorAction(G4double   beam_energy_input,
                                               G4double   beam_alpha_x_input,
                                               G4double   beam_alpha_y_input,
                                               G4double   beam_beta_x_input,
                                               G4double   beam_beta_y_input,
                                               G4double   sigma_x_input,
                                               G4double   sigma_y_input,
                                               G4double   gun_z_position_input):G4VUserPrimaryGeneratorAction(),
                                               fParticleGun         (NULL),
                                               beam_energy          (beam_energy_input),
                                               beam_alpha_x         (beam_alpha_x_input),
                                               beam_alpha_y         (beam_alpha_y_input),
                                               beam_beta_x          (beam_beta_x_input),
                                               beam_beta_y          (beam_beta_y_input),
                                               sigma_x              (sigma_x_input),
                                               sigma_y              (sigma_y_input),
                                               gun_z_position       (gun_z_position_input)
{

    if(remove("/home/fardous/Desktop/Proton_Beam/OutputFile/InitialBeam.csv") != 0 )
        perror( "Error deleting file: Boundery_data.csv");
    else
        puts("File:InitialBeam.csv is successfully deleted");

  G4int n_particle_gun = 1;
  rngSeed = 0;
  RNG = new TRandom1((UInt_t) rngSeed);
  fParticleGun  = new G4ParticleGun(n_particle_gun);

  // Energy spread in %
  beam_energy_spread = 1.0;
  epsG_x  = (sigma_x*sigma_x)/beam_beta_x;
  epsG_y  = (sigma_y*sigma_y)/beam_beta_y;


  //Create covariance matrices for x-xp [m^2, m * rad, rad^2]
  covarX.ResizeTo(2,2);
  covarX[0][0] =  beam_beta_x;
  covarX[0][1] = -beam_alpha_x;
  covarX[1][0] = -beam_alpha_x;
  covarX[1][1] = (1+beam_alpha_x*beam_alpha_x)/beam_beta_x;
  covarX *= (epsG_x);//*1e-6

  //Create covariance matrices for y-yp [m^2, m * rad, rad^2]
  covarY.ResizeTo(2,2);
  covarY[0][0] = beam_beta_y;
  covarY[0][1] = -beam_alpha_y;
  covarY[1][0] = -beam_alpha_y;
  covarY[1][1] = (1+beam_alpha_y*beam_alpha_y)/beam_beta_y;
  covarY *= (epsG_y);//*1e-6

  // Get the cholesky decomposition
  TDecompChol covarX_Utmp(covarX,1e-9);
  covarX_Utmp.Decompose();
  covarX_L.ResizeTo(covarX);
  covarX_L = covarX_Utmp.GetU();
  covarX_L.Transpose(covarX_L);

  TDecompChol covarY_Utmp(covarY,1e-9);
  covarY_Utmp.Decompose();
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
  fstream File;
  File.open("/home/fardous/Desktop/Proton_Beam/OutputFile/InitialBeam.csv", std::ios::out| std::ios::app);
  G4double xn              = RNG->Gaus(0,1);
  G4double xpn             = RNG->Gaus(0,1);
  G4double particle_energy = RNG->Gaus(beam_energy, (beam_energy_spread/100)*beam_energy);

  x  = (xn*covarX_L[0][0] + xpn*covarX_L[0][1]);
  xp = (xn*covarX_L[1][0] + xpn*covarX_L[1][1]);

  G4double yn  = RNG->Gaus(0,1);
  G4double ypn = RNG->Gaus(0,1);
  y  = (yn*covarY_L[0][0] + ypn*covarY_L[0][1]);
  yp = (yn*covarY_L[1][0] + ypn*covarY_L[1][1]);

  fParticleGun->SetParticlePosition(G4ThreeVector( x, y, gun_z_position));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(xp ,yp, 1));
  fParticleGun->SetParticleEnergy(particle_energy*MeV);

  File<<x/mm<<", ";
  File<<xp<<", ";
  File<<y/mm<<", ";
  File<<yp<<"\n";
  File.close();

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
