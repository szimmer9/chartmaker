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
sh convert.sh <input file name> <output file name>
```

### Using the executable directly
If you want to use the executables individually, you can do that as well. Chartmaker is ran with the following arguments:
`./chartmaker <filename> <a/p>`

* filename: specifies the input file name
* a/p: specifies PPM file or ASCII art file (ASCII art not yet implemented)

Chartmaker outputs the jgraph on standard out. To put this output into a file, you can use shell redirection and specify the name of your output file. Here's an example:
```
./chartmaker myfile.ppm p > myoutput.jgr
```
So, this converts the PPM file myfile to a jgraph file called myoutput. It will create the file myoutput.jgr if it does not exist, or if it does exist, it will overwrite it.

[Please see this webpage](http://web.eecs.utk.edu/~jplank/plank/jgraph/jgraph.html) for more info on jgraph.
