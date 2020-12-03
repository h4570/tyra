EE_BIN = dolphin.elf

TYRA_DIR = ./../../engine

EE_OBJS =											\
	camera.o										\
	objects/player.o								\
	dolphin.o										\
	main.o

EE_LIBS = -ltyra 

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)

rebuild-engine: 
	cd $(TYRA_DIR) && make clean && make

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

include $(TYRA_DIR)/Makefile.pref
