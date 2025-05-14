import uproot
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from mpl_toolkits.mplot3d import Axes3D
import re

# Open the ROOT file
file = uproot.open("updated_pulse_height.root")

# Define bins for pulse height
bins = np.linspace(-0.05, 0.2, 60)  # 100 bins from -0.05 to 0.5

dGas_vals = []
hist_data = []

# Loop through all TTrees
for key in file.keys():
    match = re.match(r"pulseTree_dGas_(\d+\.\d+);1", key)
    if match:
        dGas = float(match.group(1))
        dGas_vals.append(dGas)

        tree = file[key.replace(";1", "")]
        pulse_height = tree["pulseHeight"].array(library="np")

        # Histogram of pulse height
        counts, _ = np.histogram(pulse_height, bins=bins)
        hist_data.append(counts)

# Convert to arrays and sort by dGas values
dGas_vals = np.array(dGas_vals)
hist_data = np.array(hist_data)

sorted_indices = np.argsort(dGas_vals)
dGas_vals = dGas_vals[sorted_indices]
hist_data = hist_data[sorted_indices]

# X, Y, Z setup
pulse_centers = (bins[:-1] + bins[1:]) / 2  # bin centers
X, Z = np.meshgrid(pulse_centers, dGas_vals)  # X: pulseHeight, Z: dGas
Y = hist_data  # Y-axis is counts

# 3D Plot
fig = plt.figure(figsize=(14, 8))
ax = fig.add_subplot(111, projection='3d')

surf = ax.plot_surface(X, Y, Z, cmap=cm.viridis, edgecolor='k', linewidth=0.3, antialiased=True)

# Labels
ax.set_xlabel("Pulse Height (a.u.)", labelpad=15)
ax.set_ylabel("Counts", labelpad=15)
ax.set_zlabel("dGas Thickness (cm)", labelpad=15)
ax.set_title("Pulse Height Distribution vs dGas", pad=20)

# View angle
ax.view_init(elev=30, azim=135)

# Color bar
fig.colorbar(surf, shrink=0.5, aspect=10, label="Counts")

plt.tight_layout()
plt.show()

