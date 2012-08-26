#shaderType
surface
#shaderName
as_shader
#Name|Details|Type|IsOutput|Defaults|ArraySizes
bsdf_model|uniform|string|0|"lambertian_brdf"|-1
lambertian_brdf_reflectance|uniform|color|0|<<1.0,1.0,1.0>>|-1
lambertian_brdf_reflectance_multiplier|uniform|color|0|<<1.0,1.0,1.0>>|-1

edf_model|uniform|string|0|"diffuse_edf"|-1
diffuse_edf_exitance|uniform|color|0|<<1.0,1.0,1.0>>|-1
diffuse_edf_exitance_multiplier|uniform|color|0|<<1.0,1.0,1.0>>|-1

surface_shader_model|uniform|string|0|"ao_surface_shader"|-1
ao_surface_shader_sampling_method|uniform|string|0|"uniform"|-1
#ao_surface_shader_sampling_method|uniform|string|0|"cosine"|-1
ao_surface_shader_samples|uniform|float|0|16|-1
ao_surface_shader_max_distance|uniform|float|0|1.0|-1

