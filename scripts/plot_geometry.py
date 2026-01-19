

import pyg4ometry as pg4
import vtk
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.image as mpimg
import copy

def add_vtk_text(viewer, text, pos=(0.05, 0.9), font_size=24, color=(1, 1, 1)):
    """
    Add a text box annotation to the VTK viewer window using normalized viewport coordinates.
    pos: (x, y) in normalized viewport coordinates (0-1)
    color: RGB tuple (0-1)
    """
    text_actor = vtk.vtkTextActor()
    text_actor.SetInput(text)
    text_prop = text_actor.GetTextProperty()
    text_prop.SetFontSize(font_size)
    text_prop.SetColor(*color)
    # Use normalized viewport coordinates (0-1)
    text_actor.GetPositionCoordinate().SetCoordinateSystemToNormalizedViewport()
    text_actor.SetPosition(pos[0], pos[1])
    viewer.ren.AddActor2D(text_actor)

def add_vtk_point(viewer, position, color=(1, 0, 0), radius=2.0):
    """
    Add a 3D point marker (sphere) to the VTK viewer.
    position: (x, y, z) in mm (or your geometry units)
    color: RGB tuple (0-1)
    radius: sphere radius
    """
    import vtk
    sphere = vtk.vtkSphereSource()
    sphere.SetCenter(*position)
    sphere.SetRadius(radius)
    sphere.SetThetaResolution(16)
    sphere.SetPhiResolution(16)
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputConnection(sphere.GetOutputPort())
    actor = vtk.vtkActor()
    actor.SetMapper(mapper)
    actor.GetProperty().SetColor(*color)
    viewer.ren.AddActor(actor)

def add_vtk_axes(viewer, origin=(0, 0, 0), length=50, shaft_radius=0.01, tip_length=0.1, tip_radius=0.05):
    """
    Add X (red), Y (green), Z (blue) arrows as a coordinate system to the VTK viewer.
    origin: (x, y, z) tuple for the base of the axes
    length: length of each axis arrow
    """
    import vtk
    axes = [
        ((1, 0, 0), (1, 0, 0)),  # X axis, red
        ((0, 1, 0), (0, 1, 0)),  # Y axis, green
        ((0, 0, 1), (0, 0, 1)),  # Z axis, blue
    ]
    for direction, color in axes:
        arrow = vtk.vtkArrowSource()
        arrow.SetShaftRadius(shaft_radius)
        arrow.SetTipLength(tip_length)
        arrow.SetTipRadius(tip_radius)
        # Transform arrow to correct direction and position
        transform = vtk.vtkTransform()
        # Default arrow points in +X, so rotate as needed
        if direction == (0, 1, 0):  # Y
            transform.RotateZ(90)
        elif direction == (0, 0, 1):  # Z
            transform.RotateY(-90)
        transform.Translate(origin)
        transform.Scale(length, length, length)
        transform_filter = vtk.vtkTransformPolyDataFilter()
        transform_filter.SetTransform(transform)
        transform_filter.SetInputConnection(arrow.GetOutputPort())
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputConnection(transform_filter.GetOutputPort())
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)
        actor.GetProperty().SetColor(*color)
        viewer.ren.AddActor(actor)

def plot_with_custom_colors_by_material(reg):
    colors = [
        [1.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 0.0, 1.0],
        [1.0, 1.0, 0.0],
        [1.0, 0.0, 1.0],
        [0.0, 1.0, 1.0],
        [0.5, 0.5, 0.5],
        [1.0, 0.5, 0.0],
        [0.5, 0.0, 1.0],
        [0.0, 0.5, 1.0],
    ]
    v = pg4.visualisation.VtkViewer()
    # v = pg4.visualisation.VtkViewerColouredMaterial()
    material_dict = reg.materialDict
    n = 0
    legend_entries = []
    for _, mat in material_dict.items():
        if mat.type == 'composite':
            mat_name = mat.name.split('0x')[0]
            mat_vis = pg4.visualisation.VisualisationOptions()
            mat_vis.colour = colors[n]
            mat_vis.alpha = 0.2
            v.addMaterialVisOption(mat_name , mat_vis)
            print('Added material vis for ',  mat_name)
            legend_entries.append((mat_name, colors[n]))
            n += 1
        else:
            print('Skipped material ', mat.name, ' of type ', mat.type)


    # Example: To add only a single logical volume and force solid rendering
    # Replace 'SourceMy' with your desired volume name
    # selected_vol = reg.logicalVolumeDict['SourceMy']
    # mat_name = selected_vol.material.name.split('0x')[0]
    # mat_vis = pg4.visualisation.VisualisationOptions()
    # mat_vis.colour = [1.0, 0.0, 0.0]  # Red, for example
    # mat_vis.alpha = 0.8  # Mostly solid
    # v.addMaterialVisOption(mat_name, mat_vis)
    # v.addLogicalVolume(selected_vol)

    v.addLogicalVolume(reg.getWorldVolume())  # Default: add all geometry



    # Create a legend for the materials
    fig, ax = plt.subplots()
    patches = [mpatches.Patch(color=color, label=mat_name) for mat_name, color in legend_entries]
    ax.legend(handles=patches, loc='center')
    ax.axis('off')
    plt.show(block=False)

    # Example: add a text box annotation
    # add_vtk_text(v, "Hello VTK!", pos=(0.05, 0.9), font_size=24, color=(1,1,0))
    # v.view()
    return v
    print('Done')

def plot_by_volume(reg, vol_select=None):
    colors = [
        [1.0, 0.0, 0.0],
        [0.0, 1.0, 0.0],
        [0.0, 0.0, 1.0],
        [1.0, 1.0, 0.0],
        [1.0, 0.0, 1.0],
        [0.0, 1.0, 1.0],
        [0.5, 0.5, 0.5],
        [1.0, 0.5, 0.0],
        [0.5, 0.0, 1.0],
        [0.0, 0.5, 1.0],
        [0.5, 1.0, 0.0],
        [0.0, 1.0, 0.5],
        [1.0, 0.0, 0.5],
        [0.5, 0.0, 0.0],
        [0.0, 0.5, 0.0],
        [0.0, 0.0, 0.5],        
    ]

    v = pg4.visualisation.VtkViewer()
    # v = pg4.visualisation.VtkViewerColouredMaterial()
    volumes = reg.logicalVolumeDict
    n = 0
    legend_entries = []
    for _, vol in volumes.items():
        vol_name = vol.name.split('0x')[0]
        mat_name = vol.material.name.split('0x')[0]
        mat_vis = pg4.visualisation.VisualisationOptions()
        if vol_select:
            if vol_name == vol_select:
                print('Selected volume ', vol_name)
                original_mat = vol.material
                unique_mat = copy.copy(vol.material)
                unique_mat.name = "UniqueMat"
                vol.material = unique_mat
                legend_entries.append((vol_name, [1.0, 0.0, 0.0]))
                mat_vis.colour = [1.0, 0.0, 0.0]
                mat_vis.alpha = 0.5
                v.addMaterialVisOption("UniqueMat", mat_vis)
            # else:
            else:
                mat_vis.colour = [0.8, 0.8, 0.8]
                mat_vis.alpha = 0.1
                v.addMaterialVisOption(mat_name , mat_vis)

                # set material back to original
        else:
            mat_vis.colour = colors[n]
            mat_vis.alpha = 0.3
            v.addMaterialVisOption(mat_name , mat_vis)
            print('Added volume vis for ',  vol_name)
            legend_entries.append((vol_name, colors[n]))
            n += 1
            if n >= len(colors):
                n = 0

    add_vtk_text(v, f'{vol_select}', pos=(0.05, 0.9), font_size=48, color=(1,0,0))
    # add_vtk_point(v, (0, 0, 0), color=(0, 0, 0), radius=5.0)  # Black point at origin
    v.addLogicalVolume(reg.getWorldVolume())
    for _, vol in volumes.items():
        vol_name = vol.name.split('0x')[0]
        if vol_name == vol_select:
            vol.material = original_mat


    # Create a legend for the materials
    # fig, ax = plt.subplots()
    # patches = [mpatches.Patch(color=color, label=mat_name) for mat_name, color in legend_entries]
    # ax.legend(handles=patches, loc='center')
    # # save_three_views(v, base_filename="geometry_views")
    # ax.axis('off')
    # plt.show(block=False)

    # v.view()
    return v 
    print('Done')

def save_three_views(viewer, base_filename="view"):
    """
    Set camera for XY, XZ, YZ views and save screenshots from the VTK viewer.
    """
    # XY view (looking along +Z)
    viewer.ren.GetActiveCamera().SetPosition(0, 0, 1000)
    viewer.ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
    viewer.renWin.Render()
    viewer.exportScreenShot(f"{base_filename}_xy.png")

    # XZ view (looking along +Y)
    viewer.ren.GetActiveCamera().SetPosition(0, 1000, 0)
    viewer.ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
    viewer.renWin.Render()
    viewer.exportScreenShot(f"{base_filename}_xz.png")

    # YZ view (looking along +X)
    viewer.ren.GetActiveCamera().SetPosition(1000, 0, 0)
    viewer.ren.GetActiveCamera().SetFocalPoint(0, 0, 0)
    viewer.renWin.Render()
    viewer.exportScreenShot(f"{base_filename}_yz.png")
    combine_views_to_png(f"{base_filename}_xy.png",
                         f"{base_filename}_xz.png",
                         f"{base_filename}_yz.png",
                         out_path=f"{base_filename}_three_views.png")


def combine_views_to_png(xy_path, xz_path, yz_path, out_path="three_views.png"):
    # Load images
    xy_img = mpimg.imread(xy_path)
    xz_img = mpimg.imread(xz_path)
    yz_img = mpimg.imread(yz_path)

    # Create a figure with three subplots
    fig, axes = plt.subplots(1, 3, figsize=(15, 5))
    axes[0].imshow(xy_img)
    axes[0].set_title("XY View")
    axes[0].axis('off')
    axes[1].imshow(xz_img)
    axes[1].set_title("XZ View")
    axes[1].axis('off')
    axes[2].imshow(yz_img)
    axes[2].set_title("YZ View")
    axes[2].axis('off')

    plt.tight_layout()
    plt.show(block=False)
    # plt.savefig(out_path)
    # plt.close(fig)

# Example usage after viewer is populated:
# save_three_views(v)
def print_vol_names(reg):
    volumes = reg.logicalVolumeDict
    vol_list = []
    for _, vol in volumes.items():
        vol_name = vol.name.split('0x')[0]
        mat_name = vol.material.name.split('0x')[0]
        print(f'{vol_name}; (Material: {mat_name})')
        vol_list.append(vol_name)
    return vol_list

def load_gdml(gdml_path):
    gdml_parser = pg4.gdml.Reader(gdml_path)
    reg = gdml_parser.getRegistry()
    return reg

if __name__ == "__main__":
    # Load the GDML file
    gdml_parser = pg4.gdml.Reader(r'build/geometry_export.gdml')
    reg = gdml_parser.getRegistry()
    vol_list = print_vol_names(reg)
    # v = plot_by_volume(reg, vol_select="SourceMy")
    # v.view()

    # for vol in vol_list:
    #     v = plot_by_volume(reg, vol_select=vol)
        # v.view()
    #     print(' ')
    # v = plot_with_custom_colors_by_material(reg)
    v = plot_by_volume(reg, vol_select="Envelopedet")
    # add_vtk_point(v, (0, 0, 0), color=(0, 0, 0), radius=5.0)  # Black point at origin
    add_vtk_text(v, "Axes showing origin", pos=(0.7, 0.1), font_size=48, color=(0,0,0))
    add_vtk_axes(v, origin=(0, 0, 0), length=50)
    v.view()
    