#!/usr/bin/env python3
import matplotlib.pyplot as plt
import numpy as np
from scipy.interpolate import spline
import sys

if len(sys.argv) < 2:
    print("Please pass a log file as input")
    sys.exit()

f = open(sys.argv[1])
current = f.readline()
while current:
    points = []
    plt.title(f.readline().strip())
    current = f.readline()
    currentTime = 0
    while current.strip():
        points.append(list(map(float, current.strip().split(' '))))
        points[-1][0] += currentTime
        currentTime = points[-1][0]
        current = f.readline()
    if not len(points):
        continue

    # Removes tail if data stays constant after reaching setpoint
    while len(points) > 5 and points[-1][1] == points[-3][1]:
        del(points[-1])

    x = np.array([i[0] for i in points])
    y = np.array([i[1] for i in points])
    #  Interpolation is fun and pretty but we kind of want exact data for tuning
    prettyX = np.linspace(x.min(), x.max(), 300)  # number of points to interpolate
    prettyY = spline(x, y, prettyX)
    plt.tight_layout()
    #  plt.plot(x, y)
    plt.plot(prettyX, prettyY)
    plt.show()

