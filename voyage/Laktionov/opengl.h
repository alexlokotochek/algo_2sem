//
//  opengl.h
//  voyager
//
//  Created by Lokotochek on 19.05.15.
//  Copyright (c) 2015 Alexander Laktionov. All rights reserved.
//

#ifndef __voyager__opengl__
#define __voyager__opengl__
#include <vector>

struct Point{
    double x;
    double y;
    int region;
};

struct Edge{
    double weight;
    int a;
    int b;
    bool operator< (const Edge&x) const{
        return weight < x.weight;
    }
};

void visualizer(std::vector<Point> &data, std::vector<Edge> &edges, int centers);

void reshape(int width, int height);

void display();

void DrawPoints(float x1,float y1);



#endif /* defined(__voyager__opengl__) */
