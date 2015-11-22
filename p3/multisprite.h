#ifndef MULTISPRITE__H
#define MULTISPRITE__H
#include <SDL/SDL_stdinc.h>

#include <string>
#include <vector>
#include <sstream>

#include "drawable.h"


using std::string;
using std::vector;

class ExplodingMultiSprite;

class MultiSprite : public Drawable {
public:
//  MultiSprite(const std::string&, const std::vector<Frame*>&);
  MultiSprite(const std::string&,
          const Vector2f& pos, const Vector2f& vel, vector<Frame*> frames);
  MultiSprite(const string& name);
  MultiSprite(const MultiSprite&);
  MultiSprite& operator=(const MultiSprite&);
  virtual ~MultiSprite() { } 


  void explode();
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const {
    return mulframes[currentFrame];
  }

//     void draw() const;
//    void update(Uint32 ticks);
//    const Frame* getFrame() const {
//      return frames[currentFrame];
//    }


protected:
  ExplodingMultiSprite* explosion;

  const std::vector<Frame *> mulframes;
  int worldWidth;
  int worldHeight;

  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  virtual void advanceFrame(Uint32 ticks);
  virtual void move(Uint32 ticks);


  int getDistance(const Drawable* obj) const;

};
#endif
