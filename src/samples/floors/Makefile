EE_BIN = floors.elf

EE_OBJS =											\
	../../engine/models/math/matrix.o				\
	../../engine/models/math/plane.o				\
	../../engine/models/math/point.o				\
	../../engine/models/math/vector3.o				\
	../../engine/models/mesh_frame.o				\
	../../engine/models/mesh_material.o				\
	../../engine/models/mesh.o						\
	../../engine/models/mesh_texture.o				\
	../../engine/modules/audio.o					\
	../../engine/modules/camera_base.o				\
	../../engine/modules/file_service.o				\
	../../engine/modules/gif_sender.o				\
	../../engine/modules/light.o					\
	../../engine/modules/pad.o						\
	../../engine/modules/renderer.o					\
	../../engine/modules/texture_repository.o		\
	../../engine/modules/timer.o					\
	../../engine/modules/vif_sender.o				\
	../../engine/utils/math.o						\
	../../engine/utils/string.o						\
	../../engine/loaders/bmp_loader.o				\
	../../engine/loaders/dff_loader.o				\
	../../engine/loaders/md2_loader.o				\
	../../engine/loaders/obj_loader.o				\
	../../engine/vu1_progs/draw3D.o					\
	../../engine/engine.o							\
	managers/light_manager.o						\
	managers/floor_manager.o						\
	camera.o										\
	objects/floor.o									\
	objects/player.o								\
	utils.o											\
	floors.o										\
	main.o

EE_LIBS = -ldraw -lgraph -lmath3d -lpacket -ldma -lpad -lpacket2 -laudsrv -lc -lstdc++
EE_INCS := -I./../../engine/include $(EE_INCS)
EE_DVP = dvp-as
EE_VCL = openvcl

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)

%.vsm: %.vcl
	$(EE_VCL) $< >> $@

%.o: %.vsm
	$(EE_DVP) $< -o $@

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

run: $(EE_BIN)
	killall -v ps2client || true
	ps2client reset
	ps2client reset
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)
	cd bin/ && ps2client execee host:$(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
