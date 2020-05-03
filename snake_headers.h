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

class GameObjt
{
		uint16_t x, y, mWidth, mHeight, vel, id;
		directions headDir, drs[8];
		SDL_Texture* mTexture;
	public:
		//Initializes variables
		GameObjt(uint16_t, uint16_t, directions, unsigned char);

		//Deallocates memory
		~GameObjt();

		//Loads image at specified path
		bool loadFromFile( std::string path );

		//Deallocates texture
		void free();

		//Renders texture at given point
		void render(SDL_Renderer*);
		
		void setPos(uint16_t, uint16_t);
		
		directions updatePos();
		
		void enQueue(directions);
		
		void updateDir(directions);

		//Gets image dimensions
		uint16_t getWidth();
		uint16_t getHeight();
		uint16_t gx();
		uint16_t gy();
		void sx(uint16_t);
		void sy(uint16_t);
		
};