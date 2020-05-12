
//Using SDL and standard IO

#include "snake_headers.h"

//Screen dimension constants
const uint16_t SCREEN_WIDTH = 0x20*0x10;
const uint16_t SCREEN_HEIGHT = 0x20*0x10;
const uint32_t FRAMES_PER_SECOND = 60;
unsigned char sectIndex = 0;

gameState currentState = MAIN_MENU;

SDL_Event e;

GameObjt* sects[0x20];

GameObjt* head;

GameObjt* apple;

bool init();

void close();

bool doMenu();

bool doGame();

bool doGameOver();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

TexLoader* snkTexLoader;

TexLoader* fontTexLoader;

TextProcessor* proc;

std::default_random_engine generator;

std::uniform_int_distribution<int> distribution(1,0x10);

Uint32 frameTime = 1000/FRAMES_PER_SECOND;

Uint32 startTime;

bool init(){
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO) < 0){
		printf("SDL could  not initialize!! SDL_Error: %s\n", SDL_GetError());
		success = false;
	}else{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, 0 ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		gWindow = SDL_CreateWindow("MY SDL TUTORIAL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if( gWindow == NULL){
			printf("Window could not be created and that's it. Error: %s\n", SDL_GetError());
			success = false;
		}else{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if( gRenderer == NULL){
				printf("Renderer could not be created and that's it. Error: %s\n", SDL_GetError());
				success = false;
			}else{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				int imgFlags = IMG_INIT_PNG;

				if(!(IMG_Init(imgFlags) & imgFlags)){
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}
	sects[0] = new SnakeHead(SCREEN_WIDTH/0x02,SCREEN_HEIGHT/0x02, RIGHT, 0x00);
	apple = new GameObjt(0x00,0x00, DEFAULT, 0xFF);
	snkTexLoader = new TexLoader();
	fontTexLoader = new TexLoader();
	snkTexLoader->loadFromFile("assets/snak_ssheat.png", gRenderer);
	fontTexLoader->loadFromFile("assets/fontst.png", gRenderer);
	proc = new TextProcessor(fontTexLoader->getTex());
	sects[0]->seText(snkTexLoader->getTex(), 0, 0);
	apple->seText(snkTexLoader->getTex(), 0, 2);
	generator.seed(SDL_GetTicks());
	distribution(generator);
	return success;
}

void close(){

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
	for(int i = 0; i < 0x20; i++){
		if(sects[i] != nullptr){
			delete sects[i];
		}
	}
	delete sects;
	delete apple;
	delete snkTexLoader;
	delete fontTexLoader;
	delete proc;
}

bool doMenu(){
	bool quit = false;
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}else if( e.type == SDL_KEYDOWN ){
			switch(e.key.keysym.sym){
				case SDLK_SPACE:
					currentState = GAME;
					break;
			}
		}
	}
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear(gRenderer);
	std::string posString = "fuckin snake";
	proc->render(gRenderer, 2*0x20, 7*0x20, posString, 2);
	posString = "press enter to start";
	proc->render(gRenderer, 2.5*0x20, 9*0x20, posString);
	SDL_RenderPresent( gRenderer);

	Uint32 endTime = SDL_GetTicks();
	Uint32 renderTimePassed = endTime - startTime;
	if(renderTimePassed < frameTime){
		SDL_Delay(frameTime - renderTimePassed);
	}

	return quit;
}

bool doGame(){
	bool quit = false;
	while(SDL_PollEvent(&e) != 0){
		if(e.type == SDL_QUIT){
			quit = true;
		}else if( e.type == SDL_KEYDOWN ){
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
	std::string posString = "apple score: ";
	posString += std::to_string(sectIndex);
	proc->render(gRenderer, 0x0A, 0x0A, posString);
	if(sects[0]->gx() > SCREEN_WIDTH || sects[0]->gx() + 0x20 > SCREEN_WIDTH || sects[0]->gy() > SCREEN_HEIGHT || sects[0]->gy() + 0x20 > SCREEN_HEIGHT){
		currentState = GAME_OVER;
		delete sects;
		sects[0] = new SnakeHead(SCREEN_WIDTH/0x02,SCREEN_HEIGHT/0x02, RIGHT, 0x00);
		sects[0]->seText(snkTexLoader->getTex(), 0, 0);
		apple->sx((distribution(generator)-1)*0x20);
		apple->sy((distribution(generator)-1)*0x20);
 	}
	if(sects[0]->gx() < apple->gx() + 0x20 && sects[0]->gx() + 0x20 > apple->gx() && sects[0]->gy() < apple->gy() + 0x20 && sects[0]->gy() + 0x20 > apple->gy()){
		sectIndex++;
		sects[sectIndex] = new SnakeSect(sects[sectIndex-1]->gx(),sects[sectIndex -1]->gy(), DEFAULT, sectIndex);
		sects[sectIndex]->seText(snkTexLoader->getTex(), 0, 1);
		apple->sx((distribution(generator)-1)*0x20);
		apple->sy((distribution(generator)-1)*0x20);
	}
	directions currDir;
	for(int i = 0; i<=sectIndex;i++){
		if(i > 0){
			sects[i]->enQueue(currDir);
		}
		currDir = sects[i]->updatePos();

		if(i > 0 && currDir != DEFAULT && SDL_HasIntersection(sects[0]->ghitbox(), sects[i]->ghitbox()) == SDL_TRUE){
			currentState = GAME_OVER;
			delete sects;
			sects[0] = new SnakeHead(SCREEN_WIDTH/0x02,SCREEN_HEIGHT/0x02, RIGHT, 0x00);
			sects[0]->seText(snkTexLoader->getTex(), 0, 0);
			apple->sx((distribution(generator)-1)*0x20);
			apple->sy((distribution(generator)-1)*0x20);
		}
		sects[i]->render(gRenderer);
	}
	apple->render(gRenderer);
	SDL_RenderPresent( gRenderer);

	Uint32 endTime = SDL_GetTicks();
	Uint32 renderTimePassed = endTime - startTime;
	if(renderTimePassed < frameTime){
		SDL_Delay(frameTime - renderTimePassed);
	}

	return quit;
}

bool doGameOver(){
	bool quit = false;
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}else if( e.type == SDL_KEYDOWN ){
				switch(e.key.keysym.sym){
					case SDLK_SPACE:
						currentState = GAME;
						sectIndex = 0;
						break;
				}
			}
		}
		SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear(gRenderer);
		std::string posString = "game over bitch";
		proc->render(gRenderer, 1*0x20, 7*0x20, posString, 2);
		posString = "final apple score: ";
		posString += std::to_string(sectIndex);
		proc->render(gRenderer, 2.5*0x20, 9*0x20, posString);
		posString = "press space to start again";
		proc->render(gRenderer, 2.5*0x20, 10*0x20, posString);
		SDL_RenderPresent( gRenderer);

		Uint32 endTime = SDL_GetTicks();
		Uint32 renderTimePassed = endTime - startTime;
		if(renderTimePassed < frameTime){
			SDL_Delay(frameTime - renderTimePassed);
		}

		return quit;
}

int main( int argc, char* args[] )
{
	if(!init()){
		printf("Failed to Initialize!\n");
	}else{
		bool quit = false;
		apple->sx((distribution(generator)-1)*0x20);
		apple->sy((distribution(generator)-1)*0x20);

		while(!quit){
			startTime = SDL_GetTicks();
			if(currentState == MAIN_MENU){
				quit = doMenu();
			}else if(currentState == GAME){
				quit = doGame();
			}else if(currentState == GAME_OVER){
				quit = doGameOver();
			}
		}
	}
	close();
	return 0;
}
