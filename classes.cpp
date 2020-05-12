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
	vel = 4;
	headDir = hDir;
	mTexture = NULL;
	mWidth = 0x20;
	mHeight = 0x20;
	SDL_Texture* mTexture = NULL;
	for(int i = 0; i<0x10;i++){
		drs[i] = DEFAULT;
	}
	hitbox = new SDL_Rect{x+0x08,y+0x08,0x08,0x08};
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
	delete hitbox;

}

void GameObjt::render(SDL_Renderer* gRenderer, SDL_Rect* xclip, SDL_RendererFlip flp)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_Rect* thisClp = NULL;
	if(xclip != NULL){
		thisClp = xclip;
	}else{
		thisClp = &clip;
	}
	SDL_RenderCopyEx( gRenderer, mTexture, thisClp, &renderQuad, 0, NULL, flp);
	delete xclip;
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
			hitbox->y -= vel;
			break;
		case DOWN:
			y += vel;
			hitbox->y += vel;
			break;
		case LEFT:
			x -= vel;
			hitbox->x -= vel;
			break;
		case RIGHT:
			x += vel;
			hitbox->x += vel;
			break;
		case DEFAULT:
			break;
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

SDL_Rect* GameObjt::ghitbox(){
	return hitbox;
}

void GameObjt::sx(uint16_t ex){
	x = ex;
}

void GameObjt::sy(uint16_t yi){
	y = yi;
}

void SnakeHead::render(SDL_Renderer* gRenderer, SDL_Rect* xclip, SDL_RendererFlip flp){
	SDL_Rect rct = {0, 0, 0x20, 0x20};
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	switch(headDir){
			case UP:
				rct.y = 0x20;
				break;
			case DOWN:
				rct.y = 0x20;
				flip = SDL_FLIP_VERTICAL;
				break;
			case LEFT:
				break;
			case RIGHT:
				flip = SDL_FLIP_HORIZONTAL;
				break;
			case DEFAULT:
				break;
		}
	GameObjt::render(gRenderer, &rct, flip);
}

void SnakeSect::render(SDL_Renderer* gRenderer, SDL_Rect* xclip, SDL_RendererFlip flp){
	SDL_Rect rct = {0x20, 0, 0x20, 0x20};
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	switch(headDir){
			case UP:
				rct.y = 0x20;
				break;
			case DOWN:
				rct.y = 0x20;
				flip = SDL_FLIP_VERTICAL;
				break;
			case LEFT:
				break;
			case RIGHT:
				flip = SDL_FLIP_HORIZONTAL;
				break;
			case DEFAULT:
				break;
		}
	GameObjt::render(gRenderer, &rct, flip);
}

TextProcessor::TextProcessor(SDL_Texture* mTexture){
	fonts = mTexture;
}

TextProcessor::~TextProcessor(){
	free();
}

void TextProcessor::free()
{
	//Free texture if it exists
	if( fonts != NULL )
	{
		SDL_DestroyTexture( fonts );
	}
}

void TextProcessor::render(SDL_Renderer* gRenderer,uint16_t x, uint16_t y, std::string str, uint8_t scale){
	//Set rendering space and render to screen
	SDL_Rect renderQuad = {x, y, 0x10*scale, 0x10*scale};
	for(char const &c: str){
		SDL_Rect clip = getCharPos(c);
		SDL_RenderCopy(gRenderer, fonts, &clip, &renderQuad);
		renderQuad.x += 0x10*scale;
	}
}

SDL_Rect TextProcessor::getCharPos(char chr){
	uint8_t row = 0;
	uint8_t col = 0;
	switch(chr){
		case 'a':
			break;
		case 'b':
			col = 1;
			break;
		case 'c':
			col = 2;
			break;
		case 'd':
			col = 3;
			break;
		case 'e':
			col = 4;
			break;
		case 'f':
			col = 5;
			break;
		case 'g':
			row = 1;
			col = 0;
			break;
		case 'h':
			row = 1;
			col = 1;
			break;
		case 'i':
			row = 1;
			col = 2;
			break;
		case 'j':
			row = 1;
			col = 3;
			break;
		case 'k':
			row = 1;
			col = 4;
			break;
		case 'l':
			row = 1;
			col = 5;
			break;
		case 'm':
			row = 2;
			col = 0;
			break;
		case 'n':
			row = 2;
			col = 1;
			break;
		case 'o':
			row = 2;
			col = 2;
			break;
		case 'p':
			row = 2;
			col = 3;
			break;
		case 'q':
			row = 2;
			col = 4;
			break;
		case 'r':
			row = 2;
			col = 5;
			break;
		case 's':
			row = 3;
			col = 0;
			break;
		case 't':
			row = 3;
			col = 1;
			break;
		case 'u':
			row = 3;
			col = 2;
			break;
		case 'v':
			row = 3;
			col = 3;
			break;
		case 'w':
			row = 3;
			col = 4;
			break;
		case 'y':
			row = 3;
			col = 5;
			break;
		case '0':
			row = 4;
			col = 0;
			break;
		case '1':
			row = 4;
			col = 1;
			break;
		case '2':
			row = 4;
			col = 2;
			break;
		case '3':
			row = 4;
			col = 3;
			break;
		case '4':
			row = 4;
			col = 4;
			break;
		case '5':
			row = 4;
			col = 5;
			break;
		case '6':
			row = 5;
			col = 0;
			break;
		case '7':
			row = 5;
			col = 1;
			break;
		case '8':
			row = 5;
			col = 2;
			break;
		case '9':
			row = 5;
			col = 3;
			break;
		case '.':
			row = 5;
			col = 4;
			break;
		case ' ':
			row = 5;
			col = 5;
			break;
	}

	SDL_Rect ret = {col*0x10, row*0x10, 0x10, 0x10};
	return ret;
}
