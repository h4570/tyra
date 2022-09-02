;//--------------------------------------------------------------------------------
;// MinecraftPipeline cull macros library
;//--------------------------------------------------------------------------------

;//---------------------------------------------------------
;// StoreBlockizerStaticData - Load and store lod, clut and prim.
;// Push dest address by 5 qwords
;//---------------------------------------------------------
#macro StoreBlockizerLodClutTexPrim: t_destAddr, t_clutTex
   lq       setTag,     VU1_MCPIP_CULL_STATIC_SET_TAG(vi00)
   lq       lodTag,     VU1_MCPIP_CULL_STATIC_LOD(vi00)
   sq       setTag,     0(t_destAddr)
   sq       lodTag,     1(t_destAddr)
   sq       setTag,     2(t_destAddr)
   sq       t_clutTex,  3(t_destAddr)
   lq       primTag,    VU1_MCPIP_CULL_STATIC_PRIM(vi00)
   sq       primTag,    4(t_destAddr)
   iaddiu   t_destAddr, t_destAddr,    5
#endmacro

;//---------------------------------------------------------
;// GetDestinationAddress - Load VU1 options and check
;// in which double buffer we are. If 0, return first double buff
;// if 1, return second double buffer.
;// Also toggle VU1 dbuff option, so next call of this macro will result
;// opposite buffer.
;//---------------------------------------------------------
#macro GetDestinationAddress: t_destAddr, t_kickAddr
   ilw.w    static1,             VU1_MCPIP_CULL_STATIC_VU1_OPTIONS(vi00)
   ilw.x    currDBufferOffset,   VU1_MCPIP_CULL_STATIC_VU1_OPTIONS(vi00)
   ibgtz    currDBufferOffset,   get_dest_addr_second

   get_dest_addr_first:
   iaddiu   t_destAddr,          vi00,    VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF1_ADDR
   isw.x    static1,             VU1_MCPIP_CULL_STATIC_VU1_OPTIONS(vi00)
   b get_dest_addr_finish

   get_dest_addr_second:
   iaddiu   t_destAddr,          vi00,    VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF2_ADDR
   isw.x    vi00,                VU1_MCPIP_CULL_STATIC_VU1_OPTIONS(vi00)

   get_dest_addr_finish:
   iaddiu   t_kickAddr,          t_destAddr,    0
#endmacro

;//---------------------------------------------------------
;// LoadScaleAndBlocksCount - Load scale vec3 and blocks count
;// from w component
;//---------------------------------------------------------
#macro LoadBufferDynamicData: t_buffer, t_scale, t_clutTex, t_blocksCount, t_viewProj
   lq.xyz   t_scale,          VU1_MCPIP_CULL_DYNAMIC_SCALE_AND_BLOCKS_COUNT_ADDR(t_buffer)
   ilw.w    t_blocksCount,    VU1_MCPIP_CULL_DYNAMIC_SCALE_AND_BLOCKS_COUNT_ADDR(t_buffer)
   lq       t_clutTex,        VU1_MCPIP_CULL_DYNAMIC_CLUT_TEX(t_buffer)
   lq       t_viewProj[0],    0+VU1_MCPIP_CULL_DYNAMIC_VIEW_PROJ_MATRIX_ADDR(t_buffer)
   lq       t_viewProj[1],    1+VU1_MCPIP_CULL_DYNAMIC_VIEW_PROJ_MATRIX_ADDR(t_buffer)
   lq       t_viewProj[2],    2+VU1_MCPIP_CULL_DYNAMIC_VIEW_PROJ_MATRIX_ADDR(t_buffer)
   lq       t_viewProj[3],    3+VU1_MCPIP_CULL_DYNAMIC_VIEW_PROJ_MATRIX_ADDR(t_buffer)
#endmacro

;//---------------------------------------------------------
;// GetVertAndStData - Get static vertex data and tex coords
;//---------------------------------------------------------
#macro GetVertAndStData: t_vertData, t_stData
   iaddiu  t_vertData,  vi00, VU1_MCPIP_CULL_STATIC_VERTEX_DATA
   iaddiu  t_stData,    vi00, VU1_MCPIP_CULL_STATIC_TEX_COORD_DATA
#endmacro

;//---------------------------------------------------------
;// GetBlockData - Get mvp matrix, color, st offset for single block
;//---------------------------------------------------------
#macro GetBlockData: t_blockData, t_model, t_color, t_stOffset
   lq    t_model[0],    0(t_blockData)
   lq    t_model[1],    1(t_blockData)
   lq    t_model[2],    2(t_blockData)
   lq    t_model[3],    3(t_blockData)
   lq    t_color,       4(t_blockData)
   lq    t_stOffset,    5(t_blockData)
#endmacro

;//---------------------------------------------------------
;// GetVertexData - Get vertex and tex coord
;//---------------------------------------------------------
#macro GetVertexData: t_vertData, t_stqData, t_vertex, t_stq, t_offset
   lq    t_vertex,   t_offset(t_vertData)
   lq    t_stq,      t_offset(t_stqData)
#endmacro

;//---------------------------------------------------------
;// GetVertexData - Get vertex and tex coord
;//---------------------------------------------------------
#macro AddSTOffset: t_stq, t_stqOffset
   add    t_stq,   t_stq, t_stqOffset
#endmacro
