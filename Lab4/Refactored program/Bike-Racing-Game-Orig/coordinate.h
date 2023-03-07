
//! Class for coordinates.
/*!
    Class which represents a rectangle with certain x and y coordinates, width and height. Is parent to wall and background.
*/
class cord          //Holds the coordinates of the images
{
     protected:
        //! A SDL_Rect which may represent either a wall or background rectangle.
     SDL_Rect cord_rect;

     public:
        //! A constructor
        /*
            \param x x coordinate (how far in pixels is rectangle from left side of the window).
            \param y y coordinate (how far in pixels is rectangle from top of the window).
            \param w width in pixels.
            \param h height in pixels.
        */
     cord(int x=0,int y=0,int w=0,int h=0)
     {
          cord_rect.x=x;
          cord_rect.y=y;
          cord_rect.w=w;
          cord_rect.h=h;
     }
     //! Gets x coordinate of rectangle.
     int get_cord_x()
     {
          return cord_rect.x;
     }
     //! Gets y coordinate of rectangle.
     int get_cord_y()  
     {
          return cord_rect.y;
     }
     //! Gets width of rectangle.
     int get_cord_w()
     {
          return cord_rect.w;
     }
     //! Gets height of rectangle.
     int get_cord_h()
     {
          return cord_rect.h;
     }
     //! Gets rectangle itself.
     SDL_Rect get_rect()
     {
          return cord_rect;
     }
};

//! Class for backgrounds.
/*!
    Class which represents a background rectangle.
*/

class background:public cord
{
     private:

     public:
         //!A constructor.
     background()
     {    }
     //!A constructor (with set x,y coords, width and height).
     background(int x,int y,int w,int h):cord(x,y,w,h)
     {    }
     //!Adds (n-1) pixels to background rectangle's x coordinate.
     void add_background_x(int n)
     {
          cord_rect.x+=(n-1);
          if(cord_rect.x < -WIDTH)
          cord_rect.x+=(2*WIDTH);
     }
}
;
//! Class for walls.
/*!
    Class which represents a wall rectangle.
*/
class wall:public cord
{
     private:
         //!Visibility of wall on the window.
     bool state; 
     public:
         //!A constructor.
     wall()
     {
          state=false;
     }
     //!A constructor (with set x,y coords, width and height).
     wall(int x,int y,int w,int h):cord(x,y,w,h)
     {
          state=false;
     }
     //!Gets visibility of the wall.
     bool get_state()
     {
          return state;
     }
     //!Sets visibility of the wall.
     void set_state(bool s)
     {
          state=s;
     }
     //!Adds n pixels to wall rectangle's x coordinate. If the wall should be drawn according to its rectangle's x coord. and width, set its state to true, otherwise false.
     void add_wall_x(int n)
     {
          cord_rect.x+=n;
          if( cord_rect.x>WIDTH or ( (cord_rect.x+cord_rect.w) < 0) ) //If wall can be drawn only outside of the screen, make it invisible,
              //otherwise visible.
          state=false;
          else
          state=true;
     }
};
