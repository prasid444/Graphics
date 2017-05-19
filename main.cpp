#include "ObjectModel.h"
#include "MatrixVector.h"
#include "Graphics.h"
#include "Transform.h"

Vec3 camera(-10,0,-40);
//Vec3 fcamera(-5,0,-80);

int color = 1;
int wireframe = 0;
float intensity = 1;
int lightsource = 5;

//Vec3 camera(-25,-25,-25);
Vec3 LookTo(0,0,0);
vector<Vec3> Lightposition1;
vector<Vec3> Lightposition2;
vector<Vec3> Lightposition3;

int processInput(){
     Uint8* keystate = SDL_GetKeyState(NULL);

    //continuous-response keys
    if(keystate[SDLK_z]){
        camera.z += 1;
    }
    if(keystate[SDLK_x]){
        camera.z -= 1;
    }
    if(keystate[SDLK_1]){
        color = 1;
    }
    if(keystate[SDLK_2]){
        color = 2;
    }
    if(keystate[SDLK_3]){
        color = 3;
    }
    if(keystate[SDLK_4]){
        color = 4;
    }
    if(keystate[SDLK_8]){
        wireframe = 1;
    }
    if(keystate[SDLK_9]){
        wireframe = 0;
    }
    if(keystate[SDLK_5]){
        lightsource = 5;
    }
    if(keystate[SDLK_6]){
        lightsource = 6;
    }

    if(keystate[SDLK_LEFT]){
        RotateY(camera,-2);
    }

    if(keystate[SDLK_RIGHT]){
        RotateY(camera,2);
    }

    if(keystate[SDLK_UP]){
        RotateX(camera,2);
    }

    if(keystate[SDLK_DOWN]){
        RotateX(camera,-2);
    }

    if(keystate[SDLK_a]){
        intensity = intensity + 0.1;
    }

    if(keystate[SDLK_s]){
        intensity = intensity - 0.1;
    }





    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    return -1;
            break;
        }
    }
    return 0;
}

int main(int argc, char* args[]){
    Screen Scr(640,480);

    Object3d model(0,0);
    model.LoadObject("objects/chasma3.obj");


    //select this light source when 5 pressed
    Lightposition1.push_back(Vec3(0,0,400));

    //select this light source when 6 pressed
    Lightposition2.push_back(Vec3(400,0,0));








    while (processInput() != -1){

        Scr.clrscr();
        Scr.resetZ();
        if(wireframe == 1){
            model.drawWire(&Scr,camera,LookTo,0.5,0.5);
        }
        else{
            if(lightsource == 5){
                model.render(&Scr,camera,LookTo,Lightposition1,0.5,0.5,color,intensity);
            }
            else if(lightsource == 6){
                model.render(&Scr,camera,LookTo,Lightposition2,0.5,0.5,color,intensity);
            }
        }
        Scr.refresh();

    }

    return 0;
}
