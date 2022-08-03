//
// Created by pat on 5/2/22.
//

#ifndef TETRIS_DISPLAY_H
#define TETRIS_DISPLAY_H
#include<SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <chrono>
#include <string>

#define systime std::chrono::high_resolution_clock::now()
#define microcast(time) (double)std::chrono::duration_cast<std::chrono::microseconds>(time).count()
typedef std::chrono::system_clock::time_point timepoint;

class display {

	SDL_Renderer *renderer;
	timepoint start = systime;
	timepoint last_fps_update = systime;
	timepoint next_frame;
	timepoint last_frame[60];
	double max_fps=60;
	unsigned int *disp;
	int score;
	SDL_Window *window=nullptr;
	TTF_Font *font;

public:
	display(){
		disp= static_cast<unsigned int *>(malloc(10 * 22 * sizeof(int)));
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Stackunderfl0w 8080", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
		TTF_Init();
		font = TTF_OpenFont("PressStart2P.ttf", 24);
		if(!font) {
			printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
		}
	}
	~display(){
		free(disp);
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
	}
	void flush_data(unsigned int* display,int score){
		memcpy(disp,display,10*22* sizeof(int));
		this->score=score;
	}
	void render(){
		SDL_SetRenderDrawColor(renderer, 55, 55, 55, 255);
		SDL_RenderClear(renderer);

		SDL_Rect rect;
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		rect.x=0;
		rect.y=480-20*22;
		rect.w = 10*20;
		rect.h = 22*20;
		SDL_RenderDrawRect(renderer, &rect);

		rect.w = 20;
		rect.h = 20;

		for (int x = 0; x < 10; ++x) {
			for (int y = 0; y < 22; ++y) {
				unsigned int color=disp[y*10+x];
				if(!color)
					continue;
				SDL_SetRenderDrawColor(renderer, (color>>24)&0xff, (color>>16)&0xff, (color>>8)&0xff, 255);

				rect.x=x*20;
				rect.y=(480-20*22)+y*20;
				SDL_RenderFillRect(renderer, &rect);
			}
		}
		char str[256];
		sprintf(str,"Score %i",score);
		SDL_Surface * surface = TTF_RenderText_Solid(font,str, { 255, 255, 255 ,255});
		SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);

		int texW = 0;
		int texH = 0;
		SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
		SDL_Rect dstrect = { 250, 200, texW, texH };

		SDL_RenderCopy(renderer, texture, NULL, &dstrect);		SDL_RenderPresent(renderer);

		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);

	}

	void update(){
		render();
		memmove(&last_frame[1],last_frame,59* sizeof(timepoint));

		last_frame[0]=systime;

		next_frame=last_frame[0]+std::chrono::microseconds(1000000/60);

		double fps =(1000000.0/(microcast(last_frame[0]-last_frame[59]))*60);
		//printf("%f\n",microcast(last_frame[0]-last_frame[59]));
		if(microcast(last_frame[0]-last_frame[59])>1000){
			last_fps_update=systime;
			SDL_SetWindowTitle(window,std::to_string(fps*max_fps/60).c_str());
		}
	}

};


#endif //TETRIS_DISPLAY_H
