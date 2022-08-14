;//--------------------------------------------------------------------------------
;// Tyra's standard macros library
;//--------------------------------------------------------------------------------

;//---------------------------------------------------------
;// LoadTyraStaticData - Load "Set" gif tag
;//---------------------------------------------------------
#macro LoadTyraStaticData: t_gifSetTagName
   lq             t_gifSetTagName, VU1_SET_GIFTAG_ADDR(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadTyraLightMatrix - Loads single color. Color is placed in 4th slot of Lights matrix
;//---------------------------------------------------------
#macro LoadTyraSingleColor: t_singleColor, t_singleColorEnabled, t_singleColorAddr, t_optionsAddr
   lq       t_singleColor,          t_singleColorAddr(vi00)
   ilw.x    t_singleColorEnabled,   t_optionsAddr(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadTyraLerpValue - Loads interpolation value.
;//---------------------------------------------------------
#macro LoadTyraLerpValue: t_lerpValue, t_optionsAddr
   lq.y  t_lerpValue, t_optionsAddr(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadTyraScaleValue - Loads screen scales.
;//---------------------------------------------------------
#macro LoadTyraScaleValue: t_scale, t_buffer
   lq.xyz  t_scale,  0(t_buffer)
#endmacro

;//---------------------------------------------------------
;// LoadTyraTags - Load lod, texture buffer and clut
;// 1 - GIF tag - texture LOD
;// 2 - GIF tag - Z buffer tests method
;// 3 - GIF tag - texture buffer & CLUT
;//---------------------------------------------------------
#macro LoadTyraTagsTexture: t_lodGifTag, t_testsGifTag, t_texBufferClutGifTag, t_lodAddr, t_testsAddr, t_ClutAddr
   lq      t_lodGifTag,             t_lodAddr(vi00)
   lq      t_testsGifTag,           t_testsAddr(vi00)
   lq      t_texBufferClutGifTag,   t_ClutAddr(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadTyraTags - Load lod, texture buffer and clut
;// 1 - GIF tag - texture LOD
;// 2 - GIF tag - Z buffer tests method
;//---------------------------------------------------------
#macro LoadTyraTags: t_lodGifTag, t_testsGifTag, t_lodAddr, t_testsAddr
   lq      t_lodGifTag,             t_lodAddr(vi00)
   lq      t_testsGifTag,           t_testsAddr(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadTyraPrimTag - Load prim tag
;// 2 - GIF tag - tell GS how many data we will send
;//---------------------------------------------------------
#macro LoadTyraPrimTag: t_primTag, t_buffer
   lq      t_primTag,   1(t_buffer)
#endmacro

;//---------------------------------------------------------
;// LoadTyraBufferTags - Load scales and prim tag
;// 1 - float : X, Y, Z - scale vector that we will use to scale the verts after projecting them, float : W - vert count.
;// 2 - GIF tag - tell GS how many data we will send
;//---------------------------------------------------------
#macro LoadTyraBufferTags: t_scale, t_primTag, t_buffer
   lq.xyz  t_scale,                  0(t_buffer)
   lq      t_primTag,                1(t_buffer)
#endmacro

;//---------------------------------------------------------
;// LoadTyraDirectionalLights - Load directions and colors. All are always present
;// - 3 directional lights directions
;// - 3 directional lights colors + ambient color
;//---------------------------------------------------------
#macro LoadTyraDirectionalLights: t_lightMatrix, t_lightDirections, t_lightsColors, t_ambientColor, t_dirOffset, t_colorOffset, t_matrixOffset
   lq.xyz      t_lightMatrix[0],       t_matrixOffset+0(vi00)
   lq.xyz      t_lightMatrix[1],       t_matrixOffset+1(vi00)
   lq.xyz      t_lightMatrix[2],       t_matrixOffset+2(vi00)
   lq.xyz      t_lightDirections[0],   t_dirOffset(vi00)
   lq.xyz      t_lightDirections[1],   t_dirOffset+1(vi00)
   lq.xyz      t_lightDirections[2],   t_dirOffset+2(vi00)
   lq.xyz      t_lightsColors[0],      t_colorOffset(vi00)
   lq.xyz      t_lightsColors[1],      t_colorOffset+1(vi00)
   lq.xyz      t_lightsColors[2],      t_colorOffset+2(vi00)
   lq.xyz      t_ambientColor,         t_colorOffset+3(vi00)
#endmacro

;//---------------------------------------------------------
;// StoreTyraGifTagsTexture - Store gif tags. 
;// Not using sqi instruction, because VCL cannot optimize it.
;// Primtag contains information about how many polys we will send
;//---------------------------------------------------------
#macro StoreTyraGifTagsTexture: t_gifSetTag, t_lodGifTag, t_texBufferClutGifTag, t_primTag, t_testsTag, t_destAddress
   sq t_gifSetTag,            0(t_destAddress)
   sq t_testsTag,             1(t_destAddress)
   sq t_gifSetTag,            2(t_destAddress)
   sq t_lodGifTag,            3(t_destAddress)
   sq t_gifSetTag,            4(t_destAddress)
   sq t_texBufferClutGifTag,  5(t_destAddress)
   sq t_primTag,              6(t_destAddress)
   iaddiu                     t_destAddress,    t_destAddress,    7
#endmacro

;//---------------------------------------------------------
;// StoreTyraGifTags - Store gif tags. 
;// Not using sqi instruction, because VCL cannot optimize it.
;// Primtag contains information about how many polys we will send
;//---------------------------------------------------------
#macro StoreTyraGifTags: t_gifSetTag, t_lodGifTag, t_primTag, t_testsTag, t_destAddress
   sq t_gifSetTag,            0(t_destAddress)
   sq t_testsTag,             1(t_destAddress)
   sq t_gifSetTag,            2(t_destAddress)
   sq t_lodGifTag,            3(t_destAddress)
   sq t_primTag,              4(t_destAddress)
   iaddiu                     t_destAddress,    t_destAddress,    5
#endmacro

;//---------------------------------------------------------
;// CalculateLights - Based on Dr Fortuna's work
;//
;// 1. Transform by the rotation part of the world matrix
;// 2. "Transform" the normal by the light direction matrix
;// 3. Four intensities, one for each light.
;// 4. Clamp the intensity to 0..1
;// 5. Transform the intensities by the light colour matrix
;// 6. Load 128 and put it into the alpha value
;// 7. Clamp result to 0-128 values
;// 8. And write to the output buffer
;//---------------------------------------------------------
#macro CalculateTyraDirectionalLights: t_outputColor, t_normal, t_lightDirections, t_lightColors, t_lightMatrix, t_ambientColor
   mul.xyz     acc,              t_lightMatrix[0],       t_normal[x]	
   madd.xyz    acc,              t_lightMatrix[1],       t_normal[y]
   madd.xyz    t_normal,         t_lightMatrix[2],       t_normal[z]
   mula.xyz    acc,              t_lightDirections[0],   t_normal[x]
   madd.xyz    acc,              t_lightDirections[1],   t_normal[y]
   madd.xyz    t_outputColor,    t_lightDirections[2],   t_normal[z]
   mini.xyz    t_outputColor,    t_outputColor,          vf00[w]
   max.xyz     t_outputColor,    t_outputColor,          vf00[x]
   mula.xyz    acc,              t_lightColors[0],       t_outputColor[x]
   madda.xyz   acc,              t_lightColors[1],       t_outputColor[y]
   madda.xyz   acc,              t_lightColors[2],       t_outputColor[z]
   madd.xyz    t_outputColor,    t_ambientColor,         vf00[w]
   loi         128
	addi.w      t_outputColor,    vf00,    i
#endmacro

;//---------------------------------------------------------
;// LerpXYZ - Linear interpolation between two points
;//---------------------------------------------------------
#macro LerpXYZ: t_output, t_from, t_to, t_interp
   sub.xyz   temp1,      t_to,    t_from
   mul.xyz   temp2,      temp1,   t_interp[y]
   add.xyz   t_output,   temp2,   t_from
#endmacro

;//---------------------------------------------------------
;// Lerp - Linear interpolation between two points
;//---------------------------------------------------------
#macro Lerp: t_output, t_from, t_to, t_interp
   sub   temp1,      t_to,    t_from
   mul   temp2,      temp1,   t_interp[y]
   add   t_output,   temp2,   t_from
#endmacro