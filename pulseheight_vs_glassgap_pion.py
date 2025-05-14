import uproot
import numpy as np
import matplotlib.pyplot as plt
from numpy.polynomial.polynomial import Polynomial

# File paths
files = {
    "Freon": "thickness_scan_dgas_0.2_freon.root",
    "Freon + Isobuten": "thickness_scan_dgas_0.2_freon+isobuten.root"
}

data = {}

# Load data
for label, filepath in files.items():
    with uproot.open(filepath) as file:
        d_vals = []
        pulse_heights = []

        for key in file.keys():
            if key.startswith("pulseTree"):
                tree = file[key]
                d = tree["glaverbelThickness"].array(library="np")[0]
                ph = tree["pulseHeight"].array(library="np")
                mean_ph = np.mean(ph)

                d_vals.append(d)
                pulse_heights.append(mean_ph * 1e5)  # Scale

        data[label] = (np.array(d_vals), np.array(pulse_heights))

# Plotting with curve fitting
plt.figure(figsize=(10, 6))
for label, (d, ph) in data.items():
    sort_idx = np.argsort(d)
    d_sorted = d[sort_idx]
    ph_sorted = ph[sort_idx]

    # Fit: You can change deg=2 to a higher value if needed
    fit_coeffs = Polynomial.fit(d_sorted, ph_sorted, deg=2)
    fit_line = fit_coeffs(d_sorted)

    # Plot original points and fit
    plt.plot(d_sorted, ph_sorted, 'o', label=label)
    plt.plot(d_sorted, fit_line, '-', label=f"{label} Fit")

plt.xlabel("Glass Thickness [cm]")
plt.ylabel("Mean Pulse Height [×10⁵ a.u.]")
plt.title("Pulse Height vs Glass Thickness with Fit")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()

