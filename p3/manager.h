#include <SDL.h>
#include <list>
#include <string>
#include <vector>

#include "drawable.h"
#include "Hud.h"
#include "world.h"

class Clock;
class Player;

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  struct Less {
	  bool operator()(Drawable* d1, Drawable* d2){
		  return d1->getZoom() < d2->getZoom();
	  }
  };

  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  World nearBg;
  World farBg;
  Viewport& viewport;


  std::list<Drawable*> sprites;
  std::vector<Drawable*> obs;
  std::vector<Drawable*> stars;
  std::list<Drawable*>::iterator currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

//  Drawable* runner;
//  bool isRunnerFly;
  Player* player;

  Hud hud;

  struct Less less;

  vector<vector<Drawable*> > foodGroups;

  void draw() const;
  void update();
  bool checkCollision();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
