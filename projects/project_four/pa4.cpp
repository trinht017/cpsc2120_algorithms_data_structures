#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <bitset>
#include <fstream>
#include <map>
#include <cmath>
#include <queue>
#include <unordered_map>
using namespace std;

struct Pixel {
    int x, y;
    unsigned char r, g, b;

    bool operator == (const Pixel &b) const {return x == b.x && y == b.y;}
    bool operator != (const Pixel &b) const {return x != b.x || y != b.y;}
    bool operator < (const Pixel &b) const {
        if (x == b.x) return y < b.y;
        return x < b.x;
    }
};

struct Node{
    Pixel pix;
    Pixel prev;
    uint8_t passengers;

    bool operator < (const Node &b) const {
        if (passengers == b.passengers) {
            if (prev == b.prev) {
                return pix < b.pix;
            }
            return prev < b.prev;
        }
        return passengers < b.passengers;
    }

    bool operator == (const Node &b) const {
        return passengers == b.passengers && prev == b.prev && pix == b.pix;
    }

    bool operator != (const Node &b) const {
       return passengers != b.passengers || prev != b.prev || pix != b.pix;
    }
};

vector<Pixel> img;
map<Node, double> dist;
map<Node, Node> pred;
int width, height;

string ppmFile;
Node start;
Node goal;
Node Dean;
Node Lin;
Node Perez;
Node Reeder;

int T;
double minDist;
double maxDist;
int maxDeg;
int D;

const uint8_t deanb = 1;
const uint8_t linb = 1 << 1;
const uint8_t johnb = 1 << 2;
const uint8_t all = deanb | linb | johnb;


double getDist(Pixel a, Pixel b) {
    if (a == b) return 0.0;
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

pair<double, double> toUnit(pair<int, int> V) {
    double length = sqrt(pow(V.first, 2) + pow(V.second, 2));
    return make_pair(V.first / length, V.second / length);
}

double dotProduct(pair<double, double> a, pair<double, double> b) {
    return a.first * b.first + a.second * b.second;
}

bool isGoodDegree(Pixel prev, Pixel curr, Pixel next) {
    pair<int, int> v1 = make_pair(curr.x - prev.x, curr.y - prev.y);
    pair<int, int> v2 = make_pair(next.x - curr.x, next.y - curr.y);
    pair<double, double> u1 = toUnit(v1);
    pair<double, double> u2 = toUnit(v2);
    double dot = dotProduct(u1, u2);

    return dot >= cos (maxDeg * (M_PI / 180.0));
}

vector <Node> getNbrs(Node &n) {
    double dist = 0;
    int j = n.pix.x;
    int i = n.pix.y;
    vector<Node> nbrs;
    Node nbrNode;
    nbrNode.passengers = 0;
    int x = j - maxDist < 0 ? 0 : j - maxDist;
    int y = i - maxDist < 0 ? 0 : i - maxDist;

    for (int k = y; k <= i + maxDist && k < height; k++) {
        for (int l = x; l <= j + maxDist && l < width; l++) {
            nbrNode.pix = img[k * width + l];

            if (nbrNode.pix.r + nbrNode.pix.g + nbrNode.pix.b > T) continue;
            dist = sqrt(pow(l - j, 2) + pow(k - i, 2));
            if (dist > maxDist || dist < minDist) continue;
            if (n != start) {
                if (!isGoodDegree(n.prev, n.pix, nbrNode.pix)) continue;
            }
   
            nbrNode.prev = n.pix;
            nbrNode.passengers = n.passengers;

            if (getDist(nbrNode.pix, Dean.pix) <= D) {
                nbrNode.passengers |= deanb;
            }   
            if (getDist(nbrNode.pix, Lin.pix) <= D) {
                nbrNode.passengers |= linb;
            }
            if (getDist(nbrNode.pix, Perez.pix) <= D) {
                nbrNode.passengers |= johnb;
            }
            if (getDist(nbrNode.pix, Reeder.pix) <= D) {
                nbrNode.passengers |= johnb;
            }
            nbrs.push_back(nbrNode);
        }
    }
    return nbrs;
}

void readImage() {
    FILE *inFile = fopen(ppmFile.c_str(), "r");
    fscanf(inFile, "P6%*c%d%*c%d%*c255%*c", &width, &height);
    Pixel n;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fscanf(inFile, "%c%c%c", &n.r, &n.g, &n.b);
            n.x = j;
            n.y = i;
            img.push_back(n);
        }
    }
    fclose (inFile);
}


void color(Pixel &n, int r, int g, int b, int s) {
    int x = n.x - s < 0 ? 0 : n.x - s;
    int y = n.y - s < 0 ? 0 : n.y - s;

    for (int i = y; i <= n.y + s && i < height; i++) {
        for (int j = x; j <= n.x + s && j < width; j++) {
            img[i * width + j].r = r;
            img[i * width + j].g = g;
            img[i * width + j].b = b;
        }
    }
}

void printImage() {
    FILE *outFile = fopen("path.ppm", "w");
    fprintf(outFile, "P6\n%d %d\n255\n", width, height);
    color(img[Dean.pix.y * width + Dean.pix.x], 0, 0, 255, 5);
    color(img[Lin.pix.y * width + Lin.pix.x], 0, 255, 0, 5);
    color(img[Perez.pix.y * width + Perez.pix.x], 255, 0, 0, 5);
    color(img[Reeder.pix.y * width + Reeder.pix.x], 128, 0, 128, 5);
    color(img[start.pix.y * width + start.pix.x], 0, 255, 255, 5);
    color(img[goal.pix.y * width + goal.pix.x], 0, 255, 255, 5);
    
    for (int i = 0; i < height * width; i++) {
        fprintf(outFile, "%c%c%c", img[i].r, img[i].g, img[i].b);
    }
    fclose(outFile);
}

void markPath(Node source, Node dest, ofstream &outFile)
{   
    if (source.pix != dest.pix) markPath(source, pred[dest], outFile);
    if (!(getDist(pred[dest].pix, goal.pix) <= D)) outFile << dest.pix.x << " " << dest.pix.y << "\n";
    color(dest.pix, 245, 102, 0, 3);
}

int inf = 9999999;  
typedef pair<double,Node> pin;
void dijkstra(Node source, Node dest)
{
    dist[source] = 0;
    priority_queue<pin, vector<pin>, greater<pin>> to_visit; 
    to_visit.push(make_pair(0, source));
    while (!to_visit.empty()) {
        Node x = to_visit.top().second;
        to_visit.pop();

        double distCurrDest = getDist(x.pix, dest.pix);
        if (distCurrDest <= D && x.passengers == all) {
            if (x != dest) pred[dest] = x;
            return;
        }

        for (Node &n : getNbrs(x)) {
            if (dist.find(n) == dist.end()) dist[n] = inf;
            double weight = getDist(x.pix, n.pix);          
            if (dist[x] + weight < dist[n]) {
                dist[n] = dist[x] + weight;
                pred[n] = x;
                to_visit.push(make_pair(dist[n], n));
            }
        }
    }
}

void readConfigFile(string configFile) {
    ifstream in(configFile);
    in >> ppmFile;
    in >> start.pix.x >> start.pix.y;
    in >> goal.pix.x >> goal.pix.y;
    in >> Dean.pix.x >> Dean.pix.y;
    in >> Lin.pix.x >> Lin.pix.y;
    in >> Perez.pix.x >> Perez.pix.y;
    in >> Reeder.pix.x >> Reeder.pix.y;
    in >> T >> minDist >> maxDist >> maxDeg >> D;
    in.close();
}

int main(int arg, char *argv[]) {
    ofstream pathFile("path.txt");
    readConfigFile(argv[1]);
    readImage();
    dijkstra(start, goal);
    markPath(start, goal, pathFile);
    printImage();
    pathFile.close();
    return 0;
}
