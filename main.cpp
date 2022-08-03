#include <iostream>
#include<SDL2/SDL.h>
#include "display.h"

//just realized i could make invisible rows and columns on the sides for collisions instead of checking the math but eh.
struct shape_position{
	int offset,width,height;
	char arr[16];
};
struct shape{
	shape_position pos[4];
};
shape t_shape = {
		0,3,3,{
				0,0,0,0,
				1,1,1,0,
				0,1,0,0,
				0,0,0,0},
		0,2,3,{
				0,1,0,0,
				1,1,0,0,
				0,1,0,0,
				0,0,0,0},
		0,3,2,{
				0,1,0,0,
				1,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,3,{
				0,1,0,0,
				0,1,1,0,
				0,1,0,0,
				0,0,0,0}
};
shape z_shape = {
		0,3,2,{
				1,1,0,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0},
		0,2,3,{
				0,1,0,0,
				1,1,0,0,
				1,0,0,0,
				0,0,0,0},
		0,3,2,{
				1,1,0,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0},
		0,2,3,{
				0,1,0,0,
				1,1,0,0,
				1,0,0,0,
				0,0,0,0}
};
shape s_shape = {
		0,3,2,{
				0,1,1,0,
				1,1,0,0,
				0,0,0,0,
				0,0,0,0},
		0,2,3,{
				1,0,0,0,
				1,1,0,0,
				0,1,0,0,
				0,0,0,0},
		0,3,2,{
				0,1,1,0,
				1,1,0,0,
				0,0,0,0,
				0,0,0,0},
		0,2,3,{
				1,0,0,0,
				1,1,0,0,
				0,1,0,0,
				0,0,0,0},
};
shape square_shape = {
		1,3,2,{
				0,1,1,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,2,{
				0,1,1,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,2,{
				0,1,1,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,2,{
				0,1,1,0,
				0,1,1,0,
				0,0,0,0,
				0,0,0,0}
};
shape l_shape = {
		0,3,3,{
				0,0,0,0,
				1,1,1,0,
				1,0,0,0,
				0,0,0,0},
		0,2,3,{
				1,1,0,0,
				0,1,0,0,
				0,1,0,0,
				0,0,0,0},
		0,3,2,{
				0,0,1,0,
				1,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,3,{
				0,1,0,0,
				0,1,0,0,
				0,1,1,0,
				0,0,0,0}
};
shape j_shape = {
		0,3,3,{
				0,0,0,0,
				1,1,1,0,
				0,0,1,0,
				0,0,0,0},
		0,2,3,{
				0,1,0,0,
				0,1,0,0,
				1,1,0,0,
				0,0,0,0},
		0,3,2,{
				1,0,0,0,
				1,1,1,0,
				0,0,0,0,
				0,0,0,0},
		1,3,3,{
				0,1,1,0,
				0,1,0,0,
				0,1,0,0,
				0,0,0,0}
};
shape line_shape = {
		0,4,2,{
				0,0,0,0,
				1,1,1,1,
				0,0,0,0,
				0,0,0,0},
		1,2,4,{
				0,1,0,0,
				0,1,0,0,
				0,1,0,0,
				0,1,0,0},
		0,4,2,{
				0,0,0,0,
				1,1,1,1,
				0,0,0,0,
				0,0,0,0},
		1,2,4,{
				0,1,0,0,
				0,1,0,0,
				0,1,0,0,
				0,1,0,0}
};


shape shapes[7]{t_shape,z_shape,s_shape,square_shape,l_shape,line_shape,j_shape};

unsigned int colors[7]={0xFF690000,0xFCB90000,0x00D08400,0x8ED1FC00,0xEB144C00,0xF78DA700,0x9900EF00};

struct tetris_game{
	unsigned int* board;
	unsigned int* overlay;
	int falling_x,falling_y,falling_shape,rot;
	int score;
	int next_blocks[5];
};
tetris_game* new_game(){
	auto* game= (tetris_game *)(malloc(sizeof(tetris_game)));
	game->board= (unsigned int*)(calloc(10 * 22, sizeof(int)));
	game->overlay= (unsigned int*)(calloc(10 * 22, sizeof(int)));
	game->falling_x=3;
	game->falling_y=0;
	game->rot=0;
	game->falling_shape=rand()%7;
	game->score=0;
	return game;
}
void delete_game(tetris_game* game){
	free(game->board);
	free(game->overlay);
	free(game);
}

void get_display(tetris_game* game, unsigned int* disp){
	memcpy(disp,game->board,10*22* sizeof(int));
	memset(game->overlay,0,10*22* sizeof(int));
	for (int i = 0; i < 16; ++i) {
		if(shapes[game->falling_shape].pos[game->rot].arr[i]){
			game->overlay[(game->falling_y+i/4)*10+(game->falling_x+i%4)]=colors[game->falling_shape];
		}
	}
	for (int i = 0; i < 10*22; ++i) {
		if(game->overlay[i]){
			disp[i]=game->overlay[i];
		}
	}
}
bool game_check_legal(tetris_game* game){
	for (int i = 0; i < 16; ++i) {
		if(shapes[game->falling_shape].pos[game->rot].arr[i]){
			if(game->board[(game->falling_y+i/4)*10+(game->falling_x+i%4)]){
				return false;
			}
		}
	}
	return true;
}
void game_left(tetris_game* game){
	int old_x=game->falling_x;
	if(game->falling_x>0-shapes[game->falling_shape].pos[game->rot].offset){
		game->falling_x--;
	}
	if(!game_check_legal(game)){
		game->falling_x=old_x;
	}
}
void game_right(tetris_game* game){
	int old_x=game->falling_x;
	if(game->falling_x<10-shapes[game->falling_shape].pos[game->rot].width){
		game->falling_x++;
	}
	if(!game_check_legal(game)){
		game->falling_x=old_x;
	}
}
void game_rotate(tetris_game* game){
	int old_rot=game->rot;
	game->rot=(game->rot+1)%4;
	//check the edges
	if(game->falling_x+shapes[game->falling_shape].pos[game->rot].width>10){
		game->falling_x=10-shapes[game->falling_shape].pos[game->rot].width;
	}
	if(game->falling_x+shapes[game->falling_shape].pos[game->rot].offset<0){
		game->falling_x=0;
	}
	//check legality
	if(!game_check_legal(game)){
		//check if we can make it work by moving to the sides
		game->falling_x--;
		if(!game_check_legal(game)){
			game->falling_x+=2;
			if(!game_check_legal(game)){
				//undo if all fails
				game->falling_x--;
				game->rot=old_rot;
			}
		}
	}
}
void next_block(tetris_game* game){
	game->falling_x=3;
	game->falling_y=0;
	game->rot=0;
	game->falling_shape=rand()%7;
	if(!game_check_legal(game)){
		exit(0);
	}
}
void placedown_block(tetris_game* game){
	int scores[5]={0,40,100,300,1200};
	for (int i = 0; i < 16; ++i) {
		if(shapes[game->falling_shape].pos[game->rot].arr[i]){
			game->board[(game->falling_y+i/4)*10+(game->falling_x+i%4)]=colors[game->falling_shape];
		}
	}
	int lines=0;
	for (int i = 0; i < shapes[game->falling_shape].pos[game->rot].height; ++i) {
		//printf("%u\n",i);
		bool clear=true;
		for (int j = 0; j < 10; ++j) {
			if(!game->board[(game->falling_y+i)*10+j]){
				clear=false;
			}
		}
		if (clear){
			lines++;
			//printf("clear\n");
			memmove(&game->board[10],game->board,(game->falling_y+i)*10* sizeof(int));
			memset(game->board,0,10*sizeof(int));
		}
	}
	game->score+=scores[lines];
}
void down_block(tetris_game* game){
	if(game->falling_y+shapes[game->falling_shape].pos[game->rot].height>=22){
		placedown_block(game);
		next_block(game);
	}
	else{
		game->falling_y++;
		if(!game_check_legal(game)){
			game->falling_y--;
			placedown_block(game);
			next_block(game);
		}
	}
}

int main() {
	srand(time(0));
	//look into this later
	/*
	 int x[7]={};
	for (int i = 0; i < 100000; ++i) {
		x[rand()%7]++;
	}
	for (int i = 0; i < 7; ++i) {
		printf("  %i: %i",i,x[i]);
	}*/

	std::cout << "Hello, World!" << std::endl;

	auto *screen=new display;

	SDL_Event e;
	bool running = true;

	tetris_game* game=new_game();
	auto* disp= (unsigned int*)(calloc(10 * 22, sizeof(int)));
	unsigned long last_down=SDL_GetTicks64();
	while (running){
		while (SDL_PollEvent(&e)){
			if (e.type == SDL_QUIT){
				running = false;
			}
			if (e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym) {
					case SDLK_w:
						game_rotate(game);
						break;
					case SDLK_a:
						game_left(game);
						break;
					case SDLK_s:
						last_down=SDL_GetTicks64();
						down_block(game);
						break;
					case SDLK_d:
						game_right(game);
						break;
				}

			}
		}
		if(SDL_GetTicks64()-last_down>1000){
			last_down=SDL_GetTicks64();
			down_block(game);

		}
		get_display(game, disp);
		screen->flush_data(disp,game->score);
		screen->update();
		SDL_Delay(1);
	}
	delete screen;
	free(disp);
	delete_game(game);


	return 0;
}
