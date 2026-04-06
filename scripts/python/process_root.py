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
        self.hist_en_min = 0.0
        self.hist_en_max = 3.5
        self.bin_number = 200

    def load(self):
        ff = uproot.open(self.root_file)
        self.df = ff['simData'].arrays(ff['simData'].keys(), library='pd')
        self.df_primaries = ff['primaryInput'].arrays(ff['primaryInput'].keys(), library='pd')

    def rebin_energy(self, energy_array):
        bins = np.linspace(self.hist_en_min, self.hist_en_max, self.bin_number + 1)
        if np.min(energy_array) < self.hist_en_min or np.max(energy_array) > self.hist_en_max:
            print(f"Warning: Energy values outside the specified range [{self.hist_en_min}, {self.hist_en_max}].")
        counts, bin_edges = np.histogram(energy_array, bins=bins)
        return counts, bin_edges[:-1]

    def plot(self, root_file):
        self.root_file = root_file
        self.load()
        df_prim = self.df_primaries[(self.df_primaries['pid'] == 11) | (self.df_primaries['pid'] == -11)]
        df = self.df[(self.df['pid'] == 11) | (self.df['pid'] == -11 )]
        en, bins = self.rebin_energy(df_prim['primaryenergy'])
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Primary Particle - Initial Energy (Primaries)', color='orange')
        
        en, bins = self.rebin_energy(df[df['volumeid'] == 3]['depenergy'])
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator A - Deposited Energy', color='royalblue')
        en, bins = self.rebin_energy(df[df['volumeid'] == 4]['depenergy'])
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Scintillator B - Deposited Energy', color='mediumseagreen')

        # self.ax.set_title(f'Histogram of Energy for Different Volumes\n{title}')
        self.ax.set_xlabel('Energy (MeV)')
        self.ax.set_ylabel('Frequency')
        self.ax.set_yscale('log')
        self.ax.grid(True)
        # self.ax.legend()
        # plt.show()

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
        en, bins = self.rebin_energy(df[df['detSQ_En'] != 0.0]['detSQ_En'])
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Old Code - Scintillator A - Dep Energy', color='royalblue', linestyle=':')
        # scintillator_b energy
        en, bins = self.rebin_energy(df[df['detTrig_En'] != 0.0]['detTrig_En'])
        self.ax.plot(bins, en, drawstyle='steps-mid', label='Old Code - Scintillator B - Dep Energy', color='mediumseagreen', linestyle=':')

        if not cal:
            en, bins = self.rebin_energy(df[df['detVac_InEn'] != 0.0]['detVac_InEn'])
            self.ax.plot(bins, en, drawstyle='steps-mid', label='Old Code - Vaccuum - Dep Energy', color='orange', linestyle=':')
        
        self.ax.set_xlabel('Energy (MeV)')
        self.ax.set_ylabel('Frequency')
        self.ax.set_yscale('log')
        self.ax.grid(True)
        self.ax.legend()
        # plt.show()


    def plot_pdf_from_cdf(self, cdf_file, n_primaries):
        """
        Reads a CDF file (2 columns: energy, cdf), computes the PDF, and plots the spectrum.
        The PDF is normalized so the total number of events is n_primaries.
        If output_pdf_path is given, saves the plot to that file.
        """


        # Read CDF file
        df = pd.read_csv(cdf_file, names=['energy', 'cdf'], delimiter='\t')
        # Compute PDF as the discrete derivative of CDF
        energies = df['energy'].values/1e6 # Convert eV to MeV
        cdf = df['cdf'].values
        pdf = np.diff(cdf, prepend=cdf[0])
        # The first value is zero (since prepend), so shift
        pdf[0] = pdf[1]
        # Normalize PDF so sum(pdf) = n_primaries
        pdf_sum = np.sum(pdf)
        pdf_scaled = pdf * (n_primaries / pdf_sum)
        bins = np.linspace(self.hist_en_min, self.hist_en_max, self.bin_number + 1)
        counts, bin_edges = np.histogram(energies, bins=bins, weights=pdf_scaled)

        self.ax.plot(bin_edges[:-1], counts, drawstyle='steps-mid', color='red', label='PDF from BM Code (normalized)')
        self.ax.grid(True)

    def plot_from_ddep(self, ddep_file, use_experimental=False, n_primaries=None, interpolate=True,
                       plot_error_bars=False, errorbar_stride=1):
        """
        Parse a BetaShape .bs spectrum file and plot it rebinned to the class energy grid.

        The expected data table columns are:
          E(keV), dN/dE calc., unc., dN/dE exp., unc.
        """
        table_header_idx = None
        with open(ddep_file, 'r') as f:
            lines = f.readlines()

        for idx, line in enumerate(lines):
            if line.strip().startswith('E(keV)'):
                table_header_idx = idx + 1
                break

        if table_header_idx is None:
            raise ValueError("Could not find BetaShape data table header 'E(keV)'.")

        df = pd.read_csv(
            ddep_file,
            sep=r'\s+',
            skiprows=table_header_idx,
            names=['E_keV', 'dNdE_calc', 'unc_calc', 'dNdE_exp', 'unc_exp'],
            engine='python',
        )

        for col in df.columns:
            df[col] = pd.to_numeric(df[col], errors='coerce')
        df = df.dropna()

        energies_mev = df['E_keV'].to_numpy() / 1000.0
        dnde = df['dNdE_exp'].to_numpy() if use_experimental else df['dNdE_calc'].to_numpy()
        dnde_unc = df['unc_exp'].to_numpy() if use_experimental else df['unc_calc'].to_numpy()

        bins = np.linspace(self.hist_en_min, self.hist_en_max, self.bin_number + 1)
        bin_edges = bins
        bin_width = np.diff(bin_edges)
        bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])

        if interpolate:
            # Interpolate dN/dE to requested bin centers so fine binning stays smooth.
            dnde_interp = np.interp(bin_centers, energies_mev, dnde, left=0.0, right=0.0)
            unc_interp = np.interp(bin_centers, energies_mev, dnde_unc, left=0.0, right=0.0)
            counts = dnde_interp * bin_width
            counts_unc = unc_interp * bin_width
        else:
            # Legacy behavior: integrate from input sample points via weighted histogram.
            dE = np.gradient(energies_mev)
            weights = dnde * dE
            counts, _ = np.histogram(energies_mev, bins=bin_edges, weights=weights)
            unc_weights = dnde_unc * dE
            counts_unc_sq, _ = np.histogram(energies_mev, bins=bin_edges, weights=unc_weights**2)
            counts_unc = np.sqrt(counts_unc_sq)

        if n_primaries is not None:
            total = np.sum(counts)
            if total > 0:
                scale = float(n_primaries) / total
                counts = counts * scale
                counts_unc = counts_unc * scale

        label = 'DDEP Exp. spectrum' if use_experimental else 'DDEP Calc. spectrum'
        self.ax.plot(bin_edges[:-1], counts, color='purple', drawstyle='steps-mid', label=label)
        if plot_error_bars:
            stride = max(1, int(errorbar_stride))
            self.ax.errorbar(
                bin_centers[::stride],
                counts[::stride],
                yerr=counts_unc[::stride],
                fmt='none',
                ecolor='purple',
                elinewidth=1,
                capsize=2,
                alpha=0.7,
                label=f'{label} uncertainty'
            )
        self.ax.set_xlabel('Energy (MeV)')
        self.ax.set_ylabel('Counts')
        self.ax.grid(True)
    
    def show(self, title=None):
        if title:
            self.ax.set_title(title)
        self.ax.legend()
        plt.show()


if __name__ == "__main__":
    # old_root = "/Users/harperumfress/UW/betamonitor_data/original_singlethread_data/19Ne_1e6_original.root"
    # 6He
    old_root = "./output/6He_1e6_original_newgeo.root"
    new_root = "./output/6He_1e6_11-4_qgsp.root"
    cdf = './dat/6HeDecay_cdf.txt'
    ddep = '/Users/harperumfress/UW/betamonitor_data/He6_ddep/beta-_He6_trans0.bs'

    # 19Ne
    

    plotter = DepEnergyHistograms()``
    plotter.plot_pdf_from_cdf(cdf, n_primaries=1e6)
    plotter.plot_from_ddep(ddep, n_primaries=1e6, 
                           use_experimental=True, plot_error_bars=False)
    plotter.plot_old_root(old_root, cal=False)
    plotter.plot(new_root)
    plotter.show(title='')
    print('end')


    