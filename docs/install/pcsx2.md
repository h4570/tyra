## PCSX2 preparation

- Download PCSX2 from https://pcsx2.net/
- Install
- Extract bios files from https://romsmania.cc/bios/pcsx2-playstation-2-bios-3 to `%UserProfile%\Documents\PCSX2\bios`
- Map pad to your keyboard, for example:

```
WASD = Left joy
Arrows = Right joy
Space = X
Circle = C
```

### For PCSX2 1.6.X
- Close `PCSX2` if you have opened!
- Open `%UserProfile%\Documents\PCSX2\inis\PCSX2_vm.ini` in notepad and change HostFs option:

```
HostFs=enabled
```

### For PCSX2 1.7.X
- Open `PCSX2` 
- Go to Settings -> Emulation -> Enable `Host file system`