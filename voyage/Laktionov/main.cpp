#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <cmath>
#include <random>
#include <chrono>
#include <assert.h>

#include "opengl.h"


using namespace std;

//struct Edge{
//    double weight;
//    int a;
//    int b;
//    bool operator< (const Edge&x) const{
//        return weight < x.weight;
//    }
//};

//struct Point{
//    double x;
//    double y;
//};

struct singleEdge{
    int v;
    double weight;
};


class Disjoint {
public:
    Disjoint() = default;
    
    explicit Disjoint(size_t size_input){
        size = size_input;
        ancestor.assign(size, -1);
        height.assign(size, -1);
    }
    
    void make_set(int v){
        ancestor[v] = v;
        height[v] = 0;
    }
    
    int find_set(int v){
        if (v == ancestor[v])
            return v;
        return ( ancestor[v] = find_set(ancestor[v]) );
    }
    
    void union_sets(int a, int b){
        a = find_set(a);
        b = find_set(b);
        if (a != b){
            if (height[a] < height[b])
                swap (a, b);
            else{
                ancestor[b] = a;
                if (height[a] == height[b])
                    ++height[a];
            }
        }
    }
    
private:
    
    vector<int> ancestor;
    vector<int> height; // можно попробовать зарандомить
    size_t size;
    
};

class Kruskal{
    
public:
    
    explicit Kruskal(size_t size_input){
        size = size_input;
        //allEdges.resize(size*(size-1)/2 + 1);
        //cout << "\nallEdges resized to " << size*(size-1)/2+1 << "\n";
        sets = Disjoint(size);
    }
    
    void getData(vector<Point> &data){
        Edge e;
        for (int i = 0; i < data.size()-1; ++i){
            for (int j = i+1; j < data.size(); ++j){
                e.a = i;
                e.b = j;
                e.weight = sqrt(( (data[i].x-data[j].x)*(data[i].x-data[j].x) + (data[i].y-data[j].y)*(data[i].y-data[j].y)));
                allEdges.push_back(e);
            }
        }
        //cout << "\ngot " << data.size() << " nodes and " << allEdges.size() << " edges in allEdges\n";
    }
    
    void solve(){
        sort(allEdges.begin(), allEdges.end());
        for (int i = 0; i < size; ++i){
            sets.make_set(i);
        }
        int a, b;
        minimalTree.resize(size);
        singleEdge x, y;
        for (int i = 0; i < allEdges.size(); ++i){
            a = sets.find_set(allEdges[i].a);
            b = sets.find_set(allEdges[i].b);
            if (a != b){
                sets.union_sets(a, b);
                x.weight = allEdges[i].weight;
                x.v = a;
                y.weight = allEdges[i].weight;
                y.v = b;
                minimalTree[a].push_back(y);
                minimalTree[b].push_back(x);
            }
        }
        cycle.reserve(size);
        ///dfs
        vector<char> visited(size, 0);
        dfs(0, visited);
        cycle.push_back(0);
        
        Edge cycleE;
        cycleE.weight = -1;
        for (int i = 0; i < size; ++i){
            // cycle = { v0, v1, v2, v3, ..., vi, ... , v[size-1] }
            cycleE.a = cycle[i];
            cycleE.b = cycle[i+1];
            cycleEdges.push_back(cycleE);
        }
        assert(cycleEdges.size() == size);
        
    }
    
    // DFS
    void dfs(int u, vector<char> &visited){
        visited[u] = 1;
        cycle.push_back(u);
        for (int i = 0; i < minimalTree[u].size(); ++i){
            int neighbour = minimalTree[u][i].v;
            if (visited[neighbour] == 0){
                dfs(neighbour, visited);
            }
        }
    }
    
    
//    void printAnswer(){
//        cout << "\nANSWER:::\n\n";
//        for (int i = 0; i < minimalTree.size(); ++i){
//            cout << minimalTree[i].a << " <---> " << minimalTree[i].b << "\n";
//        }
//        cout << "\n" << minimalTree.size() << " edges there!";
//    }
//    
    vector<Edge> returnEdgesToDraw(){
        return cycleEdges;
    }
    
private:
    
    vector<Edge> allEdges;
    vector<vector<singleEdge>> minimalTree;
    vector<int> cycle;
    vector<Edge> cycleEdges;
    
    size_t size;
    Disjoint sets;
    
};

int main() {
    
    cout << "Enter [center Sigma] [points Sigma] [number of centers] [number of points per center]\n";
    
    int centerSigma, pointsSigma, centers, pointPerCenter;
    cin >> centerSigma >> pointsSigma >> centers >> pointPerCenter;

    vector<Point> nodes;
    Point input;
    
    // генерация точек вокруг центров
//    std::random_device rd;
//    std::mt19937 gen(rd());
    std::default_random_engine generator ((int)std::chrono::system_clock::now().time_since_epoch().count());
    double x, y, centerX, centerY;
    std::normal_distribution<double> distributionCenters(0, centerSigma); // nu, sigma
    for (int i = 0; i < centers; ++i){
        centerX = distributionCenters(generator);
        centerY = distributionCenters(generator);
        //cout << centerX << ";\t" << centerY << " = (x, y) ----- radius from (0,0): " << sqrt(centerX*centerX + centerY*centerY);
        std::normal_distribution<double> distributionPointX(centerX, pointsSigma), distributionPointY(centerY, pointsSigma);
        for (int j = 0; j < pointPerCenter; ++j){
            x = distributionPointX(generator);
            y = distributionPointY(generator);
            input.x = x;
            input.y = y;
            input.region = i;
            nodes.push_back(input);
            //cout << "\n\t\tradius from this center: " << sqrt((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY)) << " (x,y) = " << x << ";\t" << y;
        }
        //cout << "\n\n------------\n\n";
    }
    
    Kruskal task(centers*pointPerCenter);
    task.getData(nodes);
    task.solve();
    //task.printAnswer();
    
    
    
    /////
    auto tree = task.returnEdgesToDraw();
    visualizer(nodes, tree, centers);
    
    
    return 0;
}
