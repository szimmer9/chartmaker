# Chartmaker

Chartmaker is a program that generates a knitting chart from a PPM
file using jgraph.

## Setup the repository
1. Clone this repository
2. Run setup script with command `sh setup.sh`

All set up!

## Using Chartmaker
Use the script `convert.sh` to easily utilize jgraph and chartmaker:
```
sh convert.sh <input file name> <output file name> <arguments>
```

This converts your input file to the specified output file and applies the arguments (outlined below). Specifying no arguments will output a chart of the same dimensions of the input file, with the same colors, and with gridlines and labels at increments of 10 across the bottom and left sides. Whatever the extension of the output file is, it will convert it to that file type. So, for example, if you specify .pdf, it will convert to PDF, and if you specify .jpg, it will convert to JPG. 

### Arguments
The following arguments are currently supported:
* `no_gridlines` : turns off bold gridlines and numbering.
* `grid_spacing X` : specifies increment of gridlines and number labeling. The default is every 10 blocks.
* `no_numbering` : turns off number labels along width and height. Will still print bold gridlines.
* `all_numbers` : will print a number on every box. Not recommended for very large charts. Will still print gridlines.

The following arguments are currently planned to be added:
* `tile X Y P` : tile the input image, by the given dimensions, where X is the width and Y is the height. Third parameter P specifies how many padding blocks should occur between each tile.
* `pad X` : pads the overall image by X blocks on each edge. If used with tiling, this will pad the resulting image after the tiling and tile padding is performed.
* `background R G B` : specifies the color of the padding blocks as an RGB value with each field ranging 0 to 255. White by default.

### Using the executables directly
If you want to use the executables individually, you can do that as well. Chartmaker is ran with the following arguments:
`./chartmaker <filename> <a/p> <arguments>`

* filename: specifies the input file name
* a/p: specifies PPM file or ASCII art file (Note: ASCII art not yet implemented)

Chartmaker outputs the jgraph on standard out. To put this output into a file, you can use shell redirection and specify the name of your output file. Here's an example:
```
./chartmaker myfile.ppm p > myoutput.jgr
```
So, this converts the PPM file myfile to a jgraph file called myoutput. It will create the file myoutput.jgr if it does not exist, or if it does exist, it will overwrite it.

[Please see this webpage](http://web.eecs.utk.edu/~jplank/plank/jgraph/jgraph.html) for more info on how to use jgraph.
