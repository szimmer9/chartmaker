/* Samantha Zimmermann
   COSC 494
   Lab 3 - Jgraph
   This program generates a jgraph to create a knitting chart from an input
   PGM image file or ASCII art in a plaintext file. It outputs the 
   jgraph info onto standard output.
 */

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <string.h>

#include "../inc/queries.h"

using namespace std;

struct pixel
{
    /* Common 3-digit 0-255 color representation */
    unsigned char red, green, blue;

    /* Update the values of the pixels */
    void set(int r, int g, int b) { red = r; green = g; blue = b; }
};

struct arguments
{
    /* Tiling parameter: enable tiling and give parameters */
    bool tile;
    int tHoriz, tVert;

    /* Enable/disable gridlines */
    bool gridlines;

    /* If spacing specified, use this number, o/w use default */
    int gridSpacing;

    /* Enable/disable gridline numbering */
    bool numbering;

    /* Enable/disable printing numbers on every tile of the grid */
    bool allNumbers;

    /* Enable padding */
    bool padding;
    /* Must specify int to pad by */
    int padSize;

    /* Background color, for padding and ASCII art */
    /* TODO: Implement */
    pixel bgColor;

    /* Function that sets to the default arguments */
    void setDefault()
    {
        tile = false;
        tHoriz = 1;
        tVert = 1;
        gridlines = true;
        gridSpacing = 10;
        numbering = true;
        allNumbers = false;
        padding = false;
        padSize = 0;
        bgColor.set(255, 255, 255);
    }
};

/* Opens the PPM file specified in fn, reads in the pixels, and 
   returns a vector of pixels.
*/
vector< vector<pixel> > openPPM(char* fn)
{
    ifstream inputFile;
    string line;
    int x, y, maxval, count = 0, numPixels;
    vector< vector<pixel> > image;
    char c;
    // printf("openPPM\n");

    inputFile.open(fn, ios::binary);
    if(!inputFile.is_open())
    {
        perror(fn);
        image.resize(0);
        return image;
    }

    /* Check the header -- P6 for PPM files */
    inputFile >> line;
    if(line != "P6")
    {
        fprintf(stderr, "wrong format -- header not P5\n");
        image.resize(0);
        return image;
    }

    /* PPM file lists dimensions as rows cols */
    inputFile >> y >> x;

    /* Resize the image to be x by y */
    image.resize(x);
    for(int i = 0; i < x; i++)
        image[i].resize(y);
    numPixels = x*y;

    inputFile >> maxval;

    // For some reason, my PPM file was off by one? 
    inputFile.get(c);

    /* Doesn't handle the file having extra pixels, just too few */
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            /* Read in the pixel by char and normalize to 255 */
            inputFile.get(c); 
            image[i][j].red = (unsigned) c;
            inputFile.get(c); 
            image[i][j].green = (unsigned) c;
            inputFile.get(c); 
            image[i][j].blue = (unsigned) c;

            /* Count the total number of pixels that have been read in */
            count++;
        }
    }

    /* Checking if too few pixels were read in */
    if (count < numPixels)
    {
        fprintf(stderr, "Error: bad file, expected %d pixels but got %d\n", numPixels, count);
        image.resize(0);
        return image;
    }

    inputFile.close();

    return image;
}

/* Opens the text file specified in fn, reads in the pixels, and 
   returns a vector of pixels.
*/
vector< vector<pixel> > openText(char* fn)
// TODO: on hold, not tested
{
    ifstream inputFile;
    string line;
    istringstream iss;
    pixel tempPixel;
    char tempSymbol;
    map<char, pixel> symbols;
    int numSymbols;
    int x, y;

    vector< vector<pixel> > image;

    /* Set to a shade of gray, in case of no header info */
    tempPixel.red = 125;
    tempPixel.green = 125;
    tempPixel.blue = 125;
    
    inputFile.open(fn);
    if(!inputFile.is_open())
    {
        perror(fn);
        image.resize(0);
        return image;
    }

    /* First, read the header and grab the size and symbols */
    inputFile >> y >> x;

    while(inputFile >> tempSymbol)
    {
        /* ~ symbol marks the end of the header */
        if(tempSymbol == '~') break;
        /* If not the end of the header, grab pixels */
        inputFile >> tempPixel.red >> tempPixel.green >> tempPixel.blue;
        symbols.insert(pair<char, pixel>(tempSymbol, tempPixel));
    }

    /* If there was no header, map was empty -> insert first symbol */
    if(symbols.empty())
        symbols.insert(pair<char, pixel>(line[0], tempPixel));

    /* Add a background character, white by default */
    // TODO: Make this handle the "background" specifier
    tempPixel.set(0, 0, 0);
    symbols.insert(pair<char, pixel>(' ', tempPixel));

    /* Next, read in the info line at a time */
    while(inputFile >> line)
    {
        /* Convert each char into a pixel one at a time */

        /* Use stringstream to get rid of whitespace */
        iss.clear();
        iss.str(line);

        /* Compare each symbol to a saved symbol and make a pixel
           to put into the image vector */
        // FIXME: thing["abc"];
        // map operator[]
        for(int i = 0; i < line.size(); i++)
        {
            /* "Search" for the symbol, use that pixel */
            tempPixel = symbols[line[i]];

        }
    }

    inputFile.close();

    return image;
}

/* Manipulates the chart based on input arguments */
void manipulateChart(vector<vector<pixel> >& image, arguments args)
{
    int oldX = image.size(), oldY = image[0].size();
    int newX, newY;

    /* Perform the tiling */
    if(args.tile)
    {
        /* Calculate new total size */
        newX = args.tHoriz * oldX;
        newY = args.tVert * oldY;

        image.resize(newX);
        for(int i = 0; i < newX; i++)
            image[i].resize(newY);

        /* Tile the image */
        /* Across the big image */
        for(int i = 0; i < newX; i++)
        {
            for(int j = 0; j < newY; j++)
            {
                image[i][j] = image[i % oldX][j % oldY];
            }
        }

        oldX = newX;
        oldY = newY;
    }

    if(args.padding)
    {
        /* Add outer padding */
        newX = oldX + (args.padSize*2); 
        newY = oldY + (args.padSize*2); 

        image.resize(newX);
        for(int i = 0; i < newX; i++)
            image[i].resize(newY);

        for(int i = oldX-1; i > 0; i--)
        {
            for(int j = oldY-1; j >= 0; j--)
            {
                image[i+args.padSize][j+args.padSize] = image[i][j];
            }
        }

        /* Do the top row */
        for(int i = 0; i < newX; i++)
        {
            for(int j = 0; j < args.padSize; j++)
                image[i][j] = args.bgColor;
        }

        /* Do the bottom row */
        for(int i = 0; i < newX; i++)
        {
            for(int j = newY - args.padSize; j < newY; j++)
                image[i][j] = args.bgColor;
        }

        /* Do the left side */
        for(int i = 0; i < args.padSize; i++)
        {
            for(int j = 0; j < newY; j++)
                image[i][j] = args.bgColor;
        }

        /* Do the right side */
        for(int i = newX - args.padSize; i < newX; i++)
        {
            for(int j = 0; j < newY; j++)
                image[i][j] = args.bgColor;
        }
    }
}

/* Writes the chart stored in image to standard out, as a jgraph. 
 */
void writeGraph(vector< vector<pixel> >& image, arguments args)
{
    int x, y;
    /* Coordinate of where to print the thing */
    int xbl = 0, ybl = 0, xbr = 0, ybr = 0; 
    int xtl = 0, ytl = 0, xtr = 0, ytr = 0;
    double r, g, b;

    /* Call graph manipulation function */
    manipulateChart(image, args);
    
    x = image.size();
    y = image[0].size();

    /* Print the header info */
    printf(newGraph.c_str());

    /* X axis */
    printf(xaxis.c_str(), 0, y);
    printf(hashOptions.c_str());
    if(args.numbering && args.gridlines)
    {
        if(args.allNumbers)
        {
            /* Print a number on every line */
            for(int i = y; i > 0; i--)
                printf(gridLabel.c_str(), i, y-i);
        }
        else
        {
            /* Print a number on line at every increment */
            for(int i = y - args.gridSpacing; i > 0; i-=args.gridSpacing)
                printf(gridLabel.c_str(), i, y-i);
        }
    }

    /* Y axis */
    printf(yaxis.c_str(), 0, x);
    printf(hashOptions.c_str());
    if(args.numbering && args.gridlines)
    {
        if(args.allNumbers)
        {
            /* Print a number on every line */
            for(int i = 1; i <= x; i++)
                printf(gridLabel.c_str(), i, i);
        }
        else
        {
            /* Print a number on line at every increment */
            for(int i = args.gridSpacing; i <= x; i += args.gridSpacing)
                printf(gridLabel.c_str(), i, i);
        }
    }

    /* First coordinate */
    ytl = x;
    ytr = x;
    ybr = x-1;
    ybl = x-1;

    for(int i = 0; i < x; i++)
    {
        /* Start at the left each time */
        xtl = 0;
        xtr = 1;
        xbl = 0;
        xbr = 1;
        for(int j = 0; j < y; j++)
        {
            r = ((unsigned int) image[i][j].red) / 255.0;
            g = ((unsigned int) image[i][j].green) / 255.0;
            b = ((unsigned int) image[i][j].blue) / 255.0;
            /* Order: bl, tl, tr, br  */
            printf(newBlock.c_str(), 
                   r, g, b,
                   xbl, ybl, xtl, ytl, xtr, ytr, xbr, ybr);
            /* Move to the right */
            xtl++;
            xtr++;
            xbl++;
            xbr++;
        }
        /* Move down */
        ytl--;
        ytr--;
        ybl--;
        ybr--;
    }

    if(args.gridlines)
    {
        /* Print out bold gridlines */
        for(int i = args.gridSpacing; i <= x; i += args.gridSpacing)
            printf(gridLine.c_str(), 0, i, y, i);
        for(int i = y - args.gridSpacing; i > 0; i-=args.gridSpacing)
            printf(gridLine.c_str(), i, 0, i, x);
    }
}

/* Main function */
int main(int argc, char** argv)
{
    /* Error and usage */
    if(argc < 3)
    {
        fprintf(stderr, "usage: chartmaker inputfilename [a/p] [arguments]\n"
               "p = PPM, a = ASCII art text file\n");
        return -1;
    }

    vector<vector<pixel> > image;
    arguments args;
    int r, g, b;

    if(argv[2][0] == 'a') /* ASCII art file */
    {
        printf("ascii art not yet implemented!\n");
        // TODO:
        return 0;
        // image = openText(argv[1]);
    }
    else if(argv[2][0] == 'p') /* PPM image file */
        image = openPPM(argv[1]);
    else /* Bad argument */
    {
        fprintf(stderr, "error: bad argument %c\n"
               "p = PPM, (a = ASCII art text file)\n", argv[2]);
        return -1;
    }

    /* Check if image was actually empty */
    if (image.size() == 0) 
    {
        fprintf(stderr, "Empty image\n");
        return -1;
    }

    /* Process arguments */
    args.setDefault();

    /* Start at 3 to skip "./chartmaker inputfilename [a/p]" */
    for(int i = 3; i < argc; i++)
    {
        if(!strcmp(argv[i], "no_gridlines"))
            args.gridlines = false;
        else if(!strcmp(argv[i],"grid_spacing"))
        {
            args.gridlines = true;
            sscanf(argv[i+1], "%d", &args.gridSpacing);

            /* Skip the next argument already read in */
            i++;
        }
        else if(!strcmp(argv[i], "no_numbering"))
        {
            args.numbering = false;
            args.allNumbers = false;
        }
        else if(!strcmp(argv[i], "all_numbers"))
            args.allNumbers = true;
        else if(!strcmp(argv[i], "tile"))
        {
            args.tile = true;
            sscanf(argv[i+1], "%d", &args.tHoriz);
            sscanf(argv[i+2], "%d", &args.tVert);

            /* Skip the next argument already read in */
            i+=2;
        }
        else if(!strcmp(argv[i], "pad"))
        {
            args.padding = true;
            sscanf(argv[i+1], "%d", &args.padSize);
            i++;
        }
        else if(!strcmp(argv[i], "background"))
        {
            sscanf(argv[i+1], "%d", &r);
            sscanf(argv[i+2], "%d", &g);
            sscanf(argv[i+3], "%d", &b);
            args.bgColor.red = (unsigned char) r;
            args.bgColor.green = (unsigned char) g;
            args.bgColor.blue = (unsigned char) b;
            i+=3;
        }
        else /* Bad arg */
            fprintf(stderr, "Argument %s not accepted\n", argv[i]);
    }

    writeGraph(image, args);
}