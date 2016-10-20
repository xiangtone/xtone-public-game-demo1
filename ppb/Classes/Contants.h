#ifndef __Contants_H__
#define __Contants_H__

#define DESIGN_WIDTH	(640)
#define DESIGN_HEIGHT   (960)

//debug draw
//#define __DEBUG_DRAW__

//tags
#define   GAME_PLAY_POPUP_LAYER_TAG   (3001)

//msg string
#define     MSG_BUTTON_PRESS_ID			  "buttonPress"
#define     MSG_BUTTON_ONLINE			  "online"
//msg id
enum
{
	MSG_ID_STARTLAYER_BUTTON_PRESS = 1,
	MSG_ID_GAVEOVERLAYER_BUTTON_PRESS = 2,
	MSG_ID_POPUPLAYER_BUTTON_PRESS = 3,
	MSG_ID_POPUPLAYER_BACKTOGAME = 4,
	MSG_ID_POPUPLAYER_GAMEPLAYING_BUTTON_PRESS = 5,
	MSG_ID_POPUPLAYER_JION = 6,
	MSG_ID_POPUPLAYER_JUMP = 7,
	MSG_ID_GAME_START= 8,
	MSG_ID_MAX
};

//pet
typedef enum
{
	PET01,
	PET02,
	PET03,
	PET04,
	PET05,
	PET_MAX
}PetType;

static const char *s_pet_list[PET_MAX] = {
	"pet01",
	"pet02",
	"pet03",
	"pet04",
	"pet05"
};

static const int s_pet_price[PET_MAX] = {
	0,
	188,
	288,
	388,
	588
};

//bubble time
#define  BIRD_BUBBLE_DELAY_TIME	 (8.f)

#endif  //#ifndef __Contants_H__