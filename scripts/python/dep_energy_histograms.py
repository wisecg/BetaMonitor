"""
Script: dep_energy_histograms.py

Description:
    Reads simulation output data from a ROOT file and plots deposited energy histograms.
    Assumes eventwise-logged output (1 entry per event per volume entered), with 'depenergy'
    containing the total deposited energy per event per volume.

Usage:
    - Requires: uproot, numpy, pandas, matplotlib
"""

import numpy as np
import uproot
import matplotlib.pyplot as plt
import pandas as pd


class DepEnergyHistograms:

    def __init__(self, root_file):
        self.root_file = root_file
        self.df = None
        self.df_primaries = None
        self._load()

    def _load(self):
        ff = uproot.open(self.root_file)
        self.df = ff['simData'].arrays(ff['simData'].keys(), library='pd')
        self.df_primaries = ff['primaryInput'].arrays(ff['primaryInput'].keys(), library='pd')

    @staticmethod
    def rebin_energy(energy_array, bin_number):
        bins = np.linspace(np.min(energy_array), np.max(energy_array), bin_number + 1)
        counts, bin_edges = np.histogram(energy_array, bins=bins)
        return counts, bin_edges[:-1]

    def plot(self, title='', bin_number=150):
        plt.figure(figsize=(10, 6))
        en, bins = self.rebin_energy(self.df_primaries['primaryenergy'], bin_number)
        plt.plot(bins, en, drawstyle='steps-mid', label='Primary Particle - Initial Energy (Primaries)', color='orange')
        
        en, bins = self.rebin_energy(self.df[self.df['volumeid'] == 3]['depenergy'], bin_number)
        plt.plot(bins, en, drawstyle='steps-mid', label='Scintillator A - Deposited Energy', color='royalblue')
        en, bins = self.rebin_energy(self.df[self.df['volumeid'] == 4]['depenergy'], bin_number)
        plt.plot(bins, en, drawstyle='steps-mid', label='Scintillator B - Deposited Energy', color='mediumseagreen')

        plt.title(f'Histogram of Energy for Different Volumes\n{title}')
        plt.xlabel('Energy (MeV)')
        plt.ylabel('Frequency')
        plt.yscale('log')
        plt.grid(True)
        plt.legend()
        plt.show()

    def plot_cdf(self, cdf_file):
        df = pd.read_csv(cdf_file, names=['energy', 'cdf'], delimiter='\t')
        plt.figure(figsize=(10, 6))
        plt.plot(df['energy'], df['cdf'], label='CDF of Decay Energy', color='green')
        plt.title('Cumulative Distribution Function of Decay Energy')
        plt.xlabel('Decay Energy (eV)')
        plt.ylabel('CDF')
        plt.grid(True)
        plt.legend()
        plt.show()

    def print_num_events(self):
        print(f"Total number of events: {len(self.df)}")
        print(f"Number of events in Scintillator A (volumeid=3): {len(self.df[self.df['volumeid'] == 3])}")
        print(f"Number of events in Scintillator B (volumeid=4): {len(self.df[self.df['volumeid'] == 4])}")
        print(f"Number of events in Vaccuum (volumeid=1): {len(self.df[self.df['volumeid'] == 1])}")
        print(f"Number of primary events: {len(self.df_primaries[self.df_primaries['pid'] == 11])}")


if __name__ == "__main__":
    outfile = "./output/outfile_6He_1e6.root"
    # outfile = "./build/output/outfile_19Ne_3e6.root"
    # outfile = "./build/output/outfile_90Sr_3e6.root"
    # outfile = "./output/outfile_6He_3e6_tw.root"
    # outfile = "./output/outfile_90Sr_gps_1e6_ds.root"
    outfile = "./output/outfile_6He_1e6_bert_rdm.root"
    plotter = DepEnergyHistograms(outfile)
    plotter.print_num_events()
    plotter.plot(title=outfile)
    plt.show()
    plotter2 = DepEnergyHistograms(outfile2)
    plotter2.plot(title=outfile2)
    plt.show()


    # plotter.plot_cdf("./dat/6HeDecay_cdf.txt")
    print('end')

