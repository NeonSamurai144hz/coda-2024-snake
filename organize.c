
#include "snake.h"

#define TILE_SIZE 16 // Size of each tile in pixels

#define WINDOW_WIDTH 864
#define WINDOW_HEIGHT 560
#define SNAKE_SIZE 16

char * get_file(FILE * fp)
{
    char * content;
    char buffer[10];

    content = malloc(sizeof(char));
    content[0] = '\0';

    while(fgets(buffer, 10, fp) != NULL)
    {
        content = realloc(content, (strlen(buffer) * sizeof(char)) + strlen(content) * sizeof(char) + 1);
        content = strcat(content, buffer);
    }

    return content;
}

int count_lines(char *str)
{
    int i = 0;
    int lines = 0;

    while(str[i] != '\0')
    {
        if(str[i] == '\n')
        {
            lines++;
        }

        i++;
    }

    return lines;
}

void display_tab(char **tab, int lines)
{
    int i = 0;

    while(i < lines)
    {
        printf("%s\n", tab[i]);
        i++;
    }
}

void render_map(SDL_Renderer *renderer, SDL_Texture *snakeTexture, char **tab, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < 54; j++)
        {
            if (tab[i][j] == '#')
            {
                SDL_Rect rect = {j * 16, i * 16, 16, 16};
                SDL_Rect rect_img = {0, 0, 16, 16};
                SDL_RenderCopy(renderer, snakeTexture, &rect_img, &rect);
            }
        }
    }
}

SDL_Rect foodRect;

void place_food() {
    foodRect.x = (rand() % 20) * SNAKE_SIZE;  // Random x-coordinate
    foodRect.y = (rand() % lines) * SNAKE_SIZE;  // Random y-coordinate based on number of map lines
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("map.txt", "r");
    char * file_content;
    char **tab;
    int lines;
    char * line;
    int i = 0;
    int j = 0;

    if (fp == NULL)
    {
        printf("Le fichier map.txt n'a pas pu être ouvert\n");
        return EXIT_FAILURE;
    }

    file_content = get_file(fp);

    fclose(fp);
    
    lines = count_lines(file_content);

    tab = malloc(lines * sizeof(*tab));

    line = strtok(file_content, "\n");
    while((line != NULL) && (i < lines))
    {
        tab[i] = malloc((strlen(line) + 1) * sizeof(char));
        tab[i] = strcpy(tab[i], line);
        i++;

        line = strtok(NULL, "\n");
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("erreur SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("Erreur SDL_IMAGE: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SNAKEEEEEE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if(!window)
    {
        printf("Erreur creation remderer: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer)
    {
        printf("Erreur creation renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Surface *snakeSurface = IMG_Load("snake_levels_final.png");
    if(!snakeSurface)
    {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture *snakeTexture = SDL_CreateTextureFromSurface(renderer, snakeSurface);
    SDL_FreeSurface(snakeSurface);

    SDL_Rect srcRects[4] = {
        {0, 0, SNAKE_SIZE, SNAKE_SIZE}, //BORDER
        {16, 0, SNAKE_SIZE, SNAKE_SIZE}, //FOOD
        {32, 0, SNAKE_SIZE, SNAKE_SIZE}, //snake
        {48, 0, SNAKE_SIZE, SNAKE_SIZE}, // SNeiK
    };

    SDL_Rect snakeRect = {424, 272, SNAKE_SIZE, SNAKE_SIZE};


    while(i < lines)
        {
            SDL_Event e;
            if (SDL_WaitEvent(&e)) 
            {
                if (e.type == SDL_QUIT) 
                {
                    break;
                }
            }
            j = 0;
            while(j < 20)
            {
                if(tab[i][j] == '#')
                {
                    SDL_Rect rect = {j * 16, i * 16, 16, 16};
                    SDL_Rect rect_img = {0,0, 16, 16};
                    SDL_RenderCopy(renderer, snakeTexture, &rect_img, &rect);
                }
                
                j++;
            }

            i++;
        }

    SDL_RenderPresent(renderer);
    int quit = 0;
    SDL_Event event;
    int currentDirection = 0;
    Uint32 startTime = 0;
    const int frameDelay = 10;  // Delay in milliseconds (20 frames per second)

while (!quit)
{
    startTime = SDL_GetTicks();  // Get the time at the beginning of the frame

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            quit = 1;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_w:
                    snakeRect.y -= SNAKE_SIZE;
                    currentDirection = 3;
                    break;
                case SDLK_s:
                    snakeRect.y += SNAKE_SIZE;
                    currentDirection = 3;
                    break;
                case SDLK_a:
                    snakeRect.x -= SNAKE_SIZE;
                    currentDirection = 3;
                    break;
                case SDLK_d:
                    snakeRect.x += SNAKE_SIZE;
                    currentDirection = 3;
                    break;
            }
        }
    }

    SDL_RenderClear(renderer);

    // Render the map
    render_map(renderer, snakeTexture, tab, lines);

    // Render the snake
    SDL_RenderCopy(renderer, snakeTexture, &srcRects[currentDirection], &snakeRect);

    SDL_RenderPresent(renderer);

    // Delay the frame to achieve desired frame rate
    Uint32 frameTime = SDL_GetTicks() - startTime;
    if (frameDelay > frameTime)
    {
        SDL_Delay(frameDelay - frameTime);
    }
}

    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    for (i = 0; i < lines; i++) {
        free(tab[i]);
    }
    free(tab);
    free(file_content);

    return 0;
}