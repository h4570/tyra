TYRA_DIR = ./

EE_OBJS =								\
	models/math/matrix.o				\
	models/math/plane.o					\
	models/math/point.o					\
	models/math/vector3.o				\
	models/mesh_frame.o					\
	models/bounding_box.o				\
	models/mesh_material.o				\
	models/mesh.o						\
	models/sprite.o						\
	models/texture.o					\
	modules/audio.o						\
	modules/camera_base.o				\
	modules/file_service.o				\
	modules/gif_sender.o				\
	modules/light.o						\
	modules/pad.o						\
	modules/renderer.o					\
	modules/texture_repository.o		\
	modules/timer.o						\
	modules/vif_sender.o				\
	utils/math.o						\
	utils/string.o						\
	loaders/bmp_loader.o				\
	loaders/dff_loader.o				\
	loaders/md_loader.o				\
	loaders/obj_loader.o				\
	loaders/png_loader.o				\
	vu1_progs/draw3D.o					\
	engine.o

all: $(EE_OBJS) 
	ar rcs $(LIB_NAME) $(EE_OBJS)
	rm -f $(EE_OBJS)

# %.vcl: %.vclpp
#	 $(EE_VCLPP) $< $@

# %.vsm: %.vcl
#	$(EE_VCL) $< >> $@

%.o: %.vsm
	$(EE_DVP) $< -o $@

clean:
	rm -f $(EE_OBJS)
	rm -f $(LIB_NAME)

include Makefile.pref