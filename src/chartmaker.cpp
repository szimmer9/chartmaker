/* Samantha Zimmermann
   COSC 494
   Lab 3 - Jgraph
   This program generates a jgraph to create a knitting chart from an input
   PGM image file or ASCII art in a plaintext file. It outputs the 
   jgraph info onto standard output.

   TODO: Takes manipulation args on stdin (can use input file)
 */

#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "../inc/queries.h"

using namespace std;

struct pixel
{
    /* Common 3-digit 0-255 color representation */
    unsigned char red, green, blue;

    /* Update the values of the pixels */
    void set(int r, int g, int b) { red = r; green = g; blue = b; }

    // TODO: make a scale function
};

// TODO: make an "arguments" struct, or a struct with all the info?

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
    fprintf(stderr, "maxval: %d\n", maxval);

    // For some reason, my PPM file was off by one? 
    inputFile.get(c);

    /* Doesn't handle the file having extra pixels, just too few */
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            /* Read in the pixel by char and normalize to 255 */
            inputFile.get(c); 
            // fprintf(stderr, "%c ", c);
            image[i][j].red = (unsigned) c;
            inputFile.get(c); 
            // fprintf(stderr, "%c ", c);
            image[i][j].green = (unsigned) c;
            inputFile.get(c); 
            // fprintf(stderr, "%c\n", c);
            image[i][j].blue = (unsigned) c;
            // inputFile >> image[i][j].red >> image[i][j].green >> image[i][j].blue;
            // image[i][j].red *= sf;
            // image[i][j].green *= sf;
            // image[i][j].blue *= sf;
            count++;
            fprintf(stderr, "pixel: %d %d %d\n", image[i][j].red, image[i][j].green, image[i][j].blue);
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
// TODO: on hold until I get the first thing working
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
    inputFile >> x >> y;

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

/* Writes the chart stored in image to standard out, as a jgraph. 
 */
void writeGraph(vector< vector<pixel> >& image)
{
    int x = image.size(), y = image[0].size();
    /* Coordinate of where to print the thing */
    int xbl = 0, ybl = 0, xbr = 0, ybr = 0; 
    int xtl = 0, ytl = 0, xtr = 0, ytr = 0;
    double r, g, b;

    // FIXME: make labels going right to left
    /* Print the header info */
    printf(newGraph.c_str());

    printf(xaxis.c_str(), 0, x);
    printf(hashOptions.c_str());
    // Grid labels, at given increment (change from 10)
    for(int i = 10; i <= x; i += 10)
        printf(gridLabel.c_str(), i, i);

    printf(yaxis.c_str(), 0, y);
    printf(hashOptions.c_str());
    // Grid labels, at given increment (change from 10)
    for(int i = 10; i <= y; i += 10)
        printf(gridLabel.c_str(), i, i);


    /* First coordinate */
    ytl = y;
    ytr = y;
    ybr = y-1;
    ybl = y-1;

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

    // Print out bold grid lines
    for(int i = 10; i <= y; i += 10)
        printf(gridLine.c_str(), 0, i, x, i);
    for(int i = 10; i <= x; i += 10)
        printf(gridLine.c_str(), i, 0, i, y);
}

// TODO: Add a manipulation function, that applies the given
//       arguments to the image and manipulates it before writing
// TODO: Add a separate function for each manipulation that needs
//       to be applied, ie tiling, justification, padding, etc.

int main(int argc, char** argv)
{
    /* Error and usage */
    if(argc != 3)
    {
        fprintf(stderr, "usage: chartmaker inputfilename [a/p]\n"
               "p = PPM, a = ASCII art text file\n");
        return -1;
    }

    vector<vector<pixel> > image;

    if(argv[2][0] == 'a') /* ASCII art file */
    {
        // vector< vector<pixel> > image = openText(argv[1]);
        printf("ascii art not yet implemented!\n");
        return 0;
    }
    else if(argv[2][0] == 'p') /* PPM image file */
        image = openPPM(argv[1]);
    else /* Bad argument */
    {
        fprintf(stderr, "error: bad argument %c\n"
               "p = PPM, (a = ASCII art text file)\n", argv[2]);
        return -1;
    }

    if (image.size() == 0) return -1;
    writeGraph(image);
}