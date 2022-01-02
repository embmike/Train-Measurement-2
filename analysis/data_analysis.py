# -*- coding: utf-8 -*-
""" Plotte die Zugmessungen
@author: Mike Netz

"""

import numpy as np
from matplotlib import pyplot as plt
import pandas as pd

pd_data = pd.read_csv("./analysis/log_data.csv")
print(pd_data.head(5))

time_stamps = pd_data['time'].to_numpy()
measurements = pd_data['measurement'].to_numpy()
velocities = pd_data['velocity'].to_numpy()
positions = pd_data['position'].to_numpy()

plt.plot(time_stamps, positions)
plt.xlabel('Abtastwerte [s]')
plt.ylabel('Position [m]')
plt.title('Zugposition')
plt.show()

plt.plot(time_stamps, measurements, color='gray', linestyle=':')
plt.plot(time_stamps, velocities)
plt.legend(['Gemessene Geschwindigkeit', 'Gefilterte Geschwindigkeit'])
plt.xlabel('Abtastwerte [s]')
plt.ylabel('Geschwindigkeit [m/s]')
plt.title('Zuggeschwindigkeit')
plt.show()
