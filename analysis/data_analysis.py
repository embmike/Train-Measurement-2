# -*- coding: utf-8 -*-
""" Plotte die Zugmessungen
@author: Mike Netz

"""

import numpy as np
from matplotlib import pyplot as plt
import pandas as pd

pd_data = pd.read_csv("./analysis/log_data.csv")
print(pd_data.head(5))

time_stamps = pd_data.time.to_numpy()
jerk = pd_data.jerk.to_numpy()
acc = pd_data.acceleration.to_numpy()
vel = pd_data.velocity.to_numpy()
measurements = pd_data.velocity_m.to_numpy()
velocities = pd_data.velocity_f.to_numpy()
positions = pd_data.position.to_numpy()

cm = 1/2.54
fig, (ax1, ax2, ax3) = plt.subplots(3, figsize=(25*cm, 20*cm))
plt.tight_layout(pad=12.0*cm, h_pad=5.0*cm)
fig.suptitle('PT2-Modell')
ax1.set_title('Ruck')
ax2.set_title('Beschleunigung', pad=6.0)
ax3.set_title('Geschwindigkeit', pad=6.0)
ax1.set(ylabel='r [m/s^3]')
ax2.set(ylabel='a [m/s^2]')
ax3.set(xlabel='Abtastwerte [s]', ylabel='v [m/s]')
ax1.plot(time_stamps, jerk, color='red')
ax2.plot(time_stamps, acc, color='blue')
ax3.plot(time_stamps, vel, color='green')
fig.align_labels()
plt.show()

plt.plot(time_stamps, positions)
plt.xlabel('Abtastwerte [s]')
plt.ylabel('Position [m]')
plt.title('Zugposition')
plt.show()

plt.plot(time_stamps, measurements, color='gray', linestyle=':')
plt.plot(time_stamps, velocities, color='red')
plt.legend(['Gemessene Geschwindigkeit', 'Gefilterte Geschwindigkeit'])
plt.xlabel('Abtastwerte [s]')
plt.ylabel('Geschwindigkeit [m/s]')
plt.title('Zuggeschwindigkeit')
plt.show()

