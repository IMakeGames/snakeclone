/*
 * Section.cpp
 *
 *  Created on: 19-01-2020
 *      Author: gabriel.jara
 */

#include "snake_headers.h"

GameObjt::GameObjt(uint16_t ex, uint16_t yi, directions hDir, unsigned char xid)
{
	//Initialize
	id = xid;
	x = ex;
	y = yi;
	vel = 2;
	headDir = hDir;
	mTexture = NULL;
	mWidth = 0x20;
	mHeight = 0x20;
	for(int i = 0; i<8;i++){
		drs[i] = DEFAULT;
	}
}

GameObjt::~GameObjt()
{
	//Deallocate
	free();
}

bool GameObjt::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        //newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

void GameObjt::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
	}
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	x = 0;
	y = 0;
	vel = 0;
	delete drs;
}

void GameObjt::render(SDL_Renderer* gRenderer)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	//SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
	SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	SDL_RenderFillRect( gRenderer, &renderQuad );
}

void GameObjt::setPos(uint16_t ex, uint16_t yi){
	x = ex;
	y = yi;
}

directions GameObjt::updatePos(){
	switch(headDir){
		case UP:
			y -= vel;
			break;
		case DOWN:
			y += vel;
			break;
		case LEFT:
			x -= vel;
			break;
		case RIGHT:
			x += vel;
			break;
		case DEFAULT:
			printf("DEFAULTED!!");
			fflush(stdout);
	}

	return headDir;
}

void GameObjt::enQueue(directions toQ){
	int ind = 7;
	if(drs[0] != DEFAULT){
		bool shifting = true;
		directions retVal = drs[7];
		while(shifting){
			if(ind > 0){
				drs[ind] = drs[ind-1];
				ind--;
			}else{
				drs[ind] = toQ;
				shifting = false;
			}
		}
		headDir = retVal;
	}else{
		bool enqueuing = true;
		while(enqueuing){
			if(drs[ind] == DEFAULT){
				drs[ind] = toQ;
				enqueuing = false;
			}else{
				ind--;
			}
		}
	}
}

void GameObjt::updateDir(directions dr){
	headDir = dr;
}

uint16_t GameObjt::getWidth()
{
	return mWidth;
}

uint16_t GameObjt::getHeight()
{
	return mHeight;
}

uint16_t GameObjt::gx(){
	return x;
}

uint16_t GameObjt::gy(){
	return y;
}

void GameObjt::sx(uint16_t ex){
	x = ex;
}

void GameObjt::sy(uint16_t yi){
	y = yi;
}
