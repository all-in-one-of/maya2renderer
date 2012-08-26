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

surface_shader_model|uniform|string|0|"fast_sss_surface_shader"|-1
fast_sss_surface_shader_scale|uniform|float|0|1.0|-1
fast_sss_surface_shader_ambient_sss|uniform|float|0|0.0|-1
fast_sss_surface_shader_view_dep_sss|uniform|float|0|0.0|-1
fast_sss_surface_shader_diffuse|uniform|float|0|0.0|-1
fast_sss_surface_shader_power|uniform|float|0|1.0|-1
fast_sss_surface_shader_distortion|uniform|float|0|0.0|-1
fast_sss_surface_shader_albedo|uniform|color|0|<<1.0,1.0,1.0>>|-1
fast_sss_surface_shader_light_samples|uniform|float|0|1.0|-1
fast_sss_surface_shader_occlusion_samples|uniform|float|0|1.0|-1