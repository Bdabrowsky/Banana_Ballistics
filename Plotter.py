import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv("biprop_output.csv")

# Strip any extra spaces from column names
df.columns = df.columns.str.strip()

# Check columns (optional debug)
print("Columns:", df.columns.tolist())

# Create figure and subplots
fig, axs = plt.subplots(2, 2, figsize=(16, 8), sharex=False)

# Plot 1: Thrust and Chamber Pressure
axs[0][0].plot(df["Time"], df["Thrust"], label="Thrust (N)")
axs[0][0].plot(df["Time"], df["Chamber_pressure"]*100.0, label="Chamber Pressure * 100 (MPa)")
axs[0][0].set_ylabel("Thrust / Pressure")
axs[0][0].set_title("Rocket Engine Test Data Over Time")
axs[0][0].legend()
axs[0][0].grid(True)

# Plot 2: Mass Flow Rates
axs[1][0].plot(df["Time"], df["Total_mass_flow"], label="Total Mass Flow (kg/s)")
axs[1][0].plot(df["Time"], df["Oxidizer_mass_flow"], label="Oxidizer Mass Flow (kg/s)")
axs[1][0].plot(df["Time"], df["Fuel_mass_flow"], label="Fuel Mass Flow (kg/s)")
axs[1][0].set_ylabel("Mass Flow (kg/s)")
axs[1][0].legend()
axs[1][0].grid(True)

# Plot 3: Remaining Mass
axs[0][1].plot(df["Time"], df["Oxidizer_mass"], label="Oxidizer Mass (kg)")
axs[0][1].plot(df["Time"], df["Fuel_mass"], label="Fuel Mass (kg)")
axs[0][1].set_ylabel("Mass (kg)")
axs[0][1].legend()
axs[0][1].grid(True)

# Plot 4: Specific Impulse
axs[1][1].plot(df["Time"], df["Isp"], label="Specific Impulse (s)", color="tab:purple")
axs[1][1].set_xlabel("Time (s)")
axs[1][1].set_ylabel("Isp (s)")
axs[1][1].legend()
axs[1][1].grid(True)

plt.tight_layout()
plt.show()
