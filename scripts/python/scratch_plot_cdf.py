import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
cdf_file = './dat/6HeDecay_cdf.txt'
df = pd.read_csv(cdf_file, delim_whitespace=True, names=['Energy', 'CDF'])
df['diff'] = df['CDF'].diff()
hist_file = './dat/6HeDecay_hist.txt'
hist_df = pd.read_csv(hist_file, delim_whitespace=True, names=['Energy', 'Density'])

plt.figure(figsize=(10, 6))
plt.plot(df['Energy'], df['diff'], color='blue')
plt.plot(hist_df['Energy']*1e6, hist_df['Density'], color='red', label='Histogram Density')
plt.title('Cumulative Distribution Function (CDF) of 6He Decay')
plt.xlabel('Decay Energy (MeV)')
plt.ylabel('CDF')
plt.grid()
plt.show()