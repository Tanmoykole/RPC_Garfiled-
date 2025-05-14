#include <TLatex.h>   // For displaying text on canvas
#include <TSpline.h>  // For smooth interpolation line

void dGasFit() {
    const int nPoints = 40;

    double dGas[nPoints] = {
        0.10, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19,
        0.20, 0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28, 0.29,
        0.30, 0.31, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38, 0.39,
        0.40, 0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48, 0.49
    };

    double timingRes[nPoints] = {
        0.147, 0.177, 0.205, 0.226, 0.239, 0.248, 0.257, 0.263, 0.267, 0.270,
        0.271, 0.272, 0.272, 0.274, 0.272, 0.273, 0.273, 0.274, 0.274, 0.273,
        0.274, 0.275, 0.273, 0.274, 0.273, 0.274, 0.273, 0.272, 0.271, 0.271,
        0.270, 0.269, 0.270, 0.269, 0.270, 0.269, 0.268, 0.270, 0.269, 0.268
    };

    double timingResErr[nPoints];
    for (int i = 0; i < nPoints; ++i) {
        timingResErr[i] = 0.002;
    }

    TGraphErrors* graph = new TGraphErrors(nPoints, dGas, timingRes, 0, timingResErr);
    graph->SetTitle("Timing Resolution (Mean Value) vs dGas;dGas Thickness (cm);Timing Resolution (ns)");
    graph->SetMarkerStyle(20);
    graph->SetMarkerColor(kBlue+2);
    graph->SetLineColor(kBlue+2);
    graph->SetLineWidth(2);

    TF1* fitFunc = new TF1("fitFunc", "[0] * (1 - exp(-[1] * x))", 0.1, 0.5);
    fitFunc->SetParameters(0.28, 10);

    TCanvas* c1 = new TCanvas("c1", "Timing Resolution Fit", 900, 700);
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
    for (int i = 0; i < nPoints; ++i) mean += timingRes[i];
    mean /= nPoints;

    double variance = 0;
    for (int i = 0; i < nPoints; ++i) variance += pow(timingRes[i] - mean, 2);
    double sigma = sqrt(variance / nPoints);

    // ---- Output in Console ----
    cout << "----------------------------------------" << endl;
    cout << "Mean Timing Resolution: " << mean << " ns" << endl;
    cout << "Standard Deviation (σ): " << sigma << " ns" << endl;
    cout << "----------------------------------------" << endl;

    // ---- Draw stats text outside plot area ----
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.035);
    latex.SetTextColor(kRed+2);
    latex.DrawLatex(0.70, 0.85, Form("Mean timing resolution : %.3f ns", mean));
    latex.DrawLatex(0.70, 0.80, Form("Std Dev: %.3f ns", sigma));

    c1->Update();
}

