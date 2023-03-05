

     //SDL_Texture *screen=NULL;
  //   SDL_Texture *back[2]={NULL,NULL};       //Holds three background..
//     SDL_Texture *upback=NULL;
//     SDL_Texture *bike=NULL;
//     SDL_Texture *obs_wall[2]={NULL,NULL};
//     SDL_Texture *menu_pic=NULL;
//     SDL_Texture *play_menu=NULL;
//     SDL_Texture *instruction_menu=NULL;
//     SDL_Texture *quit_menu=NULL;

    SDL_Texture* back[2] = { NULL, NULL };
    SDL_Texture *upback=NULL;
     SDL_Texture *bike=NULL;
     SDL_Texture *obs_wall[2]={NULL,NULL};
     SDL_Texture *menu_pic=NULL;
     SDL_Texture *play_menu=NULL;
     SDL_Texture *instruction_menu=NULL;
     SDL_Texture *quit_menu=NULL;
    SDL_Window* screen;
     SDL_Renderer* renderer;

     TTF_Font *font=NULL;

     /*SDL_Texture *times=NULL;
     SDL_Texture *time_limit=NULL;
     SDL_Texture *die=NULL;*/
     
     SDL_Texture *times=NULL;
     SDL_Texture *time_limit=NULL;
     SDL_Texture *die=NULL;
     SDL_Texture *win=NULL;


     SDL_Color textColor={225,0,0};
     SDL_Color textColor1={0,255,0};
     SDL_Event event;                             //For checking the event happen...
     const Uint8 *keystate = SDL_GetKeyboardState(NULL);       //for check key event keys..

//Variable for background...
     background bg[2];

//Variable for wall...
     wall w1[SMAX_WALL];
     wall w2[BMAX_WALL];

//Game states..
     int game_on=1;
