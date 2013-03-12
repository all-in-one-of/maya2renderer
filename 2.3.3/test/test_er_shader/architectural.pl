#shaderType
surface
#shaderName
architectural
#Name|Details|Type|IsOutput|Defaults|ArraySizes
surface_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1

diffuse_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
diffuse_weight|uniform|float|0|1.0|-1

specular_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
specular_weight|uniform|float|0|0.2|-1

roughness|uniform|float|0|0.0|-1
specular_mode|uniform|int|0|0|-1
glossiness|uniform|float|0|1.0|-1

reflection_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
reflection_weight|uniform|float|0|0.0|-1

refraction_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
refraction_weight|uniform|float|0|0.0|-1
refraction_glossiness|uniform|float|0|0.0|-1
refraction_thickness|uniform|float|0|0.0|-1

translucency_color|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
translucency_weight|uniform|float|0|0.0|-1

anisotropy|uniform|float|0|1.0|-1
rotation|uniform|float|0|0.0|-1
ior|uniform|float|0|1.5|-1

fresnel_by_ior|uniform|float|0|0.0|-1
fresnel_0_degree_refl|uniform|float|0|0.2|-1
fresnel_90_degree_refl|uniform|float|0|1.0|-1
fresnel_curve|uniform|float|0|5.0|-1

is_metal|uniform|int|0|1|-1
diffuse_samples|uniform|int|0|8|-1
reflection_samples|uniform|int|0|4|-1
refraction_samples|uniform|int|0|4|-1
cutoff_threshold|uniform|float|0|0.01|-1
#bump_shader|uniform|string|0|""|-1
bump_factor|uniform|float|0|0.3|-1
