#include <SDL2/SDL.h>

int start_menu()
{
     bool running=true;
     int x,y;

     bike_process bk;
//https://www.willusher.io/sdl2%20tutorials/2013/08/20/lesson-4-handling-events
     while(running)
     while(SDL_PollEvent( &event ) )
     {
          if(event.type == SDL_MOUSEMOTION)
          {
               cerr<<"inside"<<endl;
               x = event.motion.x;
               y = event.motion.y;
               SDL_RenderClear(renderer);
               if((x>594) && (x<756) && (y>340) && (y<357))
               {
                    apply_surface(0,0,play_menu,renderer);
               }
               else if((x>572) && (x<778) && (y>414) && (y<431))
               {
                    apply_surface(0,0,instruction_menu, renderer);
               }
               else if((x>641) && (x<708) && (y>490) && (y<506))
               {
                    apply_surface(0,0,quit_menu, renderer);
               }
               else
               apply_surface(0,0,menu_pic, renderer);
               /*SDL_Flip(screen);*/
               SDL_RenderPresent(renderer);

          }
          else
          {
              SDL_RenderClear(renderer);
              apply_surface(0, 0, menu_pic, renderer);
              //SDL_Flip(screen);
              SDL_RenderPresent(renderer);
          }
        if(event.type == SDL_MOUSEBUTTONDOWN)
          {
               x = event.motion.x;
               y = event.motion.y;

               if((x>594) && (x<756) && (y>340) && (y<357))
               {
                   game_on = 1;
                    while(bk.process())
                    {
                         //SDL_Flip(screen);
                        SDL_RenderPresent(renderer);
                    }
               }
               else if((x>572) && (x<778) && (y>414) && (y<431))
               {
                    apply_surface(0,0,instruction_menu, renderer);
               }
               else if((x>641) && (x<708) && (y>490) && (y<506))
               {
                    return 0;
               }
          }
     }

     return 0;
}
