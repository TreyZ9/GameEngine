
import json
import bpy

supported_shapes = ['cube', 'sphere', 'cylinder', 'plane', 'cone']

json_data = {}
for obj in bpy.data.collections['Physics'].objects:
    json_data[obj.name] = {
        'position': [obj.location.x, obj.location.y, obj.location.z],
        'rotation': [obj.rotation_euler.x, obj.rotation_euler.y, obj.rotation_euler.z],
        'dimensions': [obj.dimensions.x, obj.dimensions.y, obj.dimensions.z]
    }

with open(bpy.path.abspath('//test.json'), 'w') as file:
    json.dump(json_data, file, indent=4, sort_keys=True)
