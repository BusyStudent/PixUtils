#include <SDL2/SDL.h>
#include <iostream>
#include "../src/pixutils.hpp"
void test_clamp(){
    PixUtils::Rect rect = {
        20,
        500,
        500,
        400
    };
    PixUtils::Rect r1 = {
        0,
        0,
        500,
        400
    };
    std::cout << rect << std::endl;
    std::cout << rect.intersect(r1) << std::endl;
}
int main(){
    using PixUtils::Array;
    test_clamp();
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
    Array surf_array(surf->pixels,200,200);
    surf_array.copyfrom(array,{100,100,200,200},0,0,PixUtils::SDLConverter(
        s->format,
        surf->format
    ));
    //PixUtils::Unsafe::FillRect(static_cast<Uint32*>(surf->pixels),surf->w,0,0,100,100,22);
    SDL_SaveBMP(surf,"copy.bmp");
    SDL_FreeSurface(surf);
    SDL_FreeSurface(s);
    SDL_DestroyWindow(win);
    SDL_Quit();
}