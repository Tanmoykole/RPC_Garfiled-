import ROOT
import numpy as np
import matplotlib.pyplot as plt

# Open the ROOT file
file = ROOT.TFile("voltage_scan_dgas_0.15.root")

voltages = []
timing_resolutions = []

# Voltage scan range (same as the keys you shared)
voltage_range = range(2000, 15500, 500)  # from 2000 to 15000 in steps of 500

for voltage in voltage_range:
    tree_name = f"pulseTree_Voltage_{voltage}"
    tree = file.Get(tree_name)
    if not tree:
        print(f"Tree {tree_name} not found!")
        continue

    # Get timing resolution from the tree
    # Assuming the variable is called 'timingResolution'
    timing_array = []
    for event in tree:
        timing_array.append(event.timingResolution)

    if timing_array:
        mean_timing = np.mean(timing_array)
        voltages.append(voltage)
        timing_resolutions.append(mean_timing)

# Plotting
plt.figure(figsize=(10, 6))
plt.plot(voltages, timing_resolutions, marker='o', linestyle='-')
plt.xlabel("Voltage (V)")
plt.ylabel("Timing Resolution (ns)")
plt.title("Timing Resolution vs Voltage (dGas = 0.10 cm)")
plt.grid(True)
plt.tight_layout()
plt.show()

