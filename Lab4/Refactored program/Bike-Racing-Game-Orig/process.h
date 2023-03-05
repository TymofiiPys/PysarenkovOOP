


class bike_process
{
    private:
        int speed;
        int jump_state;     
        int start_time;
        int time_gap;
        int bike_stand;
        int bike_x;
        int bike_y;
    public:

         //bike_process() 
         //{
         //    speed = 0;
         //    jump_state = 0;
         //    bike_stand = LAYER_Y;
         //    bike_x = START_BIKE_X;
         //    int h;
         //    SDL_QueryTexture(bike, NULL, NULL, NULL, &h);
         //    bike_y = LAYER_Y - h;
         //}
         void bpreset() {
             speed = 0;
             jump_state = 0;
             bike_stand = LAYER_Y;
             bike_x = START_BIKE_X;
             int h;
             SDL_QueryTexture(bike, NULL, NULL, NULL, &h);
             bike_y = LAYER_Y - h;
             int w;
             SDL_QueryTexture(obs_wall[0], NULL, NULL, &w, &h);
             w1[0] = wall(WIDTH + 50, LAYER_Y - h, w, h);
             w1[1] = wall(WIDTH + 1350, LAYER_Y - h, w, h);
             w1[2] = wall(WIDTH + 3350, LAYER_Y - h, w, h);
             w1[3] = wall(WIDTH + 5050, LAYER_Y - h, w, h);
             w1[4] = wall(WIDTH + 7100, LAYER_Y - h, w, h);

             SDL_QueryTexture(obs_wall[1], NULL, NULL, &w, &h);
             w2[0] = wall(WIDTH + 2050, LAYER_Y - h, w, h);
             w2[1] = wall(WIDTH + 4350, LAYER_Y - h, w, h);
             w2[2] = wall(WIDTH + 6050, LAYER_Y - h, w, h);
             w2[3] = wall(WIDTH + 7800, LAYER_Y - h, w, h);
             SDL_QueryTexture(back[0], NULL, NULL, &w, &h);
             bg[0] = background(0, 0, w, h);
             SDL_QueryTexture(back[1], NULL, NULL, &w, &h);
             bg[1] = background(WIDTH, 0, w, h);
         }
         void set_start_time() {
             start_time = SDL_GetTicks();
         }
         void set_speed(int k) {
             speed += k;
         }
         int get_speed()
         {
             return speed;
         }
         void set_bike_x() {
             bike_x += speed;
         }
         int get_bike_x() {
             return bike_x;
         }
    void load_layer()
    {

        apply_surface(bg[0].get_cord_x(),0,back[0], renderer);      //Blit the screen;
        apply_surface(bg[1].get_cord_x(),0,back[1], renderer);      //Blit the screen;
        apply_surface(0,0,upback, renderer);
    
        for(int i=0;i<SMAX_WALL;i++)
            if(w1[i].get_state())
                apply_surface(w1[i].get_cord_x(),w1[i].get_cord_y(),obs_wall[0], renderer);
    
        for(int i=0;i<BMAX_WALL;i++)
            if(w2[i].get_state())
                apply_surface(w2[i].get_cord_x(),w2[1].get_cord_y(),obs_wall[1], renderer);

        apply_surface(bike_x,bike_y,bike, renderer);         //bike is blitted..

    }

    bool finished() {
        if ((w2[BMAX_WALL - 1].get_cord_x()) + w2[BMAX_WALL - 1].get_cord_w() < 0)
        {
            return true;
        }
        return false;
    }

    void finish() {
        apply_surface(100, 300, win, renderer);
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);
    }

    void change_coordinate(int );       //Set the coordinate of the labels..

    bool process();

};

bool bike_process::process()
{
    int bike_h, times_h;
    SDL_QueryTexture(bike, NULL, NULL, NULL, &bike_h);
    SDL_QueryTexture(times, NULL, NULL, NULL, &times_h);
    while( SDL_PollEvent(&event) )
    {
        if(event.type==SDL_QUIT)
            return false;
    }

    if(keystate[SDL_SCANCODE_ESCAPE] || game_on == 0 || time_gap >= 90) //return to menu
        return false;
    
    if(keystate[SDL_SCANCODE_SPACE] or keystate[SDL_SCANCODE_UP])//If space or arrow up is pressed...
    {
         if(jump_state == 0) //if we are not in jump already
            jump_state = 36;                       //set the jump or SPACE state in 20*2 steps...
    }
     if( (bike_y <(bike_stand-bike_h)) or jump_state >= 16  )    //Condition true untill the bike goes upward and downward to surface below it..
     {
          int jump_height;
          jump_height = 19 - jump_state;         //using the formula n/2*(n+1)...
          bike_y += jump_height;
          jump_state--;
          if(jump_state == 0)
            jump_state--;
     }
     else
     {
          bike_y = bike_stand-bike_h;
          jump_state = 0; 
     }

    if(keystate[SDL_SCANCODE_RIGHT])
        if(speed<=20)                 //Speed only increase to 20 (in fact, to 10 as while changing coordinates speed is divided by 2..
            speed++;
    if(keystate[SDL_SCANCODE_LEFT])
        if(speed>0)                    //speed cannot be less than 0..
            speed--;

    //cout most info about texture state
     if (keystate[SDL_SCANCODE_A])
     {
         cerr << "speed: " << speed << std::endl;
         cerr << "bike_x: " << bike_x << std::endl;
         cerr << "bike_y: " << bike_y << std::endl;
         cerr << "bike_h: " << bike_h << std::endl;
         cerr << "jump_state: " << jump_state << std::endl;
         cerr << "timegap: " << time_gap << std::endl << std::endl;
         for (int i = 0; i < SMAX_WALL; i++)
         {
             cerr << "w1[" << i << "]: \n";
             cerr << "state: " << w1[i].get_state() << "\n";
             cerr << "x: " << w1[i].get_cord_x() << "\n";
             cerr << "y: " << w1[i].get_cord_y() << "\n";
             cerr << "w: " << w1[i].get_cord_w() << "\n";
             cerr << "h: " << w1[i].get_cord_h() << "\n\n";
         }
         for (int i = 0; i < BMAX_WALL; i++)
         {
             cerr << "w1[" << i << "]: \n";
             cerr << "state: " << w2[i].get_state() << "\n";
             cerr << "x: " << w2[i].get_cord_x() << "\n";
             cerr << "y: " << w2[i].get_cord_y() << "\n";
             cerr << "w: " << w2[i].get_cord_w() << "\n";
             cerr << "h: " << w2[i].get_cord_h() << "\n\n";
         }
         for (int i = 0; i < 2; i++)
         {
             cerr << "bg[" << i << "]: \n";
             cerr << "x: " << bg[i].get_cord_x() << "\n";
             cerr << "y: " << bg[i].get_cord_y() << "\n";
             cerr << "w: " << bg[i].get_cord_w() << "\n";
             cerr << "h: " << bg[i].get_cord_h() << "\n\n";
         }
     }

    if(bike_x < FINISH_BIKE_X) //at the beginning bike's texture moves around the screen
        bike_x+=speed;
    else//and afterwards its texture remains static but everything moves instead
        change_coordinate(-speed/2);
    change_coordinate(0);//background moves regardless of speed
    SDL_RenderClear(renderer);
    load_layer();
    if(finished())
    {
        finish();
        return false;
    }

    std::stringstream time;
    //Convert the timer's time to a string
    time_gap =(SDL_GetTicks() - start_time)/1000;
    int mm=time_gap/60;
    int ss=time_gap%60;
    if(ss < 10)
        time << "Timer: " << mm << ":0" << ss;
    else
        time << "Timer: " << mm << ":" << ss;
     times = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font,time.str().c_str(),textColor));
     apply_surface(600, 50, times, renderer);
     apply_surface(500, 55+times_h, time_limit, renderer);
     SDL_Delay(40);
     return true;
}

void bike_process::change_coordinate(int n)
{
     if(is_collision(&bike_x, &bike_y, &bike_stand))
     {
          game_on=0;
     }
     else if(n!=0)
     {
          bg[0].add_background_x(n);         //add the background coordinate..
          bg[1].add_background_x(n);

          for(int i=0;i<SMAX_WALL;i++)
            w1[i].add_wall_x(n);              //add the wall's coordinte
          for(int i=0;i<BMAX_WALL;i++)
            w2[i].add_wall_x(n);              //add the wall's coordinate
     }
     else
     {
          bg[0].add_background_x(n);
          bg[1].add_background_x(n);
     }
}
