# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
from matplotlib.colors import BoundaryNorm
from matplotlib.ticker import MaxNLocator
import numpy as np

x,y = np.loadtxt("D:\\projects\\walkByEnergyFromGs\\averageTrying_15_5000.dat", unpack=True)

plt.plot(x,y)
plt.title('chaos level')
plt.xlabel(u'spin excess')
plt.ylabel(u'average energy')
plt.show()