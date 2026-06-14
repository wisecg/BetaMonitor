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


class ProcessRoot:
    def __init__(self):
        self.df_dep = None
        self.df_primaries = None
        self.fig = None
        self.ax = None
        self.hist_en_min = 0.0
        self.hist_en_max = 3.6
        self.hist_photon_min = 0
        self.hist_photon_max = 1e3
        self.bin_number = 200
        self.df_counts = pd.DataFrame()
        self.df_counts_response = pd.DataFrame()
        self.df_counts_coincidence = pd.DataFrame()

    def load(self, root_file):
        self.root_file = root_file
        ff = uproot.open(self.root_file)
        self.df_dep = ff['simData'].arrays(ff['simData'].keys(), library='pd')
        self.df_primaries = ff['primaryInput'].arrays(ff['primaryInput'].keys(), library='pd')
    
    def process_counts(self, apply_response=False):
        # counts_prim, en_prim = self.get_primary_spectrum()
        df_dep = self.df_dep
        counts_scint_a, en_scint_a = self.get_scint_spectrum(df_dep, volumeid=3, apply_response=apply_response)
        df_scint_a = pd.DataFrame({'scint_id': 1, 'energy': en_scint_a, 'counts': counts_scint_a})
        counts_scint_b, en_scint_b = self.get_scint_spectrum(df_dep, volumeid=4, apply_response=apply_response)
        df_scint_b = pd.DataFrame({'scint_id': 2, 'energy': en_scint_b, 'counts': counts_scint_b})
        if apply_response:
            self.df_counts_response = pd.concat([df_scint_a, df_scint_b], ignore_index=True)
            # self.df_counts_response = self.df_counts_response.rename(columns={'energy': 'n_photons'})
        else:
            self.df_counts = pd.concat([df_scint_a, df_scint_b], ignore_index=True)
        
    def rebin_energy(self, energy_array):
        bins = np.linspace(self.hist_en_min, self.hist_en_max, self.bin_number + 1)
        if np.min(energy_array) < self.hist_en_min or np.max(energy_array) > self.hist_en_max:
            print(f"Warning: Energy values outside the specified range [{self.hist_en_min}, {self.hist_en_max}].")
        counts, bin_edges = np.histogram(energy_array, bins=bins)
        return counts, bin_edges[:-1]
    
    def rebin_photon_counts(self, photon_array):
        bins = np.linspace(self.hist_photon_min, self.hist_photon_max, self.bin_number + 1)
        if np.min(photon_array) < self.hist_photon_min or np.max(photon_array) > self.hist_photon_max:
            print(f"Warning: Photon count values outside the specified range [{self.hist_photon_min}, {self.hist_photon_max}].")
        counts, bin_edges = np.histogram(photon_array, bins=bins)
        return counts, bin_edges[:-1]
    
    def n_photons_from_energy(self, energy_array):
        # implement simple response function based on following steps
        # 1. Convert dep energy to number of scintillation photons from datasheet 1e4 photon/MeV 
        scintillator_response = 1e4 # photons / MeV
        total_efficiency = 0.02 # includes geometric effects and SIPM detection efficiency, from Heather's measurements. 
        rng = np.random.default_rng()
        n_photons_initial = energy_array * scintillator_response
        n_photons = rng.poisson(n_photons_initial)
        n_detected = rng.binomial(n_photons, total_efficiency)
        return n_detected
    
    def get_primary_spectrum(self):
        # filtering all except electrons and positrons
        df_prim = self.df_primaries[(self.df_primaries['pid'] == 11) | (self.df_primaries['pid'] == -11)]
        counts, en = self.rebin_energy(df_prim['primaryenergy'])
        return counts, en
    
    def get_primary_gamma_spectrum(self):
        # filtering all except gammas
        df_prim = self.df_primaries[self.df_primaries['pid'] == 22]
        counts, en = self.rebin_energy(df_prim['primaryenergy'])
        return counts, en
    
    def get_scint_spectrum(self, df_dep, volumeid, apply_response=False, apply_pileup=False):
        df_vol = df_dep[df_dep['volumeid'] == volumeid]
        df_vol_e = df_vol[(df_vol['pid'] == 11) | (df_vol['pid'] == -11)] # filter for electrons and positrons only
        if apply_pileup:
            try:
                dep_energy = df_vol_e.groupby('eventid_pileup')['depenergy'].sum()
            except KeyError:
                print("Eventid_pileup column not found. Make sure to run simulate_pileup() before calling this function with apply_pileup=True.")
                dep_energy = df_vol_e.groupby('eventid')['depenergy'].sum()
        else:
            dep_energy = df_vol_e.groupby('eventid')['depenergy'].sum()
        if apply_response:
            n_detected = self.n_photons_from_energy(dep_energy)
            counts, en = self.rebin_photon_counts(n_detected)
        else:
            counts, en = self.rebin_energy(dep_energy)
        return counts, en
    
    def get_scint_spectrum_photons(self, volumeid):
        df_dep = self.df_dep
        df_vol = df_dep[df_dep['volumeid'] == volumeid]
        dep_energy = df_vol.groupby('eventid')['depenergy'].sum()
        n_detected = self.n_photons_from_energy(dep_energy)
        counts, en = self.rebin_photon_counts(n_detected)
        return counts, en
    
    def plot_spectra(self, ax, df_dep=None, plot_primary=True, plot_primary_gamma=False, plot_pileup=False):
        if df_dep is None:
            df_dep = self.df_dep    
        if plot_primary:
            counts_prim, en_prim = self.get_primary_spectrum()
            ax.plot(en_prim, counts_prim, drawstyle='steps-mid', label='Primary Energy, $e^-$ & $e^+$', color='orange', alpha=0.4)
        
        if plot_primary_gamma:
            counts_prim_gamma, en_prim_gamma = self.get_primary_gamma_spectrum()
            ax.plot(en_prim_gamma, counts_prim_gamma, drawstyle='steps-mid', label='Primary Energy, $\gamma$', color='red', alpha=0.4)

        counts_scint_a, en_scint_a = self.get_scint_spectrum(df_dep, volumeid=3, apply_pileup=plot_pileup)
        ax.plot(en_scint_a, counts_scint_a, drawstyle='steps-mid', label='Scintillator A - Deposited Energy, $e^-$ & $e^+$', color='royalblue')

        counts_scint_b, en_scint_b = self.get_scint_spectrum(df_dep, volumeid=4, apply_pileup=plot_pileup)
        ax.plot(en_scint_b, counts_scint_b, drawstyle='steps-mid', label='Scintillator B - Deposited Energy, $e^-$ & $e^+$', color='mediumseagreen')

        ax.set_xlabel('Energy (MeV)')
        ax.set_ylabel('Counts')
        ax.set_yscale('log')
        ax.grid(True)
        ax.legend(loc='upper center', bbox_to_anchor=(0.5, -0.15), ncol=2)

    def plot_photon_spectra(self, ax, df_dep=None):
        if df_dep is None:
            df_dep = self.df_dep
        counts_photons_a, en_photons_a = self.get_scint_spectrum_photons(df_dep, volumeid=3)
        ax.plot(en_photons_a, counts_photons_a, drawstyle='steps-mid', label='Scintillator A - Detected Photons', color='royalblue')

        counts_photons_b, en_photons_b = self.get_scint_spectrum_photons(df_dep, volumeid=4)
        ax.plot(en_photons_b, counts_photons_b, drawstyle='steps-mid', label='Scintillator B - Detected Photons', color='mediumseagreen')

        ax.set_xlabel('Number of Detected Photons')
        ax.set_ylabel('Counts')
        ax.set_yscale('log')
        ax.grid(True)
        ax.legend()

    def test_monoenergetic_dep(self, energy_mev, n_events):
        dep_energy = np.full(n_events, energy_mev)
        n_detected = self.n_photons_from_energy(dep_energy)
        counts, en = self.rebin_photon_counts(n_detected)
        plt.figure()
        plt.plot(en, counts, drawstyle='steps-mid', label=f'Monoenergetic Dep Energy = {energy_mev} MeV', color='purple')
        plt.xlabel(f'Number of Detected Photons for N={n_events} Events')
        plt.ylabel('Counts')
        plt.yscale('log')
        plt.grid(True)
        plt.legend()
        plt.show() 
    
    def process_coincidence(self):
        df_dep = self.df_dep
        # find where the same event is present in both scintillators (volumeid 3 and 4)
        df_a = df_dep[df_dep['volumeid'] == 3]
        df_a = df_a[df_a['depenergy'] > 0] # filter for events with nonzero energy deposition in scintillator A
        df_b = df_dep[df_dep['volumeid'] == 4]
        df_b = df_b[df_b['depenergy'] > 0] # filter for events with nonzero energy deposition in scintillator B
        coinc_events = set(df_a['eventid']).intersection(set(df_b['eventid']))
        df_coin = df_dep[df_dep['eventid'].isin(coinc_events)]
        self.df_dep_coin  = df_coin
        print(f"Number of coincident events in both scintillators: {len(coinc_events)}")
        # further analysis of coincident events can be done here, e.g. comparing
    
    def simulate_pileup(self, activity_bq, time_window_s):
        # first need to add timestamps to the events in the root file based randomly on the activity.
        df_dep = self.df_dep
        df_dep['eventid_pileup'] = df_dep['eventid'] # initialize pileup eventid column to original eventid

        events = self.df_primaries['eventid'].unique()
        n_primary_events = len(events)
        rng = np.random.default_rng() 
        event_times = np.zeros(n_primary_events, dtype=float) # initialize event_time column
        # df_dep.loc[df_dep['eventid'] == events[0], 'eventid_pileup'] = events[0]
        event_time = 0
        event_times[0] = event_time
        
        for i in range(n_primary_events-1):
            u = rng.random()
            dt = -np.log(1-u) / activity_bq # time until next event in seconds, from exponential distribution
            event_time += dt
            event_times[i+1] = event_time
            # if (df_dep['eventid'] == events[i+1]).any(): # this is probably very inefficient 
            #     df_dep.loc[df_dep['eventid'] == events[i+1], 'event_time'] = event_time
        
        dep_events = df_dep['eventid'].unique()

        i = 1
        dep_event = dep_events[0]
        # event_time0 = 
        time_window_s_remaining = time_window_s
        while i < len(dep_events):
            time_between_events = event_times[dep_events[i]]-event_times[dep_events[i-1]]
            if time_between_events < time_window_s_remaining:
                df_dep.loc[df_dep['eventid'] == dep_events[i], 'eventid_pileup'] = dep_event
                time_window_s_remaining -= time_between_events
            else:
                dep_event = dep_events[i]
                time_window_s_remaining = time_window_s
            i += 1
        
        # for i in range(len(dep_events)-1):
        #     for j in range(len(dep_events)-i-1):
        #         dt = event_times[i+j+1] - event_times[i] # this wont work
        #         if dt >= time_window_s:
        #             break
        #     event_range = range(i, i+j+1)
        #     df_dep.loc[df_dep['eventid'].isin(dep_events[event_range]), 'eventid_pileup'] = dep_events[i]
        
        # self.df_dep = df_dep
            



    def create_plot(self, plot_num=1):
        fig, ax = plt.subplots(plot_num, 1, figsize=(10, 6))
        if plot_num == 1:
            ax = [ax]
        for axis in ax:
            axis.set_ylim(ymin=1e-1, ymax=3e5)
        self.fig = fig
        self.ax = ax


    def plot(self, root_file, ax_num = 0, isotope = ''):
        self.root_file = root_file
        self.load()
        df_prim = self.df_primaries[(self.df_primaries['pid'] == 11) | (self.df_primaries['pid'] == -11)]
        df = self.df_dep[(self.df_dep['pid'] == 11) | (self.df_dep['pid'] == -11 )]
        # df = df[df['trackid']==4]
        en, bins = self.rebin_energy(df_prim['primaryenergy'])
        self.ax[ax_num].plot(bins, en, drawstyle='steps-mid', label=f'{isotope} Primary Energy', color='orange')

        # df_slice = df[df['volumeid'] == 2]
        # # df_slice = df_slice[(df_slice['pid'] == 11) | (df_slice['pid'] == -11)]
        # dep_energy = df_slice.groupby('eventid')['depenergy'].sum()
        # en, bins = self.rebin_energy(dep_energy)
        # self.ax.plot(bins, en, drawstyle='steps-mid', label=f'{isotope} Window - Deposited Energy', color='darkred')

        df_slice = df[df['volumeid'] == 3]
        # df_slice = df_slice[(df_slice['pid'] == 11) | (df_slice['pid'] == -11)]
        dep_energy = df_slice.groupby('eventid')['depenergy'].sum()
        # dep_energy = df[df['volumeid'] == 3]['depenergy']
        en, bins = self.rebin_energy(dep_energy)
        self.ax[ax_num].plot(bins, en, drawstyle='steps-mid', label=f'{isotope} Scintillator A - Deposited Energy', color='royalblue')
        
        df_slice = df[df['volumeid'] == 4]
        # df_slice = df_slice[(df_slice['pid'] == 11) | (df_slice['pid'] == -11)]
        dep_energy = df_slice.groupby('eventid')['depenergy'].sum()
        en, bins = self.rebin_energy(dep_energy)
        self.ax[ax_num].plot(bins, en, drawstyle='steps-mid', label=f'{isotope} Scintillator B - Deposited Energy', color='mediumseagreen')

        # self.ax.set_title(f'Histogram of Energy for Different Volumes\n{title}')
        self.ax[ax_num].set_xlabel('Energy (MeV)')
        self.ax[ax_num].set_ylabel('Counts')
        self.ax[ax_num].set_yscale('log')
        self.ax[ax_num].grid(True)
        # self.ax.legend()
        # plt.show()

    def print_num_events(self):
        print(f"Total number of events: {len(self.df_dep)}")
        print(f"Number of events in Scintillator A (volumeid=3): {len(self.df_dep[self.df_dep['volumeid'] == 3])}")
        print(f"Number of events in Scintillator B (volumeid=4): {len(self.df_dep[self.df_dep['volumeid'] == 4])}")
        print(f"Number of events in Vaccuum (volumeid=1): {len(self.df_dep[self.df_dep['volumeid'] == 1])}")
        print(f"Number of primary events: {len(self.df_primaries[self.df_primaries['pid'] == 11])}")

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

        self.ax.plot(bin_edges[:-1], counts, drawstyle='steps-mid', color='red', label='PDF from Code (normalized)')
        self.ax.grid(True)
    
    def show(self, title=None):
        if title:
            self.fig.suptitle(title)
        for axis in self.ax:
            axis.legend()
        self.fig.tight_layout()
        plt.show()


if __name__ == "__main__":
    root_6He = "~/dev/BetaMonitor/output/6He_1e6.root"
    root_19Ne = "~/dev/BetaMonitor/output/19Ne_1e6.root"
    root_90Sr = "~/dev/BetaMonitor/output/90Sr_1e6.root"
    plotter = ProcessRoot()

    # plotter.plot_pdf_from_cdf(cdf, n_primaries=1e6)
    # plotter.plot_from_ddep(ddep, n_primaries=1e6, use_experimental=True, plot_error_bars=False)
    # plotter.plot_old_root(root_6He_original, n_primaries=1e6, cal=False)
    plotter.load(root_90Sr)
    plotter.simulate_pileup(activity_bq=1e6, time_window_s=1e-5)
    plotter.process_coincidence()
    plotter.process_counts()

    plotter.create_plot(plot_num=2)
    plotter.plot_spectra(plotter.df_dep, plotter.ax[0])
    plotter.plot_photon_spectra(plotter.df_dep, plotter.ax[1])
    plotter.show(title='Spectra for 1e6 90Sr Primaries')
    # plotter.test_monoenergetic_dep(1, 100000)
    print('end')


    