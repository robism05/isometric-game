// environment game in c
// compile: clang main.c -o main -lm -SDL2 -lSDL2_image -lSDL2_mixer
// date: Mon 3/4/2023


#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
//#include <SDL2/SDL_ttf.h>

#define MAP_W 400
#define MAP_H 300
#define BLOCK_SIZE 32
#define SCALE 2

// scale up game things
#define SCREEN_WIDTH (MAP_W * SCALE)
#define SCREEN_HEIGHT (MAP_H * SCALE)
const int moveX = (BLOCK_SIZE * SCALE);
const int moveY = (BLOCK_SIZE * SCALE) / 2;

// TODO: add a zoom feature
float zoomLevel = 1.0;
int playerSize = (64 * SCALE);


// define colors for terminal
// RED for error GREEN for working, etc
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"


// Managing our window with structure
struct {
    SDL_Window *window;
    SDL_Texture *texture;
    SDL_Surface *surface;
    SDL_Renderer *renderer;

    SDL_Event event;
    int ticks;
    bool quit;
} game;


// player animation structure
#define MAX_FRAMES 10
typedef struct {
    SDL_Texture* frames[MAX_FRAMES]; // Array of textures for each frame
    int numFrames; // Number of frames in the animation
    int currentFrame; // Current frame of the animation
    int frameWidth; // Width of each frame
    int frameHeight; // Height of each frame
    int frameDuration; // Duration of each frame in milliseconds
    int elapsedTime; // Elapsed time since last frame change
} PlayerAnimation;


// function prototypes
// TODO: add camera tracking so the player doesnt go off screen

//void handleInput(SDL_Rect* playerRect);
//void updateBackground(SDL_Rect* playerRect, SDL_Rect* backgroundRect);
PlayerAnimation LoadIdleAnimation(SDL_Renderer* renderer);
PlayerAnimation LoadRunAnimation(SDL_Renderer* renderer);
PlayerAnimation LoadJumpAnimation(SDL_Renderer* renderer);

void UpdatePlayerAnimation(PlayerAnimation* animation, int deltaTime);
void RenderPlayerAnimation(PlayerAnimation* animation, SDL_Renderer* renderer, int x, int y);


// Example loading function for a player idle animation with multiple frames
PlayerAnimation LoadIdleAnimation(SDL_Renderer* renderer) 
{
    PlayerAnimation idleAnimation;
    idleAnimation.numFrames = 6;
    idleAnimation.currentFrame = 0;
    idleAnimation.frameWidth = playerSize;
    idleAnimation.frameHeight = playerSize;
    idleAnimation.frameDuration = 200; // 200ms per frame
    idleAnimation.elapsedTime = 0;

    // Load textures for each frame
    for (int i = 0; i < idleAnimation.numFrames; i++) {
        char filename[60];
        sprintf(filename, "../res/textures/animation/player_idle/player-idle%d.png", i + 1);
        idleAnimation.frames[i] = IMG_LoadTexture(game.renderer, filename);
    }

    return idleAnimation;
}

// function for loading run animation when player runs
PlayerAnimation LoadRunAnimation(SDL_Renderer* renderer)
{
    PlayerAnimation runAnimation;
    runAnimation.numFrames = 3;
    runAnimation.currentFrame = 0;
    runAnimation.frameWidth = playerSize;
    runAnimation.frameHeight = playerSize;
    runAnimation.frameDuration = 200;
    runAnimation.elapsedTime = 0;

    for (int i = 0; i < runAnimation.numFrames; i++){
        char filename[60];
        sprintf(filename, "../res/textures/animation/player_run/player-run%d.png", i + 1);
        runAnimation.frames[i] = IMG_LoadTexture(game.renderer, filename);
    }
    return runAnimation;
}

PlayerAnimation LoadJumpAnimation(SDL_Renderer* renderer)
{
    PlayerAnimation jumpAnimation;
    jumpAnimation.numFrames = 3;
    jumpAnimation.currentFrame = 0;
    jumpAnimation.frameWidth = playerSize;
    jumpAnimation.frameHeight = playerSize;
    jumpAnimation.frameDuration = 500;
    jumpAnimation.elapsedTime = 0;

    for (int i = 0; i < jumpAnimation.numFrames; i++){
        char filename[60];
        sprintf(filename, "../res/textures/animation/player_jump/player-jump%d.png", i + 1);
        jumpAnimation.frames[i] = IMG_LoadTexture(game.renderer, filename);
    }
    return jumpAnimation;
}

void UpdatePlayerAnimation(PlayerAnimation* animation, int deltaTime) 
{
    animation->elapsedTime += deltaTime;
    if (animation->elapsedTime >= animation->frameDuration) {
        animation->elapsedTime = 0;
        animation->currentFrame = (animation->currentFrame + 1) % animation->numFrames;
    }
}

void RenderPlayerAnimation(PlayerAnimation* animation, SDL_Renderer* renderer, int x, int y) 
{
    SDL_Rect srcRect = { 0, 0, animation->frameWidth, animation->frameHeight };
    SDL_Rect destRect = { x, y, animation->frameWidth, animation->frameHeight };
    SDL_RenderCopy(renderer, animation->frames[animation->currentFrame], &srcRect, &destRect);
}


// Don't forget to free the loaded textures when they are no longer needed
void FreePlayerAnimation(PlayerAnimation* animation) 
{
    for (int i = 0; i < animation->numFrames; i++) {
        SDL_DestroyTexture(animation->frames[i]);
    }
}


// Function to set audio specification based on audio file properties
void setAudioSpec(SDL_AudioSpec* spec, int freq, Uint16 format, Uint8 channels, int samples) 
{
    // set audio specifications for each file
    spec->freq = freq;
    spec->format = format;
    spec->channels = channels;
    spec->samples = samples;
    spec->callback = NULL;
}

/*
void loadMusicFile(Mix_Music* audio_track, const char filename)
{
    // Mix_Music is for longer tracks, typically music

}

void loadSoundEffect(Mix_Chunk* sound_effect, const char filename)
{
    // Mix_Chunk is for short sound effects
    sound_effect = Mix_LoadWAV(filename);
    if (sound_effect == NULL) {
        // handle .wav file loading failure
        fprintf(stderr,"File might be corrupt or does not exist at this path: %s\n",SDL_GetError());
    }

    // play sound effect
    int channel = Mix_PlayChannel(-1, sound_effect, 0);
    Mix_PlayChannel(-1, sound_effect, 0);

    Mix_FreeChunk(sound_effect);
    Mix_CloseAudio();
}
*/

void playSoundEffect(Mix_Chunk* sound_effect)
{
    if (sound_effect == NULL){
        fprintf(stderr,"Sound file is corrupt or does not exist: %s\n",SDL_GetError());
    }
    Mix_PlayChannel(-1, sound_effect, 0);
}

// main game code
int main(int argc, char *argv[])
{    
    game.quit = false;
    game.ticks = 0;


    // initialize SDL2
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        fprintf(stderr,"SDL2 could not be initialized! SDL_Error: %s\n",SDL_GetError());
        exit(1);      
    }
    if (IMG_Init(IMG_INIT_PNG) < 0){
        fprintf(stderr,"SDL_image failed to load: %s\n",IMG_GetError());
        exit(1);
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
        fprintf(stderr,"Mix_OpenAudio failed to load: %s\n",SDL_GetError());
    }

    // create window and give context
    game.window = SDL_CreateWindow(
        "Isometric Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_ALLOW_HIGHDPI); // | SDL_WINDOW_FULLSCREEN);
    if (!game.window){ // error checking
        fprintf(stderr,"Failed to open window: %s\n",SDL_GetError()); 
        exit(1);
    }
    
    // create le renderer and give context
    game.renderer = SDL_CreateRenderer(
        game.window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (!game.renderer){
        fprintf(stderr,"Failed to create renderer: %s\n",SDL_GetError());
        exit(1);
    }


    // player position
    int playerX = BLOCK_SIZE * SCALE;
    int playerY = BLOCK_SIZE * SCALE;

    // load player animations
    PlayerAnimation idleAnimation = LoadIdleAnimation(game.renderer);
    PlayerAnimation runAnimation = LoadRunAnimation(game.renderer);
    PlayerAnimation jumpAnimation = LoadJumpAnimation(game.renderer);

    bool isJumping, superJump;
    Uint32 jumpStartTime;
    int jump = 0;
 

    // Define a generic audio specification
    SDL_AudioSpec beep_spec;
    setAudioSpec(&beep_spec,44100,MIX_DEFAULT_FORMAT,2,2048);
    
    // sound effects
    Mix_Chunk* beep_sound = Mix_LoadWAV("../res/sfx/effects/beep.wav");
    Mix_Chunk* jump_sound = Mix_LoadWAV("../res/sfx/effects/jump.wav");
    Mix_Chunk* superJump_sound = Mix_LoadWAV("../res/sfx/effects/super_jump.wav");
    Mix_Chunk* grass_sound = Mix_LoadWAV("../res/sfx/effects/grass_0.wav");
    

    Uint32 prevTime = SDL_GetTicks();
    while(game.quit == false){
       
        // update times and stuff
        Uint32 currentTime = SDL_GetTicks(); // Get current time
        int deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        UpdatePlayerAnimation(&idleAnimation, deltaTime);
        
        // set green background
        SDL_SetRenderDrawColor(game.renderer, 113,170,52, 255);
        SDL_RenderClear(game.renderer);

        // set grid background
        SDL_Rect gridRect = {0,0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2};
        SDL_Surface* S_grid = IMG_Load("../res/textures/grid.png");
        SDL_Texture* grid = SDL_CreateTextureFromSurface(game.renderer, S_grid);
        
        SDL_FreeSurface(S_grid);
        SDL_RenderCopy(game.renderer, grid, NULL, &gridRect);
       
        /*
        // create player and manage position
        SDL_Rect playerRect = { playerX,playerY, playerSize,playerSize }; 
        SDL_Surface* S_player = IMG_Load("../res/textures/player.png");
        if (S_player == NULL){
            fprintf(stderr,"Image may be corrupt or does not exist: %s\n",SDL_GetError());
            exit(1);
        }
        SDL_Texture* player = SDL_CreateTextureFromSurface(game.renderer, S_player);
        */

        // render animation
        


        // game events
        while(SDL_PollEvent(&game.event)){
            switch (game.event.type){
            case SDL_QUIT:
                game.quit = true;

            // gets current key pressed down
            case SDL_KEYDOWN:
                switch (game.event.key.keysym.sym){
                case SDLK_LEFT: 
                    playerX -= moveX * 2;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case 'a':
                    playerX -= moveX;
                    playerY += moveY;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case SDLK_RIGHT: 
                    playerX += moveX * 2;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case 'd':
                    playerX += moveX;
                    playerY -= moveY;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case SDLK_UP: 
                    playerY -= moveY * 2;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case 's':
                    playerY -= moveY;
                    playerX -= moveX;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case SDLK_DOWN: 
                    playerY += moveY * 2;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case 'w':
                    playerY += moveY;
                    playerX += moveX;
                    jump = 0;

                    playSoundEffect(grass_sound);
                    break;
                case SDLK_SPACE: 
                    if (!isJumping){
                        if (jump == 3){
                            isJumping = true;
                            superJump = true;
                            jumpStartTime = SDL_GetTicks();
                            jump = 0;
                            playSoundEffect(superJump_sound);
                        } else {
                            isJumping = true;
                            jumpStartTime = SDL_GetTicks();
                            jump++;
                            playSoundEffect(jump_sound);
                        }
                    }
                    break;
                case SDLK_ESCAPE:
                    printf("Esc: %d\n",SDLK_ESCAPE);
                    break;
                case 'c':
                    printf("playerX: %d\nplayerY: %d\n",playerX,playerY);
                    break;
                }
            // gets mouse scroll                
            case SDL_MOUSEWHEEL:
                switch (game.event.wheel.y){
                case 1:
                    zoomLevel += 0.5;
                    //printf("Zoom in: %f\n",zoomLevel);
                    break;
                case -1:
                    zoomLevel -= 0.5;
                    //printf("Zoom out: %f\n",zoomLevel);
                    if (zoomLevel < 0.1){
                        zoomLevel = 0.1;
                    }
                    break;
                } 
            }
        }


        // Update player position
        if (isJumping) {
            Uint32 timePassed = jumpStartTime - currentTime; // Calculate time passed since jump start
            int jumpDuration = 100;

            UpdatePlayerAnimation(&jumpAnimation, timePassed);

            // Update player Y position based on jump height and time passed
            if (superJump){
                playerY -= (moveY * 2) * 2;
            } else {
                playerY -= moveY;
            }


            // Check if jump is complete
            if (timePassed >= jumpDuration) {
                isJumping = false; // Reset jump flag
                superJump = false;
                playerY += moveY + moveY;
                RenderPlayerAnimation(&jumpAnimation, game.renderer, playerX, playerY);
            }

        } else {
            RenderPlayerAnimation(&idleAnimation, game.renderer, playerX, playerY);
        }

        /*
        SDL_FreeSurface(S_player);
        SDL_RenderCopy(game.renderer, player, NULL, &playerRect);
        */


        // Update Screen
        SDL_RenderPresent(game.renderer); 
 
    }

    // free resources from memory
    FreePlayerAnimation(&idleAnimation);

    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyTexture(game.texture);
    SDL_DestroyWindow(game.window);
    
    IMG_Quit();
    SDL_Quit();
    return 0;
}


// end of code
// finish date: 
