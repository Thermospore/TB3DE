// heads up, this code is NOT pretty lol

#include <iostream>
#include <cmath>
#include <sstream>
#include <windows.h>
#include <vector>
using namespace std;

const double INF = 9999999; // used to represent depth of empty space

struct engineSettings
{
	// cam dist to origin
	double d;
	
	// font size
	int fontW;
	int fontH;
	
	// console res (in chars)
	int resW;
	int resH;
	
	// window coord range
	double windowH;
	double windowW;
	
	// change in angle per frame
	double radPerFrame;
	
	// enable rotations
	bool xyRot;
	bool xzRot;
	
	// change in x per frame
	double xDelta;
	
	// enable perspective correction
	bool persp;
	
	// label vertices (a b c)
	bool vertLabels;
	
	// in hz. not exactly frame rate,
	// it's how long the engine waits between drawing frames
	int framerate;
	
	bool specialFill;
};

struct vert
{
	double x;
	double y;
	double z;
};

struct tri
{
	vert a;
	vert b;
	vert c;
	char fill;
};

int main()
{
	engineSettings eng;
	
	// default engine setings
	eng.d = 2;
	eng.fontW = 8;
	eng.fontH = 16;
	eng.resW = 120;
	eng.resH = 30;
	eng.windowH = 1;
	eng.windowW = eng.windowH/(eng.resH*eng.fontH)*(eng.resW*eng.fontW);
	
	eng.radPerFrame = .05;
	eng.xyRot = false;
	eng.xzRot = true;
	eng.xDelta = 0;
	
	eng.persp = true;
	eng.vertLabels = true;
	eng.specialFill = false;
	eng.framerate = 67;
	
	// init tri list
	vector<tri> tris;
	
	/*// og test
	eng.xyRot = true;
	tris.push_back(
		{vert{-.45,-.22,.46},
		 vert{-.15,-.46,.16},
		 vert{-.26,-.15,.11},
		 '.'});
	tris.push_back(
		{vert{-.3,.3,0},
		 vert{.3,.15,0},
		 vert{0,-.3,0},
		 '{'});
	tris.push_back(
		{vert{-.3,.3,0},
		 vert{.3,.15,0},
		 vert{.4,.3,.4},
		 'N'});
	tris.push_back(
		{vert{.3,-.3,0},
		 vert{.3,-.5,0},
		 vert{.4,-.3,-.4},
		 ';'});/**/
	
	/*// 100 randos
	eng.d = 3;
	eng.windowH = 1.5;
	eng.radPerFrame = .03;
	eng.vertLabels = false;
	eng.specialFill = true;
	tris.push_back(
		{vert{-0.01,-0.04,-0.53},
		vert{-0.97,0.49,0.71},
		vert{-0.19,0.65,0.55},
		'.'});
	tris.push_back(
		{vert{0.63,0.92,0.96},
		vert{-0.38,0.29,-0.82},
		vert{-0.19,-0.79,0.57},
		'.'});
	tris.push_back(
		{vert{0.26,-0.97,-0.39},
		vert{-0.54,0.23,-0.14},
		vert{-0.89,-0.04,0.07},
		'.'});
	tris.push_back(
		{vert{0.22,0.38,0.23},
		vert{-0.25,0.19,-0.66},
		vert{-0.13,0.96,0.20},
		'.'});
	tris.push_back(
		{vert{-0.05,0.93,-0.53},
		vert{0.20,-0.05,0.68},
		vert{0.88,-0.09,0.01},
		'.'});
	tris.push_back(
		{vert{0.62,0.25,-0.04},
		vert{-0.15,-0.81,0.07},
		vert{0.11,0.33,0.01},
		'.'});
	tris.push_back(
		{vert{-0.96,-0.80,0.85},
		vert{0.70,0.61,-0.19},
		vert{-0.53,0.64,0.91},
		'.'});
	tris.push_back(
		{vert{0.88,0.03,0.56},
		vert{-0.83,-0.30,0.63},
		vert{0.90,-0.09,0.13},
		'.'});
	tris.push_back(
		{vert{0.63,-0.72,-0.70},
		vert{0.94,-0.07,0.71},
		vert{-0.42,-0.69,0.22},
		'.'});
	tris.push_back(
		{vert{0.71,0.76,0.31},
		vert{-0.72,-0.51,-0.65},
		vert{-0.06,0.89,-0.16},
		'.'});
	tris.push_back(
		{vert{-0.48,0.51,0.45},
		vert{0.97,-0.38,-0.02},
		vert{0.60,0.93,0.40},
		'.'});
	tris.push_back(
		{vert{0.73,-0.22,-0.66},
		vert{0.40,0.74,-0.25},
		vert{0.32,-0.26,0.85},
		'.'});
	tris.push_back(
		{vert{0.45,0.32,-0.51},
		vert{0.33,0.54,-0.22},
		vert{0.34,0.10,-0.10},
		'.'});
	tris.push_back(
		{vert{-0.23,-0.42,-0.01},
		vert{-0.98,0.58,-0.65},
		vert{-0.75,0.50,0.14},
		'.'});
	tris.push_back(
		{vert{0.40,-0.36,0.45},
		vert{-0.23,0.80,-0.83},
		vert{-0.05,0.05,-0.82},
		'.'});
	tris.push_back(
		{vert{-0.09,0.54,0.70},
		vert{0.28,0.08,-0.12},
		vert{0.41,-0.65,0.48},
		'.'});
	tris.push_back(
		{vert{-0.19,-0.52,-0.26},
		vert{-0.93,-0.40,-0.62},
		vert{-0.12,0.94,-0.68},
		'.'});
	tris.push_back(
		{vert{0.31,-0.44,0.46},
		vert{-0.81,0.67,0.78},
		vert{-0.45,-0.92,0.60},
		'.'});
	tris.push_back(
		{vert{0.60,0.96,-0.25},
		vert{-0.91,-0.74,-0.43},
		vert{0.88,0.81,-0.71},
		'.'});
	tris.push_back(
		{vert{-0.63,-0.81,-0.60},
		vert{0.98,-0.60,-0.04},
		vert{0.14,-0.23,-0.51},
		'.'});
	tris.push_back(
		{vert{-0.30,0.45,0.33},
		vert{-0.71,-0.31,-0.01},
		vert{0.34,0.45,0.91},
		'.'});
	tris.push_back(
		{vert{-0.26,0.66,-0.35},
		vert{0.91,-0.79,0.31},
		vert{0.00,0.15,0.53},
		'.'});
	tris.push_back(
		{vert{0.93,0.67,-0.32},
		vert{0.16,0.63,0.78},
		vert{0.06,0.76,0.21},
		'.'});
	tris.push_back(
		{vert{0.24,0.92,0.95},
		vert{0.13,0.24,0.05},
		vert{-0.88,0.40,0.52},
		'.'});
	tris.push_back(
		{vert{0.24,-0.64,0.94},
		vert{0.51,0.64,0.60},
		vert{0.21,-0.99,0.60},
		'.'});
	tris.push_back(
		{vert{0.40,-0.08,0.70},
		vert{-0.83,0.89,0.09},
		vert{0.24,0.41,-0.71},
		'.'});
	tris.push_back(
		{vert{-0.95,-0.77,0.30},
		vert{-0.13,0.81,-0.52},
		vert{-0.95,0.11,0.26},
		'.'});
	tris.push_back(
		{vert{-0.93,-0.78,-0.32},
		vert{0.80,0.27,-0.39},
		vert{0.82,-0.10,-0.25},
		'.'});
	tris.push_back(
		{vert{0.22,-0.91,0.32},
		vert{0.50,0.33,-0.11},
		vert{0.43,0.85,0.81},
		'.'});
	tris.push_back(
		{vert{-0.85,-0.85,0.64},
		vert{0.24,0.85,0.71},
		vert{0.87,-0.75,0.30},
		'.'});
	tris.push_back(
		{vert{-0.08,0.08,0.20},
		vert{-0.64,0.69,0.60},
		vert{0.78,0.96,-0.17},
		'.'});
	tris.push_back(
		{vert{0.30,0.15,0.46},
		vert{-0.74,-0.36,0.67},
		vert{0.56,-0.84,-0.65},
		'.'});
	tris.push_back(
		{vert{0.35,0.73,-0.46},
		vert{0.43,-0.16,0.86},
		vert{-0.75,0.08,0.54},
		'.'});
	tris.push_back(
		{vert{-0.68,0.45,0.89},
		vert{-0.08,0.03,-0.70},
		vert{-0.43,0.74,-0.75},
		'.'});
	tris.push_back(
		{vert{-0.11,0.01,0.20},
		vert{-0.26,0.24,0.87},
		vert{-0.52,0.18,0.52},
		'.'});
	tris.push_back(
		{vert{-0.48,-0.66,-0.61},
		vert{0.87,0.55,0.29},
		vert{-0.37,0.59,-0.75},
		'.'});
	tris.push_back(
		{vert{-0.94,0.38,-0.70},
		vert{-0.05,-0.54,-1.00},
		vert{0.57,-0.77,-0.79},
		'.'});
	tris.push_back(
		{vert{-0.70,-0.31,0.59},
		vert{-0.23,0.58,0.56},
		vert{0.72,0.97,-0.55},
		'.'});
	tris.push_back(
		{vert{-0.14,-0.09,0.28},
		vert{0.72,-0.38,-0.45},
		vert{-0.23,-0.11,0.12},
		'.'});
	tris.push_back(
		{vert{0.19,0.26,0.93},
		vert{-0.45,0.03,-0.76},
		vert{0.07,0.40,0.27},
		'.'});
	tris.push_back(
		{vert{0.64,0.46,1.00},
		vert{-0.36,0.91,-0.05},
		vert{-0.33,0.47,-0.52},
		'.'});
	tris.push_back(
		{vert{-0.36,-0.78,-0.91},
		vert{0.61,-0.33,-0.61},
		vert{0.02,0.39,-0.09},
		'.'});
	tris.push_back(
		{vert{0.77,-0.14,0.59},
		vert{0.31,0.12,-0.36},
		vert{-0.43,-0.93,-0.50},
		'.'});
	tris.push_back(
		{vert{-0.07,0.62,-0.29},
		vert{-0.20,0.87,0.46},
		vert{-0.92,0.83,-0.88},
		'.'});
	tris.push_back(
		{vert{0.73,-0.23,-0.76},
		vert{-0.96,-0.23,0.79},
		vert{-0.66,-0.90,-0.52},
		'.'});
	tris.push_back(
		{vert{-0.90,-0.10,0.70},
		vert{-0.56,-0.79,0.31},
		vert{-0.34,0.56,0.60},
		'.'});
	tris.push_back(
		{vert{-0.50,-0.04,-0.99},
		vert{-0.43,-0.78,0.67},
		vert{0.70,-0.88,-0.67},
		'.'});
	tris.push_back(
		{vert{0.67,0.07,-0.93},
		vert{-0.23,-0.93,-0.35},
		vert{-0.02,0.76,-0.39},
		'.'});
	tris.push_back(
		{vert{-0.90,-0.57,0.54},
		vert{-0.94,0.72,0.44},
		vert{-0.52,-0.42,0.17},
		'.'});
	tris.push_back(
		{vert{-0.85,0.99,0.05},
		vert{-0.38,-0.93,0.67},
		vert{0.33,0.08,0.67},
		'.'});
	tris.push_back(
		{vert{0.03,0.78,0.29},
		vert{0.67,0.63,-0.15},
		vert{0.31,-0.57,-0.23},
		'.'});
	tris.push_back(
		{vert{-0.35,0.92,0.36},
		vert{0.19,-0.92,-0.60},
		vert{0.38,0.60,0.79},
		'.'});
	tris.push_back(
		{vert{0.97,0.45,-0.03},
		vert{0.78,-0.72,-0.35},
		vert{-0.13,-0.68,0.57},
		'.'});
	tris.push_back(
		{vert{0.88,0.76,0.24},
		vert{0.58,0.17,0.29},
		vert{0.10,-0.52,0.91},
		'.'});
	tris.push_back(
		{vert{-0.49,-0.37,0.37},
		vert{0.05,0.37,0.10},
		vert{0.08,0.83,-0.62},
		'.'});
	tris.push_back(
		{vert{-0.87,0.78,0.26},
		vert{0.67,-0.73,0.56},
		vert{0.29,-0.37,0.40},
		'.'});
	tris.push_back(
		{vert{0.19,0.02,-0.91},
		vert{-0.41,0.97,0.32},
		vert{-0.28,-0.27,-0.65},
		'.'});
	tris.push_back(
		{vert{-0.91,0.45,-0.03},
		vert{-0.09,-0.93,-0.92},
		vert{-0.63,-0.76,0.66},
		'.'});
	tris.push_back(
		{vert{-0.93,-0.47,-0.38},
		vert{0.06,0.93,0.74},
		vert{0.12,-0.07,-0.41},
		'.'});
	tris.push_back(
		{vert{0.84,0.14,-0.38},
		vert{-0.92,0.25,-0.80},
		vert{0.47,-0.44,-0.09},
		'.'});
	tris.push_back(
		{vert{0.21,0.06,-0.95},
		vert{0.96,0.97,-0.57},
		vert{0.07,-0.78,0.85},
		'.'});
	tris.push_back(
		{vert{0.98,-0.77,-0.72},
		vert{-0.41,-0.13,-0.51},
		vert{-0.09,0.13,-0.54},
		'.'});
	tris.push_back(
		{vert{0.35,-0.08,-0.20},
		vert{0.14,-0.74,-0.15},
		vert{-0.18,-0.34,-0.25},
		'.'});
	tris.push_back(
		{vert{-0.08,0.62,0.85},
		vert{-0.19,0.50,-0.11},
		vert{0.22,-0.57,0.63},
		'.'});
	tris.push_back(
		{vert{-0.68,-0.21,0.32},
		vert{-0.49,0.18,-0.98},
		vert{-0.77,-0.31,-0.65},
		'.'});
	tris.push_back(
		{vert{-0.99,0.29,0.12},
		vert{-0.07,-0.40,0.64},
		vert{0.79,-0.96,0.62},
		'.'});
	tris.push_back(
		{vert{0.09,-0.43,0.82},
		vert{-0.02,0.79,0.64},
		vert{0.22,0.90,1.00},
		'.'});
	tris.push_back(
		{vert{0.17,0.63,0.41},
		vert{0.66,-0.68,0.60},
		vert{0.97,-0.02,0.61},
		'.'});
	tris.push_back(
		{vert{0.57,-0.27,0.03},
		vert{-0.31,-0.76,-0.22},
		vert{-0.09,-0.92,-0.80},
		'.'});
	tris.push_back(
		{vert{0.46,-0.34,-0.82},
		vert{0.01,-0.71,-0.41},
		vert{-0.19,-0.01,0.81},
		'.'});
	tris.push_back(
		{vert{-0.18,-0.57,-0.31},
		vert{0.73,-0.56,0.30},
		vert{0.60,0.12,0.48},
		'.'});
	tris.push_back(
		{vert{-0.13,-0.49,0.11},
		vert{0.38,-0.76,-0.81},
		vert{0.22,0.20,-0.75},
		'.'});
	tris.push_back(
		{vert{0.78,-0.68,0.53},
		vert{-0.55,0.75,0.34},
		vert{-0.32,-0.21,0.27},
		'.'});
	tris.push_back(
		{vert{0.58,-0.71,0.53},
		vert{0.26,-0.21,0.41},
		vert{0.61,-0.55,0.77},
		'.'});
	tris.push_back(
		{vert{0.80,0.65,0.36},
		vert{0.70,0.90,-0.71},
		vert{-0.93,0.14,0.08},
		'.'});
	tris.push_back(
		{vert{-0.45,0.48,-0.80},
		vert{-0.29,0.25,-0.54},
		vert{-0.54,0.81,0.36},
		'.'});
	tris.push_back(
		{vert{-0.54,0.62,-0.24},
		vert{-0.85,0.41,0.14},
		vert{0.27,-0.90,-0.80},
		'.'});
	tris.push_back(
		{vert{0.85,-0.45,-0.17},
		vert{-0.95,-0.14,-0.81},
		vert{-0.71,-0.55,0.09},
		'.'});
	tris.push_back(
		{vert{-0.50,0.41,0.25},
		vert{0.29,-0.78,-0.09},
		vert{0.28,-0.84,-0.08},
		'.'});
	tris.push_back(
		{vert{0.98,0.52,0.68},
		vert{0.95,-0.06,-0.93},
		vert{0.79,-0.39,-0.86},
		'.'});
	tris.push_back(
		{vert{0.04,0.67,-0.37},
		vert{-0.29,0.05,-0.59},
		vert{-0.65,0.65,0.53},
		'.'});
	tris.push_back(
		{vert{-0.63,0.39,0.95},
		vert{0.31,0.21,-0.27},
		vert{0.37,-0.14,-0.53},
		'.'});
	tris.push_back(
		{vert{-0.52,-0.74,-0.09},
		vert{-0.02,0.82,0.12},
		vert{0.56,0.58,-0.92},
		'.'});
	tris.push_back(
		{vert{-0.28,0.43,0.88},
		vert{-0.03,-0.06,0.69},
		vert{0.87,-0.12,0.53},
		'.'});
	tris.push_back(
		{vert{0.22,0.33,-0.98},
		vert{-0.59,-0.64,0.02},
		vert{0.07,-0.20,0.58},
		'.'});
	tris.push_back(
		{vert{0.43,-0.09,-0.59},
		vert{-0.03,0.30,0.68},
		vert{-0.25,-0.67,0.22},
		'.'});
	tris.push_back(
		{vert{-0.28,0.16,0.14},
		vert{0.85,-0.32,-0.90},
		vert{0.51,-0.75,0.67},
		'.'});
	tris.push_back(
		{vert{0.40,0.88,0.21},
		vert{-0.10,-0.43,-0.99},
		vert{0.26,0.02,-0.34},
		'.'});
	tris.push_back(
		{vert{0.68,-0.77,-0.07},
		vert{0.18,0.85,-0.84},
		vert{-0.97,-0.49,0.36},
		'.'});
	tris.push_back(
		{vert{-0.99,-0.32,-0.69},
		vert{-0.94,0.44,-0.25},
		vert{-0.89,0.40,-0.30},
		'.'});
	tris.push_back(
		{vert{0.50,-0.99,-0.62},
		vert{-0.81,-0.84,0.87},
		vert{0.32,-0.72,0.12},
		'.'});
	tris.push_back(
		{vert{-0.45,0.14,0.47},
		vert{-0.36,-0.78,0.96},
		vert{0.84,0.40,-0.91},
		'.'});
	tris.push_back(
		{vert{-0.12,-0.95,0.49},
		vert{-0.08,-0.50,0.97},
		vert{-0.40,0.58,0.85},
		'.'});
	tris.push_back(
		{vert{0.37,0.12,-0.12},
		vert{-0.23,0.45,0.89},
		vert{-0.72,0.75,-0.11},
		'.'});
	tris.push_back(
		{vert{-0.93,-0.40,-0.85},
		vert{-0.54,0.05,-0.31},
		vert{0.91,0.66,-0.74},
		'.'});
	tris.push_back(
		{vert{0.78,0.74,-1.00},
		vert{0.91,0.57,-0.71},
		vert{-0.35,0.14,-0.58},
		'.'});
	tris.push_back(
		{vert{-0.30,-0.24,0.29},
		vert{-0.91,-0.36,0.44},
		vert{-0.57,0.60,0.63},
		'.'});
	tris.push_back(
		{vert{-0.71,-0.27,-0.18},
		vert{0.26,-0.62,-0.98},
		vert{0.12,0.98,0.49},
		'.'});
	tris.push_back(
		{vert{-0.55,-0.74,-0.74},
		vert{0.44,-0.14,0.50},
		vert{0.71,0.63,0.96},
		'.'});
	tris.push_back(
		{vert{0.22,0.47,0.23},
		vert{-0.11,0.40,-0.82},
		vert{-0.10,-0.37,0.51},
		'.'});/**/
	
	/*// depth buffer test
	// front to back
	tris.push_back(
		{vert{-.7,-.25,.1},
		 vert{-.3,-.25,.1},
		 vert{-.5,.25,.1},
		 'F'});
	tris.push_back(
		{vert{-.7,-.25,-.1},
		 vert{-.3,-.25,-.1},
		 vert{-.5,.25,-.1},
		 '.'});
	// cross over
	tris.push_back(
		{vert{.7,-.25,.25},
		 vert{.3,-.25,-.25},
		 vert{.5,.25,0},
		 'R'});
	tris.push_back(
		{vert{.7,-.25,-.25},
		 vert{.3,-.25,.25},
		 vert{.5,.25,0},
		 'l'});
	// right flank
	tris.push_back(
		{vert{.9,-.25,-.25},
		 vert{.9,-.25,.25},
		 vert{.9,.25,0},
		 '~'});/**/
	
	/*// z-fight
	eng.xyRot = true;
	eng.persp = false;
	tris.push_back(
		{vert{-.5,-.25,0},
		 vert{-.5,.25,0},
		 vert{.5,-.25,0},
		 '.'});
	tris.push_back(
		{vert{.5,-.25,0},
		 vert{.5,.25,0},
		 vert{-.5,.25,0},
		 '.'});
	tris.push_back(
		{vert{-.25,-.5,0},
		 vert{.25,-.5,0},
		 vert{-.25,.5,0},
		 '%'});
	tris.push_back(
		{vert{-.25,.5,0},
		 vert{.25,.5,0},
		 vert{.25,-.5,0},
		 '%'});/**/
	
	// plane perspective test
	eng.d = 1.5;
	eng.xyRot = true;
	eng.vertLabels = false;
	eng.xDelta = .005;
	tris.push_back(
		{vert{.5,-.5,0},
		 vert{.5,-.5,.25},
		 vert{.75,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,0},
		 vert{.75,-.5,0},
		 vert{.75,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-0},
		 vert{.5,-.5,-.25},
		 vert{.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-0},
		 vert{.25,-.5,-0},
		 vert{.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,0},
		 vert{.5,-.5,.25},
		 vert{.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,0},
		 vert{.25,-.5,0},
		 vert{.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-0},
		 vert{.5,-.5,-.25},
		 vert{.75,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-0},
		 vert{.75,-.5,-0},
		 vert{.75,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,0},
		 vert{-.5,-.5,.25},
		 vert{-.75,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,0},
		 vert{-.75,-.5,0},
		 vert{-.75,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-0},
		 vert{-.5,-.5,-.25},
		 vert{-.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-0},
		 vert{-.25,-.5,-0},
		 vert{-.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,0},
		 vert{-.5,-.5,.25},
		 vert{-.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,0},
		 vert{-.25,-.5,0},
		 vert{-.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,-0},
		 vert{-.5,-.5,-.25},
		 vert{-.75,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,-0},
		 vert{-.75,-.5,-0},
		 vert{-.75,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{0,-.5,.75},
		 vert{.25,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{.25,-.5,.5},
		 vert{.25,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{0,-.5,.25},
		 vert{-.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{-.25,-.5,.5},
		 vert{-.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{0,-.5,.75},
		 vert{-.25,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{-.25,-.5,.5},
		 vert{-.25,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{0,-.5,.25},
		 vert{.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{0,-.5,.5},
		 vert{.25,-.5,.5},
		 vert{.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.5,-.5,.75},
		 vert{.75,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.75,-.5,.5},
		 vert{.75,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.5,-.5,.25},
		 vert{.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.25,-.5,.5},
		 vert{.25,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.5,-.5,.75},
		 vert{.25,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.25,-.5,.5},
		 vert{.25,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.5,-.5,.25},
		 vert{.75,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,.5},
		 vert{.75,-.5,.5},
		 vert{.75,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.5,-.5,.75},
		 vert{-.75,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.75,-.5,.5},
		 vert{-.75,-.5,.75},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.5,-.5,.25},
		 vert{-.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.25,-.5,.5},
		 vert{-.25,-.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.5,-.5,.75},
		 vert{-.25,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.25,-.5,.5},
		 vert{-.25,-.5,.75},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.5,-.5,.25},
		 vert{-.75,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,.5},
		 vert{-.75,-.5,.5},
		 vert{-.75,-.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{0,-.5,-.25},
		 vert{.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{.25,-.5,-.5},
		 vert{.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{0,-.5,-.75},
		 vert{-.25,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{-.25,-.5,-.5},
		 vert{-.25,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{0,-.5,-.25},
		 vert{-.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{-.25,-.5,-.5},
		 vert{-.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{0,-.5,-.75},
		 vert{.25,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{0,-.5,-.5},
		 vert{.25,-.5,-.5},
		 vert{.25,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.5,-.5,-.25},
		 vert{.75,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.75,-.5,-.5},
		 vert{.75,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.5,-.5,-.75},
		 vert{.25,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.25,-.5,-.5},
		 vert{.25,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.5,-.5,-.25},
		 vert{.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.25,-.5,-.5},
		 vert{.25,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.5,-.5,-.75},
		 vert{.75,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{.5,-.5,-.5},
		 vert{.75,-.5,-.5},
		 vert{.75,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.5,-.5,-.25},
		 vert{-.75,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.75,-.5,-.5},
		 vert{-.75,-.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.5,-.5,-.75},
		 vert{-.25,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.25,-.5,-.5},
		 vert{-.25,-.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.5,-.5,-.25},
		 vert{-.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.25,-.5,-.5},
		 vert{-.25,-.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.5,-.5,-.75},
		 vert{-.75,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{-.5,-.5,-.5},
		 vert{-.75,-.5,-.5},
		 vert{-.75,-.5,-.75},
		 '.'});
	tris.push_back(
		{vert{.5,.5,0},
		 vert{.5,.5,.25},
		 vert{.75,.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,0},
		 vert{.75,.5,0},
		 vert{.75,.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-0},
		 vert{.5,.5,-.25},
		 vert{.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-0},
		 vert{.25,.5,-0},
		 vert{.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,0},
		 vert{.5,.5,.25},
		 vert{.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,0},
		 vert{.25,.5,0},
		 vert{.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-0},
		 vert{.5,.5,-.25},
		 vert{.75,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-0},
		 vert{.75,.5,-0},
		 vert{.75,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,0},
		 vert{-.5,.5,.25},
		 vert{-.75,.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,0},
		 vert{-.75,.5,0},
		 vert{-.75,.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-0},
		 vert{-.5,.5,-.25},
		 vert{-.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-0},
		 vert{-.25,.5,-0},
		 vert{-.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,0},
		 vert{-.5,.5,.25},
		 vert{-.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,0},
		 vert{-.25,.5,0},
		 vert{-.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,-0},
		 vert{-.5,.5,-.25},
		 vert{-.75,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,-0},
		 vert{-.75,.5,-0},
		 vert{-.75,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{0,.5,.75},
		 vert{.25,.5,.75},
		 '.'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{.25,.5,.5},
		 vert{.25,.5,.75},
		 '.'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{0,.5,.25},
		 vert{-.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{-.25,.5,.5},
		 vert{-.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{0,.5,.75},
		 vert{-.25,.5,.75},
		 'N'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{-.25,.5,.5},
		 vert{-.25,.5,.75},
		 'N'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{0,.5,.25},
		 vert{.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{0,.5,.5},
		 vert{.25,.5,.5},
		 vert{.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.5,.5,.75},
		 vert{.75,.5,.75},
		 '.'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.75,.5,.5},
		 vert{.75,.5,.75},
		 '.'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.5,.5,.25},
		 vert{.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.25,.5,.5},
		 vert{.25,.5,.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.5,.5,.75},
		 vert{.25,.5,.75},
		 'N'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.25,.5,.5},
		 vert{.25,.5,.75},
		 'N'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.5,.5,.25},
		 vert{.75,.5,.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,.5},
		 vert{.75,.5,.5},
		 vert{.75,.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.5,.5,.75},
		 vert{-.75,.5,.75},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.75,.5,.5},
		 vert{-.75,.5,.75},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.5,.5,.25},
		 vert{-.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.25,.5,.5},
		 vert{-.25,.5,.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.5,.5,.75},
		 vert{-.25,.5,.75},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.25,.5,.5},
		 vert{-.25,.5,.75},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.5,.5,.25},
		 vert{-.75,.5,.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,.5},
		 vert{-.75,.5,.5},
		 vert{-.75,.5,.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{0,.5,-.25},
		 vert{.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{.25,.5,-.5},
		 vert{.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{0,.5,-.75},
		 vert{-.25,.5,-.75},
		 '.'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{-.25,.5,-.5},
		 vert{-.25,.5,-.75},
		 '.'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{0,.5,-.25},
		 vert{-.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{-.25,.5,-.5},
		 vert{-.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{0,.5,-.75},
		 vert{.25,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{0,.5,-.5},
		 vert{.25,.5,-.5},
		 vert{.25,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.5,.5,-.25},
		 vert{.75,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.75,.5,-.5},
		 vert{.75,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.5,.5,-.75},
		 vert{.25,.5,-.75},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.25,.5,-.5},
		 vert{.25,.5,-.75},
		 '.'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.5,.5,-.25},
		 vert{.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.25,.5,-.5},
		 vert{.25,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.5,.5,-.75},
		 vert{.75,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{.5,.5,-.5},
		 vert{.75,.5,-.5},
		 vert{.75,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.5,.5,-.25},
		 vert{-.75,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.75,.5,-.5},
		 vert{-.75,.5,-.25},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.5,.5,-.75},
		 vert{-.25,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.25,.5,-.5},
		 vert{-.25,.5,-.75},
		 'N'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.5,.5,-.25},
		 vert{-.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.25,.5,-.5},
		 vert{-.25,.5,-.25},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.5,.5,-.75},
		 vert{-.75,.5,-.75},
		 '.'});
	tris.push_back(
		{vert{-.5,.5,-.5},
		 vert{-.75,.5,-.5},
		 vert{-.75,.5,-.75},
		 '.'});/**/
	
	// loop for each frame
	while(1<2)
	{
		// init frame and depth buffer
		char frame [eng.resW][eng.resH];
		double depth [eng.resW][eng.resH];
		for(int h = eng.resH-1; h >= 0; h--)
		{
			for(int w = 0; w <= eng.resW-1; w++)
			{
				frame[w][h] = ' ';
				depth[w][h] = -INF;
			}
		}
		
		// rotate each tri
		for(int i = 0; i < tris.size(); i++)
		{
			// pull out the tri we will rotate
			tri t = tris.at(i);
			
			double newX;
			double newY;
			double newZ;
			
			// rotate tri xy plane
			if(eng.xyRot)
			{
				newX = t.a.x*cos(eng.radPerFrame) - t.a.y*sin(eng.radPerFrame);
				newY = t.a.x*sin(eng.radPerFrame) + t.a.y*cos(eng.radPerFrame);
				t.a.x = newX;
				t.a.y = newY;
				
				newX = t.b.x*cos(eng.radPerFrame) - t.b.y*sin(eng.radPerFrame);
				newY = t.b.x*sin(eng.radPerFrame) + t.b.y*cos(eng.radPerFrame);
				t.b.x = newX;
				t.b.y = newY;
				
				newX = t.c.x*cos(eng.radPerFrame) - t.c.y*sin(eng.radPerFrame);
				newY = t.c.x*sin(eng.radPerFrame) + t.c.y*cos(eng.radPerFrame);
				t.c.x = newX;
				t.c.y = newY;
			}
			
			// rotate tri xz plane
			if(eng.xzRot)
			{
				newX = t.a.x*cos(eng.radPerFrame) - t.a.z*sin(eng.radPerFrame);
				newZ = t.a.x*sin(eng.radPerFrame) + t.a.z*cos(eng.radPerFrame);
				t.a.x = newX;
				t.a.z = newZ;
				
				newX = t.b.x*cos(eng.radPerFrame) - t.b.z*sin(eng.radPerFrame);
				newZ = t.b.x*sin(eng.radPerFrame) + t.b.z*cos(eng.radPerFrame);
				t.b.x = newX;
				t.b.z = newZ;
				
				newX = t.c.x*cos(eng.radPerFrame) - t.c.z*sin(eng.radPerFrame);
				newZ = t.c.x*sin(eng.radPerFrame) + t.c.z*cos(eng.radPerFrame);
				t.c.x = newX;
				t.c.z = newZ;
			}
			
			// rise in x
			t.a.x += eng.xDelta;
			t.b.x += eng.xDelta;
			t.c.x += eng.xDelta;
			
			// store the tri back in the array
			tris.at(i) = t;
		}
			
		// render each tri
		for(int i = 0; i < tris.size(); i++)
		{
			// pull out the tri we will render
			tri t = tris.at(i);
			
			// perspective adjustment
			if(eng.persp)
			{
				double dist = eng.d - t.a.z;
				t.a.x *= eng.d/dist;
				t.a.y *= eng.d/dist;
				dist = eng.d - t.b.z;
				t.b.x *= eng.d/dist;
				t.b.y *= eng.d/dist;
				dist = eng.d - t.c.z;
				t.c.x *= eng.d/dist;
				t.c.y *= eng.d/dist;
			}
			
			// find normal vector to plane
			double v1x = t.b.x - t.a.x;
			double v1y = t.b.y - t.a.y;
			double v1z = t.b.z - t.a.z;
			double v2x = t.c.x - t.a.x;
			double v2y = t.c.y - t.a.y;
			double v2z = t.c.z - t.a.z;
			double nx = v1y*v2z - v1z*v2y;
			double ny = v1z*v2x - v1x*v2z;
			double nz = v1x*v2y - v1y*v2x;			
				
			// init triRender and triDepth
			char triRender [eng.resW][eng.resH];
			double triDepth [eng.resW][eng.resH];
			for(int h = eng.resH-1; h >= 0; h--)
			{
				for(int w = 0; w <= eng.resW-1; w++)
				{
					triRender[w][h] = t.fill;
					if(eng.specialFill)
						triRender[w][h] = (tris.at(i).a.y+1)/2*40+30;
					
					double loopX = (double)w/(eng.resW-1)*eng.windowW-(eng.windowW/2.0);
					double loopY = (double)h/(eng.resH-1)*eng.windowH-(eng.windowH/2.0);
					triDepth[w][h] = (nx*(loopX - t.a.x) + ny*(loopY - t.a.y))/(-nz) + t.a.z;
				}
			}
			
			// shave ab side
			double m = (t.b.y-t.a.y)/(t.b.x-t.a.x);
			for(int h = 0; h <= eng.resH-1; h++)
			{
				for(int w = 0; w <= eng.resW-1; w++)
				{
					double loopX = (double)w/(eng.resW-1)*eng.windowW-(eng.windowW/2.0);
					double loopY = (double)h/(eng.resH-1)*eng.windowH-(eng.windowH/2.0);
					double lineY = m*(loopX-t.a.x)+t.a.y;
					
					double lineYatOppPoint = m*(t.c.x-t.a.x)+t.a.y;
					
					bool oppPointAbove;
					if (lineYatOppPoint > t.c.y)
					{
						oppPointAbove = false;
					}
					else
					{
						oppPointAbove = true;
					}
					
					if((loopY > lineY && !oppPointAbove) || (loopY < lineY && oppPointAbove))
					triRender[w][h] = ' ';
				}
			}
			
			// shave bc side
			m = (t.b.y-t.c.y)/(t.b.x-t.c.x);
			for(int h = 0; h <= eng.resH-1; h++)
			{
				for(int w = 0; w <= eng.resW-1; w++)
				{
					double loopX = (double)w/(eng.resW-1)*eng.windowW-(eng.windowW/2.0);
					double loopY = (double)h/(eng.resH-1)*eng.windowH-(eng.windowH/2.0);
					double lineY = m*(loopX-t.b.x)+t.b.y;
					
					double lineYatOppPoint = m*(t.a.x-t.b.x)+t.b.y;
					
					bool oppPointAbove;
					if (lineYatOppPoint > t.a.y)
					{
						oppPointAbove = false;
					}
					else
					{
						oppPointAbove = true;
					}
					
					if((loopY > lineY && !oppPointAbove) || (loopY < lineY && oppPointAbove))
					triRender[w][h] = ' ';
				}
			}
			
			// shave ac side
			m = (t.a.y-t.c.y)/(t.a.x-t.c.x);
			for(int h = 0; h <= eng.resH-1; h++)
			{
				for(int w = 0; w <= eng.resW-1; w++)
				{
					double loopX = (double)w/(eng.resW-1)*eng.windowW-(eng.windowW/2.0);
					double loopY = (double)h/(eng.resH-1)*eng.windowH-(eng.windowH/2.0);
					double lineY = m*(loopX-t.c.x)+t.c.y;
					
					double lineYatOppPoint = m*(t.b.x-t.c.x)+t.c.y;
					
					bool oppPointAbove;
					if (lineYatOppPoint > t.b.y)
					{
						oppPointAbove = false;
					}
					else
					{
						oppPointAbove = true;
					}
					
					if((loopY > lineY && !oppPointAbove) || (loopY < lineY && oppPointAbove))
					triRender[w][h] = ' ';
				}
			}
			
			// label vertices
			if(eng.vertLabels)
			{
				int aW = ((t.a.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
				int aH = ((t.a.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
				int bW = ((t.b.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
				int bH = ((t.b.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
				int cW = ((t.c.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
				int cH = ((t.c.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
				triRender[aW][aH] = 'a';
				triRender[bW][bH] = 'b';
				triRender[cW][cH] = 'c';
			}
			
			// print to frame
			for(int h = eng.resH-1; h >= 0; h--)
			{
				for(int w = 0; w <= eng.resW-1; w++)
				{
					if (triRender[w][h] != ' ' // if the tri exists here and
						&& triDepth[w][h] > depth[w][h]) // it is in front of whatever is already there
					{
						frame[w][h] = triRender[w][h];
						depth[w][h] = triDepth[w][h];
					}
				}
			}	
		}
		
		// print frame
		system("cls");
		stringstream ss;
		for(int h = eng.resH-1; h >= 0; h--)
		{
			for(int w = 0; w <= eng.resW-1; w++)
			{
				ss << frame[w][h];
			}
		}
		cout << ss.str();
		Sleep(1000/eng.framerate);
	}
	
	return 0;
}
