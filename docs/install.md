# Installation tutorial

This tutorial will show you, how to:

- Setup your WSL machine for PS2DEV
- Install entire PS2DEV+Tyra environment
- Setup VSCode with Intelisense
- Setup PCSX2

So let's get started!

---

<br/>

## VSCode and WSL

- Install VSCode from
  https://code.visualstudio.com/
- Install WSL https://docs.microsoft.com/en-us/windows/wsl/install-win10 `Info: Please choose Ubuntu-20.04`
- Set root as default WSL user.
  Open ` cmd.exe`  and type:
  ` ubuntu2004 config --default-user root`

---

<br/>

## Ubuntu configuration and PS2DEV+Tyra

- Open WSL terminal (Windows search -> wsl)`Info: Be sure that are you logged as a root!`
- Type these commands

```
apt-get update && apt-get upgrade -y
apt-get install -y build-essential make texinfo cmake bison flex gettext libgmp3-dev libmpfr-dev libmpc-dev libz-dev wget patch
```

- Configure git (replace your name and email)

```
git config --global user.email "your@email.com"
git config --global user.name "Your Name"
```

- Create directories

```
mkdir /ps2dev
mkdir /ps2dev/built
mkdir /ps2dev/source
mkdir /repos
mkdir /repos/tyra
```

- Clone repos

```
git clone https://github.com/ps2dev/ps2dev.git /ps2dev/source/
git clone https://github.com/h4570/tyra.git /repos/tyra/
```

- Open bashrc via `nano ~/.bashrc` and add these lines at the end of file. Be sure that they are 1:1, so don't add additional spaces!

```
export PS2DEV=/ps2dev/built
export PS2SDK=$PS2DEV/ps2sdk
export GSKIT=$PS2DEV/gsKit
export PATH=$PATH:$PS2DEV/bin:$PS2DEV/ee/bin:$PS2DEV/iop/bin:$PS2DEV/dvp/bin:$PS2SDK/bin
export PS2HOSTNAME=192.168.0.200
export TYRA=/repos/tyra
export WSL_MAKE_WINDOWS=\\\\\\\\wsl\$\$\\\\Ubuntu-20.04
export WSL_LINUX_PCSX2=/mnt/c/Program\\\ Files\\\ \\\(x86\\\)/PCSX2
```

- Refresh env variables via -> `. ~/.bashrc`
- Close wsl terminal

---

<br/>

## Mapping WSL directories to your Windows machine

- Open cmd and type `wsl.exe --list``Info: Remember your distribution name from this list. My is "Ubuntu-20.04"`
- Click at the windows logo on your keyboard, type "This PC" and open
- At the upper menu click "Map disk drive"`Info: Replace my name with your WSL name!`
  - Folder: \\\\wsl$\\Ubuntu-20.04
  - Click OK

`Info: You will have now new disk on your PC, this is the WSL filesystem.`

---

<br/>

## Configuring VSCode

- Open VSCode
- Install extension "Remote - WSL"
- Install extension "C/C++"
- Restart VSCode if required
- VSCode: Click on "File" -> "Open folder in WSL"
  - Open "repos/tyra"
- VSCode: Click on "Terminal" -> "New terminal"

`Info: New terminal should appear. This is the same WSL terminal :)`

---

<br/>

## PS2DEV installation

- In VSCode's WSL terminal type:

```
cd /ps2dev/source
unset ZLIB
unset LIBTIFF
unset LIBJPEG
unset LIBPNG
./build-all.sh
```

`Info: This can take some time...`

- And open again .bashrc via `nano ~/.bashrc` and add these lines at the end of file.

```
export ZLIB=${PS2SDK}/ports
export LIBTIFF=${PS2SDK}/ports
export LIBJPEG=${PS2SDK}/ports
export LIBPNG=${PS2SDK}/ports
```

- Refresh env variables via `. ~/.bashrc`

### Please don't forgot to remove/refresh these 4 exports when you will want to update PS2DEV (./build-all.sh), because there is circular dependency between these libs and gsKit which is included in PS2DEV!!!

---

<br/>

## Build Tyra

- Just hit make all on the root of tyra folder or you can go via `cd /src/engine` and hit `make all` there.
- If you wanna build the debug build or rebuild the engine, just `make rebuild-engine ` on the root of tyra or `make rebuild-dbg-engine` to build debug version or go to one of the samples via `cd /repos/tyra/src/samples/floors `and do the same process above

<br/>

## Install & configure PCSX2

- Download & install PCSX2 from (1.6+ version)https://pcsx2.net/
- Extract bios files fromhttps://romsmania.cc/bios/pcsx2-playstation-2-bios-3to `C:\Users%USERNAME%\Documents\PCSX2\bios`
- Close `PCSX2` if you have opened!
- Open `C:\Users\hasto\Documents\PCSX2\inis\PCSX2_vm.ini` in notepad

```
HostFs=enabled
```

- Open PCSX2 and map pad to your keyboard, for example:

```
WASD = Left joy
Arrows = Right joy
Space = X
Circle = C
```

---

<br/>

## Run one of the samples - Method 1

- Add assets for "floors" sample
- Download assets from https://github.com/h4570/tyra/tree/master/src/samples/floors
- Windows: Open your WSL folder (This PC -> WSL folder)
- Extract assets to /repos/tyra/src/samples/floors/bin/ (create bin folder if not exists)
- Compile sample via `make``Info: Elf file should be produced in /bin `
- Run sample in PCSX2 via `make run-pcsx2`

## Run one of the samples Method 2

- Run `make floors` on the root of tyra project or change directory to the sampels via `cd src/samples/floors` and do the `make install` command

---

<br/>

## How to code?

Do some changes in your sample and just type `make clean all run-pcsx2` in your sample folder.
You can create your own sample, by just copying cube folder.

---

<br/>

## Setup Intelisense

- Download
  http://apgcglz.cluster028.hosting.ovh.net/tyra/intellisense.zip
- Unpack .vscode folder to WSL's `/repos/tyra/Info: This configuration is set for cube sample, but you can change it in tasks.json file`

---

<br/>

## Advanced: Compiling VU1 programs

`Info: Based on https://github.com/microsoft/wsl/issues/2468#issuecomment-374904520`

- Download VCL
  http://lukasz.dk/files/vcl.1.4.beta7.x86.tgz
- Rename `vcl_14beta7_x86` binary to `vcl`
- Put `vcl` to WSL's `/usr/bin`

```
apt-get install qemu qemu-user-static binfmt-support
update-binfmts --install i386 /usr/bin/qemu-i386-static --magic '\x7fELF\x01\x01\x01\x03\x00\x00\x00\x00\x00\x00\x00\x00\x03\x00\x03\x00\x01\x00\x00\x00' --mask '\xff\xff\xff\xff\xff\xff\xff\xfc\xff\xff\xff\xff\xff\xff\xff\xff\xf8\xff\xff\xff\xff\xff\xff\xff'
dpkg --add-architecture i386
apt-get update
apt-get install libstdc++5:i386
chmod 755 /usr/bin/vcl
```

`Info: In Ubuntu 18.04 I was obligated to install lib32ncurses5 also`

- Add this line to `~/.bashrc`

```
service binfmt-support start &>/dev/null
```

- Uncomment `.vsm`/`.vcl` recipe from Tyra's engine `Makefile`
- Download & install VCL preprocessor

```
mkdir temp && cd temp
git clone https://github.com/glampert/vclpp .
make
cp vclpp /usr/bin/
chmod 755 /usr/bin/vclpp
cd .. && rm -rf ./temp
```

- Uncomment `.vclpp`/`.vcl` recipe from Tyra's engine `Makefile`
