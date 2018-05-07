#ifndef EXPLOSION_H
#define EXPLOSION_H
#include <vector>
#include <list>
#include "chunk.h"

class Explosion : public Sprite {
public:
  Explosion(const Sprite&);
  Explosion(const std::string&, const Vector2f&, const Vector2f&, const Image*);
  ~Explosion();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  void makeChunks(unsigned int); 	//creates chunks for explosions
  unsigned int chunkCount() const { return chunks.size(); }
  unsigned int freeCount()  const { return freeList.size(); }
private:
  std::list<Chunk*> chunks;
  std::list<Chunk*> freeList;
  int randomness;
  float minspeed;

  Explosion(const Explosion&);
  Explosion& operator=(const Explosion&);
};

#endif
