
//Using SDL and standard IO

#include "snake_headers.h"

//Screen dimension constants
const uint16_t SCREEN_WIDTH = 0x20*0x10;
const uint16_t SCREEN_HEIGHT = 0x20*0x10;
const uint32_t FRAMES_PER_SECOND = 60;

GameObjt* sects[0xFF];

GameObjt* apple;

bool init();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

bool init(){
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO) < 0){
		printf("SDL could  not initialize!! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}else{
		gWindow = SDL_CreateWindow("MY SDL TUTORIAL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL){
			printf("Window could not be created and that's it. Error: %s\n", SDL_GetError());
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( gRenderer == NULL){
				printf("Renderer could not be created and that's it. Error: %s\n", SDL_GetError());
				success = false;
			}
		}
	}
	sects[0] = new GameObjt(SCREEN_WIDTH/0x02,SCREEN_HEIGHT/0x02, RIGHT, 0x00);
	apple = new GameObjt(0x00,0x00, DEFAULT, 0xFF);
	return success;
}

void close(){

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
	for(int i = 0; i < 0xFF; i++){
		if(sects[i] != nullptr){
			delete sects[i];
		}
	}
	delete sects;
}

int main( int argc, char* args[] )
{
	if(!init()){
		printf("Failed to Initialize!\n");
	}else{
		bool quit = false;
		SDL_Event e;
		Uint32 frameTime = 1000/FRAMES_PER_SECOND;
		Uint32 startTime;
		std::default_random_engine generator;
		generator.seed(SDL_GetTicks());
		std::uniform_int_distribution<int> distribution(1,0x10);
		distribution(generator);
		unsigned char sectIndex = 0;
		apple->sx((distribution(generator)-1)*0x20);
		apple->sy((distribution(generator)-1)*0x20);

		while(!quit){
			startTime = SDL_GetTicks();
			while(SDL_PollEvent(&e) != 0){
				if(e.type == SDL_QUIT){
					quit = true;
				}
				else if( e.type == SDL_KEYDOWN ){
					switch(e.key.keysym.sym){
						case SDLK_UP:
							sects[0]->updateDir(UP);
							break;
						case SDLK_DOWN:
							sects[0]->updateDir(DOWN);
							break;
						case SDLK_LEFT:
							sects[0]->updateDir(LEFT);
							break;
						case SDLK_RIGHT:
							sects[0]->updateDir(RIGHT);
							break;
					}
				}
			}
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear(gRenderer);
			if(sects[0]->gx() < apple->gx() + 0x20 && sects[0]->gx() + 0x20 > apple->gx() && sects[0]->gy() < apple->gy() + 0x20 && sects[0]->gy() + 0x20 > apple->gy()){
				printf("Collision!!");
				fflush(stdout);
				sectIndex++;
				sects[sectIndex] = new GameObjt(sects[sectIndex -1]->gx(),sects[sectIndex -1]->gy(), DEFAULT, sectIndex);
				apple->sx((distribution(generator)-1)*0x20);
				apple->sy((distribution(generator)-1)*0x20);
			}
			directions currDir;
			for(int i = 0; i<=sectIndex;i++){
				if(i > 0){
					sects[i]->enQueue(currDir);
				}
				currDir = sects[i]->updatePos();

				sects[i]->render(gRenderer);
			}
			apple->render(gRenderer);

			SDL_RenderPresent( gRenderer );

			Uint32 endTime = SDL_GetTicks();
			Uint32 renderTimePassed = endTime - startTime;
			if(renderTimePassed < frameTime){
				SDL_Delay(frameTime - renderTimePassed);
			}
		}

	}

	close();

	return 0;
}
