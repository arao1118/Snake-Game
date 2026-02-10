#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdlib.h>
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

static inline void move_snake(snakeElement *head, Direction *direction)
{
    if (head == NULL) return;

    int prev_x = head->x;
    int prev_y = head->y;

    head->x += direction->dx * CELL_SIZE;
    head->y += direction->dy * CELL_SIZE;

    if (head->x >= 900) head->x = 0;
    if (head->x < 0) head->x = 900 - CELL_SIZE;
    if (head->y >= 900) head->y = 0;
    if (head->y < 0) head->y = 900 - CELL_SIZE;

    snakeElement *temp = head->next;

    while (temp != NULL)
    {
        int current_x = temp->x;
        int current_y = temp->y;

        temp->x = prev_x;
        temp->y = prev_y;

        prev_x = current_x;
        prev_y = current_y;

        temp = temp->next;
    }
}

void draw_snake(SDL_Surface *surface, snakeElement *snakeHead, Uint32 color)
{
   // SDL_Rect rect={snake->x, snake->y, 30, 30};
    

    //printf("%d %d\n",snake->x, snake->y);
    //SDL_FillSurfaceRect(surface,&rect, color);

    snakeElement *temp = snakeHead;

    while (temp != NULL)
    {
        SDL_Rect rect = { temp->x, temp->y, CELL_SIZE, CELL_SIZE };

        SDL_FillSurfaceRect(surface, &rect, color);

        temp = temp->next;
    } 
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
    int frameCounter = 0;

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

        if (frameCounter % 3 == 0) {
        lengthen_snake(&fullSnake);
        }
    
        move_snake(fullSnake.head, &direction);
        frameCounter++; 

        clear_screen(surface, black);
        draw_grid(surface, CELL_SIZE, white);
  //      move_snake(fullSnake.head, &direction);
   //     lengthen_snake(&fullSnake);
        draw_snake(surface, fullSnake.head, white);
        SDL_UpdateWindowSurface(window); 
        SDL_Delay(300);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}