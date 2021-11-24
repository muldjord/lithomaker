# LithoMaker
Creates 3D lithophanes from PNG image files and exports them to STL files, ready for slicing and 3D printing.

Download the latest release [here](https://github.com/muldjord/lithomaker/releases).

I've made a quick video tutorial. Check that out [here](https://youtu.be/Bjbdk0XiiNY).

For an example lithophane sliced with PrusaSlicer and printed on a Prusa i3 MK3S+ go [here](https://youtu.be/mxQnZgb7caI).

LithoMaker DOES NOT upload or process your image files online. All processing is done on your own computer requiring no internet access. I basically made this tool because most Youtube lithophane 3D printing instructions included a step that required me to upload my images to a private company for processing. I am not a fan of that, so I scratched the itch and created LithoMaker.

LithoMaker is a fairly simple piece of software. It reads the input PNG image, inverts it and creates a height-mapped 3D mesh from the grayscale values. It then creates the remaining triangles needed for a 3D printable lithophane with a frame and optional hangers and printing stabilizers. The mesh is then exported as an STL file, ready for importing into a 3D printing slicer.

## Running LithoMaker
### Ubuntu Linux
* Download the latest LithoMaker AppImage from [here](https://github.com/muldjord/lithomaker/releases).
* Right-click the AppImage file, choose Properties, Permissions, and check "Allow executing file as program".
* Double-click the AppImage file to run it.

The AppImage will probably work with many other distributions as well. Download and try it out.

### Windows
* Download the latest win32 zip file from [here](https://github.com/muldjord/lithomaker/releases).
* Unpack the zip file.
* Run the LithoMaker.exe program.

## Using LithoMaker
Most of the options in LithoMaker should be pretty self-explanatory. But here are some pointers to get you started:
* Options relating to the physical dimensions of the lithophane are directly visible in the main UI when starting LithoMaker.
* Options that require change less often are placed under **Options->Edit Preferences...**.

The default settings when running LithoMaker for the first time are great as a starting point. Adjust if needed:
* *Minimum thickness* does exactly what it says. It defines the thickness of the lithophane in places where the image is brightest. The minimum is 0.8 mm. Thinner would compromize the print structure.
* *Total thickness* is the *total* thickness of the lithophane *including* the minimum thickness. So basically this is the thickness that make up the darkest areas of the lithophane, corresponding to the darkest tones of the input image. I would never go above 5.0 mm on this. But you can if you absolutely insist!
* *Frame border* is simply the width of the frame border in millimeters.
* *Width* defines the total width of the lithophane, including the frame borders. The height is adjusted relative to this automatically using the dimensions of the input image.
* *Input image filename* is the PNG image you want to convert to a lithophane.
* *Output STL filename* is the export STL filename that you will later import into the 3d printing slicer.

### Render preferences
* *Stabilizers* are sloped pieces of plastic that lean against the lithophane from the front and back. They provide support when printing to avoid wobbling which increases the risk of print failure. Unless you configure them to be permanent, they can be easily removed after the print is finished.
* Stabilizers will only be added if the lithophane is higher than *Minimum height before adding stabilizers*.
* Stabilizer height factor decides the height of the stabilizers in relation to the total height of the frame.
* The frame slope factor decides how sloped the connection between the front inside of the frame is to the back inside of the frame inwards towards the image.
* *Hangers* are tiny plastic loops that are placed on top of the lithophane, allowing you to thread them and suspend the print in a window frame or in front of a light source.

### Export preferences
* The STL 3D mesh file format supports both an ascii and a binary format. If you don't know what that means, just leave it on *Binary*. *Binary* takes up less space and the result is exactly the same when importing the file into a slicer.
* *Always overwrite existing file* simply does what it says. Normally LithoMaker asks you if you want to overwrite an existing file. Checking this will disable that dialog and simply *always* overwrite it without asking.

### Preparing a photo for conversion
First of all, make sure your image is of high quality. Low quality JPEG's, often grabbed from the internet, look terrible as lithophanes due to their many JPEG artifacts. So make sure you use a high quality image with no artifacts to begin with.

To get the best results, you need to do a bit of work on your photo to ensure it is optimal for conversion. The following describes a workflow which will give you optimal results using the open source image editor [Gimp](https://www.gimp.org/).
* Open the photo you want to convert.
* Choose **Filters->Enhance->Noise Reduction**. Set *Strength* so that it removes noisy prickling pixels without loosing too much detail. For a large image a value of 4 is good. For smaller images you need to go lower. Experiment! There are no wrong answers. The point is to smooth over surfaces of the subject, but keep details.
* Choose **Colors->Auto->White balance**. This stretches the color dynamics of the image as far as it can go, meaning it will have the best possible contrast. This is hugely important to get the best looking lithophane. Don't worry if the colors look slightly odd. We remedy that in the next steps.
* Now select and crop the image if you want to using the select tool and **Image->Crop to Selection**.
* If your image is wider than 1500 pixels, I recommend now scaling it to 1500 pixels width using **Image->Scale Image...** and adjusting *Width* to 1500. **Don't scale it if it's already lower than 1500 pixels in width**. This step is optional of course, but keep in mind that more pixels means a more complex 3D mesh, and at some point the details are simply too tiny to print anyway, so the lithophane will just take longer to process, take up more space on your disk, and take up more memory when importing into the slicer.
* Now convert the image to grayscale using **Image->Mode->Grayscale** and voila! It suddenly looks really good!
* The final step is to export the image as a PNG using **File->Export As...**.

## Release notes

#### Version 0.7.1 (In progress, unreleased)
* Added 'PNG' to main UI input filename label
* Now always converts to grayscale if color image is detected
* Added default values as tooltips for all config widgets

#### Version 0.7.0 (14th Nov 2021)
* Added optional hangers to top of frame
* Added config options for hangers
* Fixed resetToDefault for sliders
* Added 'elephant.png', 'hummingbird.png' and 'cheetah.png' to repository. All 3 images under public domain (I took them myself)
* Now shows preferences on first run

#### Version 0.6.1 (13th Nov 2021)
* Corrected frame front side distance from backside
* Fixed config key to 'LithoMaker'
* Changed lineedits to sliders to avoid decimal locale conversion issues and for ease of use
* Moved some configs from 'main' to 'render'

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
