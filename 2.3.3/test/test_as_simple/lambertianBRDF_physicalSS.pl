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

surface_shader_model|uniform|string|0|"physical_surface_shader"|-1
physical_surface_shader_color_multiplier|uniform|float|0|1.0|-1
physical_surface_shader_alpha_multiplier|uniform|float|0|1.0|-1
physical_surface_shader_aerial_persp_mode|uniform|string|0|"none"|-1
#physical_surface_shader_aerial_persp_mode|uniform|string|0|"environment_shader"|-1
#physical_surface_shader_aerial_persp_mode|uniform|string|0|"sky_color"|-1
physical_surface_shader_aerial_persp_sky_color|uniform|color|0|<<1.0,1.0,1.0>>|-1
physical_surface_shader_aerial_persp_distance|uniform|float|0|1000.0|-1
physical_surface_shader_aerial_persp_intensity|uniform|float|0|0.01|-1