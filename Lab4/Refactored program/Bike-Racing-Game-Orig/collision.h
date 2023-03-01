

//bool check_collision(SDL_Rect,SDL_Rect);

bool check_collision_wall(SDL_Rect ,SDL_Rect , int*, int*, int*);
bool is_collision(int *bike_x, int *bike_y, int *bike_stand)              //Returns true when collision happen ,otherwise returns false..
{
     SDL_Rect rect;
     int bike_w, bike_h;
     SDL_QueryTexture(bike, NULL, NULL, &bike_w, &bike_h);
     rect.x=*bike_x;
     rect.y=*bike_y;
     rect.w=bike_w;
     rect.h=bike_h;

          for(int i=0;i<SMAX_WALL;i++)    //Check collision of wall..
               if(w1[i].get_state())     //When enabled wall is found only then check the collision and exit the loop...
               {
                    if (check_collision_wall(rect ,w1[i].get_rect(), bike_x, bike_y, bike_stand))   //Check collision of the MARIO and PIPE 0...
                        return true;
                    break;
               }
          for(int i=0;i<BMAX_WALL;i++)    //Check collision of wall..
               if(w2[i].get_state())     //When enabled wall is found only then check the collision and exit the loop...
               {

                    if (check_collision_wall(rect ,w2[i].get_rect(), bike_x, bike_y, bike_stand))   //Check collision of the MARIO and PIPE 0...
                    {
                         return true;
                         break;
                    }

               }
          return false;
}

bool check_collision_wall(SDL_Rect source,SDL_Rect destination, int *bike_x, int *bike_y, int *bike_stand)
{
//Detect collision when moving RIGHT or LEFT side...
    int bike_h;
    SDL_QueryTexture(bike, NULL, NULL, NULL, &bike_h);
         if( ((source.x+source.w) < destination.x ) or  source.x > ( destination.x+destination.w))
         {
              if( (*bike_stand-bike_h) == destination.y - source.h)
                *bike_stand=LAYER_Y;
              return false;
         }
         if(( (source.y+source.h) <= destination.y ))
         {
              *bike_stand=destination.y;
              return false;
         }
         if(((source.y+source.h)<=(destination.y+20)) and ( (source.y+source.h)>=destination.y-10 ) )
          {
               *bike_stand=destination.y;
               *bike_y=destination.y-source.h;
               return false;
          }
         apply_surface(100,300,die, renderer);
         //SDL_Flip(screen);
         SDL_RenderPresent(renderer);
         SDL_Delay(2000);
         return true;
}
