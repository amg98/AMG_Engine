# Imports
import bpy
import bmesh
import struct
import os
from mathutils import *

# Search a material and return its ID
def searchMaterialID(name):
    i = 0
    for m in bpy.data.materials:
        if(m.name == name):
            return i
        i += 1
    return -1

# Triangulate an object
def triangulate_object(obj):
    me = obj.data
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces[:], quad_method=0, ngon_method=0)
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

# Search a vertex in a buffer
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

# Search a bone
def buscaBone(bones, name):
    index = 0
    for b in bones:
        if b.name == name:
            return index
        index += 1
    return -1

# Get a local bone matrix (relative to its parent)
def getBoneMatrix(poseBone):
    if poseBone.parent is None:
        return poseBone.matrix_local
    else:
        return poseBone.parent.matrix_local.inverted() * poseBone.matrix_local

# Get a local pose bone matrix (relative to its parent)
def getPoseBoneMatrix(poseBone):
    if poseBone.parent is None:
        return poseBone.matrix
    else:
        return poseBone.parent.matrix.inverted() * poseBone.matrix

# The actual export script
def export(context, filepath, use_some_setting):
    
    # Open file
    f = open(filepath, 'wb')
    
    # Get number of objects
    nobjects = 0
    for obj in bpy.data.objects:
        if(obj.type == "MESH"):
            nobjects += 1
    
    # Write signature
    f.write(bytes("AMD", "utf-8"))
    
    # Write material data
    f.write(struct.pack("@B", len(bpy.data.materials)))
    print("NMaterials: " + str(len(bpy.data.materials)) + "\n");
    for mat in bpy.data.materials:
        print(mat.name + "\n");
        print("Diffuse: " + str(mat.diffuse_color) + " " + str(mat.diffuse_intensity) + " " + str(mat.alpha) + "\n");
        print("Specular: " + str(mat.specular_color) + " " + str(mat.specular_intensity) + str(mat.specular_alpha) + "\n");
        print("Ambient: " + str(mat.ambient) + "\n");
        f.write(struct.pack("@fffff", mat.diffuse_color[0], mat.diffuse_color[1], mat.diffuse_color[2], mat.diffuse_intensity, mat.alpha))
        f.write(struct.pack("@fffff", mat.specular_color[0], mat.specular_color[1], mat.specular_color[2], mat.specular_intensity, mat.specular_alpha))
        f.write(struct.pack("@f", mat.ambient))
        if(len(mat.texture_slots) > 0 and mat.texture_slots[0] != None):
            try:
                print("Texture: " + str(mat.texture_slots[0].texture.image.filepath) + "\n")
                texturepath = os.path.splitext(os.path.basename(mat.texture_slots[0].texture.image.filepath))[0]+'.dds'
                f.write(struct.pack("@B", len(texturepath)))
                f.write(bytes(texturepath, "utf-8"))
            except AttributeError:
                f.write(struct.pack("@B", 0))
        else:
            f.write(struct.pack("@B", 0))
    
    objects = []
    
    # Write object data
    for obj in bpy.data.objects:
        if(obj.type == "MESH"):
            triangulate_object(obj)
            
            print(obj.name + "\n")
            
            vertices = []
            normals = []
            texcoords = []
            indices = []
            groups = []
            weights = []
            weight_bones = []
            bones = None
            currentMaterial = -1
            
            # Get bone data
            armature = obj.find_armature()
            if armature != None:
                bones = armature.data.bones
            
            # Fill in vertices and indices buffers
            for face in obj.data.polygons:
                for vert, loop in zip(face.vertices, face.loop_indices):
                    normal = obj.data.vertices[vert].normal
                    vertex = obj.data.vertices[vert].co
                    texcoord = (obj.data.uv_layers.active.data[loop].uv if obj.data.uv_layers.active is not None else (0.0, 0.0))
                    
                    weight = []
                    weight_bonelist = []
                    
                    if len(obj.data.vertices[vert].groups) > 4:
                        print("Warning: " + str(len(obj.data.vertices[vert].groups)) + " weights per vertex is not allowed (max 4)")
                        return {'FINISHED'}
                    else:
                        for g in obj.data.vertices[vert].groups:
                            weight.append(g.weight)
                            weight_bonelist.append(g.group)
                        if len(weight) != 0:
                            while len(weight) != 4:
                                weight.append(0)
                                weight_bonelist.append(0)
                    index = buscaVertex(vertex, texcoord, normal, vertices, texcoords, normals)
                    if(index == -1):        # New vertex in our list
                        indices.append(len(vertices))
                        vertices.append(vertex)
                        texcoords.append([texcoord[0], 1-texcoord[1]])
                        
                        if len(weight) != 0:
                            weights.append(weight)
                            weight_bones.append(weight_bonelist)
                        
                        normals.append(normal)
                    else:               # It was in the list
                        indices.append(index)
                        
                # Don't forget material groups
                if(face.material_index != currentMaterial):
                    if(len(groups) > 0):
                        groups[len(groups)-1][1] = len(indices)/3
                    matid = searchMaterialID(obj.material_slots[face.material_index].material.name)
                    groups.append([len(indices)/3-1, -1, matid])
                    currentMaterial = face.material_index
            groups[len(groups)-1][1] = len(indices)/3
            objects.append([vertices, texcoords, normals, indices, groups, weights, weight_bones, bones, obj])
    print("NObjects: " + str(len(objects)) + "\n")
    f.write(struct.pack("@B", len(objects)))
    for o in objects:
        print("NVertices: " + str(len(o[0]))+"\n")
        f.write(struct.pack("@H", len(o[0])))       # Numero de elementos
        for v in o[0]:
            #print("{" + str(v[0]) + ", " + str(v[1]) + ", " + str(v[2]) + "}\n")
            f.write(struct.pack("@fff", v[0], v[1], v[2]))
        print("\n")
        print("NTexcoords: " + str(len(o[1]))+"\n")
        for t in o[1]:
            #print("{" + str(t[0]) + ", " + str(t[1]) + "}\n")
            f.write(struct.pack("@ff", t[0], t[1]))
        print("\n")
        print("NNormals: " + str(len(o[2]))+"\n")
        for n in o[2]:
            #print("{" + str(n[0]) + ", " + str(n[1]) + ", " + str(n[2]) + "}\n")
            f.write(struct.pack("@fff", n[0], n[1], n[2]))
        print("\n")
        print("NTriangles: " + str(len(o[3])/3)+"\n")
        f.write(struct.pack("@H", len(o[3])))       # Tamaño del buffer de indices
        #newline = 0
        for i in o[3]:
            #f.write(str(i))
            f.write(struct.pack("@H", i))
        print("NGroups: " + str(len(o[4]))+"\n")
        f.write(struct.pack("@B", len(o[4])))
        for g in o[4]:
            print("From " + str(g[0]) + " to " + str(g[1]) + " (mat " + str(g[2]) + ")\n")
            f.write(struct.pack("@HHH", int(g[0]), int(g[1]), int(g[2])))
        print("\n")
        
        # Print Bounding Box
        min = o[8].bound_box[0]
        max = o[8].bound_box[6]
        delta = Vector((max[0]+min[0], max[1]+min[1], max[2]+min[2])) 
        if(delta.length > 8e-6):
            print("Warning: Bounding box is not centered!")
            print(delta)
        f.write(struct.pack("@fff", max[0], max[1], max[2]))
        
        # Print position, rotation and scale
        f.write(struct.pack("@fff", o[8].location.x, o[8].location.y, o[8].location.z))
        f.write(struct.pack("@ffff", o[8].rotation_quaternion.x, o[8].rotation_quaternion.y, o[8].rotation_quaternion.z, o[8].rotation_quaternion.w))
        f.write(struct.pack("@fff", o[8].scale.x, o[8].scale.y, o[8].scale.z))
        
        # If we have an armature
        if o[7] != None:
            print("NBones: " + str(len(o[7])))
            f.write(struct.pack("@B", len(o[7])))
            for b in o[7]:
                # Parent bone
                if b.parent != None:
                    p = buscaBone(o[7], b.parent.name)
                    f.write(struct.pack("@H", p))
                    #print("P: " + str(p))
                else:
                    f.write(struct.pack("@H", 0xffff))
                    #print("P: " + str(0xffff))
                # Children bones
                f.write(struct.pack("@H", len(b.children)))
                #print("NC: " + str(len(b.children)))
                for c in b.children:
                    id = buscaBone(o[7], c.name)
                    f.write(struct.pack("@H", id))
                    #print("C: " + str(id))
                
                # Get bone matrix
                m = getBoneMatrix(b)
                mi = b.matrix_local.inverted()
                for i in range(0,4):
                    for j in range(0,4):
                        f.write(struct.pack("@f", m[j][i])) # Izquierda->Derecha, Arriba->Abajo
                for i in range(0,4):
                    for j in range(0,4):
                        f.write(struct.pack("@f", mi[j][i])) # Izquierda->Derecha, Arriba->Abajo
                
            for w in o[5]:
                f.write(struct.pack("@ffff", w[0], w[1], w[2], w[3]))
            for wb in o[6]:
                f.write(struct.pack("@HHHH", int(wb[0]), int(wb[1]), int(wb[2]), int(wb[2])))
                #print(wb[0], wb[1], wb[2])
        else:
            f.write(struct.pack("@B", 0))
    
    # Get number of animations
    nactions = 0
    for a in bpy.data.actions:
        if a.id_root == 'OBJECT':
            nactions += 1
    
    # Only 1 object is supported using animations, and it must have a skeleton
    # If you are using a non-skeleton animated object, make a skeleton of a single bone with
    # weight 1 on all vertices, then animate the bone
    if(nobjects > 1 and nactions > 0):
        f.write(struct.pack("@B", 0))
        print("Multiobject actions are unsupported, export correct")
        f.close()
        return {'FINISHED'}
    
    if(nactions == 0):
        f.write(struct.pack("@B", 0))
        f.close()
        print("Export correct")
        return {'FINISHED'}

    ob = None
    for o in bpy.data.objects:
        if o.type == "MESH":
            ob = o.find_armature()
            if(ob == None):
                print("No armature found for animated object")
                f.close()
                return {"FINISHED"}
            bpy.context.scene.objects.active = ob
            break
    
    # Write animation data
    f.write(struct.pack("@B", nactions))                        # Number of animations
    f.write(struct.pack("@B", bpy.context.scene.render.fps))    # Frames per second
    for action in bpy.data.actions:
        if a.id_root == 'OBJECT':
            ob.animation_data.action = action
            bpy.ops.object.mode_set(mode="POSE")
            bpy.ops.pose.select_all(action="SELECT")
            bpy.ops.pose.transforms_clear()
        
            # Get all frame numbers in this animation
            frames = dict()
            for fcurve in action.fcurves:
                for key in fcurve.keyframe_points:
                    frame = key.co.x
                    frames[frame] = 1
            
            f.write(struct.pack("@H", len(frames)))
            for fr in sorted(frames):
                bpy.context.scene.frame_set(fr)             # Frame number
                bpy.context.scene.update()
                f.write(struct.pack("@f", fr))
                for bone in ob.pose.bones:                  # Frame matrix for each bone
                    m = getPoseBoneMatrix(bone)
                    #print(m)
                    pos = m.to_translation()
                    rot = m.to_quaternion()
                    f.write(struct.pack("@fff", pos[0], pos[1], pos[2]))
                    f.write(struct.pack("@ffff", rot.x, rot.y, rot.z, rot.w))
                #print("\n")
            
    f.close()
    print("Export correct")
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