#include <TLatex.h>   // For displaying text on canvas
#include <TSpline.h>  // For smooth interpolation line

void pulseheight() {
    const int nPoints = 40;

    double dGas[nPoints] = {
        0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19,
        0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28, 0.29,
        0.30, 0.31, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39,
        0.40, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48, 0.49
    };

    double pulseheigth[nPoints] = {
        0.1208, 0.1795, 0.01411, 0.006948, 0.003992, 0.002591, 0.001748, 0.00136, 0.001066, 0.0008843,
        0.0007378, 0.0006277, 0.0005798, 0.0005224, 0.000465, 0.0004365, 0.0004033, 0.0003722, 0.0003551, 0.0003382,
        0.0003284, 0.0003202, 0.000306, 0.0002888, 0.00028, 0.000279, 0.0002739, 0.0002607, 0.000255, 0.0002436,
        0.0002409, 0.0002318, 0.0002259, 0.0002146, 0.0002152, 0.0002128, 0.0002113, 0.0002037, 0.000203, 0.0001935
    };

    double pulseheigthErr[nPoints];
    for (int i = 0; i < nPoints; ++i) {
        pulseheigthErr[i] = 0.002;
    }

    TGraphErrors* graph = new TGraphErrors(nPoints, dGas, pulseheigth, 0, pulseheigthErr);
    graph->SetTitle("Timing Resolution (Mean Value) vs dGas;dGas Thickness (Cm);Timing Resolution (ns)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue+2);
    graph->SetLineColor(kBlue+2);
    graph->SetLineWidth(2);

    TF1* fitFunc = new TF1("fitFunc", "[0] * (1 - exp(-[1] * x))", 0.1, 0.5);
    fitFunc->SetParameters(0.28, 10);

    TCanvas* c1 = new TCanvas("c1", "Pulse height fit", 900, 700);
    c1->SetGrid();

    graph->Draw("AP");
    graph->Fit(fitFunc, "RQ0");  // Fit silently, no draw

    // Draw smooth spline through data points
    TSpline3* spline = new TSpline3("spline", graph);
    spline->SetLineColor(kGreen+2);
    spline->SetLineWidth(2);
    spline->Draw("same");

    // ---- Mean and Standard Deviation Calculation ----
    double mean = 0;
    for (int i = 0; i < nPoints; ++i) mean += pulseheigth[i];
    mean /= nPoints;

    double variance = 0;
    for (int i = 0; i < nPoints; ++i) variance += pow(pulseheigth[i] - mean, 2);
    double sigma = sqrt(variance / nPoints);

    // ---- Output in Console ----
    cout << "----------------------------------------" << endl;
    cout << "Mean Pulse Height : " << mean << " ns" << endl;
    cout << "Standard Deviation (σ): " << sigma << " ns" << endl;
    cout << "----------------------------------------" << endl;

    // ---- Draw stats text outside plot area ----
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.SetTextColor(kRed+2);
    latex.DrawLatex(0.70, 0.85, Form("Mean Pulse Height : %.3f ns", mean));
    latex.DrawLatex(0.70, 0.80, Form("Std Dev: %.3f ns", sigma));

    c1->Update();
}

