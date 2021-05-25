EE_BIN = floors.elf

TYRA_DIR = ./../../engine

EE_OBJS =											\
	managers/light_manager.o						\
	managers/floor_manager.o						\
	camera.o										\
	objects/enemy.o									\
	objects/floor.o									\
	objects/player.o								\
	ui.o											\
	utils.o											\
	floors.o										\
	main.o

EE_LIBS = -ltyra 

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)

rebuild-engine: 
	cd $(TYRA_DIR) && make clean && make

rebuild-dbg-engine: 
	cd $(TYRA_DIR) && make clean && make debug

clean:
	rm -f $(EE_OBJS)

run: $(EE_BIN)
	killall -v ps2client || true
	ps2client reset
	ps2client reset
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)
	cd bin/ && ps2client execee host:$(EE_BIN)

run-pcsx2:
	taskkill.exe /f /t /im pcsx2.exe || true
	$(PCSX2_PATH) --elf=$(WSL_TYRA_PATH)\\src\\samples\\floors\\bin\\floors.elf

include $(TYRA_DIR)/Makefile.pref
