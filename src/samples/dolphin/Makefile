# Project settings

DIR_NAME = dolphin
EE_LIBS = -ltyra
EE_OBJS =											\
	camera.o										\
	objects/player.o								\
	objects/collectible.o							\
	objects/mine.o									\
	dolphin.o										\
	main.o

# ----------------
# Other 

EE_BIN = $(DIR_NAME).elf

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)

rebuild-engine: 
	cd $(TYRA)/src/engine && make clean && make EE_CXXFLAGS="-DNDEBUG $(EE_CXXFLAGS)"

rebuild-dbg-engine: 
	cd $(TYRA)/src/engine && make clean && make

clean:
	rm -f $(EE_OBJS)

run:
	killall -v ps2client || true
	ps2client reset
	ps2client reset
	cd bin/ && ps2client execee host:$(EE_BIN)

run-pcsx2:
	taskkill.exe /f /t /im pcsx2.exe || true
	$(WSL_LINUX_PCSX2)/pcsx2.exe --elf=$(WSL_MAKE_WINDOWS)\\repos\\tyra\\src\\samples\\$(DIR_NAME)\\bin\\$(EE_BIN)

include $(TYRA)/src/engine/Makefile.pref
