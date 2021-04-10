#include <SDL2/SDL.h>
#include "../src/pixutils.hpp"
int main(){
    using PixUtils::Array;
    SDL_Window *win = SDL_CreateWindow("a",0,0,500,500,0);
    auto *s = SDL_GetWindowSurface(win);

    Array array(s->pixels,s->w,s->h);

    //array.clip_x = 100;
    //array.clip_y = 100;
    //array.clip_w = 400;
    //array.clip_h = 400;

    array.fill_rect(0,0,555,400,SDL_MapRGB(s->format,11,44,55));
    array.fill_rect(100,100,200,200,SDL_MapRGB(s->format,255,255,255));

    //SDL_SaveBMP(s,"output.bmp");
    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if(event.type == SDL_QUIT){
            break;
        }
        SDL_UpdateWindowSurface(win);
    }
    SDL_Surface *surf = SDL_CreateRGBSurfaceWithFormat(0,200,200,32,SDL_PIXELFORMAT_RGBA32);

    PixUtils::Unsafe::Copy(
        static_cast<Uint32*>(s->pixels),
        s->w,
        0,
        0,
        200,
        200,
        static_cast<Uint32*>(surf->pixels),
        surf->w,
        0,
        0
    );
    SDL_SaveBMP(surf,"copy.bmp");
    SDL_FreeSurface(surf);
    SDL_FreeSurface(s);
    SDL_DestroyWindow(win);
    SDL_Quit();
}