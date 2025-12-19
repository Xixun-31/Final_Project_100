#include "Game.h"
// #include "Hero.h"
#include "Level/Level.h"
#include "Menu.h"
#include "Pause.h"
#include "About.h"
#include "Portal.h" // Include Portal.h
#include "Player.h"
#include "Utils.h"
#include "monsters/Monster.h"
#include "allegro5/keycodes.h"
#include "data/DataCenter.h"
#include "data/FontCenter.h"
#include "data/ImageCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <cstring>
#include <vector>
#include "towers/Bullet.h"
#define DEBUG
// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.jpg";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background_img_path[] = "./assets/image/StartBackground.jpg";
// constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg"; // Legacy
constexpr char menu_sound_path[] = "./assets/sound/menu.WAV";
constexpr char level0_sound_path[] = "./assets/sound/level0.WAV";
constexpr char normal_level_sound_path[] = "./assets/sound/normal_level.WAV";
constexpr char boss_level_sound_path[] = "./assets/sound/boss_level.WAV";
constexpr char menu_image_path[] = "./assets/image/scene/Menu.jpg";

static const double SECRET_X1 = 800;
static const double SECRET_Y1 = 600;
static const double SECRET_X2 = 750;
static const double SECRET_Y2 = 550;

/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state
 * to a generic data storage (i.e. DataCenter). For timer event, the game_update
 * and game_draw function will be called if and only if the current is timer.
 */
void Game::execute() {
  DataCenter *DC = DataCenter::get_instance();
  // main game loop
  bool run = true;
  while (run) {
    // process all events here
    al_wait_for_event(event_queue, &event);
    switch (event.type) {
    case ALLEGRO_EVENT_TIMER: {
      run &= game_update();
      game_draw();
      break;
    }
    case ALLEGRO_EVENT_DISPLAY_CLOSE: { // stop game
      run = false;
      break;
    }
    case ALLEGRO_EVENT_KEY_DOWN: {
      DC->key_state[event.keyboard.keycode] = true;
      break;
    }
    case ALLEGRO_EVENT_KEY_UP: {
      DC->key_state[event.keyboard.keycode] = false;
      break;
    }
    case ALLEGRO_EVENT_MOUSE_AXES: {
      DC->mouse.x = event.mouse.x;
      DC->mouse.y = event.mouse.y;
      break;
    }
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
      DC->mouse_state[event.mouse.button] = true;
      break;
    }
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
      DC->mouse_state[event.mouse.button] = false;
      break;
    }
    default:
      break;
    }
  }
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be
 * processed synchronously.
 */
Game::Game(bool testMode) {
  DataCenter *DC = DataCenter::get_instance();
  GAME_ASSERT(al_init(), "failed to initialize allegro.");

  // initialize allegro addons
  bool addon_init = true;
  addon_init &= al_init_primitives_addon();
  addon_init &= al_init_font_addon();
  addon_init &= al_init_ttf_addon();
  addon_init &= al_init_image_addon();
  addon_init &= al_init_acodec_addon();
  GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

  if (testMode) {
    timer = nullptr;
    event_queue = nullptr;
    display = nullptr;
    debug_log("Game initialized in test mode.\n");
    return;
  }

  // initialize events
  bool event_init = true;
  event_init &= al_install_keyboard();
  event_init &= al_install_mouse();
  event_init &= al_install_audio();
  GAME_ASSERT(event_init, "failed to initialize allegro events.");

  // initialize game body
  GAME_ASSERT(timer = al_create_timer(1.0 / DC->FPS),
              "failed to create timer.");
  GAME_ASSERT(event_queue = al_create_event_queue(),
              "failed to create event queue.");
  GAME_ASSERT(display = al_create_display(DC->window_width, DC->window_height),
              "failed to create display.");

  debug_log("Game initialized.\n");
  game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void Game::game_init() {
  DataCenter *DC = DataCenter::get_instance();
  SoundCenter *SC = SoundCenter::get_instance();
  ImageCenter *IC = ImageCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();
  // set window icon
  game_icon = IC->get(game_icon_img_path);
  al_set_display_icon(display, game_icon);
  al_set_window_title(display, "Enter the Gungeon 2");

  // register events to event_queue
  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_mouse_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  // init sound setting
  SC->init();

  // init font setting
  FC->init();

  ui = new UI();
  ui->init();

  DC->level->init();
  DC->menu->init();
  DC->pause->init();
  DC->hero->init();
  DC->lose->init();
  DC->win->init();

  // game start
  menu = IC->get(menu_image_path);
  debug_log("Game state: change to MENU\n");
  state = STATE::MENU;
  if(DC->portal) {
      delete DC->portal;
      DC->portal = nullptr;
  }
  al_start_timer(timer);
  level_bgm_played = false;
}

/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the
 * termination criteria (false).
 * @see Game::STATE
 */

bool Game::game_update() {
  DataCenter *DC = DataCenter::get_instance();
  OperationCenter *OC = OperationCenter::get_instance();
  SoundCenter *SC = SoundCenter::get_instance();
  static ALLEGRO_SAMPLE_INSTANCE *background = nullptr;
  switch (state) {
  case STATE::MENU: {
    static bool is_played = false;
    // Play Menu Music
    if (!level_bgm_played) { // Reusing this flag or creating a new one? 
        // Let's use 'background' variable for current BGM instance.
        if(background) al_stop_sample_instance(background); 
        background = SC->play(menu_sound_path, ALLEGRO_PLAYMODE_LOOP);
        level_bgm_played = true; 
    }

    if (!is_played) {
      // SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
      DC->level_counter = 1;
      is_played = true;
    }

    DC->menu->update();
    if(DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
        if(DC->menu->hover_btn == 1) { // Start
            debug_log("<Game> state: change to LEVEL\n");
            state = STATE::LEVEL;
            DC->level->init();
            DC->player->reset();
            DC->hero->init();
            DC->monsterBullets.clear();
            DC->heroBullets.clear();
            DC->towerBullets.clear();
            
            // Stop Menu Music immediately or let next state handle it?
            // Next state (LEVEL) should stop it and start its own.
            // But we need to reset the flag.
            level_bgm_played = false; 
            if(background) al_stop_sample_instance(background);
            background = nullptr;

        } else if(DC->menu->hover_btn == 2) { // About
            debug_log("<Game> state: change to ABOUT\n");
            state = STATE::ABOUT;
            DC->about->init();
            SC->play("./assets/sound/begin.WAV", ALLEGRO_PLAYMODE_ONCE); // Begin Sound
        } else if(DC->menu->hover_btn == 3) { // Exit
             return false;
        }
    }
    
    break;
  }
  case STATE::ABOUT: {
      DC->about->update();
      if(DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
          if(DC->about->hover_btn == 1) {
              state = STATE::MENU;
              // Don't stop music, About shares Menu music usually.
              // But if we defined it per state, Menu will restart it?
              // My logic above checks !level_bgm_played.
              // If About doesn't change flag, Menu won't restart.
              // So Music keeps playing. Good.
          } else if(DC->about->hover_btn == 2) {
              DC->player->is_god_mode = !DC->player->is_god_mode;
              debug_log("God Mode toggled: %d\n", DC->player->is_god_mode);
          }
      }
      break;
  }
  case STATE::LEVEL: {
    // State management for transitions
    static bool was_boss = false;
    static bool was_level0 = false;
    
    if (DC->curr_level != DC->level_counter) {
      DC->curr_level = DC->level_counter;
      DC->level->load_level(DC->curr_level);
      
      // Level Changed. Check Music.
      const char* target_music = normal_level_sound_path;
      if(DC->curr_level == 0) target_music = level0_sound_path;
      else if(DC->curr_level >= 3) target_music = boss_level_sound_path;
      
      bool is_boss = (DC->curr_level >= 3);
      bool is_level0 = (DC->curr_level == 0);
      
       // Just restart it if type changes.
       if(level_bgm_played) {
            if(is_boss != was_boss || is_level0 != was_level0) {
                if(background) al_stop_sample_instance(background);
                background = SC->play(target_music, ALLEGRO_PLAYMODE_LOOP);
                was_boss = is_boss;
                was_level0 = is_level0;
            }
       }
    }
    
    // Initial Play Logic (Start of Game or after Pause/Menu)
    if (!level_bgm_played) {
      const char* target_music = normal_level_sound_path;
      if(DC->curr_level == 0) target_music = level0_sound_path;
      else if(DC->curr_level >= 3) target_music = boss_level_sound_path;
      
      if(background) al_stop_sample_instance(background);
      background = SC->play(target_music, ALLEGRO_PLAYMODE_LOOP);
      level_bgm_played = true;
      
      // Init static state
      was_boss = (DC->curr_level >= 3);
      was_level0 = (DC->curr_level == 0);
    }

    if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
      SC->toggle_playing(background);
      debug_log("<Game> state: change to PAUSE\n");
      state = STATE::PAUSE;
    }
    int dangerous_monsters = 0;
    int barrels_count = 0;
    for(const auto& m : DC->monsters) {
        if(m->peek_type() != MonsterType::BARREL) dangerous_monsters++;
        else barrels_count++;
    }
    // if(DC->level_counter == 0) debug_log("Level 0: Dang=%d, Barrels=%d\n", dangerous_monsters, barrels_count);

    if (DC->level->all_waves_done() && dangerous_monsters == 0) {
      // Last Level (Level 3): Go directly to WIN
      if (DC->level_counter >= 3) {
          debug_log("Level 3 cleared. Direct Win.\n");
          debug_log("<Game> state: change to WIN\n");
          state = STATE::WIN;
          level_bgm_played = false;
          SC->play("./assets/sound/win.WAV", ALLEGRO_PLAYMODE_ONCE); // WIN Sound
      }
      else {
             // Logic for Level 1 Transition
             if (DC->level_counter == 1) {
                 if(barrels_count > 0) {
                     // Go to Level 0
                     if(!DC->portal) {
                         DC->portal = new Portal(DC->window_width/2, DC->window_height/2, "./assets/image/circle_2.png");
                         DC->portal->init();
                     }
                     if(DC->portal && DC->hero->shape->overlap(*DC->portal->shape)) { // Simply overlap to transfer? User said "right lower corner press e" originally but now "circle_2 -> L0"
                         // Wait, user said "but existing logic requires overlap and space".
                         // User prompt: "pass Level 1... if barrels left -> circle_2 -> L0... if no barrels -> circle -> L2"
                         // Assuming standard portal interaction (Overlap + KEY?)
                         // The prompt "Start after level no background no monster" issue was fixed.
                         // The prompt "Change to: if barrels... -> circle_2 -> L0"
                         // I will stick to the existing interaction (Overlap + Space) to trigger the teleport, but change destination.
                         if(DC->key_state[ALLEGRO_KEY_SPACE]) { // Keeping Space as trigger for now unless "press e" is strictly required?
                             // User said "Start after level... press e" in previous prompt (Step 91 context which was bug report), 
                             // BUT in THIS prompt "Currently change to L0 is... right bottom press e please change to..."
                             // This implies the "press e" was the OLD way or the USER'S DESCRIPTION of current unwanted state? 
                             // "Current change to L0 is... press e... PLEASE CHANGE TO if ... circle_2 ... can convey to L0"
                             // So I should use the Portal mechanism.
                             delete DC->portal;
                             DC->portal = nullptr;
                             DC->level_counter = 0; // Go to Level 0
                         }
                     }
                 } else {
                     // Go to Level 2 (No barrels)
                     if(!DC->portal) {
                         DC->portal = new Portal(DC->window_width/2, DC->window_height/2, "./assets/image/circle.png");
                         DC->portal->init();
                     }
                     if(DC->portal && DC->hero->shape->overlap(*DC->portal->shape) && DC->key_state[ALLEGRO_KEY_SPACE]) {
                         delete DC->portal;
                         DC->portal = nullptr;
                         DC->level_counter = 2; // Go to Level 2
                     }
                 }
             }
             else {
                 // Other Levels (Standard behavior) and Level 0 (Treasure)
                 if(DC->level_counter != 0) {
                      if(!DC->portal) {
                          DC->portal = new Portal(DC->window_width/2, DC->window_height/2);
                          DC->portal->init();
                      }
                      
                      // Check collision with Portal
                      if(DC->portal && DC->hero->shape->overlap(*DC->portal->shape) && DC->key_state[ALLEGRO_KEY_SPACE]) {
                          delete DC->portal;
                          DC->portal = nullptr;
                          DC->level_counter++;
                      }
                 } else {
                     // Logic for Level 0 (Treasure Dead -> Portal to Level 2)
                     // Redundant check to ensure Treasure is really gone
                     if(dangerous_monsters == 0) {
                         if(!DC->portal) {
                              DC->portal = new Portal(DC->window_width/2, DC->window_height/2);
                              DC->portal->init();
                         }
                          
                          // Check collision with Portal
                          if(DC->portal && DC->hero->shape->overlap(*DC->portal->shape) && DC->key_state[ALLEGRO_KEY_SPACE]) {
                              delete DC->portal;
                              DC->portal = nullptr;
                              DC->level_counter = 2; // Jump to Level 2
                          }
                     }
                 }
             }
      }
    }
    if (DC->player->HP <= 0) {
      if (DC->hero->is_death_anim_done()) {
        debug_log("<Game> state: change to END\n");
        state = STATE::LOSE;
        level_bgm_played = false;
        SC->play("./assets/sound/lose.WAV", ALLEGRO_PLAYMODE_ONCE); // LOSE Sound
      }
    }
    // 按 E 才進（避免站著就瞬移）
    if (DC->hero->in_secret_zone() &&
        DC->key_state[ALLEGRO_KEY_E] && !DC->prev_key_state[ALLEGRO_KEY_E]) {

      debug_log("<Game> enter secret level (LEVEL0)\n");
      for (Monster *monster : DC->monsters) {
        delete monster;
      }
      DC->monsters.clear();
      for (Bullet *bullet : DC->heroBullets) {
        delete bullet;
      }
      DC->heroBullets.clear();
      state = STATE::LEVEL0;
      DC->level->load_level(0);          // 0 = hidden level
    }
    break;
  }
  case STATE::LEVEL0: {
  // 還沒弄  
  // 玩家死了照樣輸
  if (DC->player->HP <= 0){
    state = STATE::LOSE;
  }
  break;
}

  case STATE::PAUSE: {
    DC->pause->update();
    if (DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
      SC->toggle_playing(background);
      debug_log("<Game> state: change to LEVEL\n");
      state = STATE::LEVEL;
    }
    if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
        if (DC->pause->hover_btn == 1) { // Resume
            SC->toggle_playing(background);
            debug_log("<Game> state: change to LEVEL\n");
            state = STATE::LEVEL;
            DC->hero->lock_attack(30); // Prevent accidental shot
        } else if (DC->pause->hover_btn == 2) { // Menu
             debug_log("<Game> state: change to MENU\n");
             state = STATE::MENU;
             al_stop_sample_instance(background); // Stop BGM when going to menu
             level_bgm_played = false; // Reset BGM flag so it restarts when entering level again
        } else if (DC->pause->hover_btn == 3) { // Exit
             return false;
        }
    }
    break;
  }
  case STATE::END: {
    return false;
  }
  case STATE::WIN: {
    al_stop_sample_instance(background);
    DC->win->update();
    if (DC->key_state[ALLEGRO_KEY_ENTER]) {
      debug_log("<Game> state: change to MENU\n");
      state = STATE::MENU;
    }
    if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
       if (DC->win->hover_btn == 1) { // Menu
           state = STATE::MENU;
       } else if (DC->win->hover_btn == 2) { // Exit
           return false;
       }
    }
    break;
  }
  case STATE::LOSE: {
    al_stop_sample_instance(background);
    DC->lose->update();
    if (DC->key_state[ALLEGRO_KEY_ENTER]) {
      debug_log("<Game> state: change to MENU\n");
      state = STATE::MENU;
    }
    if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) {
       if (DC->lose->hover_btn == 1) { // Menu
           state = STATE::MENU;
       } else if (DC->lose->hover_btn == 2) { // Exit
           return false;
       }
    }
    break;
  }
  default:
    break;
  }
  // If the game is not paused, we should progress update.
  if (state != STATE::PAUSE) {
    DC->player->update();
    SC->update();
    ui->update();
    DC->hero->update();
    if ((state == STATE::LEVEL || state == STATE::LEVEL0) && !DC->hero->is_dying) {
      DC->level->update();
      OC->update();
    }
  }
  // game_update is finished. The states of current frame will be previous
  // states of the next frame.
  memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
  memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
  return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void Game::game_draw() {
  DataCenter *DC = DataCenter::get_instance();
  OperationCenter *OC = OperationCenter::get_instance();
  FontCenter *FC = FontCenter::get_instance();

  // Flush the screen first.
  al_clear_to_color(al_map_rgb(100, 100, 100));
  switch (state) {
  case STATE::MENU: {
    DC->menu->draw();
    break;
  }
  case STATE::ABOUT: {
      DC->about->draw();
      break;
  }
  case STATE::LEVEL0: 
  case STATE::LEVEL: {
    DC->level->draw();
    if(DC->portal) DC->portal->draw();
    DC->hero->draw();
    OC->draw();
    ui->draw();
    
    if (DC->hero->in_secret_zone()) {
    al_draw_text(FC->caviar_dreams[FontSize::MEDIUM],
               al_map_rgb(255,255,255),
               DC->window_width / 2.0, 40,
               ALLEGRO_ALIGN_CENTRE,
               "Press E to enter Secret Level");
    }
    break;
  }
  
  case STATE::PAUSE: {
    // game layout cover
    DC->pause->draw();
    break;
  }
  case STATE::END: {
  }
  case STATE::WIN: {
    DC->win->draw();
    break;
  }
  case STATE::LOSE: {
    DC->lose->draw();
    break;
  }
  default:
    break;
  }
  al_flip_display();
}

Game::~Game() {
  if (display)
    al_destroy_display(display);
  if (timer)
    al_destroy_timer(timer);
  if (event_queue)
    al_destroy_event_queue(event_queue);
  if (ui)
    delete ui;
}
