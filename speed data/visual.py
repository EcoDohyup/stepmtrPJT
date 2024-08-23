import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# New data set
v_input = [0, 5, 10, 15]
v_actual = [0.08, 0.45, 0.77, 1]

# Convert to numpy arrays for linear regression
v_input_np = np.array(v_input)
v_actual_np = np.array(v_actual)

# Perform linear regression
slope, intercept = np.polyfit(v_input_np, v_actual_np, 1)

# Create the plot
plt.figure(figsize=(10, 6))
sns.scatterplot(x=v_input, y=v_actual, label='Data Points')

# Plot the line of best fit
x_fit = np.linspace(min(v_input), max(v_input), 100)
y_fit = slope * x_fit + intercept
plt.plot(x_fit, y_fit, color='red', label='Best Fit Line')

# Add titles and labels
plt.title('Input Velocity vs Actual Velocity')
plt.xlabel('Input Velocity (v_input)')
plt.ylabel('Actual Velocity (v_actual)')
plt.legend()

# Show the plot
plt.show()

# Print the calculated slope and intercept
print(f"Slope (m): {slope}")
print(f"Intercept (b): {intercept}")
