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
#include <cstdint>
#include <cstring>
#include <cassert>

#ifdef PIXUTILS_HAS_SDL
#include <SDL2/SDL_pixels.h>
#endif


namespace PixUtils{
    using Uint8  = std::uint8_t;
    using Uint32 = std::uint32_t;
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
        for(Uint32 l = 0;l < src_h;l ++){
            for(Uint32 c = 0;c != src_w;c ++){
                Uint32 pix = *LocatePixel(src,src_width,src_x + c,src_y + l);
                pix = converter(pix);
                *LocatePixel(dst,dst_width,dst_x + c,dst_y + l);
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
}
} // namespace PixUtils

namespace PixUtils{
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
        void fill_rect(Uint32 x,Uint32 y,Uint32 w,Uint32 h,Uint32 color);

        #ifdef PIXUTILS_HAS_SDL
        void fill_rect(const SDL_Rect &r,Uint32 color){
            fill_rect(r.x,r.y,r.w,r.h,color);
        }
        #endif

        Uint32 w;
        Uint32 h;
        //Cliprect
        Uint32 clip_x = 0;
        Uint32 clip_y = 0;
        Uint32 clip_w;
        Uint32 clip_h;
        Uint32 *ptr;
        //Cliprect
    };
    inline Array::Array(void *pix,Uint32 w,Uint32 h){
        this->ptr = static_cast<Uint32*>(pix);
        this->w = w;
        this->h = h;

        clip_w = w;
        clip_h = h;
    }
    inline void Array::fill_rect(Uint32 x,Uint32 y,Uint32 w,Uint32 h,Uint32 color){
        //Clip it
        Uint32 dst_x = x;
        Uint32 dst_y = y;
        Uint32 dst_w = w;
        Uint32 dst_h = h;

        if(x + w > clip_x + clip_w){
            dst_w = clip_w + clip_x - x;
        }
        if(y + h > clip_y + clip_h){
            dst_h = clip_h + clip_y - y;
        }

        if(x < clip_x){
            dst_x = clip_x;
            //dst_w -= clip_x - x;
        }
        if(y < clip_y){
            dst_y = clip_y;
            //dst_h -= clip_h - x;
        }


        Unsafe::FillRect(ptr,this->w,dst_x,dst_y,dst_w,dst_h,color);
    }
} // namespace PixUtils


#endif // _BTK_PIXELS_UTILS_HPP_
