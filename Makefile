# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2021, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# André Guilherme <andregui17@outlook.com>

TYRA_DIR = ./

LIB_NAME = libtyra.a

# ENGINE OBJECTS 
ENGINE_OBJS = src/engine/engine.o \
              src/engine/modules/audio.o \
              src/engine/modules/camera_base.o \
              src/engine/modules/file_service.o \
	      src/engine/modules/gif_sender.o \
              src/engine/modules/light.o \
	      src/engine/modules/pad.o \
	      src/engine/modules/renderer.o \
	      src/engine/modules/texture_repository.o \
	      src/engine/modules/timer.o \
              src/engine/modules/vif_sender.o \
	      src/engine/modules/video.o \
	      src/engine/models/math/matrix.o \
	      src/engine/models/math/plane.o \
	      src/engine/models/math/point.o \
	      src/engine/models/math/vector3.o \
	      src/engine/models/mesh_frame.o \
	      src/engine/models/bounding_box.o \
	      src/engine/models/mesh_material.o \
	      src/engine/models/mesh.o \
	      src/engine/models/sprite.o \
	      src/engine/models/texture.o \
	      src/engine/utils/math.o \
	      src/engine/utils/string.o \
              src/engine/loaders/bmp_loader.o \
              src/engine/loaders/dff_loader.o \
	      src/engine/loaders/md2_loader.o \
	      src/engine/loaders/obj_loader.o \
	      src/engine/loaders/png_loader.o \
	      src/engine/vu1_progs/draw3D.o \

EE_LIBS := $(EE_LIBS) -ldraw -lcdvd -lgraph -lmath3d -lpacket -ldma -lpacket2 -lpad -laudsrv -lc -lstdc++ -lpng -lz

EE_INCS := -I$(TYRA)/src/engine/include -I$(PS2SDK)/ports/include $(EE_INCS)

EE_LDFLAGS := -L$(PS2SDK)/ports/lib -L$(TYRA)/src/engine $(EE_LDFLAGS)

EE_CXXFLAGS := -DHAVE_LIBJPEG -DHAVE_LIBTIFF -DHAVE_LIBPNG -DHAVE_ZLIB -D_XCDVD $(EE_CXXFLAGS)

EE_DVP = dvp-as

EE_VCL = vcl

EE_VCLPP = vclpp

all: $(ENGINE_OBJS) 
	ar rcs $(LIB_NAME) $(ENGINE_OBJS)
	cp -f $(LIB_NAME) $(PS2SDK)/ee/lib
	rm -f $(ENGINE_OBJS)

# Cube example
cube:
	$(MAKE) -C src/samples/cube all clean

# Dolphin example
dolphin:
	$(MAKE) -C src/samples/dolphin all clean

# Floors example
floors:
	$(MAKE) -C src/samples/floors all clean

# Rebuild the engine
rebuild-engine: 
	$(MAKE) -C src/engine && make && make EE_CXXFLAGS="-DNDEBUG $(EE_CXXFLAGS)"

# Rebuild debug engine
rebuild-dbg-engine: 
	$(MAKE) -C src/engine && make && make

# Unit tests folder
tests: 
	$(MAKE) -C src/unit_tests all clean

# %.vcl: %.vclpp
#	 $(EE_VCLPP) $< $@

# %.vsm: %.vcl
#	$(EE_VCL) $< >> $@

%.o: %.vsm
	$(EE_DVP) $< -o $@

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
