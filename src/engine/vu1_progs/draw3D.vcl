; ______       ____   ___
;   |     \/   ____| |___|    
;   |     |   |   \  |   |       
;---------------------------
; Copyright 2020, tyra - https://github.com/h4570/tyra
; Sandro Sobczyński <sandro.sobczynski@gmail.com>
;
;---------------------------------------------------------------
; draw3D.vcl                                                   |
;---------------------------------------------------------------
; A VU1 microprogram to draw 3D object using XYZ2, RGBAQ and ST|
; This program uses double buffering (xtop)                    |
;                                                              |
; Many thanks to:                                              |
; - Dr Henry Fortuna                                           |
; - Jesper Svennevid, Daniel Collin                            |
; - Guilherme Lampert                                          |
;---------------------------------------------------------------

; TODO
; - Fix vertex clipping (is clipping visible triangles :/)
; - Move lerp() from EE to VU1 (performance)
; - Add dynamic lighting (feature)
; - Add --cont + MSCNT instead of alltime MSCAL (performance)

.syntax new
.name VU1Draw3D
.vu
.init_vf_all
.init_vi_all

--enter
--endenter

    ;//////////// --- Load data 1 --- /////////////
    ; Updated once per frame
    lq      matrixRow0,     0(vi00) ; load view-projection matrix
    lq      matrixRow1,     1(vi00)
    lq      matrixRow2,     2(vi00)
    lq      matrixRow3,     3(vi00)
    ;/////////////////////////////////////////////

	fcset   0x000000    ; VCL won't let us use CLIP without first zeroing
                        ; the clip flags

    ;//////////// --- Load data 2 --- /////////////
    ; Updated dynamically
    xtop    iBase

    lq.xyz  scale,          0(iBase) ; load program params
                                     ; float : X, Y, Z - scale vector that we will use to scale the verts after projecting them.
                                     ; float : W - vert count.
    lq      gifSetTag,      1(iBase) ; GIF tag - set
    lq      texGifTag1,     2(iBase) ; GIF tag - texture LOD
    lq      texGifTag2,     3(iBase) ; GIF tag - texture buffer & CLUT
    lq      primTag,        4(iBase) ; GIF tag - tell GS how many data we will send
    lq      rgba,           5(iBase) ; RGBA
    ;lq      clipScale,      6(iBase) ; TODO clipping tests
                                     ; u32 : R, G, B, A (0-128)
    iaddiu  vertexData,     iBase,      7           ; pointer to vertex data
    ilw.w   vertCount,      0(iBase)                ; load vert count from scale vector
    iadd    stqData,        vertexData, vertCount   ; pointer to stq
    iadd    kickAddress,    stqData,    vertCount   ; pointer for XGKICK
    iadd    destAddress,    stqData,    vertCount   ; helper pointer for data inserting
    ;////////////////////////////////////////////

    ;/////////// --- Store tags --- /////////////
    sqi gifSetTag,  (destAddress++) ;
    sqi texGifTag1, (destAddress++) ; texture LOD tag
    sqi gifSetTag,  (destAddress++) ;
    sqi texGifTag2, (destAddress++) ; texture buffer & CLUT tag
    sqi primTag,    (destAddress++) ; prim + tell gs how many data will be
    ;////////////////////////////////////////////

    ;/////////////// --- Loop --- ///////////////
    iadd vertexCounter, iBase, vertCount ; loop vertCount times
    vertexLoop:

        ;////////// --- Load loop data --- //////////
        lq vertex, 0(vertexData)  ; load xyz
                                    ; float : X, Y, Z
                                    ; any32 : _ = 0
        lq stq,    0(stqData)     ; load stq
                                    ; float : S, T
                                    ; any32 : Q = 1     ; 1, because we will mul this by 1/vert[w] and this
                                                        ; will be our q for texture perspective correction
                                    ; any32 : _ = 0                       
        ;////////////////////////////////////////////    


        ;////////////// --- Vertex --- //////////////
        mul         acc,    matrixRow0, vertex[x]   ; transform each vertex by the matrix
        madd        acc,    matrixRow1, vertex[y]
        madd        acc,    matrixRow2, vertex[z]
        madd        vertex, matrixRow3, vertex[w]

        ;add.z		acc, vf0, clipScale[w] ; TODO clipping maybe this?
        ;madd.z		clipVec, clipScale, vertex
        ;mul.xy		clipVec, clipScale, vertex
        ;mul.w		clipVec, vf0, vertex[z]
            
        ;mul.xyz         clipVec,    vertex, clipScale ; TODO clipping maybe this?
       
        clipw.xyz	vertex, vertex                  ; Dr. Fortuna: This instruction checks if the vertex is outside
                                                    ; the viewing frustum. If it is, then the appropriate
                                                    ; clipping flags are set
        fcand		VI01,   0x3FFFF                 ; Bitwise AND the clipping flags with 0x3FFFF, this makes
                                                    ; sure that we get the clipping judgement for the last three
                                                    ; verts (i.e. that make up the triangle we are about to draw)
        iaddiu		iADC,   VI01,       0x7FFF      ; Add 0x7FFF. If any of the clipping flags were set this will
                                                    ; cause the triangle not to be drawn (any values above 0x8000
                                                    ; that are stored in the w component of XYZ2 will set the ADC
                                                    ; bit, which tells the GS not to perform a drawing kick on this
                                                    ; triangle.
        
        ;ilw.w		iNoDraw, UVStart(Counter)		; Load the iNoDraw flag. If true we should set the ADC bit so the vert isn't drawn
	    ;iadd		iADC, iADC, iNoDraw ; TODO clipping maybe this?

        isw.w		iADC,   2(destAddress)
        
        div         q,      vf00[w],    vertex[w]   ; perspective divide (1/vert[w]):
        mul.xyz     vertex, vertex,     q
        mula.xyz    acc,    scale,      vf00[w]     ; scale to GS screen space
        madd.xyz    vertex, vertex,     scale       ; multiply and add the scales -> vert = vert * scale + scale
        ftoi4.xyz   vertex, vertex                  ; convert vertex to 12:4 fixed point format
        ;////////////////////////////////////////////


        ;//////////////// --- ST --- ////////////////
        mulq modStq, stq, q
        ;////////////////////////////////////////////


        ;//////////// --- Store data --- ////////////
        sq modStq,      0(destAddress)      ; STQ
        sq rgba,        1(destAddress)      ; RGBA ; q is grabbed from stq
        sq.xyz vertex,  2(destAddress)      ; XYZ2
        ;////////////////////////////////////////////

        iaddiu          vertexData,     vertexData,     1                         
        iaddiu          stqData,        stqData,        1  
        iaddiu          destAddress,    destAddress,    3

        iaddi   vertexCounter,  vertexCounter,  -1 ; decrement the loop counter 
        ibne    vertexCounter,  iBase,   vertexLoop ; and repeat if needed

    ;//////////////////////////////////////////// 

    --barrier

    xgkick kickAddress ; dispatch to the GS rasterizer.

--exit
--endexit
