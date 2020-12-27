; Load GS offsets to center xformed vertex in gs coord space and color clamping constant
#macro LoadScaleConstant: gs_scale
    loi			2047.5
    addi.xy		gs_scale, vf00, i
    loi         699050.625000 // ((float)0xFFFFFF) / 24.0F
    addi.z		gs_scale, vf00, i
#endmacro
