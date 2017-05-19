
#include "ObjectModel.h"
#include "MatrixVector.h"
#include "Graphics.h"

void Screen::setpixel(Vec2 P,Vec3 c,float intensity ){
    int *pixmem32;
    int colour;

    int width = screen->w;
    int height = screen->h;

    //Check for boundaries
    int x = P.x ;
    int y = P.y ;
    if (!(x>0 && x < width && y >0 && y<height)) return;
    if (P.z > Zbuffer[height*x + y])
       return;
    else
        Zbuffer[height*x + y] = P.z;

    colour = SDL_MapRGB (screen->format,c.x*intensity,c.y*intensity,c.z*intensity);
    pixmem32 = (int*) screen->pixels+y*screen->pitch/4+x;
    *pixmem32 = colour;
}

void Screen::st_line(Vec2 P1, Vec2 P2, Vec3 c){
    if (P1.x > P2.x){
        Vec2 temp = P1;
        P1 = P2;
        P2 = temp;
    }

    int x1 = P1.x; int y1 = P1.y;
    int x2 = P2.x; int y2 = P2.y;

    float dVal = P1.z, delta_d = P2.z - P1.z;       // The depth value of that point, and the difference delta_d
    float iVal = P1.i, delta_i = P2.i - P1.i;       //The intensity values

    float dx = x2-x1;
    for (int i=x1; i<= x2; ++i) {
        setpixel(i, y1, dVal, c, iVal);
        dVal += delta_d/dx;
        iVal += delta_i/dx;
    }

}

//Draw line considering the depth of the points.
void Screen::gn_line(Vec2 P1, Vec2 P2,Vec3 c){

    int x1 = P1.x; int y1 = P1.y;
    int x2 = P2.x; int y2 = P2.y;

    float dVal = P1.z, delta_d = P2.z - P1.z;       // The depth value of that point, and the difference delta_d
    float iVal = P1.i, delta_i = P2.i - P1.i;       //The intensity values

    int delta_x(x2 - x1);
    // if x1 == x2, then it does not matter what we set here
    signed char const ix((delta_x > 0) - (delta_x < 0));
    delta_x = ABS(delta_x) << 1;

    int delta_y(y2 - y1);
    // if y1 == y2, then it does not matter what we set here
    signed char const iy((delta_y > 0) - (delta_y < 0));
    delta_y = ABS(delta_y) << 1;


    setpixel(x1,y1,dVal,c,iVal);
    if (delta_x >= delta_y)
    {
        // error may go below zero
        int error(delta_y - (delta_x >> 1));
        float id = delta_d / (float) delta_x;
        float ii = delta_i / (float) delta_x;
        while (x1 != x2)
        {
            if ((error >= 0) && (error || (ix > 0)))
            {
                error -= delta_x;
                y1 += iy;
            }
            // else do nothing

            error += delta_y;
            x1 += ix;
            dVal += id;
            iVal += ii;
            setpixel(x1, y1 ,dVal, c,iVal);
        }
    }
    else
    {
        // error may go below zero
        int error(delta_x - (delta_y >> 1));
        float id = delta_d / (float) delta_y;
        float ii = delta_i / (float) delta_y;
        while (y1 != y2)
        {
            if ((error >= 0) && (error || (iy > 0)))
            {
                error -= delta_y;
                x1 += ix;
            }
            // else do nothing

            error += delta_x;
            y1 += iy;
            dVal += id;
            iVal += ii;
            setpixel(x1, y1 ,dVal, c,iVal);
        }
    }
}
