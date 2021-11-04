# LithoMaker
Creates 3D lithophanes from PNG image files and exports them to STL files, ready for slicing and 3D printing.

LithoMaker DOES NOT upload or process your image files using a cloud service like the online lithophane websites do. I basically made this tool because most Youtube lithophane 3D printing instructions included a step that required me to upload my images to a private company for processing. I am not a fan of that, so I scratched the itch and created LithoMaker.

LithoMaker is a fairly simple piece of software. It reads the image, inverts it and creates a height-map directly in the STL format. It then creates the remaining triangles needed for a closed-up mesh with a frame and some printing stabilizers.

## Release notes

#### Version 0.5.0 (In development, unreleased)
* First public release

#### Version 0.0.1 (Unreleased)
* Internal prototype development release

#### Todo
* Segment backside of lithophane to allow bending in third-party software
* Segment frame to allow bending in third-party software
* Fix top of stabilizers (flatten them)