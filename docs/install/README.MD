# Installing Tyra

[![](https://img.shields.io/badge/Install%20time-5%20minutes-brightgreen)](#)

## Prerequisites:
1. Git
2. VSCode
3. Docker with PowerShell as a default terminal
4. [Configured PCSX2](https://github.com/h4570/tyra/blob/master/docs/install/pcsx2.md#)

---

## Steps:
1. Download [intellisense package](https://github.com/h4570/tyra/releases) and unzip it somewhere in your computer.
2. Open terminal in your favourite repositories directory and execute: 
```
docker pull h4570/tyra
git clone https://github.com/h4570/tyra.git
```
3. Download the [assets.zip](https://github.com/h4570/tyra/releases) file and unzip it into the tyra directory
4. Open `VSCode` and open previously cloned directory.
5. In `VSCode` open `.vscode/configurations/windows-docker` and copy configuration files into `.vscode/`
6. In `c_cpp_properties.json` fix all `YOUR-INTELLISENSE-PATH` with path from point 2.
7. Click `CTRL+SHIFT+P`, type `Tasks: Run task` and run `Start docker container` - Repeat this step every time you want to start working with Tyra (every reboot)
8. Click `CTRL+SHIFT+P`, type `Tasks: Run build task`
9. Tyra should automatically compile engine, `demo` game and run `PCSX2`. If you want to compile & run tutorials, please change `buildDirectory` in `settings.json` to example `tutorials/01-hello`
