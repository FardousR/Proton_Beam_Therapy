
#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VisExtent.hh"
#include "RunAction.hh"
#include "Randomize.hh"
#include "G4NistManager.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TrackVector.hh"
#include "G4EmCalculator.hh"

SteppingAction::SteppingAction(EventAction* eventAction)
    :G4UserSteppingAction(), fEventAction(eventAction)
    {}

SteppingAction::~SteppingAction()
    {

    }

void SteppingAction::UserSteppingAction(const G4Step* step)
{

    G4String VolName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
    G4EmCalculator* emCal = new G4EmCalculator();
    G4NistManager* nist = G4NistManager::Instance();
    const G4Material* mat = nist->FindOrBuildMaterial("G4_WATER");
    const G4String& matName = mat->GetName();
    const G4String& processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();

    G4int ParticleFlag=0.;
    const G4String& particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();

    if      (particleName == "e-")                 ParticleFlag =  2;
    else if (particleName == "e+")                 ParticleFlag = -2;
    else if (particleName == "proton")             ParticleFlag =  1;
    else if (particleName == "anti_proton")        ParticleFlag = -1;
    else if (particleName == "gamma")              ParticleFlag =  3;
    else if (particleName == "neutron")            ParticleFlag =  5;
    else if (particleName == "anti_neutron")       ParticleFlag = -5;
    else                                           ParticleFlag =  0;

    // Get eventID
    G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    // Get TrackID (for primary particle TrackID==1)
    G4int TrackID = step->GetTrack()->GetTrackID();

    // Get the parentID: if not 0 then particle is a secondary
    G4int ParentID = step->GetTrack()->GetParentID();

    // Access the end points of the step
    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();


    // Get pre step position
    G4ThreeVector prePosition=step->GetPreStepPoint()->GetPosition();
    G4double preZ = prePosition.getZ();
    // Get post step position
    G4ThreeVector postPosition=step->GetPostStepPoint()->GetPosition();

    // Get pre step Momentum Direction and Kinetic Energy
    G4ThreeVector preMomentumDir = prePoint->GetMomentumDirection();
    G4double preKE = prePoint->GetKineticEnergy();


    // Get post step Momentum Direction and Kinetic Energy
    G4ThreeVector postMomentumDir = postPoint->GetMomentumDirection();
    G4double postKE = postPoint->GetKineticEnergy();

    // Get energy Deposition
    G4double EnergyDeposite = step->GetTotalEnergyDeposit();
    G4double LET = emCal->ComputeElectronicDEDX(preKE, particleName, matName, .2*mm);

    G4double steplength =step->GetStepLength();
    // Get the energy of the secondary
    G4double EnergySecondary = 0;
    const std::vector<const G4Track*>* Secondary=step->GetSecondaryInCurrentStep();
    if(Secondary->size() > 0)
        {
           EnergySecondary=Secondary->at(0)->GetKineticEnergy();
        }

    if(preZ>=0 && processName!="Transportation")
    {
        fEventAction->AddEventID(EventID);
        fEventAction->AddTrackID(TrackID);
        fEventAction->AddParticleID(ParticleFlag);
        fEventAction->AddParentID(ParentID);
        fEventAction->AddprePosition(prePosition);
        fEventAction->AddpostPosition(postPosition);
        fEventAction->AddpreMomentumDir(preMomentumDir);
        fEventAction->AddpostMomentumDir(postMomentumDir);
        fEventAction->AddEnergyDeposit(EnergyDeposite/eV);
        fEventAction->AddSecondaryEnergy(EnergySecondary/eV);
        fEventAction->AddpreKE(preKE/eV);
        fEventAction->AddpostKE(postKE/eV);
        fEventAction->AddLET(LET);
        fEventAction->AddStepLength(steplength);
        // G4cout<<"/n<<<<<<<<<<----------  "<<steplength*mm<<"   ----------->>>>>>>>>>";
    }
    delete emCal;
}
