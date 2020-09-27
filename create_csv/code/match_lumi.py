#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
df1 = pd.read_csv(str(sys.argv[1]), header=1) # TOF
df2 = pd.read_csv(str(sys.argv[2]), header=0)  # T0 file

df1[df1.columns[0]] = pd.to_datetime(df1[df1.columns[0]])
df2[df2.columns[0]] = pd.to_datetime(df2[df2.columns[0]])

mean = (df1[df1.columns[3]] + df1[df1.columns[2]]) /2.0
df1['TOF rms current'] = np.sqrt((1/2* ((df1[df1.columns[2]] - mean)**2 + (df1[df1.columns[3]] - mean)**2)))
df1[df1.columns[2]] = mean
df1.drop(df1.columns[4], axis=1, inplace=True)
df1.drop(df1.columns[3], axis=1, inplace=True)
df2.drop(df2.columns[2], axis=1, inplace=True)

df1 = df1.rename(columns= {df1.columns[0]: 'time', df1.columns[2]: 'TOF average current'})
df2 = df2.rename(columns= {df2.columns[0]: 'time', df2.columns[1]: 'T0 luminosity'})

df = pd.merge_asof(df1, df2, on='time', tolerance=pd.Timedelta('5s'), direction='nearest')
df = df[df['T0 luminosity'].notna()]

df['fSec'] = df[df.columns[0]].map((lambda x: (x -  pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1s')))
df['fNanoSec'] = df[df.columns[0]].map((lambda x: int(str((x -  pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1ns'))[10:])))
df.drop(df.columns[0], axis=1, inplace=True)
df = df.reindex(columns= ['fSec', 'fNanoSec', 'TOF average current', 'TOF rms current','luminosity', 'T0 luminosity'])

df.to_csv(path_or_buf=str(sys.argv[3]), index=False, sep=',', header=True)
