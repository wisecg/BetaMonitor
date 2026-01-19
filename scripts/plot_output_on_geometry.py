from plot_geometry import load_gdml, plot_by_volume, add_vtk_point
import numpy as np
import uproot

ff = uproot.open("build/output/outfile_6He_trans2fix.root")
ff.keys()
tree = ff['simData']

def add_points_for_det(det, reg, tree):
    det_dict = {'detVac': 'Envelopedet', 'detSQ': 'Detector_sq1', 'detTrig': 'Detector_sq2', 
                'detWind':'Copper'}
    v = plot_by_volume(reg, vol_select=det_dict[det])
    Inx = tree.arrays([f'{det}_Inx'], library='np')[f'{det}_Inx']
    Iny = tree.arrays([f'{det}_Iny'], library='np')[f'{det}_Iny']
    Inz = tree.arrays([f'{det}_Inz'], library='np')[f'{det}_Inz']
    Inx = Inx[Inx != 999]  # remove untriggered events
    Iny = Iny[Iny != 999]
    Inz = Inz[Inz != 999]
    for i in range(len(Inx)):
        add_vtk_point(v, np.array([Inx[i], Iny[i], Inz[i]]), color=(0,1,0), radius=0.5)
    
    return v

if __name__ == "__main__":
    gdml_path =r'build/geometry_export.gdml'
    reg = load_gdml(gdml_path)
    v = add_points_for_det('detTrig', reg, tree)
    v.view()
    print('end')
