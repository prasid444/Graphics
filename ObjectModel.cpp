#include "ObjectModel.h"

void Object3d::calculateNorm(){

    unsigned int len = surfaceBuffer.size();
    unsigned int t1, t2, t3;
    Vec3 V1, V2, V3;
    Vec3 norm(0,0,0);

    for(unsigned int i=0;i<len;i+=3){
        //indexes for the vertices
        t1 =surfaceBuffer[i].x-1;
        t2 =surfaceBuffer[i+1].x-1;
        t3 =surfaceBuffer[i+2].x-1;
//

        //Actual Vertices
        V1 = vertBuffer[t1].v;
        V2 = vertBuffer[t2].v;
        V3 = vertBuffer[t3].v;

        //determine the normal of the traingle and assign
        Vec3 A = (V3-V1)/((V3-V1).magnitude());
        Vec3 B = (V2-V1)/((V2-V1).magnitude());
        norm = B.crossProduct(A);
        norm = norm / norm.magnitude();

        vertBuffer[t1].norm = norm + vertBuffer[t1].norm ;
        vertBuffer[t2].norm = norm + vertBuffer[t2].norm ;
        vertBuffer[t3].norm = norm + vertBuffer[t3].norm ;


        vertBuffer[t1].norm = vertBuffer[t1].norm / vertBuffer[t1].norm.magnitude();
        vertBuffer[t2].norm = vertBuffer[t2].norm / vertBuffer[t2].norm.magnitude();
        vertBuffer[t3].norm = vertBuffer[t3].norm / vertBuffer[t3].norm.magnitude();


        vertBuffer[t1].cnt++; vertBuffer[t2].cnt++; vertBuffer[t3].cnt++;
    }

    len = vertBuffer.size();
    for(unsigned int i=0;i<len;i++){
        vertBuffer[i].norm = vertBuffer[i].norm / vertBuffer[i].cnt;
        vertBuffer[i].norm = vertBuffer[i].norm / vertBuffer[i].norm.magnitude();

    }
}




void replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void Object3d::LoadObject(string filename){
    vertBuffer.clear();
    surfaceBuffer.clear();
    normBuffer.clear();
    textureBuffer.clear();

     bool vertonly = true, vertnorm = false, vertnormtext = false,typefixed = false;
    //These three bools say that the face contains either vertex only
    //or vertex and normal only or
    //vertex normal and texture

    ifstream obj(filename.c_str());
    if (!obj.is_open()) throw "Can not open";

    vector <Vec3> temp;
    string line,keyword;;
    while(getline(obj,line)){
        while(line.compare(0,1," ")==0)
            line.erase(line.begin()); // remove leading whitespaces
        while(line.size()>0 && line.compare(line.size()-1,1," ")==0)
            line.erase(line.end()-1); // remove trailing whitespaces
        if (line == "" || line == "\n") continue;
        istringstream linestream(line);
        linestream >> keyword;

        if (keyword == "v"){
            Vec3 temp;
            unsigned int t;
            linestream >> temp.x;
            linestream >> temp.y;
            linestream >> temp.z;
            if (!(linestream>>t))
                t = 1.0f;

            //normalize wrt t
            if (t>0 && t<1)
                temp = temp / t;

            addVertex(temp);
        }
        else if (keyword == "vn"){
            Vec3 v;
            linestream >> v.x;
            linestream >> v.y;
            linestream >> v.z;
            addNormal(v);
        }

        else if(keyword == "vt"){
            texture = true;
            Vec2 v;
            linestream >> v.x;
            linestream >> v.y;
            addTexture(v);
        }

        else if(keyword == "f"){
            Vec3 v[3];

            line = line.substr(1,line.length()-1); //remove the preceding f
            while(line.compare(0,1," ")==0)
                line.erase(line.begin()); // remove leading whitespaces
            while(line.size()>0 && line.compare(line.size()-1,1," ")==0)
            line.erase(line.end()-1); // remove trailing whitespaces


            ///These sets of command run only once to determine the type of object definition
            std::size_t found = line.find("//");
            if (found!=std::string::npos && typefixed == false){
                vertonly = false; vertnorm = true; typefixed = true;
            }
            // a // means v and n

            found = line.find('/');
            if (found!=std::string::npos && typefixed == false){
                vertonly = false; vertnormtext = true; typefixed = true;
            }
            //for a single / it is v and t and n

            if (typefixed == false){
                typefixed = true;
            }
            //else it means vertonly
            ///Now we know if our face contains vertex only, vertex and normal only , or v&n&texture

            if (vertnormtext){
                replaceAll(line,"/"," "); //remove the / for easy calculatoin
                istringstream lstream(line);
                //v contains .x = vertex index, .y = texture .z= normal index

                lstream >> v[0].x;lstream >> v[0].y;lstream >> v[0].z;
                lstream >> v[1].x;lstream >> v[1].y;lstream >> v[1].z;
                lstream >> v[2].x;lstream >> v[2].y;lstream >> v[2].z;

                addSurface(v[0]);addSurface(v[1]);addSurface(v[2]);
            }

            else if (vertnorm){
                replaceAll(line,"//"," "); //remove the / for easy calculatoin
                istringstream lstream(line);
                //v contains .x = vertex index, .y = texture .z= normal index

                lstream >> v[0].x;lstream >> v[0].z;
                lstream >> v[1].x;lstream >> v[1].z;
                lstream >> v[2].x;lstream >> v[2].z;

                addSurface(v[0]);addSurface(v[1]);addSurface(v[2]);
            }

            else{
                istringstream lstream(line);
                //v contains .x = vertex index, .y = texture .z= normal index

                lstream >> v[0].x;
                lstream >> v[1].x;
                lstream >> v[2].x;

                addSurface(v[0]);addSurface(v[1]);addSurface(v[2]);
            }

        }
    }
    obj.close();
    calculateNorm();
}


void Object3d::drawWire(Screen* S,Vec3& camera,Vec3& LookTo,float pWidth,float pHeight){
    Vec3 C = Vec3(0,0,0);
    S->clrscr();
    S->resetZ();

    //Adding the 3d vertices into 2d vertex
    unsigned int len = vertBuffer.size();
    Vec2 vert2d[len];
    for (unsigned int i=0;i<len;i++)
    {
        vert2d[i] = World_To_Pixel(vertBuffer[i].v,camera,LookTo,pWidth,pHeight,640,480);
//        cout << v[i].z << endl;
    }

    len = surfaceBuffer.size();
    unsigned int t1;
    unsigned int t2;
    for (unsigned int i=0;i<len;i+=3)
    {
        t1 =surfaceBuffer[i].x-1;
        t2 =surfaceBuffer[i+1].x-1;
        S->gn_line(vert2d[t1],vert2d[t2],C);

        t1 =surfaceBuffer[i+1].x-1;
        t2 =surfaceBuffer[i+2].x-1;
        S->gn_line(vert2d[t1],vert2d[t2],C);

        t1 =surfaceBuffer[i+2].x-1;
        t2 =surfaceBuffer[i].x-1;
        S->gn_line(vert2d[t1],vert2d[t2],C);
    }

    S->refresh();
}

void Object3d::render(Screen* S,Vec3& camera,Vec3& LookTo,vector<Vec3> LightPos,float pWidth,float pHeight, int color, float intensityValue){
    unsigned int len = vertBuffer.size();
    Vec2 vert2d[len];
    float intensity = 0;
    Vec3 temp(0,0,0);
    for (unsigned int i=0;i<len;i++)
    {
        temp = Vec3(0,0,0);
        intensity = 0;

        vert2d[i] = World_To_Pixel(vertBuffer[i].v,camera,LookTo,pWidth,pHeight,640,480);
//        //assign intensity here for shading
        for(unsigned int j=0; j<LightPos.size();j++){
            Vec3 A = LightPos[j] - camera;
            A = A / A.magnitude();
            intensity = intensity + vertBuffer[i].norm.dotProduct(A);
        }
        //if it is > 1 we truncate it to be 1
        //change the intensity component
          intensity *= intensityValue;
        if (intensity > 1)
            intensity = 1;
//        cout << v[i].z << endl;

        // if the intensity is -ve we simply avoid the intensity as it is the back face
        if (intensity < 0)
            intensity = 0.05;
//        else
//            Lpos.push_back(vertBuffer[i].v);
         vert2d[i].i = intensity;
    }

    unsigned int t1,t2,t3;
    len = surfaceBuffer.size();

    //temp store of vertices and edges
    Vec2 v[3];
    Edge E[3];

    //for all surfaces
    for(unsigned int i=0;i<len;i+=3){
        //indexes for the vertices
        t1 =surfaceBuffer[i].x-1;
        t2 =surfaceBuffer[i+1].x-1;
        t3 =surfaceBuffer[i+2].x-1;
       // if (t1 <0 || t2 <0||t3 <0) continue;
      //vertexes
      v[0] = vert2d[t1];
      v[1] = vert2d[t2];
      v[2] = vert2d[t3];
        //Edges
        E[0] = Edge(v,v+1);
        E[1] = Edge(v+1,v+2);
        E[2] = Edge(v+2,v);

        //we need to find the longest edge in y-axis. to do so check the y-lenght of all edges
        float maxLen =0;
        int longIndex =0;
        for (int i=0;i<3;i++){
            float Len = E[i].v2->y - E[i].v1->y;
            if (Len>maxLen){
                maxLen = Len;
                longIndex = i;
            }
        }

        //Index for other two edges
        int shortIndex1 = (longIndex + 1) % 3;
        int shortIndex2 = (longIndex + 2) % 3;

        if (E[shortIndex1].v1->y > E[shortIndex2].v1->y)
            swap (shortIndex1,shortIndex2);

        drawSpan(S,E[longIndex],E[shortIndex1],color);
        drawSpan(S,E[longIndex],E[shortIndex2],color);
    }

}

void Object3d::drawSpan(Screen* S,Edge& E1, Edge& E2, int color){
    //calculate the y difference for interpolation
    float e1ydiff = (float)(E1.v2->y - E1.v1->y);
    if (e1ydiff == 0)
        return;

    float e2ydiff = (float)(E2.v2->y - E2.v1->y);
    if (e2ydiff == 0)
        return;


    float x1 = E1.v1->x;
    float x2 = E2.v1->x;

    float x1i = (float)(E1.v2->x - E1.v1->x)/ e1ydiff;
    float x2i = (float)(E2.v2->x - E2.v1->x)/ e2ydiff;
    // float factor,stepfactor;
    float z1= E1.v1->z;
    float z2= E2.v1->z;
    float z1i = (float)(E1.v2->z - E1.v1->z)/e1ydiff;
    float z2i = (float)(E2.v2->z - E2.v1->z)/e2ydiff;
    float i1 = E1.v1->i;
    float i2 = E2.v1->i;
    float i1i = (float)(E1.v2->i - E1.v1->i)/e1ydiff;
    float i2i = (float)(E2.v2->i - E2.v1->i)/e2ydiff;

    // float x1,x2,z1,z2,i1,i2;
    unsigned int y;
    // loop through the lines between the edges and draw spans
    for(y = E2.v1->y; y < E2.v2->y; y++) {
        // create and draw span
        x1 += x1i;
        x2 += x2i;
        z1 += z1i;
        z2 += z2i;
        i1 += i1i;
        i2 += i2i;
        if(color == 1)
        S->st_line(Vec2(x1+xoffset,y+yoffset,z1,i1),Vec2(x2+xoffset,y+yoffset,z2,i2),Vec3(0,0,255));
        if(color == 2)
        S->st_line(Vec2(x1+xoffset,y+yoffset,z1,i1),Vec2(x2+xoffset,y+yoffset,z2,i2),Vec3(255,0,0));
        if(color == 3)
        S->st_line(Vec2(x1+xoffset,y+yoffset,z1,i1),Vec2(x2+xoffset,y+yoffset,z2,i2),Vec3(0,100,0));
        if(color == 4)
        S->st_line(Vec2(x1+xoffset,y+yoffset,z1,i1),Vec2(x2+xoffset,y+yoffset,z2,i2),Vec3(50,50,50));

        //S->refresh();
    }
    *(E1.v1) = Vec2(x1,y,z1,i1);
}
