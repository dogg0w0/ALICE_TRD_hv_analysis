#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
df2 = pd.read_csv(str(sys.argv[1]), header=0) #hv file
df1 = pd.read_csv(str(sys.argv[2]), header=0) #current file

df1 = df1.rename(columns={'HV': "current"})

mean_hv = df2['HV'].mean()

df1['HV'] = mean_hv


df1.to_csv(path_or_buf=str(sys.argv[3]),
          index=False, sep=',', header=True)
