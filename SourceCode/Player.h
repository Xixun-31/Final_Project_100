#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

class Player {
public:
  Player();
  void update();
  void reset();
  int HP;
  int coin;
  bool is_god_mode; // God Mode flag

private:
  int coin_freq;
  int coin_increase;
  int coin_counter;
};

#endif
