#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Lose.h"
#include "UI.h"
#include "Win.h"
#include <allegro5/allegro.h>

/**
 * @brief Main class that runs the whole game.
 * @details All game procedures must be processed through this class.
 */
class Game {
public:
  void execute();

public:
  Game(bool testMode = false);
  ~Game();
  void game_init();
  bool game_update();
  void game_draw();

private:
  /**
   * @brief States of the game process in game_update.
   * @see Game::game_update()
   */
  enum class STATE {
    MENU, // -> LEVEL
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL, // -> PAUSE, END
    PAUSE, // -> LEVEL
    END,
    WIN,
    LOSE
  };
  STATE state;
  ALLEGRO_EVENT event;
  ALLEGRO_BITMAP *game_icon;
  // ALLEGRO_BITMAP *background;
  ALLEGRO_BITMAP *menu;

private:
  ALLEGRO_DISPLAY *display;
  ALLEGRO_TIMER *timer;
  ALLEGRO_EVENT_QUEUE *event_queue;
  UI *ui;
};

#endif
