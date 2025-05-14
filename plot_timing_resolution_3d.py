import uproot
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
import re

# Open the ROOT file
file = uproot.open("updated_pulse_height.root")

# Define bins for timing resolution
bins = np.linspace(0.1, .4, 600)  # Adjust bin range and size as needed

dGas_vals = []
hist_data = []

# Loop through all TTrees
for key in file.keys():
    match = re.match(r"pulseTree_dGas_(\d+\.\d+);1", key)
    if match:
        dGas = float(match.group(1))
        dGas_vals.append(dGas)

        tree = file[key.replace(";1", "")]
        timing_res = tree["timingResolution"].array(library="np")

        # Histogram of timing resolution
        counts, _ = np.histogram(timing_res, bins=bins)
        hist_data.append(counts)

# Convert to arrays and sort by dGas values
dGas_vals = np.array(dGas_vals)
hist_data = np.array(hist_data)

sorted_indices = np.argsort(dGas_vals)
dGas_vals = dGas_vals[sorted_indices]
hist_data = hist_data[sorted_indices]

# X, Y, Z setup
X, Z = np.meshgrid((bins[:-1] + bins[1:]) / 2, dGas_vals)  # bin centers and dGas
Y = hist_data  # frequency counts

# 3D Plot
fig = plt.figure(figsize=(14, 8))
ax = fig.add_subplot(111, projection='3d')

surf = ax.plot_surface(X, Y, Z, cmap=cm.viridis, edgecolor='k', linewidth=0.3, antialiased=True)

# Labels
ax.set_xlabel("Timing Resolution (ns)", labelpad=15)
ax.set_ylabel("Event Frequency", labelpad=15)
ax.set_zlabel("dGas Thickness (cm)", labelpad=15)
ax.set_title("Timing Resolution Distribution vs dGas", pad=20)

# View angle
ax.view_init(elev=30, azim=135)

# Color bar
fig.colorbar(surf, shrink=0.5, aspect=10, label="dGas")

plt.tight_layout()
plt.show()

