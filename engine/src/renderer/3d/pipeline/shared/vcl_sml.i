;//--------------------------------------------------------------------------------
;// VCLSML - VCL Standard Macros Library
;// Version 1.4
;//
;// Geoff Audy, January 17th 2002  Initial macro set
;// Geoff Audy, March 26th 2002    Fixed some macros that were broken
;// Geoff Audy, March 27th 2002    Added some macros, most from Colin Hughes (SCEE)
;// Geoff Audy, April 2002         Added some macros, from Dave Etherton (Angel
;//                                Studios)
;// Geoff Audy, May 16th 2002      Added the macro "MatrixMultiplyVertexW1", from
;//                                Mike Healey (Rebellion)
;// Geoff Audy, June 2002          Added "MatrixInverse" from Adrian Stephen
;//                                (Luxoflux, via newsgroup)
;// Geoff Audy, July 2002          Added macros "Arccos", "QuaternionSlerp" and
;//                                "DistanceVertexToPlane"
;// Geoff Audy, October 7th 2002   Added macros "VertexLightDir3W1" and
;//                                "VertexLightDir3AmbW1"
;// Geoff Audy, March 19th 2004    Added a better accuracy "Arccos", from
;//                                Morten Mikkelsen (IO Interactive)
;// Geoff Audy, March 19th 2004    Added macro "Arcsin", from Morten Mikkelsen
;//                                (IO Interactive)
;// Geoff Audy, June 10th 2004     Fixed a bug in the macro "VectorDotProductACC".
;//                                "[z]" was missing at the end of the last line
;// Sandro S.,  June 18th 2022     Thank you guys for your work...
;//
;// Copyright (C) 2002-2004, Sony Computer Entertainment America Inc.
;// All rights reserved.
;//
;// Note: Some macros generate the following temporary variables:
;//           vclsmlftemp:  Temporary float register
;//           vclsmlitemp:  Temporary integer register
;//       Some macros generate more temporary variables
;//--------------------------------------------------------------------------------

;//--------------------------------------------------------------------
;// MatrixLoad - Load "matrix" from VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro MatrixLoad: matrix, offset, vumemlocation
   lq             matrix[0], offset+0(vumemlocation)
   lq             matrix[1], offset+1(vumemlocation)
   lq             matrix[2], offset+2(vumemlocation)
   lq             matrix[3], offset+3(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// MatrixSave - Save "matrix" to VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro MatrixSave: matrix,offset,vumemlocation
   sq             matrix[0], offset+0(vumemlocation)
   sq             matrix[1], offset+1(vumemlocation)
   sq             matrix[2], offset+2(vumemlocation)
   sq             matrix[3], offset+3(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// MatrixIdentity - Set "matrix" to be an identity matrix
;// Thanks to Colin Hughes (SCEE) for that one
;//--------------------------------------------------------------------
#macro MatrixIdentity: matrix
   add.x          matrix[0], vf00, vf00[w]
   mfir.yzw       matrix[0], vi00

   mfir.xzw       matrix[1], vi00
   add.y          matrix[1], vf00, vf00[w]

   mr32           matrix[2], vf00

   max            matrix[3], vf00, vf00
#endmacro

;//--------------------------------------------------------------------
;// MatrixCopy - Copy "matrixsrc" to "matrixdest"
;// Thanks to Colin Hughes (SCEE) for that one
;//--------------------------------------------------------------------
#macro MatrixCopy: matrixdest, matrixsrc
   max            matrixdest[0], matrixsrc[0], matrixsrc[0]
   move           matrixdest[1], matrixsrc[1]
   max            matrixdest[2], matrixsrc[2], matrixsrc[2]
   move           matrixdest[3], matrixsrc[3]
#endmacro

;//--------------------------------------------------------------------
;// MatrixSwap - Swap the content of "matrix1" and "matrix2"
;// The implementation seems lame, but VCL will convert moves to maxes
;// if it sees fit
;//--------------------------------------------------------------------
#macro MatrixSwap: matrix1, matrix2
   move           vclsmlftemp, matrix1[0]
   move           matrix1[0], matrix2[0]
   move           matrix2[0], vclsmlftemp

   move           vclsmlftemp, matrix1[1]
   move           matrix1[1], matrix2[1]
   move           matrix2[1], vclsmlftemp

   move           vclsmlftemp, matrix1[2]
   move           matrix1[2], matrix2[2]
   move           matrix2[2], vclsmlftemp

   move           vclsmlftemp, matrix1[3]
   move           matrix1[3], matrix2[3]
   move           matrix2[3], vclsmlftemp
#endmacro

;//--------------------------------------------------------------------
;// MatrixTranspose - Transpose "matrixsrc" to "matresult".  It is safe
;// for "matrixsrc" and "matresult" to be the same.
;// Thanks to Colin Hughes (SCEE) for that one
;// Had to modify it though, it was too... smart.
;//--------------------------------------------------------------------
#macro MatrixTranspose: matresult, matrixsrc
   mr32.y         vclsmlftemp,   matrixsrc[1]
   add.z          matresult[1], vf00, matrixsrc[2][y]
   move.y         matresult[2], vclsmlftemp
   mr32.y         vclsmlftemp,   matrixsrc[0]
   add.z          matresult[0], vf00, matrixsrc[2][x]
   mr32.z         vclsmlftemp,   matrixsrc[1]
   mul.w          matresult[1], vf00, matrixsrc[3][y]
   mr32.x         vclsmlftemp,   matrixsrc[0]
   add.y          matresult[0], vf00, matrixsrc[1][x]
   move.x         matresult[1], vclsmlftemp
   mul.w          vclsmlftemp,   vf00, matrixsrc[3][z]
   mr32.z         matresult[3], matrixsrc[2]
   move.w         matresult[2], vclsmlftemp
   mr32.w         vclsmlftemp,   matrixsrc[3]
   add.x          matresult[3], vf00, matrixsrc[0][w]
   move.w         matresult[0], vclsmlftemp
   mr32.y         matresult[3], vclsmlftemp
   add.x          matresult[2], vf00, vclsmlftemp[y]

   move.x         matresult[0], matrixsrc[0]              ;// These 4 instructions will be
   move.y         matresult[1], matrixsrc[1]              ;// removed if "matrixsrc" and
   move.z         matresult[2], matrixsrc[2]              ;// "matresult" are the same
   move.w         matresult[3], matrixsrc[3]              ;//
#endmacro

;//--------------------------------------------------------------------
;// MatrixMultiply - Multiply 2 matrices, "matleft" and "matright", and
;// output the result in "matresult".  Dont forget matrix multipli-
;// cations arent commutative, i.e. left X right wont give you the
;// same result as right X left.
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro MatrixMultiply: matresult, matleft, matright
   mul            acc,           matright[0], matleft[0][x]
   madd           acc,           matright[1], matleft[0][y]
   madd           acc,           matright[2], matleft[0][z]
   madd           matresult[0],  matright[3], matleft[0][w]
   mul            acc,           matright[0], matleft[1][x]
   madd           acc,           matright[1], matleft[1][y]
   madd           acc,           matright[2], matleft[1][z]
   madd           matresult[1],  matright[3], matleft[1][w]
   mul            acc,           matright[0], matleft[2][x]
   madd           acc,           matright[1], matleft[2][y]
   madd           acc,           matright[2], matleft[2][z]
   madd           matresult[2],  matright[3], matleft[2][w]
   mul            acc,           matright[0], matleft[3][x]
   madd           acc,           matright[1], matleft[3][y]
   madd           acc,           matright[2], matleft[3][z]
   madd           matresult[3],  matright[3], matleft[3][w]
#endmacro

;//----------------------------------------------------
;// MatrixInverse - Does a full-fledge matrix inversion
;// of "matsrc", and output the result to "matdest".
;// Thanks to Adrian Stephen (Luxoflux) for that one
;// (via newsgroup)
;//
;// Note: ACC and Q registers are modified
;//----------------------------------------------------
#macro  MatrixInverse: matdest, matsrc
   opmula.xyz     acc, matsrc[2], matsrc[3]      ;c0 = src.z.xyz() ^ src.w.xyz();
   opmsub.xyz     c0,  matsrc[3], matsrc[2]
   mul.xyz        acc, matsrc[2], matsrc[3][w]   ;c1 = src.z.xyz() * src.w.w() - src.w.xyz() * src.z.w();
   msub.xyz       c1,  matsrc[3], matsrc[2][w]

   mul.xyz        t0, c0, matsrc[1]               ;dst.x = Vector((src.y.xyz() ^ c1) + c0 * src.y.w()).setw(-DotProd(c0,src.y.xyz()));

   opmula.xyz     acc, matsrc[1], c1
   madd.xyz       acc, c0, matsrc[1][w]
   opmsub.xyz     matdest[0], c1, matsrc[1]

   mul.w          acc,         vf00, vf00[x]
   msub.w         acc,         vf00, t0[x]
   msub.w         acc,         vf00, t0[y]
   msub.w         matdest[0], vf00, t0[z]

   mul.xyz        t0, c0, matsrc[0]               ;dst.y = Vector((c1 ^ src.x.xyz()) - c0 * src.x.w()).setw( DotProd(c0,src.x.xyz()));

   opmula.xyz     acc, c1, matsrc[0]
   msub.xyz       acc, c0, matsrc[0][w]
   opmsub.xyz     matdest[1], matsrc[0], c1

   mul            det, matsrc[0], matdest[0]     ; for determinant calc

   mul.w          acc,         vf00, t0[x]
   madd.w         acc,         vf00, t0[y]
   madd.w         matdest[1], vf00, t0[z]

   add.w          acc, det,   det[x]
   madd.w         acc, vf00,  det[y]
   madd.w         det, vf00,  det[z]

   opmula.xyz     acc, matsrc[0], matsrc[1]      ;c0 = src.x.xyz() ^ src.y.xyz();
   opmsub.xyz     c0,  matsrc[1], matsrc[0]
   mul.xyz        acc, matsrc[0], matsrc[1][w]   ;c1 = src.x.xyz() * src.y.w() - src.y.xyz() * src.x.w();
   msub.xyz       c1,  matsrc[1], matsrc[0][w]

   div            Q, vf00[w], det[w]

   mul.xyz        t0, c0, matsrc[3]               ;dst.z = Vector((src.w.xyz() ^ c1) + c0 * src.w.w()).setw(-DotProd(c0,src.w.xyz()));

   opmula.xyz     acc, matsrc[3], c1
   madd.xyz       acc, c0, matsrc[3][w]
   opmsub.xyz     matdest[2], c1, matsrc[3]

   mul.w          acc,         vf00, vf00[x]
   msub.w         acc,         vf00, t0[x]
   msub.w         acc,         vf00, t0[y]
   msub.w         matdest[2], vf00, t0[z]

   mul.xyz        t0, c0, matsrc[2]               ;dst.w = Vector((c1 ^ src.z.xyz()) - c0 * src.z.w()).setw( DotProd(c0,src.z.xyz()));

   opmula.xyz     acc, c1, matsrc[2]
   msub.xyz       acc, c0, matsrc[2][w]
   opmsub.xyz     matdest[3], matsrc[2], c1

   mul.w          acc,         vf00, t0[x]
   madd.w         acc,         vf00, t0[y]
   madd.w         matdest[3], vf00, t0[z]

   mul            matdest[0], matdest[0], Q
   mul            matdest[1], matdest[1], Q
   mul            matdest[2], matdest[2], Q
   mul            matdest[3], matdest[3], Q
#endmacro

;//--------------------------------------------------------------------
;// LocalizeLightMatrix - Transform the light matrix "lightmatrix" into
;// local space, as described by "matrix", and output the result in
;// "locallightmatrix"
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro LocalizeLightMatrix: locallightmatrix, matrix, lightmatrix
   mul            acc,                  lightmatrix[0], matrix[0][x]
   madd           acc,                  lightmatrix[1], matrix[0][y]
   madd           acc,                  lightmatrix[2], matrix[0][z]
   madd           locallightmatrix[0], lightmatrix[3], matrix[0][w]

   mul            acc,                  lightmatrix[0], matrix[1][x]
   madd           acc,                  lightmatrix[1], matrix[1][y]
   madd           acc,                  lightmatrix[2], matrix[1][z]
   madd           locallightmatrix[1], lightmatrix[3], matrix[1][w]

   mul            acc,                  lightmatrix[0], matrix[2][x]
   madd           acc,                  lightmatrix[1], matrix[2][y]
   madd           acc,                  lightmatrix[2], matrix[2][z]
   madd           locallightmatrix[2], lightmatrix[3], matrix[2][w]

   move           locallightmatrix[3], lightmatrix[3]
#endmacro

;//--------------------------------------------------------------------
;// MatrixMultiplyVertex - Multiply "matrix" by "vertex", and output
;// the result in "vertexresult"
;//
;// Note: Apply rotation, scale and translation
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro MatrixMultiplyVertex: vertexresult, matrix, vertex
   mul            acc,           matrix[0], vertex[x]
   madd           acc,           matrix[1], vertex[y]
   madd           acc,           matrix[2], vertex[z]
   madd           vertexresult, matrix[3], vertex[w]
#endmacro

;//--------------------------------------------------------------------
;// MatrixMultiplyVertexW1 - Multiply "matrix" by "vertex", and output
;// the result in "vertexresult".  Assumes 1.f for W field, which will
;// allow for better optimization in most cases, and leaves W free for
;// other uses (ADC flags, scale, etc).
;//
;// Note: Apply rotation, scale and translation
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro MatrixMultiplyVertexW1: vertexresult, matrix, vertex
   mul            acc,           matrix[3], vf00[w]
   madd           acc,           matrix[0], vertex[x]
   madd           acc,           matrix[1], vertex[y]
   madd           vertexresult, matrix[2], vertex[z]
#endmacro

;//---------------------------------------------------------------------
;// MatrixMultiplyVertexXYZ1 - Multiply "matrix" by "vertex", and output
;// the result in "vertexresult"
;//
;// Note: Apply rotation, scale and translation
;// Note: ACC register is modified
;//---------------------------------------------------------------------
#macro MatrixMultiplyVertexXYZ1: vertexresult, matrix, vertex
   mul            acc,           matrix[0], vertex[x]
   madd           acc,           matrix[1], vertex[y]
   madd           acc,           matrix[2], vertex[z]
   madd           vertexresult, matrix[3], vf00[w]
#endmacro

;//--------------------------------------------------------------------
;// MatrixMultiplyVector - Multiply "matrix" by "vector", and output
;// the result in "vectorresult"
;//
;// Note: Apply rotation and scale, but no translation
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro MatrixMultiplyVector: vectorresult, matrix, vector
   mul            acc,           matrix[0], vector[x]
   madd           acc,           matrix[1], vector[y]
   madd           vectorresult, matrix[2], vector[z]
#endmacro

;//--------------------------------------------------------------------
;// VectorLoad - Load "vector" from VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro VectorLoad: vector, offset, vumemlocation
   lq             vector, offset(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// VectorSave - Save "vector" to VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro VectorSave: vector, offset, vumemlocation
   sq             vector, offset(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// VectorAdd - Add 2 vectors, "vector1" and "vector2" and output the
;// result in "vectorresult"
;//--------------------------------------------------------------------
#macro VectorAdd:  vectorresult, vector1, vector2
   add            vectorresult, vector1, vector2
#endmacro

;//--------------------------------------------------------------------
;// VectorSub - Subtract "vector2" from "vector1", and output the
;// result in "vectorresult"
;//--------------------------------------------------------------------
#macro VectorSub:  vectorresult, vector1, vector2
   sub            vectorresult, vector1, vector2
#endmacro

;//--------------------------------------------------------------------
;// VertexLoad - Load "vertex" from VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro VertexLoad: vertex, offset, vumemlocation
   lq             vertex, offset(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// VertexSave - Save "vertex" to VU mem location "vumemlocation" +
;// "offset"
;//--------------------------------------------------------------------
#macro VertexSave: vertex, offset, vumemlocation
   sq             vertex, offset(vumemlocation)
#endmacro

;//--------------------------------------------------------------------
;// VertexPersCorr - Apply perspective correction onto "vertex" and
;// output the result in "vertexoutput"
;//
;// Note: Q register is modified
;//--------------------------------------------------------------------
#macro VertexPersCorr: vertexoutput, vertex
   div            q, vf00[w], vertex[w]
   mul.xyz        vertexoutput, vertex, q
;   mul            vertexoutput, vertex, q
#endmacro

;//--------------------------------------------------------------------
;// VertexPersCorrST - Apply perspective correction onto "vertex" and
;// "st", and output the result in "vertexoutput" and "stoutput"
;//
;// Note: Q register is modified
;//--------------------------------------------------------------------
#macro VertexPersCorrST: vertexoutput, stoutput, vertex, st
   div            q,             vf00[w], vertex[w]
   mul.xyz        vertexoutput, vertex, q
   move.w         vertexoutput, vertex
;   mul            vertexoutput, vertex, q
   mul            stoutput,     st,     q
#endmacro

;//--------------------------------------------------------------------
;// VertexFPtoGsXYZ2 - Convert an XYZW, floating-point vertex to GS
;// XYZ2 format (ADC bit isnt set)
;//--------------------------------------------------------------------
#macro VertexFpToGsXYZ2: outputxyz, vertex
   ftoi4.xy       outputxyz, vertex
   ftoi0.z        outputxyz, vertex
   mfir.w         outputxyz, vi00
#endmacro

;//--------------------------------------------------------------------
;// VertexFPtoGsXYZ2Adc - Convert an XYZW, floating-point vertex to GS
;// XYZ2 format (ADC bit is set)
;//--------------------------------------------------------------------
#macro VertexFpToGsXYZ2Adc: outputxyz, vertex
   ftoi4.xy       outputxyz, vertex
   ftoi0.z        outputxyz, vertex
   ftoi15.w       outputxyz, vf00
#endmacro

;//--------------------------------------------------------------------
;// VertexFpToGsXYZF2 - Convert an XYZF, floating-point vertex to GS
;// XYZF2 format (ADC bit isnt set)
;//--------------------------------------------------------------------
#macro VertexFpToGsXYZF2: outputxyz, vertex
   ftoi4          outputxyz, vertex
#endmacro

;//--------------------------------------------------------------------
;// VertexFpToGsXYZF2Adc - Convert an XYZF, floating-point vertex to GS
;// XYZF2 format (ADC bit is set)
;//--------------------------------------------------------------------
#macro VertexFpToGsXYZF2Adc: outputxyz, vertex
   ftoi4          outputxyz,  vertex
   mtir           vclsmlitemp, outputxyz[w]
   iaddiu         vclsmlitemp, 0x7FFF
   iaddi          vclsmlitemp, 1
   mfir.w         outputxyz,  vclsmlitemp
#endmacro

;//--------------------------------------------------------------------
;// ColorFPtoGsRGBAQ - Convert an RGBA, floating-point color to GS
;// RGBAQ format
;//--------------------------------------------------------------------
#macro ColorFPtoGsRGBAQ: outputrgba, color
   ftoi0          outputrgba, color
#endmacro

;//--------------------------------------------------------------------
;// ColorGsRGBAQtoFP - Convert an RGBA, GS RGBAQ format to floating-
;// point color
;//--------------------------------------------------------------------
#macro ColorGsRGBAQtoFP: outputrgba, color
   itof0          outputrgba, color
#endmacro

;//--------------------------------------------------------------------
;// CreateGsPRIM - Create a GS-packed-format PRIM command, according to
;// a specified immediate value "prim"
;//
;// Note: Meant more for debugging purposes than for a final solution
;//--------------------------------------------------------------------
#macro CreateGsPRIM: outputprim, prim
   iaddiu         vclsmlitemp, vi00, prim
   mfir           outputprim, vclsmlitemp
#endmacro

;//--------------------------------------------------------------------
;// CreateGsRGBA - Create a GS-packed-format RGBA command, according to
;// specified immediate values "r", "g", "b" and "a" (integer 0-255)
;//
;// Note: Meant more for debugging purposes than for a final solution
;//--------------------------------------------------------------------
#macro CreateGsRGBA: outputrgba, r, g, b, a
   iaddiu         vclsmlitemp, vi00, r
   mfir.x         outputrgba, vclsmlitemp
   iaddiu         vclsmlitemp, vi00, g
   mfir.y         outputrgba, vclsmlitemp
   iaddiu         vclsmlitemp, vi00, b
   mfir.z         outputrgba, vclsmlitemp
   iaddiu         vclsmlitemp, vi00, a
   mfir.w         outputrgba, vclsmlitemp
#endmacro

;//--------------------------------------------------------------------
;// CreateGsSTQ - Create a GS-packed-format STQ command, according to
;// specified immediate values "s", "t" and "q" (floats)
;//
;// Note: I register is modified
;// Note: Meant more for debugging purposes than for a final solution
;//--------------------------------------------------------------------
#macro CreateGsSTQ: outputstq, s, t, q
   loi            s
   add.x          outputstq, vf00, i
   loi            t
   add.y          outputstq, vf00, i
   loi            q
   add.z          outputstq, vf00, i
#endmacro

;//--------------------------------------------------------------------
;// CreateGsUV - Create a GS-packed-format VU command, according to
;// specified immediate values "u" and "v" (integer -32768 - 32768,
;// with 4 LSB as precision)
;//
;// Note: Meant more for debugging purposes than for a final solution
;//--------------------------------------------------------------------
#macro CreateGsUV: outputuv, u, v
   iaddiu         vclsmlitemp, vi00, u
   mfir.x         outputuv, vclsmlitemp
   iaddiu         vclsmlitemp, vi00, v
   mfir.y         outputuv, vclsmlitemp
#endmacro

;//--------------------------------------------------------------------
;// CreateGsRGBA - Create a GS-packed-format RGBA command, according to
;// a specified immediate value "fog" (integer 0-255)
;//
;// Note: Meant more for debugging purposes than for a final solution
;//--------------------------------------------------------------------
#macro CreateGsFOG: outputfog, fog
   iaddiu         vclsmlitemp, vi00, fog * 16
   mfir.w         outputfog, vclsmlitemp
#endmacro

;//--------------------------------------------------------------------
;// CreateGifTag - Create a packed-mode giftag, according to specified
;// immediate values.  Currently only support up to 4 registers.
;//
;// Note: I register is modified
;// Note: Definitely meant for debugging purposes, NOT for a final
;// solution
;//--------------------------------------------------------------------
;// MIGHT NOT BE IMPLEMENTABLE AFTER ALL (AT LEAST NOT UNTIL VCL EVALUATES CONSTANTS!)
;// THAT WOULD HAVE BEEN KINDA COOL...  DAMN.  --GEOFF
;//#macro CreateGifTag   outputgiftag,nloop,prim,nreg,reg1,reg2,reg3,reg4
;//   iaddiu         vclsmlitemp, vi00, nloop + 0x8000
;//   mfir.x         outputgiftag, vclsmlitemp
;//   loi            0x00004000 + (prim * 0x8000) + (nreg * 0x10000000)
;//   add.y          outputgiftag, vf00, i
;//   iaddiu         vclsmlitemp, vi00, reg1 + (reg2 * 16) + (reg3 * 256) + (reg4 * 4096)
;//   mfir.z         outputgiftag, vclsmlitemp
;//#endmacro

;//--------------------------------------------------------------------
;// VectorDotProduct - Calculate the dot product of "vector1" and
;// "vector2", and output to "dotproduct"[x]
;//--------------------------------------------------------------------
#macro VectorDotProduct: dotproduct, vector1, vector2
   mul.xyz        dotproduct, vector1,    vector2
   add.x          dotproduct, dotproduct, dotproduct[y]
   add.x          dotproduct, dotproduct, dotproduct[z]
#endmacro

;//--------------------------------------------------------------------
;// VectorDotProductACC - Calculate the dot product of "vector1" and
;// "vector2", and output to "dotproduct"[x].  This one does it using
;// the ACC register which, depending on the case, might turn out to be
;// faster or slower.
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VectorDotProductACC: dotproduct, vector1, vector2
   max            Vector1111,  vf00,        vf00[w]
   mul            vclsmlftemp, vector1,    vector2
   add.x          acc,         vclsmlftemp, vclsmlftemp[y]
   madd.x         dotproduct, Vector1111,  vclsmlftemp[z]
#endmacro

;//--------------------------------------------------------------------
;// VectorCrossProduct - Calculate the cross product of "vector1" and
;// "vector2", and output to "vectoroutput"
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VectorCrossProduct: vectoroutput, vector1, vector2
   opmula.xyz     ACC,           vector1, vector2
   opmsub.xyz     vectoroutput, vector2, vector1
   sub.w          vectoroutput, vf00,     vf00
#endmacro

;//--------------------------------------------------------------------
;// VectorNormalize - Bring the length of "vector" to 1.f, and output
;// it to "vectoroutput"
;//
;// Note: Q register is modified
;//--------------------------------------------------------------------
#macro VectorNormalize: vecoutput, vector
   mul.xyz        vclsmlftemp, vector,     vector
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[y]
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[z]
   rsqrt          q,           vf00[w],     vclsmlftemp[x]
   sub.w          vecoutput,  vf00,        vf00
   mul.xyz        vecoutput,  vector,     q
#endmacro

;//--------------------------------------------------------------------
;// VectorNormalizeXYZ - Bring the length of "vector" to 1.f, and out-
;// put it to "vectoroutput".  The "w" field isn't transfered.
;//
;// Note: Q register is modified
;//--------------------------------------------------------------------
#macro VectorNormalizeXYZ: vecoutput, vector
   mul.xyz        vclsmlftemp, vector,     vector
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[y]
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[z]
   rsqrt          q,           vf00[w],     vclsmlftemp[x]
   mul.xyz        vecoutput,  vector,     q
#endmacro

;//--------------------------------------------------------------------
;// VertexLightAmb - Apply ambient lighting "ambientrgba" to a vertex
;// of color "vertexrgba", and output the result in "outputrgba"
;//--------------------------------------------------------------------
#macro VertexLightAmb: rgbaout,vertexrgba,ambientrgba
   mul            rgbaout, vertexrgba, ambientrgba
#endmacro

;//--------------------------------------------------------------------
;// VertexLightDir3 - Apply up to 3 directional lights contained in a
;// light matrix "lightmatrix" to a vertex of color "vertexrgba" and
;// having a normal "vertexnormal", and output the result in
;// "outputrgba"
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VertexLightDir3: rgbaout, vertexrgba, vertexnormal, lightcolors, lightnormals
   mul            acc,      lightnormals[0], vertexnormal[x]
   madd           acc,      lightnormals[1], vertexnormal[y]
   madd           acc,      lightnormals[2], vertexnormal[z]
   madd           rgbaout, lightnormals[3], vertexnormal[w] ;// Here "rgbaout" is the dot product for the 3 lights
   max            rgbaout, rgbaout,         vf00[x]          ;// Here "rgbaout" is the dot product for the 3 lights
   mul            acc,      lightcolors[0],  rgbaout[x]
   madd           acc,      lightcolors[1],  rgbaout[y]
   madd           rgbaout, lightcolors[2],  rgbaout[z]      ;// Here "rgbaout" is the light applied on the vertex
   mul            rgbaout, vertexrgba,      rgbaout         ;// Here "rgbaout" is the amount of light reflected by the vertex
#endmacro

;//--------------------------------------------------------------------
;// VertexLightDir3W1 - Apply up to 3 directional lights contained in a
;// light matrix "lightmatrix" to a vertex of color "vertexrgba" and
;// having a normal "vertexnormal", and output the result in
;// "outputrgba".  Assumes 1.0 for the normal's W field
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VertexLightDir3W1: rgbaout, vertexrgba, vertexnormal, lightcolors, lightnormals
   mul            acc,      lightnormals[0], vertexnormal[x]
   madd           acc,      lightnormals[1], vertexnormal[y]
   madd           acc,      lightnormals[2], vertexnormal[z]
   madd           rgbaout, lightnormals[3], vf00[w]          ;// Here "rgbaout" is the dot product for the 3 lights
   max            rgbaout, rgbaout,         vf00[x]          ;// Here "rgbaout" is the dot product for the 3 lights
   mul            acc,      lightcolors[0],  rgbaout[x]
   madd           acc,      lightcolors[1],  rgbaout[y]
   madd           rgbaout, lightcolors[2],  rgbaout[z]      ;// Here "rgbaout" is the light applied on the vertex
   mul            rgbaout, vertexrgba,      rgbaout         ;// Here "rgbaout" is the amount of light reflected by the vertex
#endmacro

;//--------------------------------------------------------------------
;// VertexLightDir3Amb - Apply up to 3 directional lights, plus an
;// ambient light contained in a light matrix "lightmatrix" to a vertex
;// of color "vertexrgba" and having a normal "vertexnormal", and
;// output the result in "outputrgba"
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VertexLightDir3Amb: rgbaout, vertexrgba, vertexnormal, lightcolors, lightnormals
   mul            acc,      lightnormals[0], vertexnormal[x]
   madd           acc,      lightnormals[1], vertexnormal[y]
   madd           acc,      lightnormals[2], vertexnormal[z]
   madd           rgbaout, lightnormals[3], vertexnormal[w] ;// Here "rgbaout" is the dot product for the 3 lights
   max            rgbaout, rgbaout,         vf00[x]          ;// Here "rgbaout" is the dot product for the 3 lights
   mul            acc,      lightcolors[0],  rgbaout[x]
   madd           acc,      lightcolors[1],  rgbaout[y]
   madd           acc,      lightcolors[2],  rgbaout[z]
   madd           rgbaout, lightcolors[3],  rgbaout[w]      ;// Here "rgbaout" is the light applied on the vertex
   mul.xyz        rgbaout, vertexrgba,      rgbaout         ;// Here "rgbaout" is the amount of light reflected by the vertex
#endmacro

;//--------------------------------------------------------------------
;// VertexLightDir3AmbW1 - Apply up to 3 directional lights, plus an
;// ambient light contained in a light matrix "lightmatrix" to a vertex
;// of color "vertexrgba" and having a normal "vertexnormal", and
;// output the result in "outputrgba".  Assumes 1.0 for the normal's
;// W field.
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro VertexLightDir3AmbW1: rgbaout, vertexrgba, vertexnormal, lightcolors, lightnormals
   mul            acc,      lightnormals[0], vertexnormal[x]
   madd           acc,      lightnormals[1], vertexnormal[y]
   madd           acc,      lightnormals[2], vertexnormal[z]
   madd           rgbaout, lightnormals[3], vf00[w]          ;// Here "rgbaout" is the dot product for the 3 lights
   max            rgbaout, rgbaout,         vf00[x]          ;// Here "rgbaout" is the dot product for the 3 lights
   mul            acc,      lightcolors[0],  rgbaout[x]
   madd           acc,      lightcolors[1],  rgbaout[y]
   madd           acc,      lightcolors[2],  rgbaout[z]
   madd           rgbaout, lightcolors[3],  rgbaout[w]      ;// Here "rgbaout" is the light applied on the vertex
   mul.xyz        rgbaout, vertexrgba,      rgbaout         ;// Here "rgbaout" is the amount of light reflected by the vertex
#endmacro

;//--------------------------------------------------------------------
;// FogSetup - Set up fog "fogparams", by specifying "nearfog" and
;// "farfog".  "fogparams" will afterward be ready to be used by fog-
;// related macros, like "VertexFogLinear" for example.
;//
;// Note: I register is modified
;//--------------------------------------------------------------------
#macro FogSetup: fogparams, nearfogz, farfogz
   sub            fogparams, vf00,          vf00           ;// Set XYZW to 0
   loi            farfogz                                  ;//
   add.w          fogparams, fogparams,    i              ;// fogparam[w] is farfogz
   loi            nearfogz
   add.z          fogparams, fogparams,    fogparams[w]
   sub.z          fogparams, fogparams,    i
   loi            255.0
   add.xy         fogparams, fogparams,    i              ;// fogparam[y] is 255.0
   sub.x          fogparams, fogparams,    vf00[w]        ;// fogparam[x] is 254.0
   div            q,          fogparams[y], fogparams[z]
   sub.z          fogparams, fogparams,    fogparams
   add.z          fogparams, fogparams,    q              ;// fogparam[z] is 255.f / (farfogz - nearfogz)
#endmacro

;//--------------------------------------------------------------------
;// VertexFogLinear - Apply fog "fogparams" to a vertex "xyzw", and
;// output the result in "xyzfoutput". "xyzw" [w] is assumed to be
;// the distance from the camera.  "fogparams" must contain farfogz in
;// [w], and (255.f / (farfogz - nearfogz)) in [z]. "xyzfoutputf" [w]
;// will contain a float value between 0.0 and 255.0, inclusively.
;//--------------------------------------------------------------------
#macro VertexFogLinear: xyzfoutput, xyzw, fogparams
   move.xyz       xyzfoutput, xyzw                        ;// XYZ part won't be modified
   sub.w          xyzfoutput, fogparams,  xyzw[w]        ;// fog = (farfogz - z) * 255.0 /
   mul.w          xyzfoutput, xyzfoutput, fogparams[z]   ;//       (farfogz - nearfogz)
   max.w          xyzfoutput, xyzfoutput, vf00[x]         ;// Clamp fog values outside the range 0.0-255.0
   mini.w         xyzfoutput, xyzfoutput, fogparams[y]   ;//
#endmacro

;//--------------------------------------------------------------------
;// VertexFogRemove - Remove any effect of fog to "xyzf".  "fogparams"
;// [ x ] must be set to 254.0.  "xyzf" will be modified directly.
;//--------------------------------------------------------------------
#macro VertexFogRemove: xyzf, fogparams
   add.w          xyzf, vf00, fogparams[x]                ;// xyzw[w] = 1.0 + 254.0 = 255.0 = no fog
#endmacro

;//--------------------------------------------------------------------
;// PushInteger1 - Push "integer1" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushInteger1: stackptr, integer1
   isubiu         stackptr, stackptr, 1
   iswr.x         integer1, (stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PushInteger2 - Push "integer1" and "integer2" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushInteger2: stackptr, integer1, integer2
   isubiu         stackptr, stackptr, 1
   iswr.x         integer1, (stackptr):VCLSML_STACK
   iswr.y         integer2, (stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PushInteger3 - Push "integer1", "integer2" and "integer3" on
;// "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushInteger3: stackptr, integer1, integer2, integer3
   isubiu         stackptr, stackptr, 1
   iswr.x         integer1, (stackptr):VCLSML_STACK
   iswr.y         integer2, (stackptr):VCLSML_STACK
   iswr.z         integer3, (stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PushInteger4 - Push "integer1", "integer2", "integer3" and
;// "integer4" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushInteger4: stackptr, integer1, integer2, integer3, integer4
   isubiu         stackptr, stackptr, 1
   iswr.x         integer1, (stackptr):VCLSML_STACK
   iswr.y         integer2, (stackptr):VCLSML_STACK
   iswr.z         integer3, (stackptr):VCLSML_STACK
   iswr.w         integer4, (stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PopInteger1 - Pop "integer1" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopInteger1: stackptr, integer1
   ilwr.x         integer1, (stackptr):VCLSML_STACK
   iaddiu         stackptr, stackptr, 1
#endmacro

;//--------------------------------------------------------------------
;// PopInteger2 - Pop "integer1" and "integer2" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopInteger2: stackptr, integer1, integer2
   ilwr.y         integer2, (stackptr):VCLSML_STACK
   ilwr.x         integer1, (stackptr):VCLSML_STACK
   iaddiu         stackptr, stackptr, 1
#endmacro

;//--------------------------------------------------------------------
;// PopInteger3 - Pop "integer1", "integer2" and "integer3" on
;// "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopInteger3: stackptr, integer1, integer2, integer3
   ilwr.z         integer3, (stackptr):VCLSML_STACK
   ilwr.y         integer2, (stackptr):VCLSML_STACK
   ilwr.x         integer1, (stackptr):VCLSML_STACK
   iaddiu         stackptr, stackptr, 1
#endmacro

;//--------------------------------------------------------------------
;// PopInteger4 - Pop "integer1", "integer2", "integer3" and
;// "integer4" on "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopInteger4: stackptr, integer1, integer2, integer3, integer4
   ilwr.w         integer4, (stackptr):VCLSML_STACK
   ilwr.z         integer3, (stackptr):VCLSML_STACK
   ilwr.y         integer2, (stackptr):VCLSML_STACK
   ilwr.x         integer1, (stackptr):VCLSML_STACK
   iaddiu         stackptr, stackptr, 1
#endmacro

;//--------------------------------------------------------------------
;// PushMatrix - Push "matrix" onto the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushMatrix: stackptr, matrix
   sq             matrix[0], -1(stackptr):VCLSML_STACK
   sq             matrix[1], -2(stackptr):VCLSML_STACK
   sq             matrix[2], -3(stackptr):VCLSML_STACK
   sq             matrix[3], -4(stackptr):VCLSML_STACK
   iaddi          stackptr, stackptr, -4
#endmacro

;//--------------------------------------------------------------------
;// PopMatrix - Pop "matrix" out of the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopMatrix: stackptr, matrix
   lq             matrix[0], 0(stackptr):VCLSML_STACK
   lq             matrix[1], 1(stackptr):VCLSML_STACK
   lq             matrix[2], 2(stackptr):VCLSML_STACK
   lq             matrix[3], 3(stackptr):VCLSML_STACK
   iaddi          stackptr, stackptr, 4
#endmacro

;//--------------------------------------------------------------------
;// PushVector - Push "vector" onto the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushVector: stackptr, vector
   sqd            vector, (--stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PopVector - Pop "vector" out of the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopVector: stackptr, vector
   lqi            vector, (stackptr++):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PushVertex - Push "vector" onto the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PushVertex: stackptr, vertex
   sqd            vertex, (--stackptr):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// PopVertex - Pop "vertex" out of the "stackptr"
;//
;// Note: "stackptr" is updated
;//--------------------------------------------------------------------
#macro PopVertex: stackptr, vertex
   lqi            vertex, (stackptr++):VCLSML_STACK
#endmacro

;//--------------------------------------------------------------------
;// AngleSinCos - Returns the sin and cos of up to 2 angles, which must
;// be contained in the X and Z elements of "angle".  The sin/cos pair
;// will be contained in the X/Y elements of "sincos" for the first
;// angle, and Z/W for the second one.
;// Thanks to Colin Hughes (SCEE) for that one
;//
;// Note: ACC and I registers are modified, and a bunch of temporary
;//       variables are created... Maybe bad for VCL register pressure
;//--------------------------------------------------------------------
#macro AngleSinCos: angle, sincos
   move.xz        sincos, angle                           ; To avoid modifying the original angles...

   mul.w          sincos, vf00, sincos[z]                 ; Copy angle from z to w
   add.y          sincos, vf00, sincos[x]                 ; Copy angle from x to y

   loi            1.570796                                  ; Phase difference for sin as cos ( PI/2 )
   sub.xz         sincos, sincos, I                       ;

   abs            sincos, sincos                          ; Mirror cos around zero

   max            Vector1111, vf00, vf00[w]                 ; Initialise all 1s

   loi            -0.159155                                 ; Scale so single cycle is range 0 to -1 ( *-1/2PI )
   mul            ACC,  sincos, I                          ;

   loi            12582912.0                                ; Apply bias to remove fractional part
   msub           ACC,  Vector1111, I                       ;
   madd           ACC,  Vector1111, I                       ; Remove bias to leave original int part

   loi            -0.159155                                 ; Apply original number to leave fraction range only
   msub           ACC,  sincos, I                          ;

   loi            0.5                                       ; Ajust range: -0.5 to +0.5
   msub           sincos, Vector1111, I                    ;

   abs            sincos, sincos                          ; Clamp: 0 to +0.5

   loi            0.25                                      ; Ajust range: -0.25 to +0.25
   sub            sincos, sincos, I                       ;

   mul            anglepower2, sincos, sincos             ; a^2

   loi            -76.574959                                ;
   mul            k4angle, sincos, I                       ; k4 a

   loi            -41.341675                                ;
   mul            k2angle, sincos, I                       ; k2 a

   loi            81.602226                                 ;
   mul            k3angle, sincos, I                       ; k3 a

   mul            anglepower4, anglepower2, anglepower2     ; a^4
   mul            k4angle, k4angle, anglepower2             ; k4 a^3
   mul            ACC,  k2angle, anglepower2                ; + k2 a^3

   loi            39.710659                                 ; k5 a
   mul            k2angle, sincos, I                       ;

   mul            anglepower8, anglepower4, anglepower4     ; a^8
   madd           ACC,  k4angle, anglepower4                ; + k4 a^7
   madd           ACC,  k3angle, anglepower4                ; + k3 a^5
   loi            6.283185                                  ;
   madd           ACC,  sincos, I                          ; + k1 a
   madd           sincos, k2angle, anglepower8             ; + k5 a^9
#endmacro

;//--------------------------------------------------------------------
;// ArcsinAccurate - Returns an accurate arcsin angle of a value, which
;// must be contained in the X element of "value".  X element of "asin"
;// will contain, on return, the angle corresponding to the value.
;// Thanks to Morten Mikkelsen (IO Interactive) for that one
;//
;// Note: ACC, I registers are modified, and a bunch of temporary
;//       variables are created... Maybe bad for VCL register pressure
;//--------------------------------------------------------------------
#macro ArcsinAccurate: value, asin

   add.x          acc, vf00, vf00[w]
   msub.x         Xc, value, value
   sqrt           Q, Xc[x]                                  ; Xc = sqrt (1 - value * value)
   add.x          Xc, vf00, q

   mr32.w         bflip, value								; Sign in w field of bflip
   abs.x          Xs, value                                ; Xs = fabs (value)

   loi            0x3f3504f3                  
   subi.x         bflip, Xs, i                              ; bflip = (Xs - sqrt (0.5f)) * I
   loi            0z7F800000                                ; I = big float
   mul.xw         bflip, bflip, i                           ;
   mini.xw        bflip, bflip, vf00[w]                     ; Minf (&bflip, bflip, 1), Minf (&Sign, Sign, 1)    sign should only be zeroif bflip is, so offs should not be affected by this case

   loi            -1                                        ; I = -1.0
   max.xw         bflip, bflip, i                           ; Maxf (&bflip, bflip, -1), Maxf (&Sign, Sign, -1)   if x is already zero, then scaling by zero will not matter either when sign is applied

   add.x           acc, vf00, bflip[w]
   madd.x          offs, bflip, bflip[w]                    ; offs = Sign + Sign * bflip
   mul.x          bflip, bflip, bflip[w]                    ; bflip *= Sign

   mini.x         temp, Xc, Xs                              ; temp = fmin (Xc, Xs)
   mul.x          temp, temp, bflip                         ; temp *= bflip

   loi            0x3f490fdb                                ; 
   mul.x          acc, offs, i                              ; MULA (offs, piquat)

   loi            -1.0                                      ; s1 = -1    (0xbf800000)
   madd.x         acc, temp, i                              ; MADDA (s1, temp)

   mul.x          x2, temp, temp                            ; x2 = temp * temp
   loi            0xbe2aaaab                                ; s2 = -1 / 6
   mul.x          S2, temp, i                               ; S2 = s2 * temp

   madd.x         acc, S2, x2                               ; MADDA (S2, x2)

   mul.x          x3, temp, x2                              ; x3 = temp * x2
   loi            0xbd36db6e                                ; s4 = -5 / 112
   mul.x          S4, x3, i                                 ; S4 = s4 * x3
   mul.x          x5, x2, x3                                ; x5 = x2 * x3
   mul.x          x4, x2, x2                                ; x4 = x2 * x2

   loi            0xbcf8e38e                                ; s5 = -35 / 1152
   mul.x          S5, x4, i                                 ; S5 = s5 * x4

   loi            0xbd99999a                                ; s3 = -3 / 40
   mul.x          S3, x2, i                                 ; S3 = s3 * x2

   madd.x         acc, S3, x3                               ; MADDA (S3, x3)
   madd.x         acc, S4, x4                               ; MADDA (S4, x4)
   madd.x         asin, S5, x5                             ; MADD (&r, S5, x5)

#endmacro

;//--------------------------------------------------------------------
;// ArccosAccurate - Returns an accurate arccos angle of a value, which
;// must be contained in the X element of "value".  X element of "acos"
;// will contain, on return, the angle corresponding to the value.
;// Thanks to Morten Mikkelsen (IO Interactive) for that one
;//
;// Note: ACC, I registers are modified, and a bunch of temporary
;//       variables are created... Maybe bad for VCL register pressure
;//--------------------------------------------------------------------
#macro ArccosAccurate: value, acos

   add.x          acc, vf00, vf00[w]
   msub.x         Xc, value, value
   sqrt           Q, Xc[x]                                  ; Xc = sqrt (1 - value * value)
   add.x          Xc, vf00, q                               ;

   mr32.w         bflip, value								; Sign in w field of bflip
   abs.x          Xs, value                                ; Xs = fabs (value)

   loi            0x3f3504f3
   subi.x         bflip, Xs, i                              ; bflip = (Xs - sqrt (0.5f)) * I
   loi            0z7F800000                                ; I = big float
   mul.xw         bflip, bflip, i                           ;
   mini.xw        bflip, bflip, vf00[w]                     ; Minf (&bflip, bflip, 1), Minf (&Sign, Sign, 1)    sign should only be zeroif bflip is, so offs should not be affected by this case

   loi            -1                                        ; I = -1.0
   max.xw         bflip, bflip, i                           ; Maxf (&bflip, bflip, -1), Maxf (&Sign, Sign, -1)   if x is already zero, then scaling by zero will not matter either when sign is applied

   mul.x          bflip, bflip, bflip[w]                    ; bflip *= sign
   loi            2.0
   sub.w          bflip, bflip, i							; (Sign - 2)
   add.x		  offs, bflip, bflip[w]						; offs = (Sign - 2) + Sign * bflip

   mini.x         temp, Xc, Xs                              ; temp = fmin (Xc, Xs)
   mul.x          temp, temp, bflip                         ; temp *= bflip

   loi            0xbf490fdb
   mul.x          acc, offs, i                              ; MULA (offs, -piquat)

   loi            1.0                                       ; s1 = 1        (0x3f800000)
   madd.x         acc, temp, i                              ; MADDA (s1, temp)

   mul.x          x2, temp, temp                            ; x2 = temp * temp
   loi            0x3e2aaaab                                ; s2 = 1 / 6
   mul.x          S2, temp, i                               ; S2 = s2 * temp

   madd.x         acc, S2, x2                               ; MADDA (S2, x2)

   mul.x          x3, temp, x2                              ; x3 = temp * x2
   loi            0x3d36db6e                                ; s4 = 5 / 112
   mul.x          S4, x3, i                                 ; S4 = s4 * x3
   mul.x          x5, x2, x3                                ; x5 = x2 * x3
   mul.x          x4, x2, x2                                ; x4 = x2 * x2

   loi            0x3cf8e38e                                ; s5 = 35 / 1152
   mul.x          S5, x4, i                                 ; S5 = s5 * x4

   loi            0x3d99999a                                ; s3 = 3 / 40
   mul.x          S3, x2, i                                 ; S3 = s3 * x2

   madd.x         acc, S3, x3                               ; MADDA (S3, x3)
   madd.x         acc, S4, x4                               ; MADDA (S4, x4)
   madd.x         acos, S5, x5                             ; MADD (&r, S5, x5)

#endmacro

;//--------------------------------------------------------------------
;// Arccos - Returns the arccos angle of up to 2 values, which must be
;// contained in the X and Z elements of "value".  X and Z elements of
;// "acos" will contain, on return, the angles corresponding to the 2
;// values.
;//
;// ARCCOS(value) may be approximated with the following:
;//    mirror = value * -HALFPI + HALFPI;
;//    ARCCOS(value) = 2 * mirror -
;//                    (sin (value * HALFPI) * -HALFPI + HALFPI);
;//
;// Note: ACC and I registers are modified, and a bunch of temporary
;//       variables are created... Maybe bad for VCL register pressure
;//--------------------------------------------------------------------
#macro Arccos: value, acos
   loi            1.570796326                            ; value * HALFPI
   mul.xz         acos, value, i                       ;

   move.xz        mirror, value                         ; Important to do it here, or else it would be invalid for "value" & "acos" to be the same

   ;//// This part is really just a copy of AngleSinCos
   loi            1.570796326                            ; Phase difference for sin as cos ( PI/2 )
   sub.xz         acos, acos, I                        ;

   abs.xz         acos, acos                           ; Mirror cos around zero

   max.xz         Vector1111, vf00, vf00[w]              ; Initialise all 1s

   loi            -0.159155                              ; Scale so single cycle is range 0 to -1 ( *-1/2PI )
   mul.xz         ACC,  acos, I                         ;

   loi            12582912.0                             ; Apply bias to remove fractional part
   msub.xz        ACC,  Vector1111, I                    ;
   madd.xz        ACC,  Vector1111, I                    ; Remove bias to leave original int part

   loi            -0.159155                              ; Apply original number to leave fraction range only
   msub.xz        ACC,  acos, I                         ;

   loi            0.5                                    ; Ajust range: -0.5 to +0.5
   msub.xz        acos, Vector1111, I                   ;

   abs.xz         acos, acos                           ; Clamp: 0 to +0.5

   loi            0.25                                   ; Ajust range: -0.25 to +0.25
   sub.xz         acos, acos, I                        ;

   mul.xz         anglepower2, acos, acos              ; a^2

   loi            -76.574959                             ;
   mul.xz         k4angle, acos, I                      ; k4 a

   loi            -41.341675                             ;
   mul.xz         k2angle, acos, I                      ; k2 a

   loi            81.602226                              ;
   mul.xz         k3angle, acos, I                      ; k3 a

   mul.xz         anglepower4, anglepower2, anglepower2  ; a^4
   mul.xz         k4angle, k4angle, anglepower2          ; k4 a^3
   mul.xz         ACC,  k2angle, anglepower2             ; + k2 a^3

   loi            39.710659                              ; k5 a
   mul.xz         k2angle, acos, I                      ;

   mul.xz         anglepower8, anglepower4, anglepower4  ; a^8
   madd.xz        ACC,  k4angle, anglepower4             ; + k4 a^7
   madd.xz        ACC,  k3angle, anglepower4             ; + k3 a^5
   loi            6.283185                               ;
   madd.xz        ACC,  acos, I                         ; + k1 a
   madd.xz        acos, k2angle, anglepower8            ; + k5 a^9
   ;//// This part was really just a copy of AngleSinCos

   loi            -1.570796326                           ; mirror = value * -HALFPI + HALFPI
   mul.xz         mirror, mirror, i                      ;
   sub.xz         mirror, mirror, i                      ;

   mul.xz         acos, acos, i                        ;
   sub.xz         acos, acos, i                        ;

   add.xz         mirror, mirror, mirror                 ; acos = 2 * mirror - (sin (value * HALFPI) * -HALFPI + HALFPI)
   sub.xz         acos, mirror, acos                   ; Final result in "x"
#endmacro

;//------------------------------------------------------------------------------
;// QuaternionSlerp - Spherical interpolation between 2 quaternions. "quatfrom"
;// corresponds to time 0.0, and "quatto" corresponds to time 1.0.  The X element
;// of "qtime" must contain the desired time value (0.0 - 1.0).  The result will
;// be returned in "quatdest".
;//
;// Note: ACC, I and Q registers are modified
;//------------------------------------------------------------------------------
#macro  QuaternionSlerp: quatdest, quatfrom, quatto, qtime
   mul            vclsmlftemp, quatfrom, quatto           ; vclsmlftemp.x = dotproduct4 (quatfrom, quatto)
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[y]  ;
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[z]  ;
   add.x          vclsmlftemp, vclsmlftemp, vclsmlftemp[w]  ;

   loi            -1.0                                      ;

   move           QSLquatto1, quatto                       ; We want to keep the angle between the 2 vectors <= PI
   iaddiu         vclsmlitemp, vi00, 0x80                   ; (vclsmlftemp >= 0.0)
   fmand          vclsmlitemp, vclsmlitemp                  ;
   ibeq           vclsmlitemp, vi00, QSL1@                 ;

   mul.x          vclsmlftemp, vclsmlftemp, i               ; Inverse the "quatto" vector, so the angle between the
   mul            QSLquatto1, QSLquatto1, i                 ; 2 vectors will become < PI

QSL1@:
   loi            0.999                                     ; If the angle is too small, we gotta do a linear
   sub.x          vf00, vclsmlftemp, i                      ; interpolation, because the slerp will fail
   iaddiu         vclsmlitemp, vi00, 0x80                   ; (if (0.999 - vclsmlftemp.x < 0.0) it is too small)
   fmand          vclsmlitemp, vclsmlitemp                  ;
   ibne           vclsmlitemp, vi00, QSL2@                 ;

   add.x          vclsmlftemp, vf00, qtime                 ; vclsmlftemp.x = qtime.x
   add.z          vclsmlftemp, vf00, vf00[w]                ; vclsmlftemp.z = 1.0 - qtime.x
   sub.z          vclsmlftemp, vclsmlftemp, qtime[x]       ;

   b              QSL3@                                    ; Go straight to the weighting part

QSL2@:
   Arccos         vclsmlftemp, QSLangle                     ; QSLangle.x = acos (vclsmlftemp.x)
   AngleSinCos    QSLangle, QSLsinom                        ; QSLsinom.x = sin (QSLangle.x)

   add.z          qtime, vf00, vf00[w]                     ; qtime.z = 1.0 - qtime.x
   sub.z          qtime, qtime, qtime[x]                 ;

   mul.xz         vclsmlftemp, qtime, QSLangle[x]          ; vclsmlftemp.x = sin (time * QSLangle)
   AngleSinCos    vclsmlftemp, vclsmlftemp                  ; vclsmlftemp.z = sin ((1.0 - time) * QSLangle)

   div            q, vf00[w], QSLsinom[x]                   ; vclsmlftemp.x = vclsmlftemp.x / QSLsinom.x
   mul.xz         vclsmlftemp, vclsmlftemp, q               ; vclsmlftemp.z = vclsmlftemp.z / QSLsinom.x

QSL3@:
   mul            acc, quatfrom, vclsmlftemp[z]            ; Now that we have the src and dest scale (0.0 - 1.0), get the final result
   madd           quatdest, QSLquatto1, vclsmlftemp[x]     ;

#endmacro

;//--------------------------------------------------------------------
;// QuaternionToMatrix - Converts a quaternion rotation to a matrix
;// Thanks to Colin Hughes (SCEE) for that one
;//
;// Note: ACC and I registers are modified
;//--------------------------------------------------------------------
#macro QuaternionToMatrix: matresult, quaternion
   mula.xyz       ACC,  quaternion, quaternion            ; xx yy zz

   loi            1.414213562
   muli           vclsmlftemp, quaternion, I               ; x sqrt2 y sqrt2 z sqrt2 w sqrt2

   mr32.w         matresult[0], vf00                       ; Set rhs matrix line 0 to 0
   mr32.w         matresult[1], vf00                       ;
   mr32.w         matresult[2], vf00                       ; Set rhs matrix
   move           matresult[3], vf00                       ; Set bottom line to 0 0 0 1

   madd.xyz       vcl_2qq, quaternion, quaternion         ; 2xx       2yy       2zz
   addw.xyz       Vector111, vf00, vf00                     ; 1         1         1         -

   opmula.xyz     ACC,  vclsmlftemp, vclsmlftemp            ; 2yz       2xz       2xy       -
   msubw.xyz      vclsmlftemp2, vclsmlftemp, vclsmlftemp    ; 2yz-2xw   2xz-2yz   2xy-2zw   -
   maddw.xyz      vclsmlftemp3, vclsmlftemp, vclsmlftemp    ; 2yz+2xw   2xz+2yz   2xy+2zw   -
   addaw.xyz      ACC,  vf00, vf00                          ; 1         1         1         -
   msubax.yz      ACC,  Vector111, vcl_2qq                  ; 1         1-2xx     1-2xx

   msuby.z        matresult[2], Vector111, vcl_2qq         ; -         -         1-2xx-2yy -
   msubay.x       ACC, Vector111, vcl_2qq                   ; 1-2yy     1-2xx     1-2xx-2yy -
   msubz.y        matresult[1], Vector111, vcl_2qq         ; -         1-2xx-2zz -         -
   mr32.y         matresult[0], vclsmlftemp2
   msubz.x        matresult[0], Vector111, vcl_2qq         ; 1-2yy-2zz -         -         -
   mr32.x         matresult[2], vclsmlftemp2
   addy.z         matresult[0], vf00, vclsmlftemp3
   mr32.w         vclsmlftemp, vclsmlftemp2
   mr32.z         matresult[1], vclsmlftemp
   addx.y         matresult[2], vf00, vclsmlftemp3
   mr32.y         vclsmlftemp3, vclsmlftemp3
   mr32.x         matresult[1], vclsmlftemp3

#endmacro

;//--------------------------------------------------------------------
;// QuaternionMultiply - Multiplies "quaternion1" and "quaternion2",
;// and puts the result in "quatresult".
;// Thanks to Colin Hughes (SCEE) for that one
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro QuaternionMultiply: quatresult, quaternion1, quaternion2
   mul            vclsmlftemp, quaternion1, quaternion2   ; xx yy zz ww

   opmula.xyz     ACC,         quaternion1, quaternion2   ; Start Outerproduct
   madd.xyz       ACC,         quaternion1, quaternion2[w]; Add w2.xyz1
   madd.xyz       ACC,         quaternion2, quaternion1[w]; Add w1.xyz2
   opmsub.xyz     quatresult, quaternion2, quaternion1   ; Finish Outerproduct

   sub.w          ACC,         vclsmlftemp,  vclsmlftemp[z] ; ww - zz
   msub.w         ACC,         vf00,         vclsmlftemp[y] ; ww - zz - yy
   msub.w         quatresult, vf00,         vclsmlftemp[x] ; ww - zz - yy - xx
#endmacro

;//--------------------------------------------------------------------
;// TriangleWinding - Compute winding of triangle relative to "eyepos"
;// result is nonzero if winding is CW (actually depends on your
;// coordinate system)
;// Thanks to David Etherton (Angel Studios) for that one
;//
;// Note: ACC register is modified
;//--------------------------------------------------------------------
#macro TriangleWinding:  result, vert1, vert2, vert3, eyepos
   sub.xyz        tw_vert12, vert2, vert1
   sub.xyz        tw_vert13, vert3, vert1

   opmula.xyz     ACC,       tw_vert12, tw_vert13
   opmsub.xyz     tw_normal, tw_vert13, tw_vert12

   sub.xyz        tw_dot, eyepos, vert1

   mul.xyz        tw_dot, tw_dot, tw_normal
   add.x          tw_dot, tw_dot, tw_dot[y]
   add.x          tw_dot, tw_dot, tw_dot[z]

   fsand          result,   0x2
#endmacro

;//--------------------------------------------------------------------
;// STATUSFLAGS_BGTZ - Branch if status shows "greater than zero".
;// Thanks to David Etherton (Angel Studios) for that one
;//--------------------------------------------------------------------
#macro STATUSFLAGS_BGTZ: label
   fsand          vclsmlitemp, 0x3              ; NEG | ZERO
   ibeq           vclsmlitemp, VI00, label     ; Jump if NEITHER NEG NOR ZERO
#endmacro

;//--------------------------------------------------------------------
;// STATUSFLAGS_BGEZ - Branch if status shows "greater or equal to
;// zero".
;// Thanks to David Etherton (Angel Studios) for that one
;//--------------------------------------------------------------------
#macro STATUSFLAGS_BGEZ: label
   fsand          vclsmlitemp, 0x2              ; NEG
   ibeq           vclsmlitemp, VI00, label     ; Jump if NOT NEG
#endmacro

;//--------------------------------------------------------------------
;// STATUSFLAGS_BLEZ - Branch if status shows "less or equal to zero".
;// Thanks to David Etherton (Angel Studios) for that one
;//--------------------------------------------------------------------
#macro STATUSFLAGS_BLEZ: label
   fsand          vclsmlitemp, 0x3              ; NEG | ZERO
   ibne           vclsmlitemp, VI00, label     ; Jump if NEG OR ZERO
#endmacro

;//--------------------------------------------------------------------
;// STATUSFLAGS_BLTZ - Branch if status shows "less than zero".
;// Thanks to David Etherton (Angel Studios) for that one
;//--------------------------------------------------------------------
#macro STATUSFLAGS_BLTZ: label
   fsand          vclsmlitemp, 0x2              ; NEG
   ibne           vclsmlitemp, VI00, label     ; Jump if NEG
#endmacro

;//---------------------------------------------------------
;// DistanceVertexToPlane - Return the distance between a
;// homegeneous vertex "vertex" and a plane (described as
;// ABCD) "plane", and the result is returned in "distance".
;//---------------------------------------------------------
#macro DistanceVertexToPlane: distance, vertex, plane
   mul            distance, vertex,   plane
   add.x          distance, distance, distance[y]
   add.x          distance, distance, distance[z]
   add.x          distance, distance, distance[w]
#endmacro

;//---------------------------------------------------------
;// ResetClipFlags - 
;// VCL won't let us use CLIP without first zeroing
;//---------------------------------------------------------
#macro ResetClipFlags
   fcset   0x000000
#endmacro

;//---------------------------------------------------------
;// PerformClipCheck - checks if the vertex is outside the viewing frustum. 
;// If it is, then the appropriate clipping flags are set.
;// 2 - Bitwise AND the clipping flags with 0x3FFFF, this makes sure that 
;//     we get the clipping judgement for the last three verts 
;//     (i.e. that make up the triangle we are about to draw)
;// 3 - Add 0x7FFF. If any of the clipping flags were set this will
;//     cause the triangle not to be drawn (any values above 0x8000
;//     that are stored in the w component of XYZ2 will set the ADC
;//     bit, which tells the GS not to perform a drawing kick on this
;//     triangle.
;//---------------------------------------------------------
#macro PerformClipCheck: t_vertex, t_destAddress, t_destAddressOffset
   clipw.xyz	t_vertex,   t_vertex	
   fcand       VI01,       0x3FFFF
   iaddiu      adcBit,     VI01, 0x7FFF
   isw.w       adcBit,     t_destAddressOffset(t_destAddress)
#endmacro

;//---------------------------------------------------------
;// ScaleVertexToGSFormat - Scale vertex to GS screen space
;// 2 - Multiply and add the scales -> vert = vert * scale + scale
;// 3 - Convert vertex to 12:4 fixed point format
;//---------------------------------------------------------
#macro ScaleVertexToGSFormat: t_scale, t_vertex
   mula.xyz    acc,        t_scale,    vf00[w]
   madd.xyz    t_vertex,   t_vertex,   t_scale
   ftoi4.xyz   t_vertex,   t_vertex
#endmacro

;//---------------------------------------------------------
;// PerformTexturePerspectiveCorrection
;//---------------------------------------------------------
#macro PerformTexturePerspectiveCorrection: t_resultStq, t_stq
   mulq  t_resultStq,  t_stq,  q
#endmacro

;//---------------------------------------------------------
;// FixColor - Clamp colors to 0-255 and convert to integer
;//
;// 1. Load 255 and put it into the alpha value
;// 2. Clamp result to 0-255 values
;// 3. And write to the output buffer
;//---------------------------------------------------------
#macro FixColor: t_color
   loi         255
   mini.xyz    t_color, t_color, i
   max.xyz     t_color, t_color, vf00[x]
   ftoi0       t_color, t_color
#endmacro
