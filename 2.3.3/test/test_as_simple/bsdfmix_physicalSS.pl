#shaderType
surface
#shaderName
as_shader
#Name|Details|Type|IsOutput|Defaults|ArraySizes
bsdf_model|uniform|string|0|"bsdf_mix"|-1
bsdf_mix_bsdf0|uniform|string|0|""|-1
bsdf_mix_bsdf1|uniform|string|0|""|-1
bsdf_mix_weight0|uniform|float|0|0.5|-1
bsdf_mix_weight1|uniform|float|0|0.5|-1

edf_model|uniform|string|0|"diffuse_edf"|-1
diffuse_edf_exitance|uniform|color|0|<<1.0,1.0,1.0>>|-1

surface_shader_model|uniform|string|0|"physical_surface_shader"|-1