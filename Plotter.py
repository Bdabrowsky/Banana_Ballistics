import matplotlib.pyplot as plt
import csv
import os


# Plotting variables
x = []
F = []
P = []
Kn = []




# File system
fileList = os.listdir()
csvList = []


cnt = 0

impulse = 0
prevtime = 0

with open("biprop_outpur.csv", 'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=',')
    for row in lines:
        cnt = cnt + 1
        if cnt > 5:
            x.append(float(row[0]))
            P.append(float(row[1]))
            F.append(float(row[2]))
    
            impulse = impulse + float(row[2]) * (float(row[0]) - prevtime)
            prevtime = float(row[0])


    


plt.style.use('dark_background')

fig, ax1 = plt.subplots()
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

ax1.set_xlabel('time [s]')
ax1.grid(True)

ax1.set_ylabel('Thrust [N]', color='y')
ax1.tick_params(axis='y', labelcolor='y')
#ax1.fill_between(x, F, 0, alpha=0.2, color='y')

ax2.set_ylabel('Pressure [MPa]', color='b')  # we already handled the x-label with ax1
ax2.tick_params(axis='y', labelcolor='b')

ax1.plot(x, F, color='y')
ax2.plot(x, P, color='b')

ax1.legend(['Thrust'], loc='upper left')  # Add legend for axes
ax2.legend(['Pressure'], loc='upper right')  # Add legend for axes


print("impulse")


fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()