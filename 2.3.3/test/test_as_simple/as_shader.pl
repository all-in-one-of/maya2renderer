#shaderType
surface
#shaderName
as_shader
#Name|Details|Type|IsOutput|Defaults|ArraySizes
bsdf_model|uniform|string|0|"lambert_brdf"|-1
lambert_brdf_reflectance|uniform|color|0|<<1.0,1.0,1.0>>|-1
edf_model|uniform|string|0|"diffuse_edf"|-1
diffuse_edf_exitance|uniform|color|0|<<1.0,1.0,1.0>>|-1

surface_shader_model|uniform|string|0|"constant_surface_shader"|-1
constant_surface_shader_color|uniform|color|0|<<1.0,1.0,1.0>>|-1