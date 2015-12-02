#include <SDL.h>
#include <list>
#include <string>
#include <vector>

#include "drawable.h"
#include "Hud.h"
#include "world.h"
#include "BulletPool.h"
#include "EnemyPool.h"
#include "sound.h"

class Clock;
class Player;

class Manager {
public:
  Manager ();
  Manager& operator=(const Manager& mgr);
  ~Manager ();

  void init();
  void reset();
  void play();
  void switchSprite();

  static int score;
private:
  struct Less {
	  bool operator()(Drawable* d1, Drawable* d2){
		  return d1->getZoom() < d2->getZoom();
	  }
  };
  bool isCollision(const Drawable* d1, const Drawable* d2) const;

  const bool env;
  const IOManager& io;
  Clock& clock;
  BulletPool& bulletPool;

  SDL_Surface * const screen;
  World nearBg;
  World farBg;
  Viewport& viewport;

  //init in function init
  std::list<Drawable*> fgSprites;
  std::list<Drawable*> bgSprites;
  std::vector<Drawable*> obs;
  std::vector<Drawable*> stars;
  vector<vector<Drawable*> > foodGroups;
  std::list<Drawable*>::iterator currentSprite;

  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  Player* player;
  EnemyPool& enemyPool;
  Hud hud;

  struct Less less;

  SDLSound sound;


  void draw() const;
  void update();
  bool checkCollision();

  Manager(const Manager&);
  void makeFrame();
};
