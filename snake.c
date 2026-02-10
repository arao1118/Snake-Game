#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <unistd.h>
      
#define CELL_SIZE 30

typedef struct snakeElement{
    int x,y;
    struct snakeElement *next;
}snakeElement;

snakeElement snakeHead={30, 30, NULL};

//snakeElement *phead = &snakeHead;
//snakeElement *ptail = &snakeHead;

//typedef struct snake{
//    snakeElement **pphead;
//    snakeElement **pptail;
//}snake;

typedef struct {
    snakeElement *head;
    snakeElement *tail;
}snake;

snake fullSnake = {&snakeHead, &snakeHead};

typedef struct{
    int x,y;
}Apple;

typedef struct{
    int dx;
    int dy;
}Direction;

Direction direction={+1,0};

static inline void clear_screen(SDL_Surface *surface, Uint32 color){
    SDL_FillSurfaceRect(surface,NULL, color);
}

void draw_grid(SDL_Surface *surface, int size, Uint32 color)
{
    SDL_Rect row;
    for(int y =0; y<=30;y++){
        for(int x=0;x<900;x++){
            row=(SDL_Rect){x, y*30, 1, 1};
            SDL_FillSurfaceRect(surface, &row, color);
        }
    }

    SDL_Rect col;
    for(int x = 0; x<=30;x++){
        for(int y= 0;y<900;y++){
            col=(SDL_Rect){x*30, y, 1, 1};
            SDL_FillSurfaceRect(surface, &col, color);
        }
    }

}

static inline void move_snake(SDL_Surface *surface, snakeElement *snake, Direction *direction)
{
    snake->x+=(direction->dx*CELL_SIZE);
    snake->y+=(direction->dy*CELL_SIZE);
}

void draw_snake(SDL_Surface *surface, snakeElement *snake, Uint32 color)
{
    SDL_Rect rect={snake->x, snake->y, 30, 30};
    if (snake->x >= 900){
        snake->x = 0;
    }
    if (snake->x < 0){
        snake->x = 900 - CELL_SIZE;
    }
    if (snake->y >= 900){
        snake->y = 0;
    }
    if (snake->y < 0){
        snake->y = 900 - CELL_SIZE;
    } 

    printf("%d %d\n",snake->x, snake->y);
    SDL_FillSurfaceRect(surface,&rect, color);
}

//void draw_apple(SDL_Surface *surface, Apple *apple, Uint32 color){
//    SDL_Rect rect = {apple->x, apple->y};
//    SDL_FillSurfaceRect(surface, &rect, color);
//}

//struct snakeElement* lengthen_snake(snakeElement **tail, snake *fullSnake)
//{
//    snakeElement *newPart = (snakeElement *)malloc(sizeof(snakeElement));
//    if (newPart == NULL){
//        return NULL;
//    }
//    if((*(fullSnake-> pphead))->next == NULL){
//        (*(fullSnake-> pphead)) -> next = newPart;
//    }else{
//        (*(fullSnake-> pptail)) -> next = newPart;
//    }
//    *(fullSnake->pptail) = newPart;
//
//}

void lengthen_snake(snake *fullSnake)
{
    snakeElement *newPart = (snakeElement *)malloc(sizeof(snakeElement));

    *(newPart) = (snakeElement){(*(fullSnake->tail)).x, (*(fullSnake->tail)).y, NULL};

    if( (*(fullSnake -> head)).next == NULL){
        (*(fullSnake -> head)).next = newPart;
    }else{
        (*(fullSnake -> tail)).next = newPart;
    }
    (fullSnake -> tail) = newPart;

}

int main(int argc, char *argv[]) {

    SDL_Window *window = SDL_CreateWindow("PPM Viewer", 900, 900, 0);
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
                        direction.dy = -1;
                    }else{
                        direction.dy=+1;
                        direction.dx=0;
                    }
                }
            }
        }

        clear_screen(surface, black);
        draw_grid(surface, CELL_SIZE, white);
        move_snake(surface, &snakeHead, &direction);
        draw_snake(surface, &snakeHead, white);
        SDL_UpdateWindowSurface(window); 
        SDL_Delay(300);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}