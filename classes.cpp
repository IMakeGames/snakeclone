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
	SDL_Texture* mTexture = NULL;
	for(int i = 0; i<0x10;i++){
		drs[i] = DEFAULT;
	}
}

GameObjt::~GameObjt()
{
	//Deallocate
	free();
}

void GameObjt::seText(SDL_Texture* tex, unsigned char row, unsigned char col){
	clip = {0x20*col, 0x20*row, 0x20, 0x20};
	mTexture = tex;
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
	SDL_RenderCopy( gRenderer, mTexture, &clip, &renderQuad );
	//SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
	//SDL_RenderFillRect( gRenderer, &renderQuad );
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
	int ind = 0x0F;
	if(drs[0] != DEFAULT){
		bool shifting = true;
		directions retVal = drs[0x0F];
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
