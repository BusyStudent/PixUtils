/**
 * @file pixutils.hpp
 * @author BusyStudent (fyw90mc@gmail.com)
 * @brief Pixels Utils for Btk
 * @version 0.1
 * @date 2021-04-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#if !defined(_BTK_PIXELS_UTILS_HPP_)
#define _BTK_PIXELS_UTILS_HPP_
#include <algorithm>
#include <ostream>
#include <cstdint>
#include <cstring>
#include <cassert>

#ifdef SDL_pixels_h_
#define PIXUTILS_HAS_SDL
#endif

#ifdef PIXUTILS_HAS_SDL
#include <SDL2/SDL_pixels.h>
#endif


namespace PixUtils{
    using Uint8  = std::uint8_t;
    using Uint32 = std::uint32_t;
    using Sint64 = std::int64_t;
}

namespace PixUtils{
namespace Unsafe{
    struct PixelCopy{
        uint32_t operator ()(uint32_t pix) const noexcept{
            return pix;
        }
    };
    /**
     * @brief Locate Pixel in Pixels Array
     * 
     * @param pixels 
     * @param w The width of the array
     * @param x The pixel's x
     * @param y 
     * @return Uint32* 
     */
    inline Uint32 *LocatePixel(Uint32 *pixels,Uint32 w,Uint32 x,Uint32 y){
        assert(pixels != nullptr);
        return &pixels[w * y + x];
    }
    inline const Uint32 *LocatePixel(const Uint32 *pixels,Uint32 w,Uint32 x,Uint32 y){
        assert(pixels != nullptr);
        return &pixels[w * y + x];
    }
    //Delocate
    /**
     * @brief Delocate the pixel
     * 
     * @param pixel The pixel's pointer
     * @param w The width of the array
     * @param x 
     * @param y 
     * @return The array pointer
     */
    inline Uint32 *DelocatePixel(Uint32 *pixel,Uint32 w,Uint32 x,Uint32 y){
        assert(pixel != nullptr);
        return pixel - x - w * y;
    }
    inline const Uint32 *DelocatePixel(const Uint32 *pixel,Uint32 w,Uint32 x,Uint32 y){
        assert(pixel != nullptr);
        return pixel - x - w * y;
    }
    /**
     * @brief Copy the pixels
     * 
     * @tparam Converter The Pixel converter
     * @param src The source pixels array
     * @param src_width 
     * @param src_x 
     * @param src_y 
     * @param src_w 
     * @param src_h 
     * @param dst 
     * @param dst_width 
     * @param dst_x 
     * @param dst_y 
     * @param converter The pixels converter
     */
    template<class Converter = PixelCopy>
    void Copy(
        const Uint32 *src,
        Uint32 src_width,
        Uint32 src_x,
        Uint32 src_y,
        Uint32 src_w,
        Uint32 src_h,
        Uint32 *dst,
        Uint32 dst_width,
        Uint32 dst_x,
        Uint32 dst_y,
        Converter converter = PixelCopy()){
        //Do check
        assert(src != nullptr && dst != nullptr);
        //For each pixels
        Uint32 x = src_x;
        Uint32 y = src_y;
        Uint32 w = src_w;
        Uint32 h = src_h;

        for(Uint32 cur_y = y;cur_y < y + h;cur_y ++){
            for(Uint32 cur_x = x;cur_x < x + w;cur_x ++){
                Uint32 pix = *LocatePixel(src,src_width,cur_x,cur_y);
                Uint32 offset_x = cur_x - x;
                Uint32 offset_y = cur_y - y;
                *LocatePixel(dst,dst_width,offset_x + dst_x,offset_y + dst_y) = converter(pix);
            }
        }
    }
    /**
     * @brief Fill a rect
     * 
     * @param dst 
     * @param width 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     */
    inline void FillRect(
        Uint32 *dst,
        Uint32 width,
        Uint32 x,
        Uint32 y,
        Uint32 w,
        Uint32 h,
        Uint32 color){
        
        assert(dst != nullptr);
        for(Uint32 cur_y = y;cur_y < y + h;cur_y ++){
            for(Uint32 cur_x = x;cur_x < x + w;cur_x ++){
                *LocatePixel(dst,width,cur_x,cur_y) = color;
            }
        }
    }
    /**
     * @brief Draw a line at
     * 
     * @param dst 
     * @param width 
     * @param x1 
     * @param y1 
     * @param x2 
     * @param y2 
     * @param color
     */
    inline void DrawLine(Uint32 *dst,Uint32 width,Uint32 x1,Uint32 y1,Uint32 x2,Uint32 y2,Uint32 color){
        assert(dst != nullptr);
        if(x1 == x2){
            FillRect(dst,width,x1,y1,1,std::abs(Sint64(y2) - Sint64(y1)),color);
        }
        else if(y1 == y2){
            FillRect(dst,width,x1,y1,1,std::abs(Sint64(y2) - Sint64(y1)),color);
        }
        else{
            //Unsupported yet
            assert(false);
        }
    }
}
} // namespace PixUtils

namespace PixUtils{
    struct Rect{
        Rect() = default;
        Rect(const Rect &) = default;
        Rect(Uint32 x,Uint32 y,Uint32 w,Uint32 h){
            this->x = x;
            this->y = y;
            this->w = w;
            this->h = h;
        }
        #ifdef PIXUTILS_HAS_SDL
        /**
         * @brief Construct a new Rect object from SDL_Rect
         * 
         * @param r An unempty rect
         */
        Rect(const SDL_Rect &r){
            assert(!SDL_RectEmpty(&r));
            x = Uint32(r.x);
            y = Uint32(r.y);
            w = Uint32(r.w);
            h = Uint32(r.h);
        }
        #endif
        /**
         * @brief Limite the rect in the area
         * 
         * @param r The rect you want to limit
         * @return Rect 
         */
        Rect intersect(const Rect &r) const noexcept{
            Sint64 minx = std::max(r.x,x);
            Sint64 miny = std::max(r.y,y);
            Sint64 maxx = std::min(r.x + r.w,x + w);
            Sint64 maxy = std::min(r.y + r.h,y + h);
            return Rect(
                minx,
                miny,
                std::max(Sint64(0),maxx - minx),
                std::max(Sint64(0),maxy - miny)
            );
        }
        Uint32 x;
        Uint32 y;
        Uint32 w;
        Uint32 h;
    };
    struct Point{
        Uint32 x;
        Uint32 y;
    };
    /**
     * @brief The pixels array
     * 
     */
    struct Array{
        /**
         * @brief Construct a new Array object
         * 
         * @param pix The pixels array 
         * @param w The width of it
         * @param h The height of it
         */
        Array(void *pix,Uint32 w,Uint32 h);
        Array(const Array &) = default;
        /**
         * @brief Fill a rect
         * 
         * @param x 
         * @param y 
         * @param w 
         * @param h 
         * @param color 
         */
        void fill_rect(Uint32 x,Uint32 y,Uint32 w,Uint32 h,Uint32 color){
            fill_rect({x,y,w,h},color);
        }
        void fill_rect(const Rect &r,Uint32 color);
        /**
         * @brief Copy pixels from 
         * 
         * @param src The pixel array
         * @param area The area you want to copy from
         * @param x The dst_x
         * @param y The dst_y
         */
        template<class Converter = Unsafe::PixelCopy>
        void copyfrom(const Array &src,const Rect &area,Uint32 x,Uint32 y,Converter c = Converter()){
            Rect r = src.clip.intersect(area);
            Rect dst(x,y,r.w,r.h);
            dst = clip.intersect(dst);
            r.w = dst.w;
            r.h = dst.h;
            
            Unsafe::Copy<Converter>(src.ptr,src.w,r.x,r.y,r.w,r.h,ptr,w,dst.x,dst.y,c);
        }
        /**
         * @brief Reset the cliprect
         * 
         */
        void reset_clip();
        void set_clip(const Rect &r);

        Uint32 w;
        Uint32 h;
        //Cliprect
        Rect clip;
        Uint32 *ptr;
        //Cliprect
    };
    inline Array::Array(void *pix,Uint32 w,Uint32 h){
        this->ptr = static_cast<Uint32*>(pix);
        this->w = w;
        this->h = h;

        clip.w = w;
        clip.h = h;
        clip.x = 0;
        clip.y = 0;
    }
    inline void Array::fill_rect(const Rect &dst,Uint32 color){
        //Clip it
        Rect r = clip.intersect(dst);
        Unsafe::FillRect(ptr,this->w,r.x,r.y,r.w,r.h,color);
    }
    inline void Array::reset_clip(){
        clip.x = 0;
        clip.y = 0;
        clip.w = w;
        clip.h = h;
    }
    inline void Array::set_clip(const Rect &r){
        Rect t(0,0,w,h);
        clip = t.intersect(r);
    }
    inline std::ostream &operator <<(std::ostream &os,const Rect &r){
        os << r.x << ',' << r.y << ',' << r.w  << ',' << r.h;
        return os;
    }
} // namespace PixUtils

//SDL ext
#ifdef PIXUTILS_HAS_SDL
namespace PixUtils{
    /**
     * @brief Convert Pixels by SDL_Format
     * 
     */
    struct SDLConverter{
        SDLConverter(SDL_PixelFormat *s,SDL_PixelFormat *d){
            src = s;
            dst = d;
        }
        SDLConverter(const SDLConverter &) = default;
        SDL_PixelFormat *src;
        SDL_PixelFormat *dst;
        Uint32 operator()(Uint32 pix) const{
            SDL_Color c;
            SDL_GetRGBA(pix,src,&c.r,&c.g,&c.b,&c.a);
            return SDL_MapRGBA(dst,c.r,c.g,c.b,c.a);
        }
    };
}
#endif
#endif // _BTK_PIXELS_UTILS_HPP_
