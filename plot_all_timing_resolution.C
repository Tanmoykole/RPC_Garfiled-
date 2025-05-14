void plot_all_timing_resolution() {
    // Open the ROOT file
    TFile *file = TFile::Open("updated_pulse_height .root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    // List objects in the file (useful for debugging)
    file->ls();

    // Prepare canvas
    TCanvas *c = new TCanvas("c", "Timing Resolution Comparison", 800, 600);
    bool anyData = false; // Flag to check if any valid data is found

    // Loop over dGas values from 0.10 to 0.49
    for (double d = 0.10; d <= 0.49; d += 0.01) {
        TString formattedDGas = Form("%.2f", d); // Format to 2 decimal places
        TString treeName = Form("pulseTree_dGas_%s", formattedDGas.Data());

        // Try different possible formats for the tree name
        if (!file->Get(treeName)) {
            treeName = Form("pulseTree_dGas_%s;1", formattedDGas.Data());
        }
        if (!file->Get(treeName)) {
            treeName = Form("pulseTree_dGas_0_%d", int(d * 100));
        }
        if (!file->Get(treeName)) {
            std::cerr << "Tree for dGas " << formattedDGas << " not found!" << std::endl;
            continue;
        }

        // Retrieve tree
        TTree *tree = (TTree*)file->Get(treeName);
        if (!tree) {
            std::cerr << "Error retrieving tree: " << treeName << std::endl;
            continue;
        }

        // Create histogram
        TH1F *h = new TH1F(Form("h_%s", formattedDGas.Data()), "", 200, 0.15, 0.32);
        tree->Draw("timingResolution>>" + TString(h->GetName()), "", "goff");

        // Normalize if there are entries
        if (h->GetEntries() > 0) {
            h->Scale(5000.0 / h->GetEntries()); // Normalize to 5000 entries
            h->SetLineColor(kBlue);
            h->SetLineWidth(2);

            if (anyData) {
                h->Draw("HIST SAME");
            } else {
                h->Draw("HIST");
                anyData = true;
            }
        }
    }

    if (!anyData) {
        std::cerr << "No valid data found!" << std::endl;
        return;
    }

    // Save the plot
    c->SaveAs("timing_resolution_clear.png");
}

