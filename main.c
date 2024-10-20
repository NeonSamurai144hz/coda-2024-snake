#include "snake.h"

#define TILE_SIZE 16


SnakeSegment snake[50];
int lines;

char *get_file(FILE *fp)
{
    char *content;
    char buffer[10];

    content = malloc(sizeof(char));
    content[0] = '\0';

    while (fgets(buffer, 10, fp) != NULL)
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

    while (str[i] != '\0')
    {
        if (str[i] == '\n')
        {
            lines++;
        }
        i++;
    }

    return lines;
}

void render_map(SDL_Renderer *renderer, SDL_Texture *snakeTexture, char **tab, int lines)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < 54; j++)
        {
            if (tab[i][j] == '#')
            {
                SDL_Rect rect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_Rect rect_img = {0, 0, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, snakeTexture, &rect_img, &rect);
            }
            else if (tab[i][j] == 'F')
            {
                SDL_Rect rect = {j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE};
                SDL_Rect rect_img = {16, 0, TILE_SIZE, TILE_SIZE};
                SDL_RenderCopy(renderer, snakeTexture, &rect_img, &rect);
            }
        }
    }
}

void render_snake(SDL_Renderer *renderer, SDL_Texture *snakeTexture, SDL_Rect *srcRects)
{
    for (int i = 0; i < snakeLength; i++)
    {
        SDL_Rect snakeSegmentRect = {snake[i].x, snake[i].y, TILE_SIZE, TILE_SIZE};

        if (i == 0)
        {
            SDL_RenderCopy(renderer, snakeTexture, &srcRects[3], &snakeSegmentRect);
        } else
        {
            SDL_RenderCopy(renderer, snakeTexture, &srcRects[2], &snakeSegmentRect);
        }
    }
}

void place_food(char **tab, int lines)
{
    int x, y;
    do {
        x = rand() % 54;
        y = rand() % lines;
    } while (tab[y][x] != ' ');

    tab[y][x] = 'F';
}

int check_collision_with_walls(SnakeSegment *snake, char **tab)
{
    int x = snake->x / TILE_SIZE;
    int y = snake->y / TILE_SIZE;

    return tab[y][x] == '#';
}

int check_collision_with_food(SnakeSegment *snake, char **tab)
{
    int x = snake->x / TILE_SIZE;
    int y = snake->y / TILE_SIZE;

    if (tab[y][x] == 'F')
    {
        tab[y][x] = ' ';
        return 1;
    }
    return 0;
}

int check_collision_with_self(SnakeSegment *head)
{
    for (int i = 1; i < snakeLength; i++)
    {
        if (head->x == snake[i].x && head->y == snake[i].y)
        {
            return 1;
        }
    }
    return 0;
}
void grow_snake()
{
    snakeLength++;
    snake[snakeLength - 1].x = snake[snakeLength - 2].x;
    snake[snakeLength - 1].y = snake[snakeLength - 2].y;
}

void update_snake_position(SnakeSegment *snake)
{
    for (int i = snakeLength - 1; i > 0; i--)
    {
        snake[i] = snake[i - 1];
    }
}

int main(int argc, char *argv[])
{
    FILE *fp = fopen("map.txt", "r");
    char *file_content;
    char **tab;
    char *line;
    int i = 0;
    int fruitsEaten = 0;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 32768) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        SDL_Quit();
        return -1;
    }

    Mix_Music *backgroundMusic = Mix_LoadMUS("game_sound.mp3");
    Mix_PlayMusic(backgroundMusic, -1);

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

    while ((line != NULL) && (i < lines))
    {
        tab[i] = malloc((strlen(line) + 1) * sizeof(char));
        strcpy(tab[i], line);
        i++;
        line = strtok(NULL, "\n");
    }

    SDL_Window *window = SDL_CreateWindow("SNAKEEEEEE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface *snakeSurface = IMG_Load("snake_levels_final.png");
    SDL_Texture *snakeTexture = SDL_CreateTextureFromSurface(renderer, snakeSurface);
    SDL_FreeSurface(snakeSurface);

    snake[0].x = 424;
    snake[0].y = 272;

    place_food(tab, lines);

    int quit = 0;
    SDL_Event event;
    int direction = 0;
    Uint32 startTime = 0;
    const int frameDelay = 150;

    while (!quit)
    {
        startTime = SDL_GetTicks();

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
                    case SDLK_w: direction = 0; break;
                    case SDLK_s: direction = 1; break;
                    case SDLK_a: direction = 2; break;
                    case SDLK_d: direction = 3; break;
                }
            }
        }

        // Move the snake
        update_snake_position(snake);
        switch (direction)
        {
            case 0: snake[0].y -= SNAKE_SIZE; break;
            case 1: snake[0].y += SNAKE_SIZE; break;
            case 2: snake[0].x -= SNAKE_SIZE; break;
            case 3: snake[0].x += SNAKE_SIZE; break;
        }

        if (check_collision_with_walls(&snake[0], tab))
        {
            printf("Game Over! You hit a wall.\n");
            quit = 1;
        }

        if (check_collision_with_self(&snake[0]))
        {
            printf("Game Over! You collided with yourself.\n");
            quit = 1;
        }

        if (check_collision_with_food(&snake[0], tab))
        {
            grow_snake();
            fruitsEaten++;

            if (fruitsEaten >= 50)
            {
                printf("You win! You ate 50 fruits!\n");
                quit = 1;
            }

            place_food(tab, lines);
        }

        SDL_RenderClear(renderer);
        render_map(renderer, snakeTexture, tab, lines);

        // Render the snake
        for (int i = 0; i < snakeLength; i++)
        {
            SDL_Rect snakeRect;
            if (i == 0) {
                // Render snake head
                snakeRect = (SDL_Rect){snake[i].x, snake[i].y, SNAKE_SIZE, SNAKE_SIZE};
                SDL_RenderCopy(renderer, snakeTexture, &(SDL_Rect){48, 0, SNAKE_SIZE, SNAKE_SIZE}, &snakeRect);
            } else {
                // Render snake body
                snakeRect = (SDL_Rect){snake[i].x, snake[i].y, SNAKE_SIZE, SNAKE_SIZE};
                SDL_RenderCopy(renderer, snakeTexture, &(SDL_Rect){32, 0, SNAKE_SIZE, SNAKE_SIZE}, &snakeRect);
            }
        }

        SDL_RenderPresent(renderer);

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