[![Contributors][contributors-shield]][contributors-url]  
[![MIT License][license-shield]][license-url] 
[![LinkedIn][linkedin-shield]][linkedin-url]  

<br />
<p align="center">
  <a href="https://github.com/h4570/tyra">
    <img src="http://allvectorlogo.com/img/2016/05/ps2-logo.png" alt="Logo" width="230" height="125">
  </a>

  <h3 align="center">Tyra (Work in progress)</h3>

  <p align="center">
    Tiny game engine for PS2 (PlayStation 2™) homebrew, written in C++98
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
  * [Features](#features) 
  * [Built With](#built-with) 
* [Getting Started](#getting-started) 
  * [Prerequisites](#prerequisites) 
  * [Installation](#installation) 
* [Roadmap](#roadmap) 
* [Contributing](#contributing) 
* [License](#license) 
* [Contact](#contact) 

## About The Project  

<img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/1.0.0/tyra.gif" alt="sample" width="600" height="auto">

This repository contains a early version of game engine with simple demos for PS2 (PlayStation 2™) that I've developed using the freely available PS2DEV, unofficial SDK. 
Developing for PS2 was and is a big challenge for me, it's pretty low-level and hard (for me :D). The architecture is completely different from an Intel x86 and there are lots of extra things that you need to take into account. There are some samples available to teach on, but overall community died several years ago :/ 
  
**Why PS2 game dev is so different?** Creating games for PS2 is completly different from dragging colored buttons from side to side through an application with nice GUI (ex. Unity).  
Coding something for PS2 requires from you doing everything from scratch, even data transfer management through (/co)processors like: 
* EE (Emotion Engine) - whole set of processors and subsystems that make up the PlayStation 2 
* GS (Graphic synthesizer) - configurable rasterizer and texture mapper, geometry transformations are not done by it. Has only 2MB~ memory for textures, but very high transfer rate, so you can switch them in the blink 
* VU0 - vector processor accessed by inline assembly code injected into C program  
* VU1 - vector processor (similar vertex shader) accessed via DMAC  
* IOP - I/O processor which enables access to peripheral devices, such the game controller  

### Samples
* [Floors](https://github.com/h4570/tyra/tree/master/src/samples/floors) 
* [Ari (WIP)](https://github.com/h4570/tyra/tree/master/src/samples/ari) 

### Features
* VU1 (PATH 1 rendering) with double buffering  
* GIF (PATH 3 rendering) with double buffering 
* Optimized calculations using VU0
* Frustum culling 
* Backface culling  
* Perspective projection (like OpenGL's)  
* Audio support 
* Animation support 
* Third person view "lookAt" camera (like OpenGL's) 
* Dualshock 2 pad support 
* RGB 888 24bit texture support (with .bmp loader)  
* 3D files loaders: ".obj", ".dff" (renderware) and ".md2" 
* Multi threading management  
* 3D object management  
* Lighting support (now PATH3 only)  
* Other: collision, gravity etc.  

### Built With  

* [PS2SDK](https://github.com/ps2dev/ps2sdk)  

## Getting Started  

To get a local copy up and running follow these simple steps. 

### Prerequisites 

This is an example of how to list things you need to use the software and how to install them.  
* PS2 DEV environment - [PS2DEV](https://github.com/ps2dev/ps2dev)  
* At least #1a4587d PS2SDK - [PS2SDK](https://github.com/ps2dev/ps2sdk) 
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

Here is a list of the things I'm working on:  

* Fix PATH3 and PATH1 triangle clipping
* Add skybox with 6 textures to ari sample
* Ariana textures 
* Bugfix: Player speed on different zoom  
* Implement player rotation 
* Bugfix: Backface culling bug on rotation  
* Bugfix: Remove camera rotation reset flickering 
* Bugfix: 2x Audio tick overflow  
* Audio - change listeners logic 
* Sound effects 
* Code documentation  

See the [open issues](https://github.com/h4570/tyra/issues) for a list of proposed features (and known issues). 

## Contributing 

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. 

1. Fork the Project 
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)  
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)  
4. Push to the Branch (`git push origin feature/AmazingFeature`)  
5. Open a Pull Request  

## License  

Distributed under the Apache License 2.0 License. See `LICENSE` for more information. 

## Contact  

Sandro Sobczyński - hastoklai@gmail.com 

Project Link: [https://github.com/h4570/tyra](https://github.com/h4570/tyra)  

[contributors-shield]: https://img.shields.io/github/contributors/h4570/tyra.svg?style=flat-square  
[contributors-url]: https://github.com/h4570/tyra/graphs/contributors 
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555  
[linkedin-url]: https://linkedin.com/in/sandro-sobczyński-28820b15a/  
[license-shield]: https://img.shields.io/github/license/h4570/tyra.svg?style=flat-square  
[license-url]: https://github.com/h4570/tyra/blob/master/LICENSE  
