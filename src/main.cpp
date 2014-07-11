#include "Utilities.h"
#include "SDL_Utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <iostream>

#ifdef __APPLE__
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_getenv.h>
#else
	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_ttf.h>
	#include <SDL_getenv.h>
#endif

using namespace std;
using namespace Utilities;

SDL_Surface* screen = NULL;
SDL_Surface* application_icon = NULL ;
TTF_Font *fontSmall = NULL, *fontLarge = NULL;
const int FONTSIZE_SMALL = 18, FONTSIZE_LARGE = 30;
int sigma = 4;
int radius = 8;
const string fontDir = "./fonts/";
const string imgDir = "./img/";
const int FRAME_PER_SEC = 50;
const Uint32 INITFLAGS = (SDL_INIT_VIDEO);
int VIDEO_H = 540;
int VIDEO_W = 960;
const int VIDEO_BPP = 32;
const Uint32 VIDEOFLAGS = (SDL_HWSURFACE | SDL_RESIZABLE | SDL_DOUBLEBUF);

int man (void);
void initSDL(const string picturePath);
void start_proccess (string imagePath);            

int main(int argc, char *argv[])
{
	/*Support for input parameters*/

    if (argc != 2 || (argv[1] == string("-h") || argv[1] == string("--help")))
    	return man();
    else if (!isReadable (argv[1]))
    	return man();

    string pathToImage = argv[1];
    initSDL(pathToImage); 
	start_proccess(pathToImage);

	/* Clean up the SDL library */
	SDL_FreeSurface(application_icon);
	SDL_FreeSurface(screen);
	TTF_CloseFont(fontSmall);
    TTF_CloseFont(fontLarge);
	TTF_Quit();
	SDL_Quit();
	return EXIT_SUCCESS;
}

int man (void)
{
	cout <<"NAME" <<endl;
	cout <<tab <<"blur - Gaussian blur" <<endl;
	cout <<"SYNOPSIS" <<endl;
	cout <<tab <<"blur \033[4mPICTURE\033[0m" <<endl;
	cout <<"DESCRIPTION" <<endl;
	cout <<tab <<"Shows the result of a Gaussian blur on a picture (implementation of a convolution product)" <<endl;
	cout <<tab <<"Picture format (transparence isn't managed) : JPG, TIF, BMP, PNG, GIF, TGA" <<endl;
	return EXIT_FAILURE;
} 

void initSDL(const string picturePath)
{


	/* Initialize the SDL library */
	if ( SDL_Init(INITFLAGS) != 0 ) {
		cerr <<"Couldn't initialize SDL : " <<endl <<tab <<SDL_GetError() <<endl;
		exit (EXIT_FAILURE);
	}
	
	application_icon = SDLUtils::imageLoad(imgDir+"application_icon.png");
	SDL_WM_SetIcon(application_icon, NULL);
    SDL_WM_SetCaption(("Gaussian blur on : "+picturePath+" !").c_str(), NULL);
    //SDL_putenv((char*)"SDL_VIDEO_CENTERED=center");	
	screen = SDL_SetVideoMode(VIDEO_W, VIDEO_H, VIDEO_BPP, VIDEOFLAGS);
    if (screen == NULL) {
		cerr <<"Couldn't set " <<VIDEO_W <<"x" <<VIDEO_H <<"x" <<VIDEO_BPP <<" video mode : " <<endl <<tab <<SDL_GetError() <<endl;
		SDL_Quit();
		exit (EXIT_FAILURE);
	}


	if(TTF_Init() != 0)
	{
		cerr <<"Couldn't initialize SDL_TTF : " <<endl <<tab <<SDL_GetError() <<endl;
    	exit (EXIT_FAILURE);
	} 

	/* Font settings */
	fontSmall = SDLUtils::fontLoad("fonts/Transformers.ttf", FONTSIZE_SMALL);
    fontLarge = SDLUtils::fontLoad("fonts/Transformers.ttf", FONTSIZE_LARGE);
}

void start_proccess (string imagePath)
{
    bool quit = false;
    bool firstTime = true;
    SDL_Event event;
	SDL_Surface* inputImage;
	SDL_Surface* inputImageOrigin = SDLUtils::imageLoad(imagePath);
    double zoomX = (double)VIDEO_W / inputImageOrigin->w;
    double zoomY = (double)VIDEO_H / inputImageOrigin->h;
    double zoom = min(zoomX, zoomY);
	inputImage = SDLUtils::scaleSurface(inputImageOrigin, zoom);

	SDL_Surface* outputImage = NULL;
	SDL_Surface* textSigma = NULL;
	SDL_Surface* textRadius = NULL;
    SDL_Surface* textInfos = NULL;
    SDL_Surface* textBefore = NULL;
    SDL_Surface* textAfter = NULL;
	SDL_Surface* OK = SDLUtils::imageLoad(imgDir+"OK.png");
	SDL_Surface* STOP = SDLUtils::imageLoad(imgDir+"STOP.png");
    SDL_Surface* RecNotification = SDL_CreateRGBSurface(SDL_HWSURFACE, 2400, 80, VIDEO_BPP, 0, 0, 0, 0);
    SDL_FillRect(RecNotification, NULL, SDLUtils::colorToUint32(SDLUtils::BLACK));
    SDL_SetAlpha(RecNotification, SDL_SRCALPHA, 128);

	/* Positions */
	SDL_Rect positionSigma, positionRadius, positionInfos, positionStatement , positionRecNot, positionBefore, positionAfter;
    SDL_Rect posInputImage, posInputScreen, posOutputImage, posOutputScreen;


	/* First blur before events */
   	//cout << "Computing blur with radius=" << radius << " & sigma=" << sigma << endl;
 
	//FPSmanager manager;		/*Framerate controller*/

	//Initialisation
	//SDL_initFramerate(&manager);
	//Nombre d'images souhaités
	//SDL_setFramerate(&manager,FRAME_PER_SEC);
	while (!quit) {  
		bool compute = firstTime;

        SDL_WaitEvent(&event);

		switch (event.type) { 
			case SDL_VIDEORESIZE:
                {
    				VIDEO_W = event.resize.w;
    				VIDEO_H = event.resize.h;
    				SDL_FreeSurface (screen);
                    screen = SDL_SetVideoMode(VIDEO_W, VIDEO_H, VIDEO_BPP, VIDEOFLAGS);
    				
                    zoomX = (double)VIDEO_W / inputImageOrigin->w;
                    zoomY = (double)VIDEO_H / inputImageOrigin->h;
                    zoom = min(zoomX, zoomY);

                    SDL_FreeSurface (inputImage);
                    inputImage = SDLUtils::scaleSurface (inputImageOrigin, zoom);

                    compute = true;
                }
				break;
			case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_q:
                        quit = true;
                        break;

                    case SDLK_DOWN:
                        sigma = max(sigma-1, 1);
                        compute = true;
                        break;

                    case SDLK_UP:
                        sigma++;
                        compute = true;
                        break;

                    case SDLK_LEFT:
                        radius = max(radius-1, 0);
                        compute = true;
                        break;

                    case SDLK_RIGHT:
                        radius++;
                        compute = true;
                        break;

                    case SDLK_ESCAPE:
                        quit = true;
                        break;

                    default:
                        break;
                }
                break;
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
		}

        if(compute)
        {

			/* Blitting  statement */ 
        	SDL_BlitSurface(STOP, NULL, screen, &positionStatement); 
			SDL_Flip(screen);

        	/* Cleaning */
			SDL_FreeSurface(outputImage);

            if (!firstTime){
    			SDL_FreeSurface(textSigma);
    			SDL_FreeSurface(textRadius);
                SDL_FreeSurface(textInfos);
                SDL_FreeSurface(textBefore);
                SDL_FreeSurface(textAfter);
            }

	   		/* Blur render */
		   	//cout << "Computing blur with radius=" << radius << " & sigma=" << sigma << endl;
		   	t_start;
	        outputImage = SDLUtils::blur(inputImage, radius, sigma); 
            t_end;
            
            posInputImage.x = 0;
            posInputImage.y = 0;
            posInputImage.w = inputImage->w/2;
            posInputImage.h = inputImage->h;
            posInputScreen.x = (Sint16)(VIDEO_W - inputImage->w) / 2;
            posInputScreen.y = (Sint16)(VIDEO_H - inputImage->h) / 2;
            posInputScreen.w = VIDEO_W / 2;
            posInputScreen.h = (Sint16)(VIDEO_H - inputImage->h) / 2;
            posOutputImage.x = outputImage->w / 2;
            posOutputImage.y = 0;
            posOutputImage.w = outputImage->w;
            posOutputImage.h = outputImage->h;
            posOutputScreen.x = (Sint16)(VIDEO_W - 1) / 2;
            posOutputScreen.y = (Sint16)(VIDEO_H - outputImage->h) / 2;
            posOutputScreen.w = (Sint16)(VIDEO_W);
            posOutputScreen.h = (Sint16)(VIDEO_H);

            textSigma = TTF_RenderText_Blended(fontSmall, ("Sigma : "+to_string(sigma)+" (Up/Down)").c_str(), SDLUtils::GREY);
            textRadius = TTF_RenderText_Blended(fontSmall, ("Radius : "+to_string(radius)+" (Left/Right)").c_str(), SDLUtils::GREY);
            textInfos = TTF_RenderText_Blended(fontSmall, "ECHAP/Q to quit", SDLUtils::GREY);
            textBefore = TTF_RenderText_Blended(fontLarge, "Before", SDLUtils::WHITE);
            textAfter = TTF_RenderText_Blended(fontLarge, ("After (blur:"+to_string((int)(t_value/10e3))+" ms)").c_str(), SDLUtils::WHITE);

            positionSigma.x = VIDEO_W/2 - textSigma->w/2;
            positionSigma.y = 10;
            positionRadius.x = VIDEO_W/2 - textRadius->w/2;
            positionRadius.y = positionSigma.y + 20;
            positionInfos.x = VIDEO_W/2 - textInfos->w/2;;
            positionInfos.y = positionRadius.y + 20;
            positionStatement.x = (Sint16)(VIDEO_W-50);
            positionStatement.y = (Sint16)(VIDEO_H-50);
            positionRecNot.x = 0;
            positionRecNot.y = 0; 
            positionBefore.x = VIDEO_W/5 - textBefore->w/2;
            positionBefore.y = positionRadius.y; 
            positionAfter.x = VIDEO_W*4/5 - textAfter->w/2;
            positionAfter.y = positionRadius.y;

            /* Bliting */
            SDL_FillRect(screen, NULL, 0);
            SDL_BlitSurface(inputImage, &posInputImage, screen, &posInputScreen);
            SDL_BlitSurface(outputImage, &posOutputImage, screen, &posOutputScreen);
            SDL_BlitSurface(RecNotification, NULL, screen, &positionRecNot);
            SDL_BlitSurface(textSigma, NULL, screen, &positionSigma);
            SDL_BlitSurface(textRadius, NULL, screen, &positionRadius);
            SDL_BlitSurface (textInfos, NULL, screen, &positionInfos);
            SDL_BlitSurface (textBefore, NULL, screen, &positionBefore);
            SDL_BlitSurface (textAfter, NULL, screen, &positionAfter);
            SDL_BlitSurface(OK, NULL, screen, &positionStatement);  
            SDL_Flip(screen);
        }
      	
        if(firstTime)
            firstTime = false; 
		//SDL_framerateDelay(&manager);
	}

	SDL_FreeSurface(inputImage);
	SDL_FreeSurface(outputImage);
    SDL_FreeSurface(inputImageOrigin);
    SDL_FreeSurface(textSigma);
    SDL_FreeSurface(textRadius);
    SDL_FreeSurface(textInfos);
    SDL_FreeSurface(textBefore);
    SDL_FreeSurface(textAfter);
    SDL_FreeSurface(OK);
    SDL_FreeSurface(STOP);
    SDL_FreeSurface(RecNotification);
}

