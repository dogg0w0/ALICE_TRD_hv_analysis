#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
df = pd.read_csv(str(sys.argv[1]), header=1)  # TOF
df[df.columns[0]] = pd.to_datetime(df[df.columns[0]])
df.drop(df.columns[2], axis=1, inplace=True)
df = df.rename(
    columns={df.columns[0]: 'time', df.columns[1]: 'pressure'})
#df = df.convert_dtypes()
df['pressure'].replace('', np.nan, inplace=True)
df.dropna(subset=['pressure'], inplace=True)

df['fSec'] = df[df.columns[0]].map(
    (lambda x: (x - pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1s')))
df['fNanoSec'] = df[df.columns[0]].map((lambda x: int(
    str((x - pd.Timestamp("1970-01-01 00:00:00.000")) // pd.Timedelta('1ns'))[10:])))

df.drop(df.columns[0], axis=1, inplace=True)

df = df.reindex(columns=['fSec', 'fNanoSec', 'pressure'])

df.fSec = pd.to_numeric(df.fSec, errors='ignore')
df = df[df.fSec >= 1535894231]
df = df[df.fSec <= 1535994231]

print(df)
