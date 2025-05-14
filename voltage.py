import ROOT
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D  # For 3D plotting
import glob
import re

# Get all matching ROOT files in current directory
file_list = glob.glob("voltage_scan_dgas_*.root")

# Extract dGas from filenames and sort
file_dgas_pairs = []
pattern = re.compile(r"voltage_scan_dgas_(\d+\.\d+)\.root")
for f in file_list:
    match = pattern.search(f)
    if match:
        dgas = float(match.group(1))
        file_dgas_pairs.append((f, dgas))

# Sort by dGas value
file_dgas_pairs.sort(key=lambda x: x[1])

# Voltage values
voltages = list(range(2000, 15000, 500))
dgas_values = [pair[1] for pair in file_dgas_pairs]

# Initialize timing resolution matrix: shape (voltages, dgas)
timing_res = np.zeros((len(voltages), len(dgas_values)))

# Fill matrix
for x, (file_name, dgas) in enumerate(file_dgas_pairs):
    f = ROOT.TFile(file_name)
    
    for y, voltage in enumerate(voltages):
        tree_name = f"pulseTree_Voltage_{voltage}"
        tree = f.Get(tree_name)
        if not tree:
            print(f"Missing: {tree_name} in {file_name}")
            continue

        hist_name = f"hTemp_{voltage}"
        tree.Draw(f"timingResolution >> {hist_name}(200, 0, 10)", "", "goff")
        hist = ROOT.gDirectory.Get(hist_name)
        if hist:
            timing_res[y][x] = hist.GetMean()

# Convert axis values to meshgrid
X, Y = np.meshgrid(dgas_values, voltages)
Z = timing_res

# Plot 3D surface
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')
surf = ax.plot_surface(X, Y, Z, cmap='plasma', edgecolor='k')

ax.set_xlabel("dGas (cm)")
ax.set_ylabel("Voltage (V)")
ax.set_zlabel("Mean Timing Resolution (ns)")
ax.set_title("Timing Resolution Surface Plot")

fig.colorbar(surf, ax=ax, shrink=0.5, aspect=10, label="Mean Timing Resolution (ns)")

plt.tight_layout()
plt.show()

