#shaderType
surface
#shaderName
as_shader
#Name|Details|Type|IsOutput|Defaults|ArraySizes
bsdf_model|uniform|string|0|"specular_btdf"|-1
specular_btdf_reflectance|uniform|color|0|<<1.0,1.0,1.0>>|-1
specular_btdf_reflectance_multiplier|uniform|float|0|1.0|-1
specular_btdf_transmittance|uniform|color|0|<<1.0,1.0,1.0>>|-1
specular_btdf_transmittance_multiplier|uniform|float|0|1.0|-1
specular_btdf_from_ior|uniform|float|0|1.0|-1
specular_btdf_to_ior|uniform|float|0|1.5|-1

edf_model|uniform|string|0|"diffuse_edf"|-1
diffuse_edf_exitance|uniform|color|0|<<1.0,1.0,1.0>>|-1
diffuse_edf_exitance_multiplier|uniform|float|0|1.0|-1

surface_shader_model|uniform|string|0|"physical_surface_shader"|-1
physical_surface_shader_color_multiplier|uniform|float|0|1.0|-1
physical_surface_shader_alpha_multiplier|uniform|float|0|1.0|-1
physical_surface_shader_aerial_persp_mode|uniform|string|0|"none"|-1
#physical_surface_shader_aerial_persp_mode|uniform|string|0|"environment_shader"|-1
#physical_surface_shader_aerial_persp_mode|uniform|string|0|"sky_color"|-1
physical_surface_shader_aerial_persp_sky_color|uniform|color|0|<<1.0,1.0,1.0>>|-1
physical_surface_shader_aerial_persp_distance|uniform|float|0|1000.0|-1
physical_surface_shader_aerial_persp_intensity|uniform|float|0|0.01|-1