import matplotlib.pyplot as plt
import csv
import os


# Plotting variables
x = []
F = []
P = []



# File system
fileList = os.listdir()
csvList = []


cnt = 0

with open("LOG.csv", 'r') as csvfile:
    lines = csv.reader(csvfile, delimiter=',')
    if cnt != 0:
        for row in lines:
                x.append(float(row[0]))
                P.append(float(row[1]))
                F.append(float(row[2]))
                print(float(row[0]))   
    cnt += 1
    print(cnt)



plt.style.use('dark_background')

fig, ax1 = plt.subplots()
ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis

ax1.set_xlabel('time [s]')
ax1.grid(True)

ax1.set_ylabel('Force [N]', color='y')
ax1.tick_params(axis='y', labelcolor='y')
ax1.fill_between(x, F, 0, alpha=0.2, color='y')

ax2.set_ylabel('Pressure [MPa]', color='b')  # we already handled the x-label with ax1
ax2.tick_params(axis='y', labelcolor='b')

ax1.plot(x, F, color='y')
ax2.plot(x, P, color='b')




fig.tight_layout()  # otherwise the right y-label is slightly clipped
plt.show()