import uproot
import numpy as np
import matplotlib.pyplot as plt
from numpy.polynomial.polynomial import Polynomial

# File paths
files = {
    "Freon": "thickness_sr-90_dglass_0.2_freon.root",
    "Freon + Isobuten": "thickness_sr-90_dglass_0.2_freon+isobuten.root"
}

data = {}

# Load timing resolution data
for label, filepath in files.items():
    with uproot.open(filepath) as file:
        d_vals = []
        time_resolutions = []

        for key in file.keys():
            if key.startswith("pulseTree"):
                tree = file[key]
                d = tree["glaverbelThickness"].array(library="np")[0]
                t_res = tree["timingResolution"].array(library="np")
                mean_tres = np.mean(t_res)

                d_vals.append(d)
                time_resolutions.append(mean_tres)  # or multiply by 1e9 for ns

        data[label] = (np.array(d_vals), np.array(time_resolutions))

# Plotting with curve fitting
plt.figure(figsize=(10, 6))
for label, (d, tres) in data.items():
    sort_idx = np.argsort(d)
    d_sorted = d[sort_idx]
    tres_sorted = tres[sort_idx]

    # Polynomial fit
    fit_coeffs = Polynomial.fit(d_sorted, tres_sorted, deg=2)
    fit_line = fit_coeffs(d_sorted)

    # Plot
    plt.plot(d_sorted, tres_sorted, 'o', label=label)
    plt.plot(d_sorted, fit_line, '-', label=f"{label} Fit")

plt.xlabel("Glass Thickness [cm]")
plt.ylabel("Mean Timing Resolution [ns]")  # change unit if needed
plt.title("Timing Resolution vs Glass Thickness with Fit")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

