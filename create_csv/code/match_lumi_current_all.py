#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
# df1 = pd.read_csv(str(sys.argv[1]), header=0)  # matched hv_current
# df2 = pd.read_csv(str(sys.argv[2]), header=0)  # matched luminosities

# df1.drop('fNanoSec', axis=1, inplace=True)

# df = pd.merge_asof(df2, df1, on='fSec',
#                    tolerance=5, direction='nearest')
# df = df[df['current'].notna()]

# df.to_csv(path_or_buf=str(sys.argv[3]),
#           index=False, sep=' ', header=False)
print("Entered Matched Lumi Current All")
dfs = []
index_list = []
for i in range(0, 36, 2):
    index_list += [j + 60 * i for j in range(1, 60, 2)]
for i in index_list:
   file_name = str(sys.argv[1]) + "hv_current_" + str(i) + ".csv"
   dfs.append(pd.read_csv(file_name, header=0))

n_list = []
for df in dfs:
    n_list.append(len(df))

remove_i = n_list.index(max(n_list))
main_df = dfs[remove_i]
#dfs.remove(dfs[remove_i])
mean_current = []
std_current = []
size = len(main_df)
counter = 0
for time in main_df['fSec']:
    _current = []
    for df in dfs:
        result_index = df['fSec'].sub(time).abs().idxmin()
        if df['HV'][result_index] > 1100: #only working chambers
            _current.append(df['current'][result_index])
    mean_current.append(np.mean(_current))
    std_current.append(np.std(_current, ddof=1))
    counter+=1
    print( "{} / {}\t{:.2f}".format(counter, size, counter/size), end="\r")
main_df['mean_current'] = mean_current
main_df['std_current'] = std_current
main_df.to_csv(path_or_buf=str(sys.argv[2]),
                          index=False, sep=',', header=True)
print("Exited Matched Lumi Current All")
