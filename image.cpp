#include "drawable.h"
#include "image.h"
#include "ioMod.h"
#include "viewport.h"
#include "renderContext.h"

//gives the angle the object faces
float dir(float x, float y) {
	if (x != 0) {
		return atan(y / x);
	}
	else {return 0.0;}
}

Image::Image( SDL_Surface* surf, SDL_Texture* tex ) : 
  renderer(RenderContext::getInstance()->getRenderer()),
  surface( surf ),
  texture( tex ),
  view{0,0,surf->w,surf->h}
{ }

Image::Image( const Image& image ) :
  renderer(image.renderer),
  surface(image.surface), 
  texture(image.texture),
  view(image.view)
{ }


Image& Image::operator=(const Image& rhs) {
  renderer = rhs.renderer;
  texture = rhs.texture;
  view = rhs.view;
  return *this;
}

void Image::draw(int x, int y) const {
  draw(x,y,1.0f);
}

void Image::draw(int x, int y, float scale) const {
  x -= Viewport::getInstance().getX();
  y -= Viewport::getInstance().getY();
  int tempHeight = scale*view.h;
  int tempWidth =  scale*view.w;
  SDL_Rect dest =  {x, y, tempWidth, tempHeight};
  SDL_RenderCopy(renderer, texture, &view, &dest);
}

//added this in order to display a flipped sprite (flips both ways)
void Image::draw(int x, int y, float scale, float xvel, float yvel) const {
	x -= Viewport::getInstance().getX();
	y -= Viewport::getInstance().getY();
	int tempHeight = scale*view.h;
	int tempWidth =  scale*view.w;
	SDL_Rect dest =  {x, y, tempWidth, tempHeight};
	float angle = dir(xvel, yvel) * (180/M_PI);
	//std::cout << angle << std::endl;
	if (xvel < 0.0) {
		//uses SDL_RenderCopyEx to draw sprite flipped horizontally, rotated based on velocity
		SDL_RenderCopyEx(renderer, texture, &view, &dest, angle , NULL, SDL_FLIP_HORIZONTAL);
	}
	else {
		SDL_RenderCopyEx(renderer, texture, &view, &dest, angle, NULL, SDL_FLIP_NONE);
	}

}
//added in project 4 to allow sprites to draw at different angles that are passed in
//used for directional sprites
void Image::drawAngled(int x, int y, float scale, float angle) const {
	x -= Viewport::getInstance().getX();
	y -= Viewport::getInstance().getY();
	int tempHeight = scale*view.h;
	int tempWidth =  scale*view.w;
	SDL_Rect dest =  {x, y, tempWidth, tempHeight};
	//this is the function that does the magic
	SDL_RenderCopyEx(renderer, texture, &view, &dest, angle, NULL, SDL_FLIP_NONE);

}

void Image::draw(int sx, int sy, int dx, int dy) const {
  SDL_Rect src = { sx, sy, view.w, view.h };    
  SDL_Rect dst = { dx, dy, getWidth(), getHeight() };
  SDL_RenderCopy(renderer, texture, &src, &dst);
}

Image* Image::crop(SDL_Rect sub)const{
  if(sub.x+sub.w > view.w
  || sub.y+sub.h > view.h){
    std::cerr << "Attempted to crop image with invalid geometry."
              << std::endl
              << "(0,0 + "<<view.w << "x"<<view.h<<")"
              << " --> "
              << "("<<sub.x<<","<<sub.y<<" + "<<sub.w << "x"<<sub.h<<")"
              << std::endl;
    return nullptr;
  }

  Image* cloned = new Image(*this);
  cloned->view = sub;

  return cloned;
}

