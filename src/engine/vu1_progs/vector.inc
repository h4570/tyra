#macro VectorPerspectiveDivide: vertex
    div         q,      vf00[w],    vertex[w]
    mulq.xyz    vertex, vertex,     q
#endmacro

#macro VectorAddGSScales: output_vertex, input_vertex, gs_scale
    mula.xyz    acc,            gs_scale,       vf00[w]
    madd.xyz    output_vertex,  input_vertex,   gs_scale
    ftoi4.xyz   output_vertex,  output_vertex
#endmacro

#macro VectorTexturePerspectiveCorrection: output_vertex, input_vertex
    mulq    output_vertex,  input_vertex,   q
#endmacro

#macro VectorStore: vertex, vumem, num
    sq  vertex, num(vumem)
#endmacro

#macro VectorClip: output_vertex, input_vertex
    clipw.xyz	input_vertex,   input_vertex
    mfir.w      output_vertex,      adc_bit
#endmacro

#macro VectorLoad: output_vertex, vumem, offset
    lq output_vertex, offset(vumem)
#endmacro
