# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.?.?] - ?-?-?

### Added
- Unit test project
- Assert
- CI via Github workflows
- Install tutorial as separated .MD file

### Changed
- Improved BMP loader.
- All PRINT_LOG and PRINT_ERR to consoleLog() and assertMsg()
- Refactored makefiles
- **CHANGED ENV VARIABLES!**
- Install tutorial
- Readme

## [1.31.2] - 2021-02-06

### Added
- Id to MeshFrame
- Non textured rendering (RGBA only)

### Changed
- Moved color from mesh to mesh material.
- OBJ Loader refactor
- Logic of loadFrom()
- Migrated to GCC9 (valid with latest PS2SDK)

### Removed
- Removed MeshId from TextureLink, TextureRepository

## [1.29.1] - 2020-12-27
### Added
- Created libpacket2 in PS2SDK
- Dolphin sample
- 2D support (Sprite class)
- Alpha blending (transparency)
- PNG support
- ADPCM support in audio class
- AudioFinish event in audio class
- BoundingBox class
- A lot of useful funcs in Matrix class
- Mock of file service
- Array mode to draw() (highly optimized for small meshes)
- VCLPP - Usage of VU1 Preprocessor

### Changed
- Project setup (makefile mostly)
- Readme
- Switched into libpacket2 in `vifsender` and `gifsender`
- Timer class refactor
- Engine base refactor
- MeshTexture into Texture
- Refactored floors sample
- Refactored Audio class
- Fixed Path 3 lighting
- Refactored, optimized math classes (vector, matrix..)
- Cleanup in camera base
- Refactored Path 3 and Path 1 rendering
- Synchronization functions (`renderer->waitForRender()`)
- BMP class refactor
- Dff Loader refactor
- Optimized rendering data preparation

### Removed
- Ari sample
- VU1 class

## [1.1.0] - 2020-11-03
### Added
- A lot of useful functions to Mesh class
- MeshFrame class
- MeshMaterial class
- MeshTexture class
- TextureLink struct
- Texture wrapping settings
- TextureRepository class
- PRINT_ERR()
- Ari sample

### Changed
- Project setup (gitignore etc.)
- Readme
- DFF loader refactor
- OBJ loader refactor
- MD2 loader refactor
- 3D data classes refactor (one interface for all)
- Rendering refactor

### Removed
- MeshSpec class
- Texture class
- Md2Model class
- ObjModel class
- DffModel class

## [1.0.0] - 2020-10-28
### Added
- Project setup (gitignore etc.)
- License
- Readme
- BMP support
- DFF support
- MD2 support
- OBJ support
- Animation support
- Engine base
- AudioListener class
- Rendering base
- Path 1 renderer (VU1)
- Path 3 renderer (GIF)
- Math classes (matrix, vector, plane..)
- Audio support (bg song only)
- Camera base
- Base lighting support (bugged) for Path 3
- Pad support
- EE Timer
- Math class
- String class
- Mesh class with AABB
- Makefile
- Primitive floors sample

[1.?.?]: https://github.com/h4570/tyra/compare/v1.31.2-alpha...v1.?.?-alpha
[1.31.2]: https://github.com/h4570/tyra/compare/v1.29.1-alpha...v1.31.2-alpha
[1.29.1]: https://github.com/h4570/tyra/compare/1.1.0...v1.29.1-alpha
[1.1.0]: https://github.com/h4570/tyra/compare/1.0.0...1.1.0
[1.0.0]: https://github.com/h4570/tyra/compare/1.0.0
