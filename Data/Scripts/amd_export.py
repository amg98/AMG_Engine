import bpy
import bmesh
import struct
import os

# WINDOWS = LITTLE ENDIAN

# Vertices
# Normales
# Caras
# Materiales
# Objetos
# Grupos de materiales
# Texturas normales
# Coordenadas de textura

# Huesos
# Animaciones

def triangulate_object(obj):
    me = obj.data
    # Get a BMesh representation
    bm = bmesh.new()
    bm.from_mesh(me)

    bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)
    #bmesh.ops.recalc_face_normals(bm, faces=bm.faces)

    # Finish up, write the bmesh back to the mesh
    bm.to_mesh(me)
    bm.free()
    
def buscaVertex(v0, t0, n0, vtx, txc, nrm):
    i=0
    while(i<len(vtx)):
        v1 = vtx[i]
        t1 = txc[i]
        n1 = nrm[i]
        if(v0[0] == v1[0] and v0[1] == v1[1] and v0[2] == v1[2] and n0[0] == n1[0] and n0[1] == n1[1] and n0[2] == n1[2] and t0[0] == t1[0] and t0[1] == t1[1]):
            return i
        i += 1
    return -1

def export(context, filepath, use_some_setting):
    f = open(filepath, 'wb')
    
    nobjects = 0
    for obj in bpy.data.objects:
        if(obj.type == "MESH"):
            nobjects += 1
    
    f.write(bytes("AMD", "utf-8"))
    
    f.write(struct.pack("@B", len(bpy.data.materials)))
    #f.write("NMaterials: " + str(len(bpy.data.materials)) + "\n");
    for mat in bpy.data.materials:
        #f.write(mat.name + "\n");
        #f.write("Diffuse: " + str(mat.diffuse_color) + " " + str(mat.diffuse_intensity) + " " + str(mat.alpha) + "\n");
        #f.write("Specular: " + str(mat.specular_color) + " " + str(mat.specular_intensity) + str(mat.specular_alpha) + "\n");
        #f.write("Ambient: " + str(mat.ambient) + "\n");
        f.write(struct.pack("@fffff", mat.diffuse_color[0], mat.diffuse_color[1], mat.diffuse_color[2], mat.diffuse_intensity, mat.alpha))
        f.write(struct.pack("@fffff", mat.specular_color[0], mat.specular_color[1], mat.specular_color[2], mat.specular_intensity, mat.specular_alpha))
        f.write(struct.pack("@f", mat.ambient))
        if(len(mat.texture_slots) > 0):
            #f.write("Texture: " + str(mat.texture_slots[0].texture.image.filepath) + "\n")
            texturepath = os.path.splitext(os.path.basename(mat.texture_slots[0].texture.image.filepath))[0]+'.dds'
            f.write(struct.pack("@B", len(texturepath)))
            f.write(bytes(texturepath, "utf-8"))
    
    objects = []
    for obj in bpy.data.objects:
        if(obj.type == "MESH"):
            triangulate_object(obj)
            #f.write(obj.name + "\n")
            vertices = []
            normals = []
            texcoords = []
            indices = []
            groups = []
            currentMaterial = -1
            
            for face in obj.data.polygons:
                for vert, loop in zip(face.vertices, face.loop_indices):
                    normal = obj.data.vertices[vert].normal
                    vertex = obj.data.vertices[vert].co
                    texcoord = (obj.data.uv_layers.active.data[loop].uv if obj.data.uv_layers.active is not None else (0.0, 0.0))
                    index = buscaVertex(vertex, texcoord, normal, vertices, texcoords, normals)
                    if(index == -1):    # Nuevo vertice
                        indices.append(len(vertices))
                        vertices.append(vertex)
                        texcoords.append([texcoord[0], 1-texcoord[1])
                        normals.append(normal)
                    else:               # Ya estaba en la lista
                        indices.append(index)
                if(face.material_index != currentMaterial):
                    if(len(groups) > 0):
                        groups[len(groups)-1][1] = len(indices)/3
                    groups.append([len(indices)/3-1, -1, face.material_index])
                    currentMaterial = face.material_index
            groups[len(groups)-1][1] = len(indices)/3
            objects.append([vertices, texcoords, normals, indices, groups])
    #f.write("NObjects: " + str(len(objects)) + "\n")
    f.write(struct.pack("@B", len(objects)))
    for o in objects:
        #f.write("NVertices: " + str(len(o[0]))+"\n")
        f.write(struct.pack("@H", len(o[0])))       # Numero de elementos
        for v in o[0]:
            #f.write("{" + str(v[0]) + ", " + str(v[1]) + ", " + str(v[2]) + "}\n")
            f.write(struct.pack("@fff", v[0], v[1], v[2]))
        #f.write("\n")
        #f.write("NTexcoords: " + str(len(o[1]))+"\n")
        for t in o[1]:
            #f.write("{" + str(t[0]) + ", " + str(t[1]) + "}\n")
            f.write(struct.pack("@ff", t[0], t[1]))
        #f.write("\n")
        #f.write("NNormals: " + str(len(o[2]))+"\n")
        for n in o[2]:
            #f.write("{" + str(n[0]) + ", " + str(n[1]) + ", " + str(n[2]) + "}\n")
            f.write(struct.pack("@fff", n[0], n[1], n[2]))
        #f.write("\n")
        #f.write("NTriangles: " + str(len(o[3])/3)+"\n")
        f.write(struct.pack("@H", len(o[3])))       # Tamaño del buffer de indices
        #newline = 0
        for i in indices:
            #f.write(str(i))
            f.write(struct.pack("@H", i))
            """
            newline += 1
            if(newline == 3):
                newline = 0
                f.write("\n")
            else:
                f.write(",")
        f.write("\n")
        """
        #f.write("NGroups: " + str(len(o[4]))+"\n")
        f.write(struct.pack("@B", len(o[4])))
        for g in o[4]:
            #f.write("From " + str(g[0]) + " to " + str(g[1]) + "\n")
            f.write(struct.pack("@HHH", int(g[0]), int(g[1]), int(g[2])))
        #f.write("\n")
        
    f.close()
    return {'FINISHED'}

bl_info = \
    {
        "name" : "AMD Export Script",
        "author" : "Andrés Martínez <andresmargar98@gmail.com>",
        "version" : (1, 0, 0),
        "blender" : (2, 7, 9),
        "location" : "File > Export",
        "description" :
            "Export scene to AMD file",
        "warning" : "",
        "wiki_url" : "",
        "tracker_url" : "",
        "category" : "Export",
    }

# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportAMD(Operator, ExportHelper):
    """Exports blender scene to AMD format"""
    bl_idname = "export.amd_scene"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export AMD"

    # ExportHelper mixin class uses this
    filename_ext = ".amd"

    filter_glob = StringProperty(
            default="*.txt",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    use_setting = BoolProperty(
            name="Example Boolean",
            description="Example Tooltip",
            default=True,
            )

    type = EnumProperty(
            name="Example Enum",
            description="Choose between two items",
            items=(('OPT_A', "First Option", "Description one"),
                   ('OPT_B', "Second Option", "Description two")),
            default='OPT_A',
            )

    def execute(self, context):
        return export(context, self.filepath, self.use_setting)


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportAMD.bl_idname, text="Export AMD")


def register():
    bpy.utils.register_class(ExportAMD)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportAMD)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export.amd_scene('INVOKE_DEFAULT')
