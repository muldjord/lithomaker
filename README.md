# LithoMaker
Creates 3D lithophanes from PNG image files and exports them to STL files, ready for slicing and 3D printing.

Download the latest release [here](https://github.com/muldjord/lithomaker/releases).

Check a video with an example converted with LithoMaker, sliced with PrusaSlicer and printed on a Prusa i3 MK3S+ here: https://youtu.be/Jqr4oohogVs

LithoMaker DOES NOT upload or process your image files using a cloud service like the online lithophane websites do. I basically made this tool because most Youtube lithophane 3D printing instructions included a step that required me to upload my images to a private company for processing. I am not a fan of that, so I scratched the itch and created LithoMaker.

LithoMaker is a fairly simple piece of software. It reads the input PNG image, inverts it and creates a height-mapped 3D mesh from the grayscale values which is then scaled to the requested thickness. It then creates the remaining triangles needed for a 3D printable lithophane with a frame and some printing stabilizers. The mesh is then exported as an STL file, ready for importing into a 3D printer slicer.

NOTE! The resulting lithophane is currently lying flat down when the STL is imported into a slicer. Remember to rotate it 90 degrees to make it stand vertically on the build plate.

## Release notes

#### Version 0.6.1 (11th Nov 2021)
* Corrected frame front side distance from backside
* Fixed config key to 'LithoMaker'

#### Version 0.6.0 (11th Nov 2021)
* Added validators for variable lineedits
* Added tooltips for variable lineedits
* Now checks for grayscale input image and suggests to convert if input image is in color
* Now checks image resolution and suggests resizing before processing
* Now disables UI buttons when processing to avoid queing multiple clicks
* Fixed stabilizer meshes
* Fixed lithophane backside mesh
* Made it possible to make stabilizers permanent
* Made more mesh variables / factors configurable

#### Version 0.5.0 (7th Nov 2021)
* First public release

#### Version 0.0.1 (Unreleased)
* Internal prototype development release

#### Todo
* Segment / manifold backside of lithophane to allow bending in third-party software
* Segment frame to allow bending in third-party software
* Move mesh related functions to separate files / classes