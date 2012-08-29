#shaderType
bsdf_mix
#shaderName
bsdf_mix0
#Name|Details|Type|IsOutput|Defaults|ArraySizes
bsdf0|uniform|color|0|<<1.0,1.0,1.0>>|-1
bsdf1|uniform|color|0|<<1.0,1.0,1.0>>|-1
weight0|uniform|float|0|0.5|-1
weight1|uniform|float|0|0.5|-1
#outPlugForLiquid is used to connected two bsdf_mix nodes, e.g. bsdf_mix0.outPlugForLiquid --> bsdf_mix1.bsdf0
outPlugForLiquid|uniform|color|0|<<1.0,1.0,1.0>>|-1