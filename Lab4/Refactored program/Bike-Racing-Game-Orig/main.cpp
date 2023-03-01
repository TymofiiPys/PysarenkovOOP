#include <iostream>
using namespace std;
#include <sstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "constants.h"
#include "string"
#include "coordinate.h"
#include "variable.h"
#include "init.h"
#include "function.h"
#include "collision.h"
#include "process.h"
#include "menu.h"

#undef main

int main()
{
     if(!init_all())
     return 1;

     if(!load_files())
     return 2;

     int opt=start_menu();


     return 0;
}
