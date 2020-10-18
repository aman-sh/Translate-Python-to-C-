/**
	localizer.cpp

	Purpose: implements a 2-dimensional histogram filter
	for a robot living on a colored cyclical grid by 
	correctly implementing the "initialize_beliefs", 
	"sense", and "move" functions.

	This file is incomplete! Your job is to make these
	functions work. Feel free to look at localizer.py 
	for working implementations which are written in python.
*/

#include "localizer.h"
#include "helpers.cpp"
#include <stdlib.h>
#include "debugging_helpers.cpp"

using namespace std;

/**
	TODO - implement this function 
    
    Initializes a grid of beliefs to a uniform distribution. 

    @param grid - a two dimensional grid map (vector of vectors 
    	   of chars) representing the robot's world. For example:
    	   
    	   g g g
    	   g r g
    	   g g g
		   
		   would be a 3x3 world where every cell is green except 
		   for the center, which is red.

    @return - a normalized two dimensional grid of floats. For 
           a 2x2 grid, for example, this would be:

           0.25 0.25
           0.25 0.25
*/
vector< vector <float> > initialize_beliefs(vector< vector <char> > grid) {


	// your code here
 	//initializing grid size variables
  	int height,width;
  	height=grid.size();
  	width=grid[0].size();

  	//calculating belief per cell using the grid size
  	float belief_cell;
  	belief_cell=1.0/(height*width);

  	//initializing newGird with size(height and width) and belief per cell values
  	vector<vector<float> > newGrid(grid.size(),vector<float>(grid[0].size(),belief_cell));

	return newGrid;
}

/**
  TODO - implement this function

    Implements robot motion by updating beliefs based on the
    intended dx and dy of the robot.

    For example, if a localized robot with the following beliefs

    0.00  0.00  0.00
    0.00  1.00  0.00
    0.00  0.00  0.00

    and dx and dy are both 1 and blurring is 0 (noiseless motion),
    than after calling this function the returned beliefs would be

    0.00  0.00  0.00
    0.00  0.00  0.00
    0.00  0.00  1.00

  @param dy - the intended change in y position of the robot

  @param dx - the intended change in x position of the robot

    @param beliefs - a two dimensional grid of floats representing
         the robot's beliefs for each cell before sensing. For
         example, a robot which has almost certainly localized
         itself in a 2D world might have the following beliefs:

         0.01 0.98
         0.00 0.01

    @param blurring - A number representing how noisy robot motion
           is. If blurring = 0.0 then motion is noiseless.

    @return - a normalized two dimensional grid of floats
         representing the updated beliefs for the robot.
*/
vector< vector <float> > move(int dy, int dx,
  vector < vector <float> > beliefs,
  float blurring)
{

  vector< vector<float> > newGrid;

  // your code here
  //initialize grid size variables
  int height,width;
  height=beliefs.size();
  width=beliefs[0].size();
  //resizing newGrid with size(height,width) and initial value of 0
  newGrid = zeros(beliefs.size(),beliefs[0].size());
  //initialize variables for calculating new/updated i and j for newGrid
  int i,j,new_i,new_j;
  //starting the loop for calculating the new_i and new_j for newGrid based on dy and dx
  for (i=0;i<beliefs.size();i++) {
    for (j=0;j<beliefs[0].size();j++) {
      new_i=((i+dy+height)%width); //calculating new_i based on intended change in y position
      new_j=((j+dx+width)%height); //calculating new_j based on intended change in x position
      newGrid[new_i][new_j]=beliefs[i][j]; // updating grid values with updated beliefs
    }
  }
 return blur(newGrid, blurring);
}


/**
	TODO - implement this function

    Implements robot sensing by updating beliefs based on the
    color of a sensor measurement

	@param color - the color the robot has sensed at its location

	@param grid - the current map of the world, stored as a grid
		   (vector of vectors of chars) where each char represents a
		   color. For example:

		   g g g
    	   g r g
    	   g g g

   	@param beliefs - a two dimensional grid of floats representing
   		   the robot's beliefs for each cell before sensing. For
   		   example, a robot which has almost certainly localized
   		   itself in a 2D world might have the following beliefs:

   		   0.01 0.98
   		   0.00 0.01

   @param p_hit - the RELATIVE probability that any "sense" is
    	   correct. The ratio of p_hit / p_miss indicates how many
    	   times MORE likely it is to have a correct "sense" than
    	   an incorrect one.

   	@param p_miss - the RELATIVE probability that any "sense" is
    	   incorrect. The ratio of p_hit / p_miss indicates how many
    	   times MORE likely it is to have a correct "sense" than
    	   an incorrect one.

    @return - a normalized two dimensional grid of floats
    	   representing the updated beliefs for the robot.
*/
vector< vector <float> > sense(char color,
	vector< vector <char> > grid,
	vector< vector <float> > beliefs,
	float p_hit,
	float p_miss)
{


	// your code here
	//initialize grid size variables
  	int height,width;
  	height=beliefs.size();
  	width=beliefs[0].size();
  	//initialize newGrid with size(height,width) and initial value of 0
  	vector<vector<float> > newGrid(height,vector<float>(width,0.0));
  	//initialize int variables and check for color match
  	
  	for (int i=0;i<beliefs.size();i++) {
      for (int j=0;j<beliefs[0].size();j++) {
        //initializing bool color_check variable(true or false) checking if the colors matches the grid color
		bool color_check=(color==grid[i][j]); 
        //multiply p_hit with 1 if color matches and if not multiply p*miss that would update the newGrid
        newGrid[i][j]=beliefs[i][j]*(color_check*p_hit+(1-color_check)*p_miss);
        
      }
     }
	return normalize(newGrid);
}