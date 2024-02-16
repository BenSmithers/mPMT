/*                                                                                                                        
Author:    Mohit Gola 10th July 2023                                                                                       
*/

#include "mysteppingaction.hh"


MySteppingAction::MySteppingAction()
{
  fEventAction = nullptr;
  photonPositions.clear();
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::SetEventAction(MyEventAction* eventAction)
{
  fEventAction = eventAction;
}

double MySteppingAction::calculateIncidenceAngle(const G4ThreeVector& Momentum, const G4ThreeVector& Normal) 
{
  G4ThreeVector FacetNormal;
  
  // Calculate alpha using the distribution p(alpha) = g(alpha; 0, sigma_alpha)*std::sin(alpha)                            
  // where g(alpha; 0, sigma_alpha) is a Gaussian distribution with mean 0 and standard deviation sigma_alpha.             
  G4double alpha;
  G4double sigma_alpha = 0.0; // Set sigma_alpha value based on your requirements                                            
  const G4double halfpi = 0.5 * CLHEP::pi;
  const G4double twopi = 2.0 * CLHEP::pi;

  if (sigma_alpha == 0.0) {
    FacetNormal = Normal;
  } else {
    double f_max = std::min(1.0, 4.0 * sigma_alpha);
    double phi, SinAlpha, CosAlpha, SinPhi, CosPhi, unit_x, unit_y, unit_z;
    G4ThreeVector tmpNormal;

    do {
      do {
	alpha = G4RandGauss::shoot(0.0, sigma_alpha);
      } while (G4UniformRand() * f_max > std::sin(alpha) || alpha >= halfpi);
      
      phi = G4UniformRand() * twopi;
      SinAlpha = std::sin(alpha);
      CosAlpha = std::cos(alpha);
      SinPhi = std::sin(phi);
      CosPhi = std::cos(phi);
      unit_x = SinAlpha * CosPhi;
      unit_y = SinAlpha * SinPhi;
      unit_z = CosAlpha;
      FacetNormal.setX(unit_x);
      FacetNormal.setY(unit_y);
      FacetNormal.setZ(unit_z);
      tmpNormal = Normal;
      FacetNormal.rotateUz(tmpNormal);
    } while (Momentum * FacetNormal >= 0.0);
  }
  // Calculate the incidence angle between Momentum and FacetNormal                                            
  G4double PdotN = Momentum * FacetNormal;
  G4double magP = Momentum.mag();
  G4double magN = FacetNormal.mag();
  
  G4double incidenceAngle = CLHEP::pi - std::acos(PdotN/(magP*magN));
  return incidenceAngle;
}

  
void MySteppingAction::UserSteppingAction(const G4Step *step)
{

  G4LogicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  G4String volume = currentVolume->GetName();

  G4Track* track = step->GetTrack();
  G4int trackID = track->GetTrackID();
  /////////////////////////////////////////////////////////////////
  if(track->GetCurrentStepNumber() == 2)
    {
      G4ThreeVector hitPosition = step->GetPreStepPoint()->GetPosition(); 
      //      G4cout<<"HIT POSITON == "<<hitPosition<<G4endl;
    }

  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  //  G4cout<<"PHOTON POSITON == "<<position<<G4endl;
  photonPositions.push_back(position);
  //////////////////////////incident angle//////////////////////////////////////

  G4ThreeVector momentum = step->GetTrack()->GetMomentumDirection();
  G4ThreeVector normal = step->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetSolid()->SurfaceNormal(step->GetPreStepPoint()->GetPosition());

  // Calculate the incidence angle using the calculateIncidenceAngle function                                              
  G4double incidenceAngle = calculateIncidenceAngle(momentum, normal);
  //  G4cout<<"THE INCIDENT ANGLE BEFORE LOOP:  "<<incidenceAngle<<G4endl;
  if(track->GetCurrentStepNumber() == 1) 
    {
      G4double checkAngle = -(incidenceAngle - 0.5 * CLHEP::pi);
      //      fEventAction->SetPhotonAngle(checkAngle);
      fEventAction->SetPhotonAngle(incidenceAngle);
      G4cout<<"THE INCIDENT ANGLE IS G 83:  "<<incidenceAngle<<G4endl;
    }
  ///////////////////////////////////////////////////////////////////////////////////
  //  if (volume == "pmtGlassLogic" || volume == "pmtInnerGlassLogic"){
  if (track->GetDefinition()->GetParticleName() == "opticalphoton")
    {
      G4bool photonTransmitted = false;
      G4cout<<"TRACK ID == "<<track->GetTrackID()<<G4endl;
      // Get the pre-step and post-step points of the step                                                             
      G4StepPoint* prePoint = step->GetPreStepPoint();
      G4StepPoint* postPoint = step->GetPostStepPoint();
      
      // Get the volume at the pre-step and post-step points                                                          
      G4VPhysicalVolume* preVolume = prePoint->GetPhysicalVolume();
      G4VPhysicalVolume* postVolume = postPoint->GetPhysicalVolume();
      
      // Check if the photon is interacting with an optical boundary                                               
      if (preVolume && postVolume && preVolume != postVolume){
        WCSimOpBoundaryProcessStatus boundaryStatus = Undefined;
        WCSimOpBoundaryProcess* opBoundary = NULL;
        
        // Get the particle definition for the current track                                                      
        G4ParticleDefinition* particleDefinition = track->GetDefinition();
        // Get the process manager for the particle definition                                                   
        G4ProcessManager* processManager = particleDefinition->GetProcessManager();
        // Get the boundary process for the current track                                                        
        G4int nProcesses = processManager->GetProcessListLength();
        //	  G4cout<<"NUMBER OF PROCESSES == "<<nProcesses<<G4endl;
        G4ProcessVector* processVector = processManager->GetProcessList();
        for (G4int i = 0; i < nProcesses; i++)
          {
            G4VProcess* process = (*processVector)[i];
            
            //	      G4cout<<"PROCESS NAMES == "<<process->GetProcessName()<<G4endl;
            if (process->GetProcessName() == "OpBoundary")
              {
                opBoundary = static_cast<WCSimOpBoundaryProcess*>(process);
                //		      G4cout<<"BOUNDARY PROCESS NAME == "<<opBoundary->GetProcessName()<<G4endl;
                if (opBoundary==nullptr){
                  std::cerr << "BOUDARY IS NULL POINTER" <<std::endl;
                }
                boundaryStatus = opBoundary->GetStatus();
                
                G4cout<<"BOUNDARY PROCESS STATUS == "<< static_cast<int>(boundaryStatus)<<G4endl;
                
                
                if(boundaryStatus == 10)// && (volume == "pmtGlassLogic" || volume == "pmtInnerGlassLogic"))
                  {
                    fEventAction->IncrementNumAbsorbed();
                  }
                
                else if((boundaryStatus == 2 || boundaryStatus == 42 || boundaryStatus == 43))// && (volume == "pmtGlassLogic"|| volume == "pmtInnerGlassLogic"))
                  {
                    fEventAction->IncrementNumTransmitted();
                  }

                else if((boundaryStatus == 9 || boundaryStatus == 4 || boundaryStatus == 5 || boundaryStatus == 6 || boundaryStatus == 7 || boundaryStatus == 8 || boundaryStatus == 41))// && (volume == "pmtGlassLogic" || volume == "pmtInnerGlassLogic"))
                  {
                    fEventAction->IncrementNumReflected();
                  }
                
                else if(boundaryStatus == 11 && volume == "pmtGlassLogic")
                  {
                    fEventAction->IncrementNumDetected();
                    //			  trackStatus[trackID] = "Detected";
                  }
                
              }
          }     
	}
    }
  else{
    std::cout << track->GetDefinition()->GetParticleName();
  }
  //    }
  //	}
      
  
  
  ////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////Photon Absorption/////////////////////////////////
  const skDetCon *detectorConstruction = static_cast<const skDetCon*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  const myDetectorConstruction *pmtDetCon = static_cast<const myDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  
  G4LogicalVolume *fScoringVolume = pmtDetCon->GetScoringVolume();
  
  if(currentVolume == fScoringVolume)
    {
      //      G4cout<< "ABSORBER VOLUME  == "<<fScoringVolume->GetName()<<G4endl;
      //      G4cout<<"STEP IS WITHIN THE TARGET LOGICAL VOLUME!!!"<<G4endl;
      //  G4Track* track = step->GetTrack();
      track->SetTrackStatus(fStopAndKill);
      //    step->GetTrack()->SetTrackStatus(fStopAndKill);
      
    }
  else
    {
      G4double edep = step->GetTotalEnergyDeposit();
      fEventAction->AddEdep(edep);
      //step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
  ///////////////////////////////////////////////////////////////////////////////////
}

G4bool MySteppingAction::IsTrackCounted(G4int trackID)
{
  return (trackStatus[trackID] == "Absorbed" || trackStatus[trackID] == "Reflected" || trackStatus[trackID] == "Transmitted");
}

const std::vector<G4ThreeVector>& MySteppingAction::GetPhotonPositions() const
{
  return photonPositions;

}
