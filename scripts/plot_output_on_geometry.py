from plot_geometry import load_gdml, plot_by_volume, add_vtk_point
import numpy as np
import uproot
import pandas as pd



def cumulative_energy_and_entry_positions(df):
    energy_sum = df.groupby(['eventid', 'volumeid'])['depenergy'].sum().reset_index()

    idx = df.groupby(['eventid', 'volumeid'])['stepnumber'].idxmin()
    xyz_df = df.loc[idx, ['eventid', 'volumeid', 'x', 'y', 'z']]

    result = pd.merge(energy_sum, xyz_df, on=['eventid', 'volumeid'])
    return result

def add_points_for_det(det, reg, df):
    det_dict = {'vacuum': 'Envelopedet', 'scintillator': 'Detector_sq1', 'trigger': 'Detector_sq2', 
                'window':'Copper'}
    det_number = {'vacuum':6, 'scintillator':3, 'trigger':5, 'window':1}
    df = df[df['volumeid']==det_number[det]]
    v = plot_by_volume(reg, vol_select=det_dict[det])
    x = df['x'].to_numpy()
    y = df['y'].to_numpy()
    z = df['z'].to_numpy()


    # Inx = tree.arrays([f'{det}_Inx'], library='np')[f'{det}_Inx']
    # Iny = tree.arrays([f'{det}_Iny'], library='np')[f'{det}_Iny']
    # Inz = tree.arrays([f'{det}_Inz'], library='np')[f'{det}_Inz']
    # Inx = Inx[Inx != 999]  # remove untriggered events
    # Iny = Iny[Iny != 999]
    # Inz = Inz[Inz != 999]
    for i in range(len(x)):
        add_vtk_point(v, np.array([x[i], y[i], z[i]]), color=(0,1,0), radius=0.5)
    
    return v
    

if __name__ == "__main__":
    gdml_path =r'build/geometry_export.gdml'
    reg = load_gdml(gdml_path)

    ff = uproot.open("build/output/outfile_caltest.root")
    ff.keys()
    tree = ff['simData']

    field_names = ['pid', 'eventid', 'trackid', 'parentid', 'volumeid', 
                'stepnumber', 'inenergy', 'kineticenergy', 
                'depenergy', 'x','y','z','px','py','pz']

    df = tree.arrays(field_names, library='pd')
    result = cumulative_energy_and_entry_positions(df)

    # v = add_points_for_det('window', reg, result)
    # v.view()
    print('end')
