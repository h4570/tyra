#macro MatrixLoad: matrix, offset, vumem
    lq matrix[0], offset+0(vumem)
    lq matrix[1], offset+1(vumem)
    lq matrix[2], offset+2(vumem)
    lq matrix[3], offset+3(vumem)
#endmacro

#macro MatrixXForm: output_vertex, matrix, input_vertex
    mul  acc,           matrix[0], input_vertex[x]
    madd acc,           matrix[1], input_vertex[y]
    madd acc,           matrix[2], input_vertex[z]
    madd output_vertex, matrix[3], input_vertex[w]
#endmacro
