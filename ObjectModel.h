#ifndef OBJECT_H_
#define OBJECT_H_

#include "Graphics.h"
#include "MatrixVector.h"
#include "Transform.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>


using namespace std;


//The vertex has a vertex and vertex normal
struct Vertex{
    Vec3 v;     //the 3d point of the normal
    Vec3 norm; // The vertex normal
    int cnt = 0;

    Vertex(){
        v = Vec3(0,0,0);
        norm = Vec3(0,0,0);

    }

    Vertex(const Vec3& input){
        v = input;
        norm = Vec3(0,0,0);

    }
};

//Edge is used in scanline filling algorithm
struct Edge{
    Vec2 *v1,*v2; //we have two vertices

    //the initializer function
    Edge(){
        v1 = NULL;
        v2 = NULL;
    }

    Edge(Vec2* a, Vec2* b):v1(a),v2(b){
        //make sure v1 has small y and v2 has larger y value
        //if not so swap them
        if (v1->y > v2->y){
            Vec2 *temp = v1;
            v1 = v2;
            v2 = temp;
        }
    }

    //simply a copy overloaded with =
    void operator = (Edge Ed){
        v1 = Ed.v1; v2 = Ed.v2;
    }

};

class Object3d{
    private:
        vector <Vertex> vertBuffer;     //List of Vertices
        vector <Vec2> textureBuffer;    //List of textures
        vector <Vec3> normBuffer;       //List of normals
        vector <Vec3> surfaceBuffer;    //list of Surfaces(vert,texture,norm)
        vector <Vec3> vertexNorm;         //List of vertex normal

        bool texture;
        int xoffset,yoffset;            //to move the object by some distance in x

    //adders
    public:
        Object3d(int xffset = 0,int yffset = 0){texture = false;xoffset = xffset;yoffset = yffset;}
        void addVertex(Vec3& v){vertBuffer.push_back(Vertex(v));}
        void addSurface(Vec3& v){surfaceBuffer.push_back(v);}
        void addNormal(Vec3& v){normBuffer.push_back(v);}
        void addTexture(Vec2& v){textureBuffer.push_back(v);}

    //file manipulators
    public:
        void SaveObject(string filename);
        void LoadObject(string filename);

    //Object fillers
    public:
        void drawWire(Screen* S,Vec3& camera,Vec3& LookTo,float pWidth,float pHeight);
        void render(Screen* S,Vec3& camera,Vec3& LookTo,vector<Vec3> LightPos,float pWidth,float pHeight, int color, float intensityValue);
        void drawSpan(Screen* S,Edge& E1, Edge& E2, int color);

    //calculators
    public:
        void calculateNorm();


};



#endif // OBJECT_H_
