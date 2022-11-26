// heads up, this code is NOT pretty lol

/*
TODO:
	port to playdate!?!?!?
	implement ClearConsole() fn
		it's faster than system("cls")
		needs a sleep before it or screen will flicker?? like so:
			Sleep(3);
			ClearConsole();
	fix up framecap/framerate
	re-add vertex abc labels to new draw fn
	add a proper 3x3 rotation matrix
	maybe like, don't have all the models/scenes just hardcoded into main() lol
	add some kb controls n stuff?
	when the font size is small, some of the fonts are colored due to cleartext!!!
		could be fun to map the colors out and add sudo color support
*/

#include <iostream>
#include <cmath>
#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

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
	
	// display how long the frame took
	bool frameTimer;
	
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


// ifdef windows to clear console 
static void ClearConsole(void)
{
#ifdef _WIN32
    const HANDLE stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screenBufInfo;
    if (!GetConsoleScreenBufferInfo(stdoutHandle, &screenBufInfo)) return;
    const SMALL_RECT scrollRect =
    {
        0, 0,
        screenBufInfo.dwSize.X,
        screenBufInfo.dwSize.Y
    };
    const CHAR_INFO fill = {L' ', screenBufInfo.wAttributes};
    const COORD destOrigin = {0, -screenBufInfo.dwSize.Y};
    ScrollConsoleScreenBufferW(stdoutHandle, &scrollRect, NULL, destOrigin, &fill);
    const COORD cursorPos = {0, 0};
    SetConsoleCursorPosition(stdoutHandle, cursorPos);
#else
    printf("\033[2J\033[1;1H");
    /* printf("%b","\033c"); */
#endif
}


int main(int argc, char *argv[])
{
	engineSettings eng;
	clock_t frameStart, frameEnd;
	
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
	eng.frameTimer = true;
	eng.framerate = 67;
	
	// init tri list
	vector<tri> tris;
	
	// read obj
	eng.vertLabels = false;
	vector<vert> verts;
	double maxY = -INF;
	double minY = INF;
	
    // argv[1] is the path to the obj file
	/* ifstream objFile("objs/Queen Gohma.obj"); */
    ifstream objFile(argv[1]);


	string objLine;
	while (getline (objFile, objLine)) // loop through each line
	{
		string tag = objLine.substr(0,objLine.find(" "));
		objLine.erase(0,objLine.find(" ")+1);
		if (tag == "v") // vertex
		{
			vert newVert;
			string newVal;
			
			// read xyz vals
			newVal = objLine.substr(0,objLine.find(" "));
			newVert.x = atof(newVal.c_str());
			objLine.erase(0,objLine.find(" ")+1);
			
			newVal = objLine.substr(0,objLine.find(" "));
			newVert.y = atof(newVal.c_str());
			objLine.erase(0,objLine.find(" ")+1);
			
			newVal = objLine.substr(0,objLine.find(" "));
			newVert.z = atof(newVal.c_str());
						
			// store in the vert list
			verts.push_back(newVert);
			
			// update max/min y for normalization
			if (newVert.y > maxY)
				maxY = newVert.y;
			if (newVert.y < minY)
				minY = newVert.y;
		}
		else if (tag == "f") // face
		{
			tri newTri;
			int vertIndex;

			// read vert indices
			// luckily atoi() seems to ditch all the junk after the first slash
			vertIndex = atoi(objLine.substr(0,objLine.find(" ")).c_str());
			newTri.a = verts.at(vertIndex-1);
			objLine.erase(0,objLine.find(" ")+1);
			
			vertIndex = atoi(objLine.substr(0,objLine.find(" ")).c_str());
			newTri.b = verts.at(vertIndex-1);
			objLine.erase(0,objLine.find(" ")+1);
			
			vertIndex = atoi(objLine.substr(0,objLine.find(" ")).c_str());
			newTri.c = verts.at(vertIndex-1);
			
			// set fill (?)
			newTri.fill = '?';
			
			// store in the tri list
			tris.push_back(newTri);
		}
	}
	objFile.close();
	
	// normalize y pos + scale
	for(int i = 0; i < tris.size(); i++)
	{
		double offset = (maxY+minY)/2;
		double scale = .8/(maxY-minY);
		
		tris.at(i).a.y -= offset;
		tris.at(i).b.y -= offset;
		tris.at(i).c.y -= offset;
		
		tris.at(i).a.x *= scale;
		tris.at(i).b.x *= scale;
		tris.at(i).c.x *= scale;
		tris.at(i).a.y *= scale;
		tris.at(i).b.y *= scale;
		tris.at(i).c.y *= scale;
		tris.at(i).a.z *= scale;
		tris.at(i).b.z *= scale;
		tris.at(i).c.z *= scale;
		
		// use coords as seed for fill char
		tris.at(i).fill = (tris.at(i).a.y+tris.at(i).a.z+.5)*50+40;
	}
	
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
			
			// NEW DRAW FN
			
			// slope of line
			double mAB = (t.b.y-t.a.y)/(t.b.x-t.a.x);
			double mBC = (t.b.y-t.c.y)/(t.b.x-t.c.x);
			double mAC = (t.a.y-t.c.y)/(t.a.x-t.c.x);
			
			// Y of line at opp vert
			double lineYatC = mAB*(t.c.x-t.a.x)+t.a.y; 
			double lineYatA = mBC*(t.a.x-t.b.x)+t.b.y;
			double lineYatB = mAC*(t.b.x-t.c.x)+t.c.y;
			
			// find box around tri
			// so we don't have to loop through every char in the frame
			// jfc this is a crazy speed boost lol
			int aW = ((t.a.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
			int aH = ((t.a.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
			int bW = ((t.b.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
			int bH = ((t.b.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
			int cW = ((t.c.x+(eng.windowW/2.0))/eng.windowW)*(eng.resW-1);
			int cH = ((t.c.y+(eng.windowH/2.0))/eng.windowH)*(eng.resH-1);
			int lowerH = min(min(aH,bH),min(bH,cH));
			int upperH = max(max(aH,bH),max(bH,cH));
			int lowerW = min(min(aW,bW),min(bW,cW));
			int upperW = max(max(aW,bW),max(bW,cW));
			
			// draw tri into the frame, char by char
			for(int h = max(lowerH,0); h <= min(upperH,eng.resH-1); h++)
			{
				for(int w = max(lowerW,0); w <= min(upperW,eng.resW-1); w++)
				{
					// coords of the current char
					double curX = (double)w/(eng.resW-1)*eng.windowW-(eng.windowW/2.0);
					double curY = (double)h/(eng.resH-1)*eng.windowH-(eng.windowH/2.0);
					double curDepth = (nx*(curX - t.a.x) + ny*(curY - t.a.y))/(-nz) + t.a.z;
					
					// for each line (AB BC AC) of the tri,
					// the current char and the opposite vert (C A B) must be on the same side
					
					// check AB line
					double lineYcur = mAB*(curX-t.a.x)+t.a.y; // Y of line at current char
					if ((curY - lineYcur) * (t.c.y - lineYatC) > 0) // check to see if sign is same
					{
						// check BC line
						lineYcur = mBC*(curX-t.c.x)+t.c.y; // Y of line at current char
						if ((curY - lineYcur) * (t.a.y - lineYatA) > 0) // check to see if sign is same
						{
							// check AC line
							lineYcur = mAC*(curX-t.c.x)+t.c.y; // Y of line at current char
							if ((curY - lineYcur) * (t.b.y - lineYatB) > 0) // check to see if sign is same
							{
								// don't draw if the char is behind something
								// (this check is really slow for some reason, so I'm putting it last)
								if(curDepth > depth[w][h])
								{
									frame[w][h] = t.fill;
									depth[w][h] = curDepth;
								}
							}
						}
					}
				}
			}/**/
			
			/*// OLD DRAW FN
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
            */
		}
		
		// framecap (should change it to only apply when needed...)
#ifdef _WIN32
		Sleep(1000/eng.framerate);
#else
        usleep(1000000/eng.framerate);
#endif
        ClearConsole();
		
		// print frame

		stringstream ss;
		for(int h = eng.resH-1; h >= 0; h--)
		{
			for(int w = 0; w <= eng.resW-1; w++)
			{
				ss << frame[w][h];
			}
		}
		cout << ss.str();
				
		// frame timer
		if (eng.frameTimer)
		{
			frameEnd = clock();
			cout << "\b\b\b\b\b\b" << frameEnd - frameStart << "ms";
			frameStart = clock();
		}
	}
	
	return 0;
}
