


class bike_process
{
     private:

     public:


void load_layer(int x=0 ,int y=0)
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

void change_coardinate(int );       //Set the coardinate of the labels..

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

     if( keystate[SDL_SCANCODE_ESCAPE] || game_on==0 || time_gap>=90)
     return false;

     if(keystate[SDL_SCANCODE_SPACE] or keystate[SDL_SCANCODE_UP])                //If space is pressed...
     {
          if(jump_state==0)
          jump_state=36;                       //set the jump or SPACE state in 20*2 steps...

     }
     if( (bike_y <(bike_stand-bike_h)) or jump_state>=16  )    //Condition true untill the MARIO goes upward and downward to surface below it..
     {
          int jump_height;
          jump_height=19-jump_state;         //using the formula n/2*(n+1)...
          bike_y += jump_height;
          jump_state--;
          if(jump_state==0)
          jump_state--;
     }
     else
     {
          bike_y=bike_stand-bike_h;
          jump_state=0; 
     }

     if(keystate[SDL_SCANCODE_RIGHT])
     if(speed<=20)                 //Speed only increase to 10..
     speed++;
     if(keystate[SDL_SCANCODE_LEFT])
     if(speed>0)                    //speed cannot be less than 0..
     speed--;

     if(bike_x < FINISH_BIKE_X)
     bike_x+=speed;
     else
     change_coardinate(-speed/2);
     change_coardinate(0);
     SDL_RenderClear(renderer);
     load_layer();
     if(( w2[BMAX_WALL-1].get_cord_x())+w2[BMAX_WALL-1].get_cord_w() < 0)
     {
         apply_surface(100,300,die, renderer);
         SDL_RenderPresent(renderer);
         SDL_Delay(2000);
         return false;
     }

     std::stringstream time;
     //Convert the timer's time to a string
     time_gap =(SDL_GetTicks() - start_time)/1000;
     int mm=time_gap/60;
     int ss=time_gap%60;
     time << "Timer: " << mm<<":"<<ss;
     times=SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font,time.str().c_str(),textColor));
     apply_surface(600,50,times, renderer);
     apply_surface(500,55+times_h,time_limit, renderer);

     return true;
}

void bike_process::change_coardinate(int n)
{
     if(is_collision())
     {
          game_on=0;
     }
     else if(n!=0)
     {
          bg[0].add_background_x(n);         //add the background caordinate..
          bg[1].add_background_x(n);

          for(int i=0;i<SMAX_WALL;i++)
          w1[i].add_wall_x(n);              //add the wall's coardinte
          for(int i=0;i<BMAX_WALL;i++)
          w2[i].add_wall_x(n);              //add the wall's coardinte
     }
     else
     {
          bg[0].add_background_x(n);
          bg[1].add_background_x(n);
     }
}
