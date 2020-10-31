#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
df2 = pd.read_csv(str(sys.argv[1]), header=0)  # hv file
df1 = pd.read_csv(str(sys.argv[2]), header=0)  # current file

df1 = df1.rename(columns={'HV': "current"})

mean_hv = df2['HV'].mean()

df1['HV'] = mean_hv

chambers_weights = []
norm = 1060 * 1060
area = [
    1200 * 1200,
    1200 * 1200,
    1270 * 1270,
    1340 * 1340,
    1410 * 1410,
    1430 * 1430,
    1200 * 1200,
    1200 * 1200,
    1270 * 1270,
    1340 * 1340,
    1410 * 1410,
    1430 * 1430,
    1060 * 1060,
    1060 * 1060,
    1060 * 1060,
    1060 * 1060,
    1060 * 1060,
    1060 * 1060,
    1200 * 1200,
    1200 * 1200,
    1270 * 1270,
    1340 * 1340,
    1410 * 1410,
    1430 * 1430,
    1200 * 1200,
    1200 * 1200,
    1270 * 1270,
    1340 * 1340,
    1410 * 1410,
    1430 * 1430
]

for a in area:
    chambers_weights.append((1 / (a / norm)))

a = [i for i in range(1, 60, 2)]
b = [i for i in range(0, 30)]
dic = dict(zip(a,b))

df1['current'] = df1['current'] * chambers_weights[dic[int(sys.argv[4])]]

df1.to_csv(path_or_buf=str(sys.argv[3]),
           index=False, sep=',', header=True)
