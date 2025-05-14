import uproot
import numpy as np
import matplotlib.pyplot as plt
import re

# Open the ROOT file
file = uproot.open("updated_pulse_height.root")

# Define bins for timing resolution histogram
bins = np.linspace(0.1, 1.0, 600)  # Adjust based on your data range

# Lists to store results
dGas_vals = []
peak_timing_res = []

# Loop through trees
for key in file.keys():
    match = re.match(r"pulseTree_dGas_(\d+\.\d+);1", key)
    if match:
        dGas = float(match.group(1))
        tree = file[key.replace(";1", "")]
        timing_res = tree["timingResolution"].array(library="np")

        # Create histogram
        counts, bin_edges = np.histogram(timing_res, bins=bins)

        # Find index of max frequency (peak)
        peak_idx = np.argmax(counts)
        peak_value = (bin_edges[peak_idx] + bin_edges[peak_idx + 1]) / 2  # Center of peak bin

        # Store results
        dGas_vals.append(dGas)
        peak_timing_res.append(peak_value)

# Convert to arrays and sort by dGas
dGas_vals = np.array(dGas_vals)
peak_timing_res = np.array(peak_timing_res)
sorted_indices = np.argsort(dGas_vals)

dGas_vals = dGas_vals[sorted_indices]
peak_timing_res = peak_timing_res[sorted_indices]

# Plot: Timing resolution at peak vs dGas
plt.figure(figsize=(10, 6))
plt.plot(dGas_vals, peak_timing_res, marker='o', color='blue')
plt.xlabel("dGas Thickness (mm)")
plt.ylabel("Timing Resolution at Peak (ns)")
plt.title("Timing Resolution (at Histogram Peak) vs dGas")
plt.grid(True)
plt.tight_layout()
plt.show()

