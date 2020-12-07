# Ollie Nicholls - Interactive Terrain Deformation and Sculpting

<!-- ## Table of Contents -->
- [Ollie Nicholls - Interactive Terrain Deformation and Sculpting](#ollie-nicholls---interactive-terrain-deformation-and-sculpting)
  - [About the Project](#about-the-project)
    - [Code Design and Plan](#code-design-and-plan)
      - [Representation](#representation)
      - [Modification](#modification)
      - [Creation](#creation)
      - [High-Level Code Design](#high-level-code-design)
      - [UML Diagram](#uml-diagram)
  - [Getting Started](#getting-started)
  - [Installation](#installation)
  - [Usage](#usage)
  - [References and Acknowledgement](#references-and-acknowledgement)
    - [Existing Tools](#existing-tools)
    - [Videos](#videos)
    - [Concepts and Papers](#concepts-and-papers)

## About the Project

Creating large scale, realistic terrains for VFX is both a time and resource heavy task, with the need for many departments to iterate and adapt a design to suit the desired use.

This tool can *(will be able to)* be used by a user to build, sculpt, texture, interact, modify and iterate existing terrain geometry without the need of a full-scale team. The tool is *(will be)* fully interactive and allows the user to modify lots of terrain properties and also offers brush tools to either dig (adding holes, rivers, caves, or road tunnels) or add terrain (mountains, hills, buildings, or vegetation).

Once a terrain has been created, it can be *(will be)* saved to an either OBJ, FBX or USD file and can be loaded into Maya.

<!-- As the process of film and VFX creation evolves, there is a growing need for the development, iteration and designing of assets to become faster and more interactive. One example of this is the sculpting and generation of large scale CG terrains to form sets or environments for worlds or shots.

These terrains can vary from mountains and cities to deserts and jungles. Traditionally, making any changes to these terrains, preview and try different ideas, would involve many departments to build, sculpt and retexture the geometry, taking days to weeks for results. Your goal for this task is to create a terrain deformation and sculpting toolset that will allow users to directly interact, modify and iterate on existing terrain geometry. You will need to demonstrate a minimum of 2 environment types such as mountains and cities.

The tools should allow them to modify a plethora of the terrains properties, including height, smoothness and material (sand, rock etc). It should also provide a digging tool for creation of holes, caves or rivers for example.

Your system should ensure correct geometry, UVs and normals at all times and should also be able to export the terrain geometry into either OBJ, FBX or USD (if you feel experimental) so that it can be loaded back into Maya. Additionally, it should handle large scale terrains efficiently with good real time performance. The toolset can be developed inside a DCC (Digital Content Creation) tool, or as a standalone application. The only limitation is that Houdini may not be used. You are free to use third party libraries as long as they are open source. It is not required for the sake of this task for the toolset to work in AR/VR. However, if it is possible to do so with controller interaction, this would be very interesting. Note, the focus of this task is on the features, quality of the experience and the usability of each tool provided. Make sure you focus on the key features to produce a quality useable product and explain with accompanying documentation any features not implemented and why. There is always the risk that certain features are harder to implement than initially anticipated. If this is the case, explain in writing why these were hard to complete and provide a suggestion on what is required to complete their implementation. -->

### Code Design and Plan

See section 3 of the [report](docs/report.pdf) for a high-level description of the project specification.

The project is split into three parts: Representation, Modification, and Creation. The main goal will be to get an efficient representation of the terrain, offer basic tools to change the heights/colours of the terrain, and options for saving to different object files for use in other modelling and VFX software.

#### Representation

This part of the project will rely on loading in a predefined heightmap. Basic **saving** and **loading** will also be available at this stage.

Main Goal: Trying to get a terrain grid shown to the screen in an efficient way - will use the algorithms described in the report, based on [Geoclipmaps](https://webpages.uncc.edu/krs/courses/6127/pubs/mesh/losasso-siggraph-2004.pdf) and [NVIDIA's GPU Gems 2: Terrain Rendering Using GPU-Based Geometry Clipmaps](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.146.3203&rep=rep1&type=pdf).

- The data will be split into an X-Z vertex and index buffer for each footprint, a texture buffer that represents the height data at each vertex, and some normal and colour data.
- LOD levels span from fine to coarse around the viewer toroidally based on the position of the camera.
- The terrain world is seemingly infinite with the heightmap loaded in to the origin.
- A region of the world can be saved to an object file.
- The performance (FPS/memory usage) can be seen on screen.

Secondary Goal: Texturing terrain

- How to texture based on height?
- Is it possible for users to define own textures?
- Can you simulate biomes/different texturing approaches?
- Having sun position, setting a skybox texture?

Could use [Scape Rendering](https://www.decarpentier.nl/scape-render)'s way of projecting textures onto steep surfaces using a horizontal texture map.

#### Modification

This step I am splitting from creation as this will handle basic brushes (excavator, builder) and other modifications that don't require procedurally generating parts of the terrain.

- How to modify the height of existing terrain in a user-friendly way?
- How to change colours of terrain sections?
- How to set certain areas' biome? (want to deliver at least 2 maybe 3 different biomes)

#### Creation

This step will cover all the procedural generation brushes and only be done if time permits.

- What algorithms to use for building mountains, rivers, caves, chasms, oceans etc.?

#### High-Level Code Design

Using MVC pattern

Model:

- `Terrain` - An object that has a `std::vector` of grid points (`ngl::Vec3`) that represent the terrain. Each grid point is represented by an `ngl::Vec3` for position in space and an `ngl::Vec3` for colour (, could also include material type).
  - Contains methods for manipulating the terrain
    - `setColour(_x, _z, _colourRGBA)` - change the colour of the point, used for texturing
    - `setHeight(_x, _z, _height)` - change the height of the point, used for raising/lowering the terrain

View:

- `UVView` - A 2D view of the height map that represents the terrain
- `3DView` - A perspective, world view of the rendered terrain

Controller:

- UI System - Controls the whole system.
  - Window that handles the display of the program
  - UI with different tools and brushes to manipulate the terrain

    Initial tools:

    - Zoom tool
    - Excavator brush - brush size, shape, soft edges
    - Building brush - brush size, shape, soft edges
    - Colour changer - interactive colour wheel, brush size, shape, paint by elevation
    - Load texture
    - Load/save object - file types
    - Load height map
    - Interactive help (tooltips?)
    - View modes
      - Wire frame
      - Normals
      - Colour on height

    Other tools (time permitting/further extensions)
    - Cave digger
    - River digger (using natural hydrology algorithms, or manual)
    - Mountain builder
    - Sand dune builder
    - Terrain object brush
    - Vegetation

  - The terrain changes colour where the mouse is hovering (amount of points affected based on brush size) - will have to consider how points will be selected, need to check if points in circle. [Converting screen space cursor position to world space](https://stackoverflow.com/a/7702895).

The terrain will need to be tessellated to help with the realtime part of the program. The underlying mesh will be relatively simple but using a tesselation control shader, different parts of the model will have different levels of detail depending on how vital it is that the object looks smooth. For example, a sheer mountain face will not need as much detail (and will not have as many nooks and crannies) as a cave which would look relatively smooth or have distinct shapes that need a higher level of detail.

I'm keen on the idea of trying to use (or at least a simulation of) real geology  to generate mountains, rivers, and caves as I feel it will create more realistic looking terrain.

Further research into whether this would be a single-pass render or multi-pass, I can see benefits to both methods presently.

Ideas/scratch pad:

- Terrain texturing could be done based on height/slope (If above certain level texture as snow, if cliff then texture as cliff face not grass). Could have biome selection to change what the texturing does (no snow and texture as sand if desert)
- Research into different view modes for items based on distance to camera - higher resolution if close to camera and low resolution in distance to improve speed/performance.
- Sun/light direction
- Skybox texture
- texture readback?
- staging textures?
- linear interpolation between colours to colour vertices
- procedural brushes
- using gpu for rendering and brush work
- Geometric mip mapping
- texture splatting
- contours?
- Lock mouse to height

#### UML Diagram

![UML class diagram](img/ase_class_diagram.svg)

## Getting Started

TODO

## Installation

TODO

## Usage

TODO

## References and Acknowledgement

### Existing Tools

- [Terragen](https://planetside.co.uk/) - a powerful solution for building, rendering, and animating realistic natural environments.
- [Daylon Leveller](http://hme.sourceforge.net/) - combines intuitive pixel sculpting with the accuracy of geo-referencing and vector shapes.
- [Height Map Editor](http://hme.sourceforge.net/) - 2D program for creating, manipulating, and viewing height maps.
- [Advanced Landscape Editor](http://www.dyvision.co.uk/ale.htm) - Create large scale landscapes and edit textures with direct painting.
- [Kashmir 3D](https://www.kashmir3d.com/index-e.html) - Old tool for making 3D landscapes
- [L3DT](http://www.bundysoft.com/L3DT/) - a Windows application for generating terrain maps and textures. It is intended to help game developers and digital artists create vast high-quality 3D worlds.
- [EarthSculptor](http://www.earthsculptor.com/) - a realtime terrain height-map editor and paint program featuring an easy to use interface designed
exclusively for rapid development of 3D landscapes for visualization, multimedia and game development.
- [TerraNoise](https://www.guruware.at/main/index.html) - Node based terrain editor.

### Videos

- [Potch Explains Perlin Noise with Terrain Generation!](https://www.youtube.com/watch?v=ww5CcgG34Dc)
- [PROCEDURAL TERRAIN in Unity! - Mesh Generation](https://www.youtube.com/watch?v=64NblGkAabk)
- [River Based Terrain Generation - Sapiens Devlog 36](https://www.youtube.com/watch?v=yZHe-2Gg6zA)
- [Procedural Landmass Generation (E01: Introduction)](https://www.youtube.com/watch?v=wbpMiKiSKm8)

### Concepts and Papers

- [Artificial Terrain Generation](http://vterrain.org/Elevation/Artificial/) - Outlines details for many types of generation including information on [caves](http://vterrain.org/Elevation/Caves/). Also contains loads of links to papers on terrain generation:
  - [Terrain generation using procedural models based on hydrology](https://dl.acm.org/doi/abs/10.1145/2461912.2461996)
  - [Terrain Synthesis from Digital Elevation Models](http://www.howardzzh.com/research/terrain/)
  - [Real-Time Editing, Synthesis, and Rendering of Infinite Landscapes on GPUs](https://www.researchgate.net/profile/Jens_Schneider/publication/228909493_Real-Time_Editing_Synthesis_and_Rendering_of_Infinite_Landscapes_on_GPUs/links/0fcfd50a2c4bacf8fb000000/Real-Time-Editing-Synthesis-and-Rendering-of-Infinite-Landscapes-on-GPUs.pdf)
  - [Heightfield Synthesis by Non-Parametric Sampling](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.103.2245&rep=rep1&type=pdf)
  - [Modeling for the Plausible Emulation of Large Worlds](https://www.elibrary.ru/item.asp?id=5283463)
  - [TEXTURING & MODELING: A Procedural Approach](http://elibrary.lt/resursai/Leidiniai/Litfund/Lithfund_leidiniai/IT/Texturing.and.Modeling.-.A.Procedural.Approach.3rd.edition.eBook-LRN.pdf)
  - [Generalized Stochastic Subdivision](http://scribblethink.org/Work/Gsd/gsd.pdf)
  - [Computer rendering of stochastic models](https://dl.acm.org/doi/abs/10.1145/358523.358553)
  - [Physically Based Terrain Generation : Procedural Heightmap Generation Using Plate Tectonics](https://www.theseus.fi/handle/10024/40422)
  - [Terrain simulation using a model of stream erosion](https://dl.acm.org/doi/10.1145/378456.378519)
- [Generating Complex Procedural Terrains Using the GPU](https://developer.nvidia.com/gpugems/gpugems3/part-i-geometry/chapter-1-generating-complex-procedural-terrains-using-gpu) - NVIDIA book about terrain generation.
- [Procedural Generation](https://en.wikipedia.org/wiki/Procedural_generation) - Wikipedia
- [PCG-Based game design: enabling new play experiences through procedural content generation](https://dl.acm.org/doi/abs/10.1145/2000919.2000926) - Paper describing how games like Rust, Minecraft, and The Forest use PCG to create terrains.
- [Inside ’no man’s sky,’ the most innovative game in years](https://time.com/no-mans-sky/) - TIME article on how No Man's Sky does terrain generation.
