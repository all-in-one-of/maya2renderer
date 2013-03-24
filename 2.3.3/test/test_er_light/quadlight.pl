#shaderType
light
#shaderName
quadlight
#Name|Details|Type|IsOutput|Defaults|ArraySizes
intensity|uniform|float|0|1.0|-1
lightcolor|uniform|color|0|<<1.000000,1.000000,1.000000>>|-1
#deltaangle = eiPI / 2.0
deltaangle|uniform|float|0|1.5708|-1
direction|uniform|vector|0|<<0.000000,0.000000,-1.000000>>|-1
#spread = eiPI / 4.0f
spread|uniform|float|0|0.785398|-1
width|uniform|float|0|1.0|-1
height|uniform|float|0|1.0|-1
