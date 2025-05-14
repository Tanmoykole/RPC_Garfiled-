#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cmath>

void tres_glass() {
    TFile *file = TFile::Open("thickness_scan_dgas_0.2_voltage_6000.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    std::vector<double> glaverbelThickness_vals;
    std::vector<double> mean_vals;
    std::vector<double> err_vals;

    double totalMean = 0;
    double totalStdDev = 0;
    int count = 0;

    for (double d = 0.01; d <= 0.29 + 1e-6; d += 0.02) {
        TString treeName = TString::Format("pulseTree_dGlaverbel_%g", d);
        TTree *tree = (TTree*)file->Get(treeName);
        if (!tree) continue;

        Double_t timingResolution;
        tree->SetBranchAddress("timingResolution", &timingResolution);

        Long64_t nentries = tree->GetEntries();
        if (nentries == 0) continue;

        double sum = 0, sumSq = 0;
        for (Long64_t i = 0; i < nentries; ++i) {
            tree->GetEntry(i);
            sum += timingResolution;
            sumSq += timingResolution * timingResolution;
        }

        double mean = sum / nentries;
        double variance = (sumSq / nentries) - (mean * mean);
        double stdDev = sqrt(variance);
        double stdError = stdDev / sqrt(nentries);

        glaverbelThickness_vals.push_back(d);
        mean_vals.push_back(mean);
        err_vals.push_back(stdError);

        totalMean += mean;
        totalStdDev += stdDev;
        count++;
    }

    double meantimingResolution = totalMean / count;
    double meanStdDev = totalStdDev / count;

    // Print only final average values
    std::cout << "Overall Mean Pulse Height     = " << meantimingResolution << std::endl;
    std::cout << "Overall Mean Standard Deviation = " << meanStdDev << std::endl;

    // Plot graph
    TGraphErrors *graph = new TGraphErrors(glaverbelThickness_vals.size(), &glaverbelThickness_vals[0], &mean_vals[0], nullptr, &err_vals[0]);
    graph->SetTitle("Mean Timing Resolution vs Glass Gap;Glass Gap (cm);Mean Timing Resolution");
    graph->SetMarkerStyle(21);
    graph->SetMarkerColor(kRed + 1);
    graph->SetLineColor(kBlue + 1);
    graph->SetLineWidth(2);
    graph->Draw("ALP");

    graph->GetYaxis()->SetRangeUser(0.00, 0.30);

    // Fit function (not shown)
  //  TF1 *fitFunc = new TF1("fitFunc", "[0]*exp(-[1]*x) + [2]", 0.1, 0.5);
  //  fitFunc->SetParameters(0.1, 20, 0);
   // graph->Fit(fitFunc, "R0");

    gPad->SetGrid();
    gPad->Update();
}

