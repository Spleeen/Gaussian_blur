#include "SDL_Utilities.h"
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

const SDL_Color SDLUtils::WHITE = {255,255,255,255} ;
const SDL_Color SDLUtils::GREY = {192,192,192,192} ;
const SDL_Color SDLUtils::BLACK = {0,0,0,0};
const SDL_Color SDLUtils::RED = {255,255,0,0};
const SDL_Color SDLUtils::GREEN = {255,0,255,0}; 
const SDL_Color SDLUtils::BLUE = {255,0,0,255};
const SDL_Color SDLUtils::YELLOW = {255,255,255,0};
const SDL_Color SDLUtils::CYAN = {255,0,255,255};
const SDL_Color SDLUtils::MAGENTA = {255,255,0,255};
const SDL_Color SDLUtils::VIOLET = {255,128,0,255};
const SDL_Color SDLUtils::ORANGE = {255,255,128,0};
const SDL_Color SDLUtils::PINK = {255,255,128,128};

SDLUtils::SDLUtils(void){}

SDLUtils::~SDLUtils(void){}

bool SDLUtils::isColorEqual(SDL_Color *fColor,SDL_Color  *sColor){

    return (fColor->b == sColor->b && fColor->g == sColor->g && fColor->r == sColor->r);
}

SDL_Color SDLUtils::getPixel(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    SDL_Color color = {0, 0, 0, 0};
    Uint32 pixel = 0;
 
    switch(bpp)
    {
        case 1:
            pixel = (Uint32)*p;
            break;

        case 2:
            pixel = (Uint32)(*(Uint16 *)p);
            break;
     
        case 3:
            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixel = (Uint32)(p[0] << 16) | (Uint32)(p[1] << 8) | (Uint32)p[2];
            #else
                pixel = (Uint32)p[0] | (Uint32)(p[1] << 8) | (Uint32)(p[2] << 16);
            #endif
            break;
     
        case 4:
            pixel = *(Uint32*)p;
            break;
     
        default:
            break;
    }

    SDL_GetRGB(pixel, surface->format, &color.r, &color.g, &color.b);
    return color;
}

void SDLUtils::setPixel(SDL_Surface* surface, int x, int y, const SDL_Color& color)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    Uint32 pixel = SDL_MapRGB(surface->format, color.r, color.g, color.b);
 
    switch(bpp)
    {
        case 1:
            *p = (Uint8)pixel;
            break;
     
        case 2:
            *(Uint16 *)p = (Uint16)pixel;
            break;
     
        case 3:
            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            #else
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            #endif
            break;
     
        case 4:
            *(Uint32*)p = pixel;
            break;
        default:
            break;
    }
}

SDL_Surface* SDLUtils::getEmptySurface(int w, int h){
    SDL_Surface* cs = SDL_GetVideoSurface();
    SDL_Surface *empty = SDL_CreateRGBSurface(SDL_HWSURFACE,
                              w, h,
                              cs->format->BitsPerPixel,
                              0,
                              0,
                              0,
                              0);
    return empty;
}

Uint32 SDLUtils::colorToUint32 (SDL_Color color){
    return *((Uint32*)&color);
}

SDL_Color SDLUtils::intToSDLColor (Uint32 int_color)
{
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        SDL_Color color;
        color.r = (Uint8)((int_color & 0xff000000)>>24);
        color.g = (Uint8)((int_color & 0x00ff0000)>>16);
        color.b = (Uint8)((int_color & 0x0000ff00)>>8);
        color.unused = 0;   

    #else
        SDL_Color color;
        color.r = (Uint8)(int_color & 0x000000ff);
        color.g = (Uint8)((int_color & 0x0000ff00)>>8);
        color.b = (Uint8)((int_color & 0x00ff0000)>>16);
        color.unused = 0;    
   #endif

    return color;
}

SDL_Surface* SDLUtils::convolution(SDL_Surface* input, const vector< vector<float> >& kernel)
{

    if (kernel.size()%2 != 1) return NULL;

    int kw = (kernel.size() > 0) ? (int)kernel[0].size() : 0;
    int kh = (int)kernel.size();
    SDL_Surface* output = SDL_CreateRGBSurface(SDL_HWSURFACE, input->w, input->h, 32, 0, 0, 0, 0);

    SDL_LockSurface(input);
    SDL_LockSurface(output);

    #pragma omp parallel for
    for ( int y=0 ; y<input->h ; y++)
    {
        for ( int x=0 ; x<input->w ; x++)
        {
            float sum_r = 0;
            float sum_g = 0;
            float sum_b = 0;

            for(int cy=-kh/2 ; cy<=kh/2 ; cy++)
            {
                for(int cx=-kw/2 ; cx<=kw/2 ; cx++)
                {
                    SDL_Color pixel;

                    int nx = min(max(x + cx, 0), input->w-1);
                    int ny = min(max(y + cy, 0), input->h-1);
                    pixel = getPixel(input, nx, ny);

                    sum_r += pixel.r * kernel[cy+kh/2][cx+kw/2];
                    sum_g += pixel.g * kernel[cy+kh/2][cx+kw/2];
                    sum_b += pixel.b * kernel[cy+kh/2][cx+kw/2];
                }
            }

            /* Prevents some nonsense values ​​of the components (clamping) */
            SDL_Color finalColor;
            finalColor.r = (Uint8)min(max(int(sum_r + 0.5f), 0), 255);
            finalColor.g = (Uint8)min(max(int(sum_g + 0.5f), 0), 255);
            finalColor.b = (Uint8)min(max(int(sum_b + 0.5f), 0), 255);
            setPixel(output, x, y, finalColor);
        }
    }

    SDL_UnlockSurface(output);
    SDL_UnlockSurface(input);

    return output;
}


SDL_Surface* SDLUtils::blur(SDL_Surface* input, int radius, int sigma)
{
    /* Making Kernel */
    float sumKernel = 0;

    vector< vector<float> > kernel(radius*2+1);

    for(int cy=-radius ; cy<=radius ; cy++)
    {
        kernel[cy+radius].resize(radius*2+1);

        for(int cx=-radius ; cx<=radius ; cx++)
        {
            /* Gaussian */
            float e = exp(-float(cx*cx+cy*cy) / (2.f*(float)(sigma*sigma)))/(2*M_PI*sigma*sigma);
            sumKernel += e;
            kernel[cy+radius][cx+radius] = e;
        }
    }

    /* Normalize */
    for(int cy=0 ; cy<radius*2+1 ; cy++)
        for(int cx=0 ; cx<radius*2+1 ; cx++)
            kernel[cy][cx] /= sumKernel; 
        
    return convolution(input, kernel);
}

SDL_Surface* SDLUtils::imageLoad(const string path)
{
    SDL_Surface *imageLoad = NULL;
 
    imageLoad = IMG_Load (path.c_str());
    if (imageLoad == NULL)
    {
        cerr <<"Unable to load image" <<path <<" because : " <<endl <<SDL_GetError()<<endl; 
        exit (EXIT_FAILURE);
    }
 
    return imageLoad;
}

TTF_Font* SDLUtils::fontLoad(const string fontName, int fontSize) 
{
    TTF_Font *font = NULL;
 
    font = TTF_OpenFont(fontName.c_str(), fontSize);
    if (font == NULL)
    {
        cerr <<"Unable to load font " <<fontName <<" because : " <<endl <<SDL_GetError()<<endl; 
        exit (EXIT_FAILURE);
    }
 
    return font;
}

void SDLUtils::switchToFullscreen (SDL_Surface* screen)
{
    Uint32 previousFlags = screen->flags; 
    screen = SDL_SetVideoMode(0, 0, 0, screen->flags ^ SDL_FULLSCREEN);
    if(screen == NULL) screen = SDL_SetVideoMode(0, 0, 0, previousFlags);
    if(screen == NULL) exit (EXIT_FAILURE);
}


SDL_Surface* SDLUtils::scaleSurface (SDL_Surface* surface, double zoom)
{
    return rotozoomSurface(surface, 0, zoom, 1);
}

