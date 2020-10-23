/* Samantha Zimmermann
   COSC 494
   Lab 3 - Jgraph
   This file contains strings to generate the jgraph.
 */

/* Example query:
   newline poly pcfill X X X pts X X  X X  X X  X X  color X X X
                       ^ fill color                        ^ outline color
*/
#ifndef QUERIES_H

#include <string>

using namespace std;

/* Drawing each pixel, with specified color and end points,
   always has a black outline */
const string newBlock = "newline poly pcfill %f %f %f "
                        "pts %d %d  %d %d  %d %d  %d %d "
                        "color 0 0 0\n";

/* Required string */ 
const string newGraph = "newgraph\n";

/* Sets up chart appearance */ 
const string hashOptions = "no_grid_lines no_draw_hash_marks no_auto_hash_labels\n";

/* Sets up X and Y axis */
const string xaxis = "xaxis size %d min %d max %d no_draw_axis\n";
const string yaxis = "yaxis size %d min %d max %d no_draw_axis\n";

/* Places grid label (number) at given location
   location : label
*/
const string gridLabel = "\thash_label at %d : %d\n";

/* Creates a line of thickness double of default. Must specify starting and ending point. */
const string gridLine = "newcurve marktype none linetype solid linethickness 2 pts %d %d  %d %d\n";

#endif
