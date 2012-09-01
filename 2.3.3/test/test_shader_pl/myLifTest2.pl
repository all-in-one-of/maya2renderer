#shaderType
surface
#shaderName
myLifTest2ShaderName
#Name|Details|Type|IsOutput|Defaults|ArraySizes
intensity|uniform|float|0|1.0|-1
lightcolor|uniform|color|0|<<1.0,1.0,1.0>>|-1
decay|uniform|float|0|0.0|-1

shadownamepx|uniform|string|0|""|-1
shadownamenx|uniform|string|0|""|-1
shadownamepy|uniform|string|0|""|-1
shadownameny|uniform|string|0|""|-1
shadownamepz|uniform|string|0|""|-1
shadownamenz|uniform|string|0|""|-1

shadowbias|uniform|float|0|0.01|-1
shadowblur|uniform|float|0|0.0|-1
shadowsamples|uniform|float|0|16|-1
shadowfiltersize|uniform|float|0|1|-1
shadowcolor|uniform|color|0|<<0.0,0.0,0.0>>|-1

lightID|uniform|float|0|0|-1
__category|uniform|string|0|""|-1

__shadowF|varying|float|1|0.0|-1
__shadowC|varying|float|1|1.0|-1
__unshadowed_Cl|varying|float|1|0.0|-1
__nondiffuse|varying|float|1|0.0|-1
__nonspecular|varying|float|1|0.0|-1
