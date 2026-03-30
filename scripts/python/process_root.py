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

    def __init__(self):
        self.df = None
        self.df_primaries = None
        fig, ax = plt.subplots(figsize=(10, 6))
        self.fig = fig
        self.ax = ax

    def load(self):
        ff = uproot.open(self.root_file)
        self.df = ff['simData'].arrays(ff['simData'].keys(), library='pd')
        self.df_primaries = ff['primaryInput'].arrays(ff['primaryInput'].keys(), library='pd')

    @staticmethod
    def rebin_energy(energy_array, bin_number):
        bins = np.linspace(np.min(energy_array), np.max(energy_array), bin_number + 1)
        counts, bin_edges = np.histogram(energy_array, bins=bins)
        return counts, bin_edges[:-1]

    def plot(self, root_file, title='', bin_number=150):
        self.root_file = root_file
        self.load()
        df_prim = self.df_primaries[(self.df_primaries['pid'] == 11) | (self.df_primaries['pid'] == -11)]
        df = self.df[(self.df['pid'] == 11) | (self.df['pid'] == -11 )]
        en, bins = self.rebin_energy(df_prim['primaryenergy'], bin_number)
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Primary Particle - Initial Energy (Primaries)', color='orange')
        
        en, bins = self.rebin_energy(df[df['volumeid'] == 3]['depenergy'], bin_number)
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator A - Deposited Energy', color='royalblue')
        en, bins = self.rebin_energy(df[df['volumeid'] == 4]['depenergy'], bin_number)
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator B - Deposited Energy', color='mediumseagreen')

        self.ax.set_title(f'Histogram of Energy for Different Volumes\n{title}')
        self.ax.set_xlabel('Energy (MeV)')
        self.ax.set_ylabel('Frequency')
        self.ax.set_yscale('log')
        self.ax.grid(True)
        self.ax.legend()
        plt.show()

    def plot_cdf(self, cdf_file):
        df = pd.read_csv(cdf_file, names=['energy', 'cdf'], delimiter='\t')
        fig, ax = plt.subplots(figsize=(10, 6))
        ax.plot(df['energy'], df['cdf'], label='CDF of Decay Energy', color='green')
        ax.set_title('Cumulative Distribution Function of Decay Energy')
        ax.set_xlabel('Decay Energy (eV)')
        ax.set_ylabel('CDF')
        ax.grid(True)

    def print_num_events(self):
        print(f"Total number of events: {len(self.df)}")
        print(f"Number of events in Scintillator A (volumeid=3): {len(self.df[self.df['volumeid'] == 3])}")
        print(f"Number of events in Scintillator B (volumeid=4): {len(self.df[self.df['volumeid'] == 4])}")
        print(f"Number of events in Vaccuum (volumeid=1): {len(self.df[self.df['volumeid'] == 1])}")
        print(f"Number of primary events: {len(self.df_primaries[self.df_primaries['pid'] == 11])}")


    def plot_old_root(self, root_file, bin_number=150, cal=False):
        ff = uproot.open(root_file)
        df = ff['simData'].arrays(ff['simData'].keys(), library='pd')

        # ax.set_title(f'Histogram of Energy for Old Code\n{root_file}')
        # scintillator_a energy
        en, bins = self.rebin_energy(df[df['detSQ_En'] != 0.0]['detSQ_En'], bin_number)
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator A - Deposited Energy', color='royalblue', linestyle=':')
        # scintillator_b energy
        en, bins = self.rebin_energy(df[df['detTrig_En'] != 0.0]['detTrig_En'], bin_number)
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator B - Deposited Energy', color='mediumseagreen', linestyle=':')

        if not cal:
            en, bins = self.rebin_energy(df[df['detVac_InEn'] != 0.0]['detVac_InEn'], bin_number)
            self.ax.plot(bins, en, drawstyle='steps-mid', label='Vaccuum - Deposited Energy', color='orange', linestyle=':')
        
        self.ax.set_xlabel('Energy (MeV)')
        self.ax.set_ylabel('Frequency')
        self.ax.set_yscale('log')
        self.ax.grid(True)
        self.ax.legend()
        # plt.show()
    def show(self):
        self.ax.legend()
        plt.show()


if __name__ == "__main__":
    old_root = "/Users/harperumfress/UW/betamonitor_data/original_singlethread_data/90Sr_1e6_original.root"
    new_root = "./output/90Sr_1e6.root"
    plotter = DepEnergyHistograms()
    plotter.plot_old_root(old_root, cal=True)
    plotter.plot(new_root)
    plotter.show()
    print('end')


