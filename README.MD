[![Codacy Badge](https://app.codacy.com/project/badge/Grade/b81ab27f1c774f839729829c9f772b1a)](https://www.codacy.com/gh/h4570/tyra/dashboard?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=h4570/tyra&amp;utm_campaign=Badge_Grade)
[![Contributors][contributors-shield]][contributors-url]
[![MIT License][license-shield]][license-url]
[![Discord](https://img.shields.io/badge/discord-Tyra-brightgreen?logo=discord)](https://discord.gg/NhhTmg3Gm5)
[![Discord](https://img.shields.io/badge/discord-PS2--Scene-brightgreen?logo=discord)](https://discord.gg/7dCr6ThSN5)

<br />
<p align="center">
  <a href="https://github.com/h4570/tyra">
    <img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/github-splash.png" alt="Logo" width="100%" height="auto">
  </a>

  <h3 align="center">Tyra</h3>

  <p align="center">
    Open source game engine for PlayStation 2™
    <br />
    <a href="https://www.youtube.com/watch?v=DGR4vWDjkmc"><strong>Showcase video »</strong></a>
    &nbsp;
    <a href="https://youtube.com/playlist?list=PLdQVLBcXRRWvtMi-OqccDWLTEFEisD5RH"><strong>YouTube tutorials »</strong></a>
    &nbsp;
    <br />
    <br />
    <a href="https://github.com/h4570/tyra/issues">Report Bug</a>
    ·
    <a href="https://github.com/h4570/tyra/issues">Request Feature</a>
  </p>
</p>  

[![Stargazers repo roster for h4570/tyra](https://reporoster.com/stars/h4570/tyra)](https://github.com/h4570/tyra/stargazers)

## Table of Contents  

* [About the project](#about-the-project) 
  * [Description](#description) 
  * [Tutorials](#tutorials) 
  * [Features](#features) 
  * [Branches](#branches) 
* [Contributing](#contributing) 
* [Games](#games) 
* [License](#license) 
* [Built With](#built-with) 
* [Acronyms](#acronyms) 
* [Credits](#credits) 

## About the Project  
<p align="center">
  <img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/tyra_code.gif" alt="Sample code" width="500" height="auto">  
  <img src="http://apgcglz.cluster028.hosting.ovh.net/tyra/showcase.gif " alt="Sample game" width="500" height="auto"> 
</p> 

---
### Description
Tyra is a project that allows you to easily create games for the PlayStation 2 console.  
When I tried to make a game for PS2 a few years ago, I realized how many things had to be done to tame this exotic console architecture.
Decision was simple - I need to create an engine which will handle 3D file loading, 2D/3D rendering, animation, audio,  handling between PS2 CPUs, synchronization.. It should be simple as possible - so people with no experience can create a simple game in a few hours.  
**And that's what Tyra is all about.**  

### Tutorials
* 01 - Setup environment - [Text tutorial](https://github.com/h4570/tyra/tree/master/docs/install) or [Video](https://www.youtube.com/watch?v=9IXOU1SPdzc)
* 02 - Cooking game assets - [Video](https://www.youtube.com/watch?v=hpVDeTTLyjI)
* 03 - Create a game repository - [Video](https://www.youtube.com/watch?v=DKfIKar3GUM)
* 04 - Run game from USB in PS2 - [Video](https://www.youtube.com/watch?v=ZX8fTQqiNho)
---
* 01 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/01-hello) - Hello world
* 02 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/02-sprite) - 2D rendering
* 03 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/03-minecraft) - 3D minecraft blocks
* 04 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/04-de_dust2) - `de_dust2.obj` rendering
* 05 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/05-animation) - 3D animation
* 06 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/06-audio) - Background music and sound effects
* 07 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/07-lighting) - Static lightmap and dynamic directional lights
* 08 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/08-skybox-debug) - Skybox and debug rendering
* 09 - [Code](https://github.com/h4570/tyra/tree/master/tutorials/09-manual-mode) - Manual rendering (a'la OpenGL)
* Demo game - [Code](https://github.com/h4570/tyra/tree/master/demo)
---

### Features
* Docker image with Tyra, all tools and `PS2DEV` C++20 compiler
* 3D rendering via `VU1` coprocessor:
  * Static rendering  
  * Morph animation rendering  
  * Super optimized, custom minecraft block renderer  
  * Debug rendering (lines, boxes, bboxes)
  * Manual rendering (a'la `OpenGL`)
* 2D rendering
* Frustum culling:
  * Simple - culling whole mesh
  * Precise - culling only visible parts of mesh
* Clipping:
  * Standard "fake", but fast PS2 clipping done in `VU1`
  * Software clipping done in `EE Core`
* Optimized math functions via `VU0` coprocessor   
* Background music and sound effects support 
* Pad support
* USB support
* HDD supprt(WIP)
* Memory card support
* `.wav` Audio file support
* `.obj` 3D file support
* `.md2` 3D file support
* `.png` Texture file support

### Branches
* `master` - stable releases - tested on PS2 & PCSX2.  
* `develop` - daily builds.

See the [open issues](https://github.com/h4570/tyra/issues) for a list of proposed features (and known issues). 

## Contributing 

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**. 

1. Fork the Project 
2. Create your Feature Branch (`git checkout -b feature/amazing-feature`)  
3. Commit your Changes (`git commit -m 'Add some amazing-feature'`)  
4. Push to the Branch (`git push origin feature/amazing-feature`)  
5. Open a Pull Request  

## Games

List of games developed with Tyra.  
If you created one, please contact me - I will add it to the list :)

* [Tyracraft](https://github.com/Wellinator/tyracraft) by [Wellinator](https://github.com/Wellinator)
* [Game with car](https://github.com/freebytego/gwc-ps2) by [freebytego](https://github.com/freebytego)
* [Rock, paper, scissors](https://github.com/GuidoDQR/my-ps2-game) by [GuidoDQR](https://github.com/GuidoDQR)
* [TyraTale](https://github.com/Br4k2n/TyraTale) by [Br4k2n](https://github.com/Br4k2n)

## License  

Distributed under the Apache License 2.0 License. See `LICENSE` for more information. 

## Built With  

* [PS2DEV](https://github.com/ps2dev/ps2dev)  
* [VCLPP](https://github.com/glampert/vclpp)  
* Sony VCL

## Acronyms 
* EE (Emotion Engine) - whole set of processors and subsystems that make up the PlayStation 2 
* EE Core - main PS2 CPU (c++ done here) 
* GS (Graphic synthesizer) - configurable rasterizer and texture mapper. Has only 2MB~ memory for textures, but for the opposite have very high transfer rate, so you can switch textures in the blink 
* VU0 - programmable (ASM) vector processor that is accessed by inline assembly code injected into C program.  
* VU1 - programmable (ASM) vector processor, which is similar to vertex shader. Accessed via DMA.  
* IOP - I/O processor which enables access to peripheral devices, such the game controller  

## Credits

Without these guys, Tyra would not happen:
* [Dr Henry Fortuna](http://ps2-edu.tensioncore.com/index.html) - for code sources, PS2 academy tutorials
* Whole [PS2DEV](https://github.com/ps2dev) team, and specially to [Rick Gaiser](https://github.com/rickgaiser), [fjtrujy](https://github.com/fjtrujy) - for a lot of good tips!
* [Wellington Carvalho](https://github.com/Wellinator), [André Guilheme](https://github.com/wolfysdl) - for testing, contributing to Tyra and sharing cool ideas!
* [Leonardo Ono](https://github.com/leonardo-ono) - for software renderer example (with clipping!)
* [Lukasz D.K.](https://github.com/lukaszdk) - for huge archive of PS2 stuff
* [Guilherme Lampert](https://github.com/glampert) - for code sources
* [Jesper Svennevid](https://github.com/jsvennevid), [Daniel Collin](https://github.com/emoon) - for openvcl's code samples
* [Manieq](https://www.facebook.com/mvnieq) - for nice splash screens!
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
