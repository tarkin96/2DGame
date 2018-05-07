#ifndef IOMOD_H
#define IOMOD_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

class IOmod {
public:
  static IOmod& getInstance();
  ~IOmod();
  SDL_Texture* readTexture(const std::string& filename);
  SDL_Surface* readSurface(const std::string& filename);
  void writeText(const std::string&, int, int) const;
  //added for #5
  void writeText(const std::string&, int, int, SDL_Color) const;
  //added for HUD
  void writeText(const std::string&, int, int, SDL_Color, int) const;
  //added these to fix frame rate issue from project 5
  void writeText(SDL_Texture*, int, int) const;
  SDL_Texture * CreateText(const std::string&, int, int, SDL_Color, int) const;
  
  SDL_Renderer* getRenderer() const { return renderer; }
private:
  int init;
  SDL_Renderer* renderer;
  TTF_Font* font;
  SDL_Color textColor;
  IOmod();
  IOmod(const IOmod&);
  IOmod& operator=(const IOmod&);
};
#endif
