#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <unistd.h>
      
#define CELL_SIZE 40

typedef struct snakeElement{
    int x,y;
    struct snakeElement *next;
}snakeElement;

snakeElement snakePart={.x=40, .y=40};

//typedef struct snake{
//    snakeElement *head;
//    snakeElement *tail;
//}snake;

typedef struct{
    int x,y;
}Apple;

typedef struct{
    int dx;
    int dy;
}Direction;

Direction direction={+1,0};

void draw_grid(SDL_Surface *surface, int size, Uint32 color)
{
    SDL_Rect row;
    for(int y =0; y<=20;y++){
        for(int x=0;x<800;x++){
            row=(SDL_Rect){x, y*40, 1, 1};
            SDL_FillSurfaceRect(surface, &row, color);
        }
    }

    SDL_Rect col;
    for(int x = 0; x<=20;x++){
        for(int y= 0;y<800;y++){
            col=(SDL_Rect){x*40, y, 1, 1};
            SDL_FillSurfaceRect(surface, &col, color);
        }
    }

}

void draw_snake(SDL_Surface *surface, snakeElement *snake, Uint32 color)
{
    SDL_Rect rect={snake->x, snake->y, 40, 40};
    SDL_FillSurfaceRect(surface,&rect, color);
}

static inline void move_snake(SDL_Surface *surface, snakeElement *snake, Direction *direction)
{
    snake->x+=(direction->dx*CELL_SIZE);
    snake->y+=(direction->dy*CELL_SIZE);
}

static inline void clear_screen(SDL_Surface *surface, Uint32 color){
    SDL_FillSurfaceRect(surface,NULL, color);
}

void draw(SDL_Surface *surface, Apple *apple, Uint32 color){
    SDL_Rect rect = {apple->x, apple->y};
    SDL_FillSurfaceRect(surface, &rect, color);
}

int main(int argc, char *argv[]) {

    SDL_Window *window = SDL_CreateWindow("PPM Viewer", 800, 800, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    const SDL_PixelFormatDetails *fmt =
      SDL_GetPixelFormatDetails(surface->format);

    Uint32 white = SDL_MapRGBA(fmt, NULL, 255, 255, 255, 255);
    Uint32 black = SDL_MapRGBA(fmt, NULL, 0, 0, 0, 255);
    Uint32 red = SDL_MapRGBA(fmt, NULL, 255, 0, 0, 255);

    int isRunning = 1;

    while (isRunning) {

        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            direction = (Direction){direction.dx,direction.dy};
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = 0;
            }
            if (event.type == SDL_EVENT_KEY_DOWN){
                if(event.key.key==SDLK_RIGHT){
                    if(direction.dx == -1){
                        direction.dx = -1;
                    }else{
                        direction.dx=+1;
                        direction.dy=0;
                    }
                    
                }
                if(event.key.key==SDLK_LEFT){
                    if(direction.dx == 1){
                        direction.dx = +1;
                    }else{
                        direction.dx=-1;
                        direction.dy=0;
                    }
                    
                }
                if(event.key.key==SDLK_UP){
                    if(direction.dy == 1){
                        direction.dy = +1;
                    }else{
                        direction.dy=-1;
                        direction.dx=0;
                    }
                }
                if(event.key.key==SDLK_DOWN){
                    if(direction.dy == -1){
                        direction.dy = +1;
                    }else{
                        direction.dy=+1;
                        direction.dx=0;
                    }
                }
            }
        }

        clear_screen(surface, black);
        draw_grid(surface, CELL_SIZE, white);
        move_snake(surface, &snakePart, &direction);
        draw_snake(surface, &snakePart, white);
        SDL_UpdateWindowSurface(window); 
        SDL_Delay(250);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}