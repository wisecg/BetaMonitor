import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
infile = '/Users/harperumfress/dev/BetaMonitor/dat/90Sr90YDecay_hist_points_full.mac'
outfile = '/Users/harperumfress/dev/BetaMonitor/dat/90Sr90YDecay_hist_points.mac'

indata = pd.read_csv(infile, delim_whitespace=True, header=None, names=['string', 'energy', 'probability'])
downsample_rate = np.ceil(len(indata) / 1024)
outdata = indata[::int(downsample_rate)]
plt.plot(outdata['energy'], outdata['probability'], drawstyle='steps-mid')
plt.plot(indata['energy'], indata['probability'], drawstyle='steps-mid', alpha=0.5)

# write to outfile file with /gps/hist/point infront of each row
with open(outfile, 'w') as f:
    for index, row in outdata.iterrows():
        f.write(f"/gps/hist/point {row['energy']:.8f} {row['probability']:.8f}\n")
print('end')