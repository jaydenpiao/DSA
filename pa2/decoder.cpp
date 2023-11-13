#include "decoder.h"
#include "queue.h"
#include "stack.h"
#include <queue>
#include "algorithm"
#include <stack>
#include <map>
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

   int width = mapImg.width();
   int height = mapImg.height();
   int startx = start.first;
   int starty = start.second;
   int largest = 0;
   pair<int,int> largestpair = start;

   //init visited vector
   vector<vector<bool>> visitedvec;
   for (int x = 0; x < width; x++) {
      vector<bool> temp;
      for (int y = 0; y < height; y++) {
         temp.push_back(false);
      }
      visitedvec.push_back(temp);
   }

   //init distance vector
   vector<vector<int>> distancevec;
   for (int x = 0; x < width; x++) {
      vector<int> temp;
      for (int y = 0; y < height; y++) {
         temp.push_back(-1);
      }
      distancevec.push_back(temp);
   }

   //init paths map
   map <pair<int,int>, pair<int,int>> pathmap;


   //2. initialize queue
   std::queue<pair<int,int>> queue;

   //3. initialize start location
   //mark start as visited
   visitedvec[startx][starty] = true;
   //mark start distance to be 0
   distancevec[startx][starty] = 0;
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
      mazePts.push_back(curr);
         if (currdis > largest) {
         largest = currdis;
         largestpair = curr;
      }
   //4b.
      // enqueue neighbours
      vector<pair<int, int>> neighborvec = this->neighbors(curr);
      
      for (auto neighbor : neighborvec) {
         pair<int,int> p = neighbor;
         int px = p.first;
         int py = p.second;

         //check if "p" is good
         if (good(visitedvec, distancevec, curr, p)) {
               visitedvec[px][py] = true;
               distancevec[px][py] = currdis +1;
               queue.push(p);

            //set up the path map
            pathmap[p] = curr;
         }
      }
   }

   this->pathPts.push_back(largestpair);
   auto curr = pathmap[largestpair];

   while (curr != start)
   {
      this->pathPts.push_back(curr);
      curr = pathmap[curr];
   }

   this->pathPts.push_back(start);

   this->spot = largestpair;
   this->length = largest;
}


PNG decoder::renderSolution(){
/* YOUR CODE HERE */
PNG copy = mapImg;

for(auto point: this->pathPts) {
   RGBAPixel * p = copy.getPixel(point.first, point.second);
   p->r = 255;
   p->g = 0;
   p->b =0;
}
return copy;
}

int decoder::pathLength(){
return length +1;
}

pair<int,int> decoder::findSpot(){
return spot;
}

PNG decoder::renderMaze(){
   PNG rendered = mapImg;
   int width = mapImg.width();
   int height = mapImg.height();
   int startx = start.first;
   int starty = start.second;
for (auto pt: mazePts) {
   setGrey(rendered, pt);
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

void decoder::setGrey(PNG & im, pair<int,int> loc){

   int x = loc.first;
   int y = loc.second;
   RGBAPixel* pixel = im.getPixel(x,y);

   pixel->r = 2*(pixel->r / 4);
   pixel->g = 2*(pixel->g / 4);
   pixel->b = 2*(pixel->b / 4);

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){
int x = mapImg.width();
int y = mapImg.height();


if (next.first >= 0 && next.first < x && next.second >= 0 && next.second < y && !v[next.first][next.second]) {

   RGBAPixel* mappix = mapImg.getPixel(next.first, next.second);
   return compare(mappix, d[curr.first][curr.second]);

   }
   return false;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {
   vector<pair<int, int>> neighbors;
   neighbors.push_back({curr.first-1, curr.second}); 
   neighbors.push_back({curr.first, curr.second+1}); 
   neighbors.push_back({curr.first+1, curr.second}); 
   neighbors.push_back({curr.first, curr.second-1}); 
   return neighbors;
   }

bool decoder::compare(RGBAPixel*  p, int d){
   int r = p->r;
   int g = p->g;
   int b = p->b;
return (r%4)*16 + (g%4)*4 + (b%4) == (d+1)%64;
}
