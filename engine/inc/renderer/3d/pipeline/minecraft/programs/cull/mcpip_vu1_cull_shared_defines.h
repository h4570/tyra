//
// ______       ____   ___
//   |     \/   ____| |___|
//   |     |   |   \  |   |
//-----------------------------------------------------------------------
// Copyright 2022, tyra - https://github.com/h4570/tyra
// Licenced under Apache License 2.0
// Sandro Sobczyński <sandro.sobczynski@gmail.com>
//

// Static data for blockizer program

#define VU1_MCPIP_CULL_VERTEX_COUNT 36
#define VU1_MCPIP_CULL_QWORDS_PER_BLOCK 6

#define VU1_MCPIP_CULL_STATIC_LOD 0
#define VU1_MCPIP_CULL_STATIC_PRIM 1
#define VU1_MCPIP_CULL_STATIC_SET_TAG 2
#define VU1_MCPIP_CULL_STATIC_VU1_OPTIONS 3
#define VU1_MCPIP_CULL_STATIC_VERTEX_DATA 4
#define VU1_MCPIP_CULL_STATIC_TEX_COORD_DATA 4 + 36
#define VU1_MCPIP_CULL_STATIC_LAST_DATA_ADDR \
  VU1_MCPIP_CULL_STATIC_TEX_COORD_DATA + 36

// Dynamic data (unpack per VU1 call)
#define VU1_MCPIP_CULL_DYNAMIC_SCALE_AND_BLOCKS_COUNT_ADDR 0
#define VU1_MCPIP_CULL_DYNAMIC_CLUT_TEX 1
#define VU1_MCPIP_CULL_DYNAMIC_VIEW_PROJ_MATRIX_ADDR 2
#define VU1_MCPIP_CULL_DYNAMIC_BLOCKS_DATA 6

// Output mini double buffer inside VU1 mem
#define VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF1_ADDR 774
#define VU1_MCPIP_CULL_DYNAMIC_OUTPUT_DOUBLE_BUFF2_ADDR 887
