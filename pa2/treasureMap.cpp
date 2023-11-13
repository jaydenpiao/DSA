#include "treasureMap.h"
#include "queue.h"
#include <queue>
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

    this->base = baseim;
    this->maze = mazeim;
    this->start = s;

}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

    int x = loc.first;
    int y = loc.second;
    RGBAPixel* pixel = im.getPixel(x,y);

    pixel->r = 2*(pixel->r / 4);
    pixel->g = 2*(pixel->g / 4);
    pixel->b = 2*(pixel->b / 4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

    RGBAPixel* pixel = im.getPixel(loc.first,loc.second);
    int value = d%64;

    bool r1 = false;
    bool r2 = false;
    bool g1 = false;
    bool g2 = false;
    bool b1 = false;
    bool b2 = false;
   
    if (value >= 32) {
        value -= 32;
        r1 = true;
    }
    if (value >= 16) {
        value -= 16;
        r2 = true;
    }   
    if (value >= 8) {
        value -= 8;
        g1 = true;
    }
    if (value >= 4) {
        value -= 4;
        g2 = true;
    }
    if (value >= 2) {
        value -= 2;
        b1 = true;
    }
    if (value >= 1) {
        value -= 1;
        b2 = true;
    }

    pixel->r = newRGBVal(static_cast<int>(pixel->r), r1, r2);
    pixel->g = newRGBVal(static_cast<int>(pixel->g), g1, g2);
    pixel->b = newRGBVal(static_cast<int>(pixel->b), b1, b2);
}

char treasureMap::newRGBVal(int ogval, bool o, bool t) {

    int value = 0;

    if (ogval >= 128) {
        value += 128;
        ogval -= 128;
    }
    if (ogval >= 64) {
        value += 64;
        ogval -= 64;
    }
    if (ogval >= 32) {
        value += 32;
        ogval -= 32;
    }
    if (ogval >= 16) {
        value += 16;
        ogval -= 16;
    }   
    if (ogval >= 8) {
        value += 8;
        ogval -= 8;
    }
    if (ogval >= 4) {
        value += 4;
        ogval -= 4;
    }

    if(o && t) {
        return static_cast<char>(value + 3);
    } else if (o) {
        return static_cast<char>(value + 2);
    } else if (t) {
        return static_cast<char>(value + 1);
    } else {
        return static_cast<char>(value);
    }
}

PNG treasureMap::renderMap(){
//0.
PNG copy = base;
int x = copy.width();
int y = copy.height();

//1a. initialize the visited vector
vector<vector<bool>> visitedvec;
for ( int i = 0; i< x; i++) {
    vector<bool> currvec;
    for ( int k = 0; k<y; k++) {
        currvec.push_back(false);
    }
    visitedvec.push_back(currvec);
}

//1b. initialize the distance vector
vector<vector<int>> distancevec;
for (int i = 0; i< x; i++) {
    vector<int> currvec;
    for (int k = 0; k<y; k++) {
        currvec.push_back(-1);
    }
    distancevec.push_back(currvec);
}

//2. initialize queue
  std::queue<pair<int,int>> queue;

//3. initialize start location
int startx = start.first;
int starty = start.second;
//mark start as visited
visitedvec[startx][starty] = true;
//mark start to be 0
distancevec[startx][starty] = 0;
//encode start with distance 0
setLOB(copy, start, 0);
//enqueue start
queue.push(start);

//4.
while(!queue.empty()) {
//4a.
    pair<int, int> curr = queue.front();
    queue.pop();
    int c1 = curr.first;
    int c2 = curr.second;
    int currdis = distancevec[c1][c2];
//4b.
    // enqueue neighbours
    vector<pair<int, int>> neighborvec = this->neighbors(curr);
    
    for (auto neighbor : neighborvec) {
        pair<int,int> p = neighbor;
        int px = p.first;
        int py = p.second;

        //check if "p" is good
        if (good(visitedvec, curr, p)) {
            visitedvec[px][py] = true;
            distancevec[px][py] = currdis +1;
            setLOB(copy, p, currdis +1);
            queue.push(p);
        }
    }
}
return copy;
}


PNG treasureMap::renderMaze(){
    PNG rendered = base;
    int width = maze.width();
    int height = maze.height();
    int startx = start.first;
    int starty = start.second;
    RGBAPixel * mazestart = maze.getPixel(startx, starty);

    for (int x=0; x<width; x++) {
        for (int y=0; y<height; y++) {
            if (*maze.getPixel(x, y) == *mazestart) {
                setGrey(rendered, {x,y});
            }
        }
    }

    // draw red square
    for (int x = -3; x<=3; x++) {
        for (int y=-3; y<=3; y++) {
            int posx = startx + x;
            int posy = starty + y;
            if (posx >= 0 && posx < width && posy >= 0 && posy < height) {
                RGBAPixel* pixel = rendered.getPixel(posx, posy);
                pixel->r = 255;
                pixel->g = 0;
                pixel->b = 0;
            }
        }
    }
    return rendered;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){
int x = base.width();
int y = base.height();


return next.first >= 0 && next.first < x && 
        next.second >= 0 && next.second < y &&
        v[next.first][next.second] == false && 
        *maze.getPixel(curr.first, curr.second) == 
        *maze.getPixel(next.first, next.second); 
        
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {
    vector<pair<int, int>> neighbors;
    neighbors.push_back({curr.first-1, curr.second}); 
    neighbors.push_back({curr.first, curr.second+1}); 
    neighbors.push_back({curr.first+1, curr.second}); 
    neighbors.push_back({curr.first, curr.second-1}); 

    return neighbors;
}

