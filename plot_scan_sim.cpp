/* 
Analysis code for plotting PMT simulation results.
Usage: 
- In a terminal window, navigate to the directory this file is in.
- Type the following commands:
    - root
    - .L scan_sim.cpp
    - plot_abs_photons
- If an error occurs when loading the file, or you want to run it again, quit the root session
  and create a new one before retrying, otherwise you'll probably get an error
- The histogram that ranges from -300 to 300 in x and y is for plotting results in x and y
    - In this case make sure to run the line h1->Fill(PosX,PosY) and comment out the theta, phi one
- The other histogram is for plotting results in theta and phi
    - Fill with h1->Fill(theta,phi)
*/

#include <iostream>
#include <sstream>

#include "TH2.h"
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include <TFile.h>

using namespace std;

double c = 0.186002389*1000;
double d = c - 165; // offset b/w 0 and centre of PMT, set in the geant4 geometry
double pi = 3.14159265;

pair <double, double> GetShotAngles(double x, double y, double z)
{
    double theta = asin((z+d)/c);
    double phi = atan2(y,x);
    return make_pair(theta*180/pi, phi*180/pi);
}

void plot_abs_photons()
{
    int Scanpoint_ID;
    double PosX_Initial;
    double PosY_Initial;
    double PosZ_Initial;
    double PosX;
    double PosY;
    double PosZ;
    int Step_Number;
    int Step_Status;
    int Photon_ID;
    double theta;
    double phi;

    // initialize plotting stuff
    TCanvas *c1 = new TCanvas();
    gStyle->SetPalette(55);
    TH2 *h1 = new TH2F("h1", "Abs Photons in Normal Injection Sim: Total", 121, -181.5, 181.5, 31, -1.5, 91.5);
    // TH2 *h1 = new TH2F("h1", "Abs Photons in Normal Injection Sim: Total", 61, -300, 300, 61, -300, 300);
    h1->SetStats(0);
    h1->SetContour(100);

    ostringstream filename;
    // fill out the path to your root file:
    filename << "/path/to/file.root";

    // open the root file, link the master photons table entries to defined variables here
    TFile *file = TFile::Open(filename.str().c_str());
    TTree *mastertree = (TTree*)file->Get("Photons_Master");
    // un-comment whichever variables you want to use:
    // tree->SetBranchAddress("Scanpoint_ID", &Scanpoint_ID);
    // tree->SetBranchAddress("PosX_Initial", &PosX_Initial);
    // tree->SetBranchAddress("PosY_Initial", &PosY_Initial);
    // tree->SetBranchAddress("PosZ_Initial", &PosZ_Initial);

    // loop through each scanpoint tree
    int n = mastertree->GetEntries();

    for (int i=0; i<n; i++)
    // i happens to be the same as Scanpoint_ID
    {
        // get ith entry of master photons tree, match it to ith scanpoint tree, link entries to variables
        mastertree->GetEntry(i);
        ostringstream photontree;
        photontree << "Photons_" << i;
        TTree *photons = (TTree*)file->Get(photontree.str().c_str());
        photons->SetBranchAddress("Photon_ID", &Photon_ID);
        photons->SetBranchAddress("Step_Number", &Step_Number);
        photons->SetBranchAddress("Step_Status", &Step_Status);
        photons->SetBranchAddress("PosX", &PosX);
        photons->SetBranchAddress("PosY", &PosY);
        photons->SetBranchAddress("PosZ", &PosZ);

        // loop through each entry in scanpoint tree
        int entries = photons->GetEntries();
        for (int entry=0; entry<entries; entry++)
        {
            photons->GetEntry(entry);
            /* 
            STATUS: transmission==0, reflection==1, absorption (photocathode)==2, absorption (glass)==3,
            absorption (dynode area)==4, absorption (anywhere else)==5, photon leaves world volume==6
            anything else==7 (should not really be any 7)
            */
            if (Step_Status==2 || Step_Status==3 || Step_Status==4 || Step_Status==5)
            {
                // if absorbed, find place where photon hit PMT (first recorded step)
                photons->GetEntry(entry-Step_Number+1);
                pair<double, double> angles = GetShotAngles(PosX,PosY,PosZ);
                theta = angles.first;
                phi = angles.second;
                h1->Fill(phi, theta);
                // h1->Fill(PosX,PosY);
            }
        }
    }

    file->Close();
    delete file;

    h1->GetXaxis()->SetTitle("Phi");
    h1->GetYaxis()->SetTitle("Theta");
    h1->GetZaxis()->SetTitle("Absorption Total");
    h1->GetZaxis()->SetRangeUser(0,100); // set z axis limits, adjust this to suit what you're doing
    h1->Draw("colz");
}