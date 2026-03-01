"""
Script: dep_energy_histograms.py

Description:
    This script reads simulation output data from a ROOT file using uproot and pandas. 
    It extracts event and step information from the 'simData' tree, including deposited energy. 
    The script is intended for generating histograms or further analysis of deposited energy and related quantities.
    This assumes that the input is an eventwise logged root file (1 entry per event per volume, if the volume was entered), 
    and that the 'depenergy' field contains the total energy deposited in the volume for that event.

Usage:
    - Ensure the ROOT file exists at '../build/output/outfile_stepwisefalse6.root'.
    - Requires: uproot, numpy, pandas, matplotlib
    - Modify field_names or file path as needed for your analysis.
"""

import numpy as np
import uproot
import matplotlib.pyplot as plt
import pandas as pd


def read_root_to_df(root_file):
    """
    Reads the specified ROOT file and extracts relevant fields into a pandas DataFrame.

    Parameters:
        root_file (str): Path to the ROOT file to be read.
    Returns:
        pd.DataFrame: DataFrame containing the extracted data.
    """
    ff = uproot.open(root_file)
    tree = ff['simData']
    tree_primaries = ff['primaryInput']
    # tree.show()

    # field_names = tree.keys()

    df = tree.arrays(tree.keys(), library='pd')
    df_primaries = tree_primaries.arrays(tree_primaries.keys(), library='pd')
    return df, df_primaries

def plot_dep_energy_histogram(df, df_primaries, title='', cal=False):
    """
    Plots a histogram of deposited energy from the DataFrame for volumeids 6 (vaccuum), 3 (scint), 5 (trigger).

    Parameters:
        df (pd.DataFrame): DataFrame containing the simulation data.
        df_primaries (pd.DataFrame): DataFrame containing the primary input data.
        title (str): Title for the histogram plot.
    """
    plt.figure(figsize=(10, 6))
    # df = df[df['pid'] != 22]  # Filter out photons
    if cal == True:
        # plt.hist(df[df['volumeid']==5]['depenergy'], bins=150, 
        #          label='Trigger - Deposited Energy', alpha=1, color='mediumseagreen')
        # plt.hist(df[df['volumeid']==3]['depenergy'], bins=150, 
        #          label='Scintillator - Deposited Energy', alpha=1, color='royalblue')
        
        en, bins = rebin_energy(df[df['volumeid']==5]['depenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Trigger - Deposited Energy', color='mediumseagreen')

        en, bins = rebin_energy(df[df['volumeid']==3]['depenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Scintillator - Deposited Energy', color='royalblue')
    
    else:
        # plt.hist(df[df['volumeid']==6]['inenergy'], bins=150, 
        #          label='Primary Particle - Initial Energy', alpha=1, color='firebrick')
        
        # plt.hist(df[df['volumeid']==3]['depenergy'], bins=150, 
        #          label='Scintillator - Deposited Energy', alpha=1, color='royalblue')
        # plt.hist(df[df['volumeid']==5]['depenergy'], bins=150, 
        #          label='Trigger - Deposited Energy', alpha=1, color='mediumseagreen')
        
        en, bins = rebin_energy(df[(df['volumeid']==6) & (df['trackid']==1)]['inenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Primary Particle - Initial Energy', color='firebrick')

        en, bins = rebin_energy(df_primaries['primaryenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Primary Particle - Initial Energy (Primaries)', color='orange')
        en, bins = rebin_energy(df[df['volumeid']==3]['depenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Scintillator - Deposited Energy', color='royalblue')
        en, bins = rebin_energy(df[df['volumeid']==5]['depenergy'], bin_number=150)
        plt.plot(bins, en, drawstyle='steps-mid', label='Trigger - Deposited Energy', color='mediumseagreen')
    
    plt.title(f'Histogram of Energy for Different Volumes\n{title}')
    plt.xlabel('Energy (MeV)')
    plt.ylabel('Frequency')
    plt.yscale('log')
    plt.grid(True)
    plt.legend()
    plt.grid(True)
    plt.show()

def rebin_energy(energy_array, bin_number):
    """
    Rebins the energy array into a specified number of bins.

    Parameters:
        energy_array (np.ndarray): Array of energy values to be rebinned.
        bin_number (int): Number of bins to rebin into.
    Returns:
        np.ndarray: Rebinned energy array.
    """
    min_energy = np.min(energy_array)
    max_energy = np.max(energy_array)
    bins = np.linspace(min_energy, max_energy, bin_number + 1)
    rebinned_energy, bin_edges = np.histogram(energy_array, bins=bins)
    return rebinned_energy, bin_edges[:-1]

def plot_cdf(cdf_file):
    """
    Plots a cumulative distribution function (CDF) from the specified file.

    Parameters:
        cdf_file (str): Path to the file containing CDF data.
    """
    # Placeholder for CDF plotting logic

    df = pd.read_csv(cdf_file, names=['energy', 'cdf'], delimiter='\t')
    plt.figure(figsize=(10, 6))
    plt.plot(df['energy'], df['cdf'], label='CDF of Decay Energy', color='green')
    plt.title('Cumulative Distribution Function of Decay Energy')
    plt.xlabel('Decay Energy (ev)')
    plt.ylabel('CDF')
    plt.grid(True)
    plt.legend()
    plt.show()


if __name__ == "__main__":
    # outfile = "./build/output/outfile_6He_3e6.root"
    # outfile = "./build/output/outfile_19Ne_3e6.root"
    # outfile = "./build/output/outfile_90Sr_3e6.root"
    outfile = "./build/output/outfile_6He_3e6_tw.root"
    df, df_primaries = read_root_to_df(outfile)
    plot_dep_energy_histogram(df, df_primaries, title=outfile, cal=False)
    # plot_cdf("./dat/6HeDecay_cdf.txt")
    print('end')

