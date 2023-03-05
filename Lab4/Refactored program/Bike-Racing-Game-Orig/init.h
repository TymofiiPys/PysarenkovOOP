int reset_test() {
	bike_process b;
	b.bpreset();
	b.set_speed(20);
	b.set_bike_x();
	b.bpreset();
	if (b.get_bike_x() != START_BIKE_X || b.get_speed() != 0)
		return 1;

	return 0;
}

int change_coord_test() {
	wall test = wall(1000, 0, 0, 0);
	wall test1 = wall(1100, 0, 0, 0);
	test.add_wall_x(100);
	if (test.get_cord_x() != test1.get_cord_x())
		return 1;
	background b = background(1000, 0, 0, 0);
	background b1 = background(1200, 0, 0, 0);
	b.add_background_x(201);
	if (b.get_cord_x() != b1.get_cord_x())
		return 1;
	return 0;
}

int collide_test() {
	int w, h;
	SDL_QueryTexture(obs_wall[0], NULL, NULL, &w, &h);
	int* testx = new int, *testy = new int, *stand = new int;
	*testx = WIDTH + 150;
	*testy = LAYER_Y - h + 100;
	w1[0].set_state(true);
	if (!is_collision(testx, testy, stand))
	{
		w1[0].set_state(false);
		return 1;
	}
	w1[0].set_state(false);
	SDL_QueryTexture(obs_wall[1], NULL, NULL, &w, &h);
	*testx = WIDTH + 4450;
	*testy = LAYER_Y - h + 100;
	w2[1].set_state(true);
	if (!is_collision(testx, testy, stand))
	{w2[1].set_state(false); return 1; }
	w2[1].set_state(false);
	return 0;
}

bool init_all()
{
	 if(SDL_Init(SDL_INIT_EVERYTHING)==-1)
		return false;

	 if (reset_test())
	 {
		 cout << "reset test failed" << endl; return false;
	 }
	 else
		 cout << "reset test passed" << endl;
	 if (change_coord_test()) {
		 cout << "change coord test failed" << endl; return false;
	 }
	 else
		 cout << "change coord test passed" << endl;
	 //Creation of what everything will be drawn on
	 screen=SDL_CreateWindow("RISING SUN BIKE RACING GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,HEIGHT,SDL_SWSURFACE);
	 renderer = SDL_CreateRenderer(screen, -1, 0);

	if(screen==NULL)
		return false;
	if (renderer == NULL)
		return false;
	if(TTF_Init()==-1)                  //Font Initialization...
		return false;

	return true;
}


//Load a image file on screen and returns the address..
SDL_Surface* load_image(std::string filename, int col1=0xFF, int col2=0xFF, int col3=0xFF )
{
	SDL_Surface *loaded_image=NULL;
	SDL_Surface *optimized_image=NULL;

	loaded_image = IMG_Load(filename.c_str());
	if(loaded_image != NULL)
	{
		optimized_image = SDL_ConvertSurfaceFormat(loaded_image, SDL_GetWindowPixelFormat(screen), 0);        //Create an optimized image

		SDL_FreeSurface(loaded_image);//        //Free the old image

		//Frees the resources used by a previously created SDL_Surface. If the surface was created using SDL_CreateRGBSurfaceFrom then the pixel data is not freed.

		if(optimized_image != NULL)
		{
			SDL_SetColorKey(optimized_image, SDL_TRUE, SDL_MapRGB(optimized_image->format, col1, col2, col3));
		}
	}
//Then we check if the image was optimized.
/*
If the image was optimized, we have to map the color we want to set as the color key. 
We call SDL_MapRGB() to take the red, green, and blue values and give us the pixel value
 back in the same format as the surface. You can learn more about pixels in article 3.*/
	return optimized_image;     //Return the address of the loaded image...
}

bool load_files()
{
	//creating textures for backgrounds
	for(int i = 0; i < 2; i++)
	{
		back[i] = SDL_CreateTextureFromSurface(renderer, load_image("image/bg2.png"));         //Loads the background image..
		if(back[i] == NULL)
			return false;
	}
	int w, h;
	//creating background instances based on respective textures
	SDL_QueryTexture(back[0], NULL, NULL, &w, &h);
	bg[0] = background(0, 0, w, h);
	SDL_QueryTexture(back[1], NULL, NULL, &w, &h);
	bg[1] = background(WIDTH, 0, w, h);

	upback = SDL_CreateTextureFromSurface(renderer, load_image("image/bg1.png"));     //Loads the upper side of the background image..
	if(upback == NULL)
		return false;

	//creating bike texture
	bike = SDL_CreateTextureFromSurface(renderer, load_image("image/bike.PNG"));      //Loads the image of bike..
	if(bike == NULL)
		return false;
	SDL_QueryTexture(bike, NULL, NULL, NULL, &h);

	//creating wall textures (I suppose obs_wall means obstacle-wall...)
	obs_wall[0] = SDL_CreateTextureFromSurface(renderer, load_image("image/wall.png"));
	obs_wall[1] = SDL_CreateTextureFromSurface(renderer, load_image("image/wall1.png"));
	if(obs_wall[0] == NULL || obs_wall[1] == NULL)
		return false;

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

	if(collide_test())
	{
		cout << "collide test failed" << endl; return false;
	}
	else
		cout << "collide coord test passed" << endl;
	font=TTF_OpenFont("times.ttf",20);      //Font file is loaded....

	time_limit = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "Time limit is 1:30 minutes", textColor));
	die = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "GAME OVER", textColor));
	win = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, "YOU WIN", textColor1));

	menu_pic= SDL_CreateTextureFromSurface(renderer, load_image("image/menu.png"));
	play_menu= SDL_CreateTextureFromSurface(renderer, load_image("image/menu_play.png"));
	instruction_menu= SDL_CreateTextureFromSurface(renderer, load_image("image/menu_instruction.png"));
	quit_menu= SDL_CreateTextureFromSurface(renderer,load_image("image/menu_exit.png"));
	if(menu_pic == NULL || play_menu == NULL || instruction_menu == NULL || quit_menu == NULL)
		return false;

	return true;
}
