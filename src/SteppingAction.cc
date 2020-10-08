
#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4VisExtent.hh"
#include "RunAction.hh"
#include "Randomize.hh"

//#include "Analysis.hh"
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

/*
SteppingAction::SteppingAction(DetectorConstruction* detectorConstruction, RunAction* runAction, EventAction* eventAction)
    :G4UserSteppingAction(), fDetector(detectorConstruction), fRunAction(runAction), fEventAction(eventAction)
    {}
*/

SteppingAction::SteppingAction(EventAction* eventAction)
    :G4UserSteppingAction(), fEventAction(eventAction)
    {}

SteppingAction::~SteppingAction()
    {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{

    G4String VolName=step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();

    //if (step->GetPreStepPoint()->GetKineticEnergy()==0) return; 	//gamma rays with 0 initial energy can appear; just ignore if that is the case
    //if (step->GetNumberOfSecondariesInCurrentStep()>0) step->GetTrack()->SetTrackStatus(fSuspend); //clear the stack of secondaries immediately

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

    // Get the particle name
    G4String ParticleName = step->GetTrack()->GetDefinition()->GetParticleName();

    // Get the parentID: if not 0 then particle is a secondary
    G4int ParentID = step->GetTrack()->GetParentID();

    // Access the end points of the step
    G4StepPoint* prePoint = step->GetPreStepPoint();
    G4StepPoint* postPoint = step->GetPostStepPoint();

    //if (prePoint->GetStepStatus()==fGeomBoundary)
    {
    G4ThreeVector prePosition=step->GetPreStepPoint()->GetPosition();
    G4double preX = prePosition.getX();
    G4double preY = prePosition.getY();
    G4double preZ = prePosition.getZ();

    // Get post step position
    G4ThreeVector postPosition=step->GetPostStepPoint()->GetPosition();
    G4double postX = postPosition.getX();
    G4double postY = postPosition.getY();
    G4double postZ = postPosition.getZ();

    // Get pre step Momentum Direction and Kinetic Energy
    G4ThreeVector preMomentumDir = prePoint->GetMomentumDirection();
    G4double preMomentumDirX = preMomentumDir.getX();
    G4double preMomentumDirY = preMomentumDir.getY();
    G4double preMomentumDirZ = preMomentumDir.getZ();
    G4double preAngleX = atan(preMomentumDirX/preMomentumDirZ)*1000;//mrad
    G4double preAngleY = atan(preMomentumDirY/preMomentumDirZ)*1000;//mrad
    G4double preKE = prePoint->GetKineticEnergy();


    // Get post step Momentum Direction and Kinetic Energy
    G4ThreeVector postMomentumDir = postPoint->GetMomentumDirection();
    G4double postMomentumDirX = postMomentumDir.getX();
    G4double postMomentumDirY = postMomentumDir.getY();
    G4double postMomentumDirZ = postMomentumDir.getZ();
    G4double postAngleX = atan(postMomentumDirX/postMomentumDirZ)*1000;//mrad
    G4double postAngleY = atan(postMomentumDirY/postMomentumDirZ)*1000;//mrad
    G4double postKE = postPoint->GetKineticEnergy();

    // Get energy Deposition
    G4double EnergyDeposite = step->GetTotalEnergyDeposit();

    // Get the energy of the secondary
    G4double EnergySecondary = 0;
    const std::vector<const G4Track*>* Secondary=step->GetSecondaryInCurrentStep();
    if(Secondary->size() > 0)
        {
           EnergySecondary=Secondary->at(0)->GetKineticEnergy();
        }
    if(preZ>=0)
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
    }
 }
}



/*

if (edep <= 0.) return;

// record energy deposited in the step
fRunAction->FillEdep(edep);

// increment counter for primary particle steps
if (step->GetTrack()->GetTrackID() == 1)
{
fRunAction->AddPrimaryStep();
}
// access the end points of the step
G4StepPoint* prePoint = step->GetPreStepPoint();
G4StepPoint* postPoint = step->GetPostStepPoint();

// get the particle name
G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();

// get the parentID: if not 0 then particle is a secondary
G4int parentID = step->GetTrack()->GetParentID();

// check if particle is a proton
// -> see 'ternary operators' if not familiar with this
bool isProton = (particleName == "proton") ? true : false;

// for debugging - prints bools as 'true' or 'false'
G4cout << std::boolalpha;

// check if protons are inside tracking area
bool isInside = ((abs(xposPost) <= 100 || abs(yposPost) <= 100))
? true : false;

if (isTracked && isProton && isInside)
{
// get kinetic energy to write to file
G4double kePost = postPoint->GetKineticEnergy();

// get position in x, y
G4double xposPost = postPoint->GetPosition().x();
G4double yposPost = postPoint->GetPosition().y();

// since the source is at -4200 mm in z
// need to shift by 4200 to get distance from source
// -- relative to centre of room
G4double zposPreCentre = prePoint->GetPosition().z();
G4double zShift = 4200.;
G4double zposPre = zposPreCentre + zShift;


G4double zposPostCentre = postPoint->GetPosition().z();
G4double zposPost = zposPostCentre + zShift;

// get direction of travel
G4ThreeVector direction = prePoint->GetMomentumDirection();

// angle between x-projection and z-axis for emittance
// factor of 1000 to convert to mrad
// the approximation angle = x/z would also work
G4double angle = atan(direction.x()/direction.z())*1000;

// iterate through all the desired boundaries
for (int z_boundary : trackingDistVec)
{

// score when particle traverses boundary
// make sure very first particle is recorded
if (
(zposPre < z_boundary && zposPost >= z_boundary) ||
(zposPre == 0 && zposPre == z_boundary)
)

{
// set filenames and open output files
std::string output_filename = "data/output_z"
+ std::to_string(z_boundary) + ".txt";
std::ofstream outputfile(output_filename, std::ios::app);

if(outputfile.is_open())
{
// only score forward moving particles
if (direction.z() > 0)
{
// write to file
// the particles' current position is in postPoint
outputfile << parentID << " "
<< xposPost << " " << yposPost << " " << zposPost << " "
<< angle << " "
<< kePost << G4endl;
}
outputfile.close();
} // if is_open

} // boundary check
} // loop
// fill layers of the absorber
G4String prePointVol = prePoint->GetTouchableHandle()->GetVolume()->GetName();
if (prePointVol == "Layer")
{
G4int layerNb = prePoint->GetTouchableHandle()->GetCopyNumber();
fRunAction->FillLayerEdep(layerNb, edep);
}
} // tracking
} // UserSteppingAction
*/
