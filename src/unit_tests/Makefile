EE_BIN = unit_tests.elf
EE_LIBS = -ltyra
EE_OBJS =					\
	tests/utils/math.o		\
	main.o

all: $(EE_BIN)
	$(EE_STRIP) --strip-all $(EE_BIN)
	mv $(EE_BIN) bin/$(EE_BIN)
	rm $(EE_OBJS)

rebuild-engine: 
	cd $(TYRA)/src/engine && make clean && make EE_CXXFLAGS="-DNDEBUG $(EE_CXXFLAGS)"

rebuild-dbg-engine: 
	cd $(TYRA)/src/engine && make clean && make

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
	$(WSL_LINUX_PCSX2)/pcsx2.exe --elf=$(WSL_MAKE_WINDOWS)\\repos\\tyra\\src\\unit_tests\\bin\\unit_tests.elf

show:
	cat bin/test-result.txt

include $(TYRA)/src/engine/Makefile.pref
