// WII-MAZE Skeleton code written by Jason Erbskorn 2007
// Edited for ncurses 2008 Tom Daniels
// Updated for Esplora 2013 TeamRursch185
// Updated for DualShock 4 2016 Rursch
//gcc -o lab9 lab9.c -lncurses
// Headers

#include <stdio.h>
#include <math.h>
#include <ncurses/ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

// Mathematical constants
#define PI 3.141592653589

// Screen geometry
// Use ROWS and COLS for the screen height and width (set by system)
// MAXIMUMS
#define NUMCOLS 100
#define NUMROWS 80
#define MAXPOINTS 10000

// Character definitions taken from the ASCII table
#define AVATAR 'A'
#define WALL '*'
#define EMPTY_SPACE ' '

// Number of samples taken to form an average for the gyroscope data
// Feel free to tweak this. You may actually want to use the moving averages
// code you created last week
#define NUM_SAMPLES 10

// 2D character array which the maze is mapped into
char MAZE[NUMROWS][NUMCOLS];

// POST: Generates a random maze structure into MAZE[][]
//You will want to use the rand() function and maybe use the output %100.
//You will have to use the argument to the command line to determine how
//difficult the maze is (how many maze characters are on the screen).
//0 to 100 for difficulty
void generate_maze(int difficulty);

// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen
void draw_maze(void);

// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
void draw_character(int x, int y, char use);
void updatebuffer(double buffer[], int length, double new_item);
double avg(double buffer [], int num_items);

// Main - Run with './ds4rd.exe -d 054c:05c4 -D DS4_BT -t -g -b | ./lab9 25' piped into STDIN
void main(int argc, char* argv[])
{
double gx, gy, gz, xAvg, yAvg, zAvg, xMax, xMin, yMax, yMin, zMax, zMin;
double x[MAXPOINTS], y[MAXPOINTS], z[MAXPOINTS];
int t ,b1, b2, b3, b4, counter;
int lengthofavg = 0;
int result = 1;
int a = 39;
int b = 0;
	// setup screen
	initscr();
	refresh();
	// Generate and draw the maze, with initial avatar
	if (argc>1) {
		sscanf(argv[1], "%d", &lengthofavg );
	}
	generate_maze(lengthofavg);
	draw_maze();
	draw_character(a, b, 'A');
	
	// Read gyroscope data to get ready for using moving averages.
	int i;
	for(i=0; i<lengthofavg; ++i) {
		scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", &t, &gx, &gy, &gz, &b1, &b2, &b3, &b4);
	x[i] = gx;
	y[i] = gy;
	z[i] = gz;
	}
	// Event loop
	
	counter = 0;
	do
	{
		// Read data, update average
		scanf("%d, %lf, %lf, %lf, %d, %d, %d, %d", &t, &gx, &gy, &gz, &b1, &b2, &b3, &b4);
		updatebuffer(x, lengthofavg, gx);
		updatebuffer(y, lengthofavg, gy);
		updatebuffer(z, lengthofavg, gz);
		xAvg = avg(x, lengthofavg);
		yAvg = avg(y, lengthofavg);
		zAvg = avg(z, lengthofavg);
		
		// Is it time to move? if so, then move avatar
		++counter;
		if(counter % 100 == 0) { //delay movement
		if((MAZE[a][b+1]) == ' ') { //Can I fall?
			draw_character(a, b, ' ');
			b = b + 1; //increment y value to fall
			draw_character(a, b, 'A');
		}
		if((MAZE[a][b+1]) == '*') {
			endwin();
			printf("you lose");
		}
		}
		
		if((gx > 0.25) && ((MAZE[a-1][b] == ' '))) { //Avi moves Left if true
			draw_character(a, b, ' ');
			a = a - 1;
			draw_character(a, b, 'A');
		}
		
		if((gx < -0.25) && ((MAZE[a+1][b] == ' '))) { //Avi moves Right if true
			draw_character(a, b, ' ');
			a = a + 1;
			draw_character(a, b, 'A');
		}
			
		++counter;
		
		if((MAZE[a-1][b] == '*') && (MAZE[a-1][b] == '*') &&(MAZE[a][b+1] == '*'))
			break;
		}
		
	{	while(b < 84); // Change this to end game at right time
		endwin();
		if (result == 1){
			printf("YOU WIN!\n"); // Print the win message
	}
			else{
				printf("YOU LOSE!");
				{

//FUNCTIONS
// PRE: 0 < x < COLS, 0 < y < ROWS, 0 < use < 255
// POST: Draws character use to the screen and position x,y
//THIS CODE FUNCTIONS FOR PLACING THE AVATAR AS PROVIDED.
//
// >>>>DO NOT CHANGE THIS FUNCTION.<<<<
void draw_character(int x, int y, char use)
{
	mvaddch(y,x,use);
	refresh();
}


//**********************************************//


// POST: Generates a random maze structure into MAZE[][]
//You will want to use the rand() function and maybe use the output %100.
//You will have to use the argument to the command line to determine how
//difficult the maze is (how many maze characters are on the screen).
//0 to 100 for difficulty
void generate_maze(int difficulty) {
int i, j, r;
srand((int)time(0));
	for(i=0; i < NUMROWS; ++i) {
	for(j=0; j < NUMCOLS; ++j) {
	if (( rand() % 101) < difficulty) {
		MAZE[i][j] = '*';
		}
		else {
			MAZE[i][j] = ' ';
		}
	}
	}
}

// PRE: MAZE[][] has been initialized by generate_maze()
// POST: Draws the maze to the screen
void draw_maze(void) {
int i, j;
	for(i = 0; i < NUMROWS; ++i) {
	for(j = 0; j < NUMCOLS; ++j) {
		draw_character(i,j,MAZE[i][j]);
		}
		}
}

//shift length-1 elements of the buffer to the left and put the
//new_item on the right.
void updatebuffer(double buffer[], int length, double new_item) {
int i;
	for (i=0; i < length; ++i) {
		buffer[i] = buffer[i+1];
		}
		buffer[length - 1] = new_item;
}
	
// compute the average of the first num_items of buffer
double avg(double buffer [], int num_items) {
int i;
double average, total;
	for(i=0; i < num_items; ++i) {
		total = total + buffer[i];
	}
	average = total / (double)num_items;
	return average;
}
}
}
}
}

//END FUNCTIONS


