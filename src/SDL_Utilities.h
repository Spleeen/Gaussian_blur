/*
*   Created by N.Richard
*   Date de création : 21 novembre 2013
*	Date de version : 28 décembre 2013
*   Version 1.1
*/

#ifndef SDL_UTILITIES_H
#define SDL_UTILITIES_H

#ifdef __APPLE__
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_rotozoom.h>
#else
	#include <SDL/SDL.h>
	#include <SDL/SDL_image.h>
	#include <SDL/SDL_ttf.h>
	#include <SDL/SDL_rotozoom.h>
#endif

#include <string>
#include <vector>
 
class SDLUtils
{
	public:
		static const SDL_Color WHITE;
		static const SDL_Color GREY; 
		static const SDL_Color BLACK;  
		static const SDL_Color RED;
		static const SDL_Color GREEN;
		static const SDL_Color BLUE; 
		static const SDL_Color YELLOW;
		static const SDL_Color CYAN;
		static const SDL_Color MAGENTA;
		static const SDL_Color VIOLET;
		static const SDL_Color ORANGE;
		static const SDL_Color PINK;

	    SDLUtils(void);
        ~SDLUtils(void);
        static bool isColorEqual(SDL_Color *fColor,SDL_Color  *sColor);
		static SDL_Color getPixel(SDL_Surface* surface, int x, int y);
		static void setPixel(SDL_Surface* surface, int x, int y, const SDL_Color& color);
        static SDL_Surface* getEmptySurface(int w, int h);
		static SDL_Surface* blur(SDL_Surface* input, int radius = 1, int sigma = 1);
		static SDL_Color intToSDLColor (Uint32 int_color);
		static Uint32 colorToUint32 (SDL_Color color);
		static SDL_Surface* imageLoad (const std::string path);
		static TTF_Font* fontLoad(const std::string fontName, int fontSize);
		static void switchToFullscreen (SDL_Surface* screen);
		static SDL_Surface* scaleSurface (SDL_Surface* surface, double zoom);
	private:
		static SDL_Surface* convolution(SDL_Surface* input, const std::vector< std::vector<float> >& kernel);
};                 

#endif //SDL_UTILITIES_H
