#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
# df1 = pd.read_csv(str(sys.argv[1]), header=0)  # matched hv_current
# df2 = pd.read_csv(str(sys.argv[2]), header=0)  # matched luminosities

# df1.drop('fNanoSec', axis=1, inplace=True)

# df = pd.merge_asof(df2, df1, on='fSec',
#                    tolerance=5, direction='nearest')
# df = df[df['current'].notna()]

# df.to_csv(path_or_buf=str(sys.argv[3]),
#           index=False, sep=' ', header=False)
