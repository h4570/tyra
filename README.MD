[![Contributors][contributors-shield]][contributors-url]
[![MIT License][license-shield]][license-url]  
PS2-Scene [![Discord](https://img.shields.io/discord/652861436992946216?logo=discord)](https://discord.gg/7dCr6ThSN5)

<br />
<p align="center">
  <a href="https://github.com/h4570/tyra">
    <img src="http://allvectorlogo.com/img/2016/05/ps2-logo.png" alt="Logo" width="230" height="125">
  </a>

  <h3 align="center">Tyra alpha 1.1.0</h3>

  <p align="center">
    Open source renderer and game engine for PS2 (PlayStation 2™).
    <br />
    <a href="https://www.youtube.com/"><strong>View Demo (soon) »</strong></a>
    <br />
    <br />
    <a href="https://github.com/h4570/tyra/issues">Report Bug</a>
    ·
    <a href="https://github.com/h4570/tyra/issues">Request Feature</a>
  </p>
</p>  

## Table of Contents  

* [About the Project](#about-the-project) 
  * [Samples](#samples) 
  * [About project](#about-project) 
  * [Acronyms](#acronyms) 
  * [Tyra features](#tyra-features) 
  * [Built With](#built-with) 
* [Getting Started](#getting-started) 
  * [Prerequisites](#prerequisites) 
  * [Installation](#installation) 
* [Roadmap](#roadmap) 
* [Contributing](#contributing) 
* [License](#license) 
* [Contact](#contact) 
* [Thanks](#thanks) 

## About The Project  

<img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/1.1.0/tyra.gif" alt="sample" width="600" height="auto">

### Samples
* [Floors](https://github.com/h4570/tyra/tree/master/src/samples/floors) 
* [Ari (WIP)](https://github.com/h4570/tyra/tree/master/src/samples/ari) 

### Contributors wanted
#### I will be grateful for any help, Discord: h4570#3402  
  
### About project
Goal of Tyra project is to provide simple API for PS2 (PlayStation 2™) game creation. So you can just run PS2 emulator, grab your .obj file, and put it into live in a seconds. Project is on early stage, so a lot of stuff is changing from release to release.  
 
YouTube tutorials soon!  
  
**Why PS2 game dev is so different?** Raw PS2 development can be a big challenge. PS2 it's pretty low-level and there are a lot of stuff that you must do from scratch.  
Tyra takes care for you, and rendering, coprocessors communication and many more low-level stuff is already done.

### Acronyms 
* EE (Emotion Engine) - whole set of processors and subsystems that make up the PlayStation 2 
* GS (Graphic synthesizer) - configurable rasterizer and texture mapper. Has only 2MB~ memory for textures, but for the opposite have very high transfer rate, so you can switch textures in the blink 
* VU0 - programmable (ASM) vector processor that is accessed by inline assembly code injected into C program.  
* VU1 - programmable (ASM) vector processor, which is similar to vertex shader. Accessed via DMA.  
* IOP - I/O processor which enables access to peripheral devices, such the game controller  

### Tyra features
* VU1 rendering (PATH 1), double-buffered  
* GIF rendering (PATH 3), double-buffered    
* Frustum culling, backface culling  
* OpenGL adaptions: Perspective projection, lookAt camera  
* Texture support (.bmp loader)  
* Mesh loaders: ".obj", ".dff" (RenderWare, GTA:SA) and ".md2" (Quake II) 
* Threading support 
* Lighting support (now PATH3 only)  
* Animation support 
* Audio support 
* Pad support 
* TODO: physics, ADPCM and many more!  

### Built With  

* [PS2SDK](https://github.com/ps2dev/ps2sdk)  

## Getting Started  

To get a local copy up and running follow these simple steps.  
YouTube tutorials soon!

### Prerequisites 

This is an example of how to list things you need to use the software and how to install them.  
* PS2 DEV environment - [PS2DEV](https://github.com/ps2dev/ps2dev)  
* At least #674b6f4 PS2SDK (01.12.2020) - [PS2SDK](https://github.com/ps2dev/ps2sdk) 
* PS2 Emulator. For example [PCSX2](https://pcsx2.net/) 

### Installation  

1. Clone the repo 
```sh
git clone https://github.com/h4570/tyra.git
```
2. Get into src folder and compile it!  
```sh
make
```  

## Extra info 
* Textures can be saved using GIMP. Just replace red with green color, and when exporting select .bmp, 24bit: RGB888, without saving information about colors 
* If you want to run .elf in PCSX2, be sure that you have version >=1.6 and **Host access** enabled in PCSX2 configuration file.  
* If you want to use your own .obj file, please check "triangulate faces" in blender. PS2 is supporting only trinagle primitive type. 

## Roadmap  

### Roadmap moved to github kanban:  
[https://github.com/h4570/tyra/projects/1](https://github.com/h4570/tyra/projects/1)  


See the [open issues](https://github.com/h4570/tyra/issues) for a list of proposed features (and known issues). 

## Contributing 

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. 

1. Fork the Project 
2. Create your Feature Branch (`git checkout -b feature/amazing-feature`)  
3. Commit your Changes (`git commit -m 'Add some amazing-feature'`)  
4. Push to the Branch (`git push origin feature/amazing-feature`)  
5. Open a Pull Request  

## License  

Distributed under the Apache License 2.0 License. See `LICENSE` for more information. 

## Contact  

Sandro Sobczyński - sandro.sobczynski@gmail.com   
[![LinkedIn][linkedin-shield]][linkedin-url]  

## Thanks  

Without these guys, everything would be harder:
* Dr Henry Fortuna - Abertay University (code sources)
* Lukasz D.K. (huge archive of PS2 stuff)
* Guilherme Lampert - PlayStation London (code sources)
* Jesper Svennevid, Daniel Collin - Microcode (openvcl, code samples)
* Whole PS2SDK/PS2Scene team
* And so many other guys. Thanks!

Project Link: [https://github.com/h4570/tyra](https://github.com/h4570/tyra)  

[contributors-shield]: https://img.shields.io/github/contributors/h4570/tyra.svg?style=flat-square  
[contributors-url]: https://github.com/h4570/tyra/graphs/contributors 
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555  
[linkedin-url]: https://linkedin.com/in/sandro-sobczyński-28820b15a/  
[license-shield]: https://img.shields.io/github/license/h4570/tyra.svg?style=flat-square  
[license-url]: https://github.com/h4570/tyra/blob/master/LICENSE  
