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
    ff.keys()
    tree = ff['simData']
    # tree.show()

    field_names = ['pid', 'eventid', 'trackid', 'parentid', 'volumeid', 
                'stepnumber', 'inenergy', 'kineticenergy', 
                'depenergy', 'x','y','z','px','py','pz']

    df = tree.arrays(field_names, library='pd')
    return df 

def plot_dep_energy_histogram(df, title='', cal=False):
    """
    Plots a histogram of deposited energy from the DataFrame for volumeids 6 (vaccuum), 3 (scint), 5 (trigger).

    Parameters:
        df (pd.DataFrame): DataFrame containing the simulation data.
        title (str): Title for the histogram plot.
    """
    plt.figure(figsize=(10, 6))
    # df = df[df['pid'] != 22]  # Filter out photons
    if cal == True:
        plt.hist(df[df['volumeid']==5]['depenergy'], bins=150, 
                 label='Trigger - Deposited Energy', alpha=1, color='mediumseagreen')
        plt.hist(df[df['volumeid']==3]['depenergy'], bins=150, 
                 label='Scintillator - Deposited Energy', alpha=1, color='royalblue')

    
    else:
        plt.hist(df[df['volumeid']==6]['inenergy'], bins=150, 
                 label='Primary Particle - Initial Energy', alpha=1, color='firebrick')
        plt.hist(df[df['volumeid']==3]['depenergy'], bins=150, 
                 label='Scintillator - Deposited Energy', alpha=1, color='royalblue')
        plt.hist(df[df['volumeid']==5]['depenergy'], bins=150, 
                 label='Trigger - Deposited Energy', alpha=1, color='mediumseagreen')
    
    plt.title(f'Histogram of Energy for Different Volumes\n{title}')
    plt.xlabel('Energy (MeV)')
    plt.ylabel('Frequency')
    plt.yscale('log')
    plt.grid(True)
    plt.legend()
    plt.grid(True)
    plt.show()

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
    outfile = "./build/output/outfile_90Sr_3e6.root"
    df = read_root_to_df(outfile)
    plot_dep_energy_histogram(df, title=outfile, cal=True)
    # plot_cdf("./dat/6HeDecay_cdf.txt")
    print('end')

