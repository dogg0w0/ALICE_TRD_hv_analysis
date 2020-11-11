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

if len(sys.argv) > 5:
    df = pd.read_csv(str(sys.argv[5]), header=1)  # TOF
    df[df.columns[0]] = pd.to_datetime(df[df.columns[0]])
    df.drop(df.columns[2], axis=1, inplace=True)
    df = df.rename(
        columns={df.columns[0]: 'time', df.columns[1]: 'pressure'})
    df['pressure'].replace('', np.nan, inplace=True)
    df.dropna(subset=['pressure'], inplace=True)
    df['fSec'] = df[df.columns[0]].map(
        (lambda x: (x - pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1s')))
    df['fNanoSec'] = df[df.columns[0]].map((lambda x: int(
        str((x - pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1ns'))[10:])))
    df.drop(df.columns[0], axis=1, inplace=True)
    df = df.reindex(columns=['fSec', 'fNanoSec', 'pressure'])
    df.fSec = pd.to_numeric(df.fSec, errors='ignore')
    pres = []
    for time in df1.fSec:
        result_index = df.fSec.sub(time).abs().idxmin()
        pres.append(df['pressure'][result_index])
    df1['pressure'] = pres

df1.to_csv(path_or_buf=str(sys.argv[3]),
           index=False, sep=',', header=True)
