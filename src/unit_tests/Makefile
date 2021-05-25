EE_BIN = unit_tests.elf

TYRA_DIR = ./../engine
PCSX2_PATH = /mnt/c/Program\ Files\ \(x86\)/PCSX2/pcsx2.exe
WSL_TYRA_PATH = \\\\wsl$$\\PS2-DEV\\repos\\tyra

EE_OBJS =					\
	tests/utils/math.o		\
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

run-pcsx2:
	taskkill.exe /f /t /im pcsx2.exe || true
	$(PCSX2_PATH) --elf=$(WSL_TYRA_PATH)\\src\\unit_tests\\bin\\unit_tests.elf

show:
	cat bin/test-result.txt

include $(TYRA_DIR)/Makefile.pref
