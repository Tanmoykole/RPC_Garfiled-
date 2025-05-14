void pulse_height() {
  // Open the ROOT file
  TFile *file = new TFile("output.root", "READ");
  if (!file || file->IsZombie()) {
    std::cout << "Error: Cannot open output.root file!" << std::endl;
    return;
  }

  // Retrieve the signal histogram
  TH1D *hSignal = (TH1D*)file->Get("hSignal");
  if (!hSignal) {
    std::cout << "Error: hSignal histogram not found!" << std::endl;
    return;
  }

  // Find the minimum bin value (since the signal is negative)
  double pulseHeight = std::abs(hSignal->GetMinimum());  // Take absolute value
  double timeAtMin = hSignal->GetBinCenter(hSignal->GetMinimumBin());

  // Print only the positive value of the pulse height
  std::cout << "Pulse Height: " << pulseHeight << " (arb. units)" << std::endl;
  std::cout << "Time at Maximum Deviation: " << timeAtMin << " ns" << std::endl;

  // Draw the histogram
  TCanvas *c1 = new TCanvas("c1", "Pulse Height", 800, 600);
  hSignal->Draw();
}

