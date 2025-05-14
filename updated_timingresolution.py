import uproot
import numpy as np
import matplotlib.pyplot as plt
import glob
import re

# Voltage and dGas ranges
valid_voltages = list(range(6000, 15001, 1000))  # 4000 to 15000 inclusive
min_dgas, max_dgas = 0.15, 0.50

# List of .root files
files = sorted(glob.glob("voltage_scan_dgas_*.root"))

# Dictionary to store voltage vs dGas vs mean pulse height
voltage_data = {}

# Regex to extract dGas from filename
dgas_pattern = re.compile(r"voltage_scan_dgas_(\d+\.\d+)\.root")

for file in files:
    match = dgas_pattern.search(file)
    if not match:
        continue
    dgas = float(match.group(1))
    if not (min_dgas <= dgas <= max_dgas):
        continue

    with uproot.open(file) as f:
        for key in f.keys():
            if "pulseTree" in key:
                voltage = int(re.search(r"(\d+)", key).group(1))
                if voltage not in valid_voltages:
                    continue
                tree = f[key]
                pulse_height = tree["timingResolution"].array(library="np")
                mean_height = np.mean(pulse_height) * 1e5

                if voltage not in voltage_data:
                    voltage_data[voltage] = []
                voltage_data[voltage].append((dgas, mean_height))

# Plotting
for voltage in sorted(voltage_data.keys()):
    data = sorted(voltage_data[voltage])  # Sort by dGas
    dgas_vals = [x[0] for x in data]
    heights = [x[1] for x in data]
    plt.plot(dgas_vals, heights, label=f"{voltage} V")

plt.xlabel("Gas Gap (cm)")
plt.ylabel("Mean Pulse Height(a.u.) (×10⁵)")
plt.title("Pulse Height vs Gas Gap")
plt.legend(title="Voltage")
plt.grid(True)
plt.tight_layout()
plt.savefig("pulse_height_vs_dgas.png", dpi=1000)

plt.show()

