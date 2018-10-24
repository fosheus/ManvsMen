#pragma once

#define SETTINGS_FILENAME "settings.ini"

#define GAME_NAME "Man vs Men"

#define MAIN_VIEW_WIDTH 1920
#define MAIN_VIEW_HEIGHT 1080

//1 or 0
#define FULLSCREEN 1

#if FULLSCREEN==1
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#else
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#endif

#define BLOCKSIZE 100
#define MAP_WIDTH 40
#define MAP_HEIGHT 40

#define SPLASH_SCREEN_TIME_SHOW 3



#define SPLASH_SCREEN_BACKGROUND_FILEPATH "images/splash_background.jpg"
#define MAIN_MENU_BACKGROUND_FILEPATH "images/main_menu_background.jpg"

#define GAMEOVER_TEXTURE "images/gameover.png"

#define MAIN_MENU_PLAY_BUTTON "images/main_menu_play_button.png"
#define MAIN_MENU_ONLINE_BUTTON "images/main_menu_online_button.png"
#define MAIN_MENU_OPTIONS_BUTTON "images/main_menu_options_button.png"
#define MAIN_MENU_EXIT_BUTTON "images/main_menu_exit_button.png"

#define SELECT_WEAPON_RIGHT_ARROW "images/select_weapon_right.png"
#define SELECT_WEAPON_LEFT_ARROW "images/select_weapon_left.png"
#define SELECT_WEAPON_BACKGROUND "images/f,ef"
#define SELECT_WEAPON_RETURN_BUTTON "images/buttonBack.png"
#define SELECT_WEAPON_PLAY_BUTTON "images/buttonPlay.png"

#define PAUSE_MENU_RESUME_BUTTON "images/button_resume.png"
#define PAUSE_MENU_RESTART_BUTTON "images/button_restart.png"
#define PAUSE_MENU_EXIT_BUTTON "images/main_menu_exit_button.png"

#define CHARACTER_SPRITESHEET "images/character_spritesheet.png"
#define CHARACTER_SPRITESHEET_ROW 5
#define CHARACTER_SPRITESHEET_COLUMN 4
#define CHARACTER_SPRITESHEET_SWITCH_TIME 15.0f

#define CHARACTER_DEFAULT_HEALTH 100
#define CHARACTER_MAX_HEALTH 100
#define CHARACTER_DEFAULT_PROTECTION 0
#define CHARACTER_MAX_PROTECTION 100

#define BACKPACK_ENTITY "images/backpack.png"
#define SNIPER_ENTITY "images/sniper.png"
#define SHOTGUN_ENTITY "images/shotgun.png"
#define RIFLE_ENTITY "images/ak_47.png"
#define DESERTEAGLE_ENTITY "images/desert_eagle.png"
#define MEDIKIT_ENTITY "images/medikit.png"

#define ARIAL_FONT "font/arial.ttf"
#define PIXEL_FONT "font/pixel.ttf"

#define GUN_SOUND "sound/gun.wav"
#define RIFLE_SOUND "sound/rifle.wav"
#define SHOTGUN_SOUND "sound/shotgun.wav"
#define SNIPER_SOUND "sound/sniper.wav"
#define DESERT_EAGLE_SOUND "sound/desert_eagle.wav"

#define NUMBER_ENEMIES 3
#define HEALTHMAXSIZE sf::Vector2f(100,10)
#define DEFAULT_HEATLH 100
#define PLAYERVIEWFIELDRADIUS 1200
#define ENEMY_VIEW_FIELD_RADIUS 550
#define ENEMY_ESCAPEFAILTIMEOUT 30
#define ENEMY_SPAWN_RANGE 1200

#define WEAPON_MAX_DAMAGES 100
#define WEAPON_MAX_RANGE 800
#define WEAPON_MAX_REARMTIME 90

#define ENEMY_PERCENT_DROP_MEDIKIT 33
#define ENEMY_PERCENT_DROP_PROTECTION 20
#define ENEMY_SHOOT_TIMEOUT 20

//WEAPON SPECS//
#define SHOTGUN_MAGAZINE 8
#define SHOTGUN_DAMAGES 60
#define SHOTGUN_RANGE 200
#define SHOTGUN_REARMTIME 90
#define SHOTGUN_RELOADTIME 120

#define RIFLE_MAGAZINE 30
#define RIFLE_DAMAGES 20
#define RIFLE_RANGE 500
#define RIFLE_REARMTIME 15
#define RIFLE_RELOADTIME 120

#define GUN_MAGAZINE 12
#define GUN_DAMAGES 10
#define GUN_RANGE 400
#define GUN_REARMTIME 35
#define GUN_RELOADTIME 60

#define SNIPER_MAGAZINE 6
#define SNIPER_DAMAGES 100
#define SNIPER_RANGE 800
#define SNIPER_REARMTIME 90
#define SNIPER_RELOADTIME 210
#define SNIPER_AIMINGTIME 120
#define SNIPER_PRECISION_ANGLE 20

#define DESERT_EAGLE_MAGAZINE 8
#define DESERT_EAGLE_DAMAGES 35
#define DESERT_EAGLE_REARMTIME 45
#define DESERT_EAGLE_RANGE 430
#define DESERT_EAGLE_RELOADTIME 120


