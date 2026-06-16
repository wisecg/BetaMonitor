import numpy as np
import os

def cdf_to_histogram_mev(cdf_file, hist_file):
    # Load CDF data (energies in eV)
    data = np.loadtxt(cdf_file)
    energies_eV = data[:, 0]
    cdf = data[:, 1]

    # Convert energies to MeV
    energies_MeV = energies_eV / 1e6

    # Calculate bin edges and densities
    bin_edges = energies_MeV
    bin_probs = np.diff(cdf)
    bin_centers = 0.5 * (bin_edges[:-1] + bin_edges[1:])
    bin_widths = np.diff(bin_edges)
    densities = bin_probs / bin_widths  # Probability density

    # Save as histogram: center (MeV), density
    np.savetxt(hist_file, np.column_stack([bin_centers, densities]), fmt="%.8f")

def histogram_to_macro(hist_file, macro_file):
    """
    Reads a histogram file (energy, probability) and writes a macro file with /gps/hist/point commands.
    """
    data = np.loadtxt(hist_file)
    with open(macro_file, 'w') as f:
        for energy, prob in data:
            f.write(f"/gps/hist/point {energy:.8f} {prob:.8f}\n")

# Example usage:
# cdf_to_histogram_mev("6HeDecay_cdf.txt", "6HeDecay_hist.txt")

if __name__ == "__main__":
    pwd = os.path.dirname(os.path.realpath(__file__))
    cdf_file = '/Users/harperumfress/dev/BetaMonitor/dat/19NeDecay_cdf.txt'
    hist_file = '/Users/harperumfress/dev/BetaMonitor/dat/19NeDecay_hist.txt'
    macro_file = '/Users/harperumfress/dev/BetaMonitor/dat/19NeDecay_hist_points.mac'
    cdf_to_histogram_mev(cdf_file, hist_file)
    histogram_to_macro(hist_file, macro_file)