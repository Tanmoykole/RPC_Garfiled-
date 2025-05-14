#include <TFile.h>
#include <TTree.h>
#include <TGraphErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <cmath>

void plotPulseHeightVsDgas() {
    TFile *file = TFile::Open("updated_pulse_height.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file.\n";
        return;
    }

    std::vector<double> dgas_vals;
    std::vector<double> mean_vals;
    std::vector<double> err_vals;

    double totalMean = 0;
    double totalStdDev = 0;
    int count = 0;

    for (double d = 0.10; d <= 0.49 + 1e-6; d += 0.01) {
        TString treeName = TString::Format("pulseTree_dGas_%g", d);
        TTree *tree = (TTree*)file->Get(treeName);
        if (!tree) continue;

        Double_t pulseHeight;
        tree->SetBranchAddress("pulseHeight", &pulseHeight);

        Long64_t nentries = tree->GetEntries();
        if (nentries == 0) continue;

        double sum = 0, sumSq = 0;
        for (Long64_t i = 0; i < nentries; ++i) {
            tree->GetEntry(i);
            sum += pulseHeight;
            sumSq += pulseHeight * pulseHeight;
        }

        double mean = sum / nentries;
        double variance = (sumSq / nentries) - (mean * mean);
        double stdDev = sqrt(variance);
        double stdError = stdDev / sqrt(nentries);

        dgas_vals.push_back(d);
        mean_vals.push_back(mean);
        err_vals.push_back(stdError);

        totalMean += mean;
        totalStdDev += stdDev;
        count++;
    }

    double meanPulseHeight = totalMean / count;
    double meanStdDev = totalStdDev / count;

    // Print only final average values
    std::cout << "Overall Mean Pulse Height     = " << meanPulseHeight << std::endl;
    std::cout << "Overall Mean Standard Deviation = " << meanStdDev << std::endl;

    // Plot graph
    TGraphErrors *graph = new TGraphErrors(dgas_vals.size(), &dgas_vals[0], &mean_vals[0], nullptr, &err_vals[0]);
    graph->SetTitle("Mean Pulse Height vs dGas;dGas (cm);Mean Pulse Height");
    graph->SetMarkerStyle(21);
    graph->SetMarkerColor(kRed + 1);
    graph->SetLineColor(kBlue + 1);
    graph->SetLineWidth(2);
    graph->Draw("ALP");

    graph->GetYaxis()->SetRangeUser(-0.02, 0.13);

    // Fit function (not shown)
    TF1 *fitFunc = new TF1("fitFunc", "[0]*exp(-[1]*x) + [2]", 0.1, 0.5);
    fitFunc->SetParameters(0.1, 20, 0);
    graph->Fit(fitFunc, "R0");

    gPad->SetGrid();
    gPad->Update();
}

