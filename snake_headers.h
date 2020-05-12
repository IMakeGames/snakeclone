#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <random>

enum directions{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	DEFAULT
};

enum gameState{
	MAIN_MENU,
	GAME,
	GAME_OVER
};

class GameObjt
{
	protected:
		uint16_t x, y, mWidth, mHeight, vel;
		unsigned char id;
		directions headDir, drs[0x10];
		SDL_Texture* mTexture;
		SDL_Rect clip;
		SDL_Rect* hitbox;
	public:
		//Initializes variables
		GameObjt(uint16_t, uint16_t, directions, unsigned char);

		//Deallocates memory
		~GameObjt();
		
		void seText(SDL_Texture*, unsigned char, unsigned char);
		
		//Deallocates texture
		void free();

		//Renders texture at given point
		virtual void render(SDL_Renderer*, SDL_Rect* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);

		
		void setPos(uint16_t, uint16_t);
		
		directions updatePos();
		
		void enQueue(directions);
		
		void updateDir(directions);

		//Gets image dimensions
		uint16_t getWidth();
		uint16_t getHeight();
		uint16_t gx();
		uint16_t gy();
		SDL_Rect* ghitbox();
		void sx(uint16_t);
		void sy(uint16_t);
		
};

class TexLoader{
		SDL_Texture* mTexture;
		
	public:
		//Initializes variables
		TexLoader();

		//Deallocates memory
		~TexLoader();

		//Loads image at specified path
		bool loadFromFile( std::string path, SDL_Renderer*);
		
		SDL_Texture* getTex();

		//Deallocates texture
		void free();
};

class SnakeHead : public GameObjt{
	public:
		SnakeHead(uint16_t a , uint16_t b, directions c, unsigned char d) : GameObjt(a, b, c, d){};
		void render(SDL_Renderer*, SDL_Rect* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
};

class SnakeSect : public GameObjt{
	public:
		SnakeSect(uint16_t a , uint16_t b, directions c, unsigned char d) : GameObjt(a, b, c, d){};
		void render(SDL_Renderer*, SDL_Rect* = NULL, SDL_RendererFlip = SDL_FLIP_NONE);
};

class TextProcessor {
		SDL_Texture* fonts;
		SDL_Rect getCharPos(char);
	public:
		//Initializes variables
		TextProcessor(SDL_Texture*);

		//Deallocates memory
		~TextProcessor();
		
		//Deallocates texture
		void free();

		//Renders texture at given point
		void render(SDL_Renderer*,uint16_t, uint16_t, std::string, uint8_t = 1);
};