EE_BIN = ari.elf

EE_OBJS =											\
	../../engine/models/math/matrix.o				\
	../../engine/models/math/plane.o				\
	../../engine/models/math/point.o				\
	../../engine/models/math/vector3.o				\
	../../engine/models/dff_model.o					\
	../../engine/models/md2_model.o					\
	../../engine/models/mesh_frame.o				\
	../../engine/models/mesh_material.o				\
	../../engine/models/mesh.o						\
	../../engine/models/mesh_texture.o				\
	../../engine/models/obj_model.o					\
	../../engine/modules/audio.o					\
	../../engine/modules/camera_base.o				\
	../../engine/modules/gif_sender.o				\
	../../engine/modules/light.o					\
	../../engine/modules/pad.o						\
	../../engine/modules/renderer.o					\
	../../engine/modules/timer.o					\
	../../engine/modules/vif_sender.o				\
	../../engine/modules/vu1.o						\
	../../engine/utils/math.o						\
	../../engine/utils/string.o						\
	../../engine/loaders/bmp_loader.o				\
	../../engine/loaders/dff_loader.o				\
	../../engine/loaders/md2_loader.o				\
	../../engine/loaders/obj_loader.o				\
	../../engine/vu1_progs/draw3D.o					\
	../../engine/engine.o							\
	camera.o										\
	objects/player.o								\
	ari.o											\
	main.o

EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -laudsrv -lc -lstdc++
EE_INCS := -I./../../engine/include $(EE_INCS)
EE_DVP = dvp-as
EE_VCL = vcl

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)

# https://github.com/microsoft/wsl/issues/2468#issuecomment-374904520
#%.vsm: %.vcl # sudo service binfmt-support start
#	$(EE_VCL) $< >> $@

%.o: %.vsm
	$(EE_DVP) $< -o $@

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
