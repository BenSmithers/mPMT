/*
Modified:  Mohit Gola 10th July 2023

*/
// Tuning parameters

#include "WCSimTuningParameters.hh"
#include "WCSimTuningMessenger.hh"
#include "G4SystemOfUnits.hh"

WCSimTuningParameters::WCSimTuningParameters()
{

  TuningMessenger = new WCSimTuningMessenger(this);
  // Default values
  rayff = 0.75;
  bsrff = 2.50;
  abwff = 1.30;
  rgcff = 0.32;
  mieff = 0.0;
  thickness = 10.5 * nm;

  // the model type, like 101 or 102
  // 0 - nothing?
  // 1 - 101
  // 2 - 102
  pmtsurftype = 2;

  // cathode material
  // 0- sk
  // 1- KCsCb
  // 2- RbCsCb
  cathodepara = 1;

  // jl145 - For Top Veto
  tvspacing = 100.0;
  topveto = false;
}

WCSimTuningParameters::~WCSimTuningParameters()
{
  delete TuningMessenger;
  TuningMessenger = 0;
}

void WCSimTuningParameters::SaveOptionsToOutput(WCSimRootOptions *wcopt)
{
  wcopt->SetRayff(rayff);
  wcopt->SetBsrff(bsrff);
  wcopt->SetAbwff(abwff);
  wcopt->SetRgcff(rgcff);
  wcopt->SetMieff(mieff);
  wcopt->SetTvspacing(tvspacing);
  wcopt->SetTopveto(topveto);
}
