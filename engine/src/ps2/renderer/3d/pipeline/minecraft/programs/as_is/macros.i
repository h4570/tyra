;//--------------------------------------------------------------------------------
;// MinecraftPipeline cull macros library
;//--------------------------------------------------------------------------------

;//---------------------------------------------------------
;// LoadStaticData - Load lod, set and viewproj matrix
;//---------------------------------------------------------
#macro LoadStaticData: t_lod, t_set
   lq       t_lod,            VU1_MCPIP_AS_IS_STATIC_LOD(vi00)
   lq       t_set,            VU1_MCPIP_AS_IS_STATIC_SET_TAG(vi00)
#endmacro

;//---------------------------------------------------------
;// LoadDynamicData - Load scale, prim and clut
;//---------------------------------------------------------
#macro LoadDynamicData: t_scale, t_prim, t_clut, t_color, t_vertexCount, t_buffer
   lq       t_scale,          VU1_MCPIP_AS_IS_DYNAMIC_SCALE(t_buffer)
   lq       t_prim,           VU1_MCPIP_AS_IS_DYNAMIC_PRIM(t_buffer)
   lq       t_clut,           VU1_MCPIP_AS_IS_DYNAMIC_CLUT(t_buffer)
   lq       t_color,          VU1_MCPIP_AS_IS_DYNAMIC_COLOR(t_buffer)
   ilw.w    t_vertexCount,    VU1_MCPIP_AS_IS_DYNAMIC_SCALE(t_buffer)
#endmacro

;//---------------------------------------------------------
;// StoreTags - Store lod, prim, clut
;//---------------------------------------------------------
#macro StoreTags: t_lodTag, t_setTag, t_primTag, t_clut, t_destAddress
   sq t_setTag,         0(t_destAddress)
   sq t_lodTag,         1(t_destAddress)
   sq t_setTag,         2(t_destAddress)
   sq t_clut,           3(t_destAddress)
   sq t_primTag,        4(t_destAddress)
   iaddiu               t_destAddress,    t_destAddress,    5
#endmacro
