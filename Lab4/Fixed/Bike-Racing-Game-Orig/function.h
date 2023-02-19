
void apply_surface(int x,int y,SDL_Texture* texture, SDL_Renderer* renderer,SDL_Rect *clip=NULL)
{
    SDL_Rect offset;
    offset.x=x;
    offset.y=y;
    SDL_QueryTexture(texture, NULL, NULL, &offset.w, &offset.h);
/*    SDL_BlitSurface(source,clip,distination,&offset);  */     //Load image to the display screen i.e. on the screen..
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, clip, &offset);
}
