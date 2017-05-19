#ifndef GRAPHICS_H_
#define GRAPHICS_H_


#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "MatrixVector.h"
#include <math.h>
#include <cassert>
#include "MatrixVector.h"
#define round(a) ((int) (a+0.5))
#define ABS(a) ((a<0)? a*-1 : a)

class Screen{
    SDL_Surface* screen;
    TTF_Font* font;
    SDL_Surface* textSurface;
    float* Zbuffer;
    SDL_Rect textLocation = { 80, 10, 0, 0 };
    public:
        Uint32 initTime;

        Screen(int width = 640,int height=480){
            SDL_Init(SDL_INIT_EVERYTHING);
            TTF_Init();

            font = TTF_OpenFont("ARIAL.TTF", 40);
            SDL_Color foregroundColor = { 0, 0, 0 };
            SDL_Color backgroundColor = { 255, 255, 255 };
            textSurface = TTF_RenderText_Shaded(font, "SUNGLASS SHOWROOM",foregroundColor, backgroundColor);

            initTime = SDL_GetTicks();
            screen = SDL_SetVideoMode(width,height,32,SDL_SWSURFACE);
            Zbuffer = new float [width*height](); // () for setting everything to zero
        }

        void setpixel(Vec2 P,Vec3 c,float intensity =.8);
        inline void setpixel(int x,int y,float dVal, Vec3 c,float intensity =1 ){
            Vec2 temp(x,y);
            temp.z = dVal;
            setpixel(temp,c,intensity);
        }

        void gn_line(Vec2 P1,Vec2 P2,Vec3 c= Vec3(255,255,255)); // gn_line
        void st_line(Vec2 P1,Vec2 P2,Vec3 c= Vec3(255,255,255)); // line parallel to x axis

        void resetZ(){
            delete[] Zbuffer;
            Zbuffer = new float [(screen->w)*(screen->h)]();
        }
        void refresh(){
            initTime = SDL_GetTicks();
            SDL_BlitSurface(textSurface, NULL, screen, &textLocation);
            SDL_Flip(screen);
        }

        void clrscr(){
            SDL_FreeSurface(screen);
            SDL_Rect rect;

            rect.x = 0; rect.y = 0;
            rect.h = screen->h; rect.w = screen->w;
            SDL_FillRect(screen,&rect,16777215);
            //screen = SDL_SetVideoMode(screen->w,screen->h,32,SDL_SWSURFACE);
        }

        ~Screen(){
            if (screen) {
                SDL_FreeSurface(screen);
                screen = NULL;
            }
            SDL_FreeSurface(textSurface);
            TTF_CloseFont(font);
            TTF_Quit();
            SDL_Quit();
            delete[] Zbuffer;
        }
};



#endif // GRAPHICS_H_
