[![Contributors][contributors-shield]][contributors-url]
[![MIT License][license-shield]][license-url]
[![Discord](https://img.shields.io/badge/discord-Tyra-brightgreen?logo=discord)](https://discord.gg/NhhTmg3Gm5)
[![Discord](https://img.shields.io/badge/discord-PS2--Scene-brightgreen?logo=discord)](https://discord.gg/7dCr6ThSN5)

<br />
<p align="center">
  <a href="https://github.com/h4570/tyra">
    <img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/github-splash.png" alt="Logo" width="100%" height="auto">
  </a>

  <h3 align="center">Tyra - alpha 1.36.3</h3>

  <p align="center">
    Open source game engine for PlayStation 2™
    <br />
    <a href="https://github.com/h4570/tyra/tree/master/src/samples/dolphin"><strong>Dolphin sample »</strong></a>
    &nbsp;
    <a href="https://github.com/h4570/tyra/tree/master/src/samples/floors"><strong>Floors sample »</strong></a>
    <br />
    <br />
    <a href="https://github.com/h4570/tyra/issues">Report Bug</a>
    ·
    <a href="https://github.com/h4570/tyra/issues">Request Feature</a>
  </p>
</p>  

[![Stargazers repo roster for h4570/tyra](https://reporoster.com/stars/h4570/tyra)](https://github.com/h4570/tyra/stargazers)

## Table of Contents  

* [About the Project](#about-the-project) 
  * [Samples](#samples) 
  * [Description](#description) 
  * [Features](#features) 
  * [Branches](#branches) 
* [Getting Started](#getting-started) 
  * [Installation](#installation) 
  * [Prerequisites](#prerequisites) 
* [Roadmap](#roadmap) 
* [Contributing](#contributing) 
* [License](#license) 
* [Built With](#built-with) 
* [Acronyms](#acronyms) 
* [Thanks](#thanks) 

<br />

## About the Project  

<img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/tyra.gif" alt="Sample code" width="600" height="auto">  
<img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/dolphin.gif " alt="Sample game" width="500" height="auto">  

### Samples
* [Dolphin](https://github.com/h4570/tyra/tree/master/src/samples/dolphin) 
* [Floors](https://github.com/h4570/tyra/tree/master/src/samples/floors) 
* [Cube](https://github.com/h4570/tyra/tree/master/src/samples/cube) 
  
### Description
Tyra is a project that aims to facilitate the development of PlayStation 2 games. The goal is simple API which will allow you to develop some nice small homebrew games in a short period of time. Finally, (thanks to PS2DEV team) Tyra supports C++20, so we are free from the 2003's GCC which only supported C++98.  
**Project is on alpha stage, so keep in mind that there are many things to be done and to fix.**
Unfortunately there are not many people, which want's to help with this project. **If you want to be a volunteer**, but don't have too much knowledge in this field, write to me on Discord, **I will be happy to help.**
 
When project will be on beta stage, we plan to create some YouTube tutorials :)  
  
**Why was this project created?** 
PS2 development can be a big challenge.  
PS2 it's pretty low-level and there are a lot of stuff that you must do from scratch.  
For example, if you want to render something using only PS2DEV you must:
- Create VU1 micro-program
- Load micro-program and send it to VU1
- Create parser for your 3D file
- Create parser for texture file
- Calculate lights, shadows, MVP matrices (3D->2D)..
- Clip triangles ([sic!](https://github.com/h4570/tyra/issues/14))
- Prepare DMA packets for coprocessors communication
- Setup rasterizer (GS)
- Send texture to GS
- Send packets
- Synchronize all  

but with Tyra you can do it with methods like `loadObj()` and `draw()` 😁. 

### Features
* 3D support - VU1 "Path 1 rendering"  
* 2D support - PNG & BMP
* Frustum culling, backface culling  
* OpenGL adaptions: Perspective projection, lookAt camera  
* Texture support - .bmp and png  
* Mesh loaders: ".obj", ".dff" (RenderWare, GTA:SA) and ".md2" (Quake II) 
* Threading support 
* Animation support for obj and md2
* Audio support - .wav and .adpcm 
* Pad support 

### Branches
* Master - monthly releases - tested on PS2 & PCSX2.
* Develop - daily builds - can be unstable.

<br />

## Getting Started  

### Installation 
Step by step [tutorial here](https://github.com/h4570/tyra/tree/develop/docs)  
If you need any support, feel free to ask questions on discord:  
[![Discord](https://img.shields.io/badge/discord-Tyra-brightgreen?logo=discord)](https://discord.gg/NhhTmg3Gm5)  

### Prerequisites 

* PS2 DEV environment - [PS2DEV](https://github.com/ps2dev/ps2dev)  
* At least #58ffb33 PS2SDK (28.05.2021) - [PS2SDK](https://github.com/ps2dev/ps2sdk) 
* PS2 Emulator. For example [PCSX2](https://pcsx2.net/) 

<br />

## Extra info 
* If you want to use your own .obj file, please check "triangulate faces" in blender.  
PS2 is supporting only triangle primitive type.  

<br />

## Roadmap  

### Roadmap for next master release:  
[Next release roadmap](https://github.com/h4570/tyra/projects/4)  

See the [open issues](https://github.com/h4570/tyra/issues) for a list of proposed features (and known issues). 

<br />

## Contributing 

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. 

1. Fork the Project 
2. Create your Feature Branch (`git checkout -b feature/amazing-feature`)  
3. Commit your Changes (`git commit -m 'Add some amazing-feature'`)  
4. Push to the Branch (`git push origin feature/amazing-feature`)  
5. Open a Pull Request  

<br />

## License  

Distributed under the Apache License 2.0 License. See `LICENSE` for more information. 

<br />

## Built With  

* [PS2DEV](https://github.com/ps2dev/ps2dev)  

<br />

## Acronyms 
* EE (Emotion Engine) - whole set of processors and subsystems that make up the PlayStation 2 
* GS (Graphic synthesizer) - configurable rasterizer and texture mapper. Has only 2MB~ memory for textures, but for the opposite have very high transfer rate, so you can switch textures in the blink 
* VU0 - programmable (ASM) vector processor that is accessed by inline assembly code injected into C program.  
* VU1 - programmable (ASM) vector processor, which is similar to vertex shader. Accessed via DMA.  
* IOP - I/O processor which enables access to peripheral devices, such the game controller  

<br />

## Thanks  

Without these guys, everything would be harder:
* Dr Henry Fortuna - for code sources
* Rick Gaiser, fjtrujy - for help and new GCC!
* Lukasz D.K. - for huge archive of PS2 stuff
* Guilherme Lampert - for code sources
* Jesper Svennevid, Daniel Collin - for openvcl's code samples
* Whole PS2DEV team
* Manieq - for nice splash screens!
* And so many other guys. Thanks!

Project Link: [https://github.com/h4570/tyra](https://github.com/h4570/tyra)  
Sandro Sobczyński - sandro.sobczynski@gmail.com  
[![LinkedIn][linkedin-shield]](https://linkedin.com/in/sandro-sobczyński-28820b15a)  
  
<br />
<a href="https://github.com/h4570/tyra">
  <img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/github-splash2.png" alt="Logo" width="100%" height="auto">
</a>

[contributors-shield]: https://img.shields.io/github/contributors/h4570/tyra.svg?style=flat-square  
[contributors-url]: https://github.com/h4570/tyra/graphs/contributors 
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555  
[license-shield]: https://img.shields.io/github/license/h4570/tyra.svg?style=flat-square  
[license-url]: https://github.com/h4570/tyra/blob/master/LICENSE  
