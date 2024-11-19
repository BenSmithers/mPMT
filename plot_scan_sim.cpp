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
#include <vector>
#include <fstream>
#include <cmath>

#include "TH2.h"
#include "TChain.h"
#include "TTree.h"
#include "TCanvas.h"
#include <TFile.h>
#include <TPolyLine3D.h>
#include <TView.h>
#include <TPolyMarker3D.h>

using namespace std;

double a = 0.254564663*1000;
double b = 0.254110205*1000;
double c = 0.186002389*1000;
double e = (a+b)/2;
double d = c - 165; // offset b/w 0 and centre of PMT, set in the geant4 geometry
double pi = 3.14159265;

pair <double, double> GetShotAngles(double x, double y, double z)
{
    double theta = asin((z+d)/c);
    double phi = atan2(y,x);
    return make_pair(theta*180/pi, phi*180/pi);
}

double GetAverageValue(TH2 *h1)
{
    double sum = 0.0;
        int totalnum = 0;
        for (int ix=0; ix<61; ix++)
        {
            for (int iy=0; iy<61; iy++)
            {
                double val = h1->GetBinContent(ix,iy);
                if (val!=0)
                {
                    sum += val;
                    totalnum ++;
                }
            }
        }
    double average = sum/totalnum;
    return average;
}

void nosmallbins(TH2 *h1)
{
    double sum = 0.0;
        int totalnum = 0;
        for (int ix=0; ix<121; ix++)
        {
            for (int iy=0; iy<121; iy++)
            {
                double val = h1->GetBinContent(ix,iy);
                if (val<3)
                {
                    h1->SetBinContent(ix,iy,0);
                }
            }
        }
}

void RescaleHisto(TH2 *h1, int xbins, int ybins)
{
    for (int ix=0; ix<xbins; ix++)
    {
        for (int iy=0; iy<ybins; iy++)
        {
            double val = h1->GetBinContent(ix,iy);
            h1->SetBinContent(ix,iy,val/10000);
        }
    }
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
    c1->SetLeftMargin(0.15);
    c1->SetRightMargin(0.15);
    c1->SetTopMargin(0.1);
    c1->SetBottomMargin(0.15);
    gStyle->SetPalette(55);
    // TH2 *h1 = new TH2F("h1", "Abs Photons in Normal Injection Sim: Total Contribution", 121, -181.5, 181.5, 31, -1.5, 91.5); // 2d histo for angular coordinates
    TH2 *h1 = new TH2F("h1", "Abs Photons in Vertical Injection Sim: Photocathode Distribution", 61, -300, 300, 61, -300, 300); // regular 2d histo
    // TH3 *h1 = new TH3F("h1", "Abs Photons in Vertical Injection Sim: Elsewhere Distribution", 61, -300, 300, 61, -300, 300, 91, -600, 300); // 3d histo
    h1->SetStats(0);
    h1->SetContour(100);

    ostringstream filename;
    // fill out the path to your root file:
    filename << "/path/to/file.root";

    // open the root file, link the master photons table entries to defined variables here
    TFile *file = TFile::Open(filename.str().c_str());
    TTree *mastertree = (TTree*)file->Get("Photons_Master");
    mastertree->SetBranchAddress("Scanpoint_ID", &Scanpoint_ID);
    mastertree->SetBranchAddress("PosX_Initial", &PosX_Initial);
    mastertree->SetBranchAddress("PosY_Initial", &PosY_Initial);
    mastertree->SetBranchAddress("PosZ_Initial", &PosZ_Initial);

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
            if (Step_Status==2)
            {
                /* photons absorbed in photocathode on step 2 are the most common photons, sometimes
                it's nice to filter out these events to look at less common trends */
                // if (Step_Number!=2)
                // {
                //     h1->Fill(PosX,PosY);
                // }

                // if absorbed, find place where photon hit PMT (first recorded step)
                photons->GetEntry(entry-Step_Number+1);

                // if reaching backwards for entries, should check to make sure you're looking at the right photon
                // int pid = Photon_ID;
                // photons->GetEntry(entry-5);
                // if (pid==Photon_ID)
                // {
                //     h1->Fill(PosX,PosY);
                // }

                // finding a bin from an x (or y) position
                // int binX = h1->GetXaxis()->FindBin(PosX);

                // reconstructing initial PMT angular coordinates for angular normal injection
                // pair<double, double> angles = GetShotAngles(PosX,PosY,PosZ);
                // theta = angles.first;
                // phi = angles.second;
                // h1->Fill(phi,theta);

                h1->Fill(PosX,PosY);
            }
        }
    }

    // these functions currently only work with a 2d histogram:
    // double av_val = GetAverageValue(h1);
    // nosmallbins(h1);
    // RescaleHisto(h1, 61, 61);

    file->Close();
    delete file;

    h1->GetXaxis()->SetTitle("PosX");
    h1->GetYaxis()->SetTitle("PosY");
    // h1->GetZaxis()->SetTitle("PosZ");
    h1->GetZaxis()->SetTitle("Absorption Total");
    h1->GetXaxis()->SetTitleOffset(1.5);
    h1->GetYaxis()->SetTitleOffset(1.5);
    h1->GetZaxis()->SetTitleOffset(1.5);
    // h1->GetZaxis()->SetRangeUser(0,10000); // set z axis limits, adjust this to suit what you're doing
    // h1->GetXaxis()->SetRangeUser(-300,300);
    // h1->GetYaxis()->SetRangeUser(-300,300);
    h1->Draw("colz");

}