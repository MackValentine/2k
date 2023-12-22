/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils.h"
#include <iostream>
#include <map>
#include <string>

class Constants {
public:
	std::unordered_map<std::string, std::string> DestinyScript_str;
	std::unordered_map<std::string, int> DestinyScript;
	std::unordered_map<std::string, int> EventCode;

	Constants() {
		DestinyScript_str = {
			// Special Text Parts
			{"CR", "Chr(0x0D)"},
			{"LF", "Chr(0x0A)"},
			{"CRLF", "CR..LF"},
			{"QUOTE", "\"\""},
		};

		DestinyScript = {
			// Activation/Deactivation
			{"DISABLE", 0},
			{"ENABLE", 1},

			// Animation Properties
			{"ANIMATION_SCOPE_SINGLE", 0},
			{"ANIMATION_SCOPE_SCREEN", 1},

			{"ANIMATION_FOCUS_TOP", 0},
			{"ANIMATION_FOCUS_CENTER", 1},
			{"ANIMATION_FOCUS_BOTTOM", 2},

			{"ANIMATION_FLASHTYPE_NONE", 0},
			{"ANIMATION_FLASHTYPE_TARGET", 1},
			{"ANIMATION_FLASHTYPE_SCREEN", 2},

			{"ANIMATION_SHAKETYPE_NONE", 0},
			{"ANIMATION_SHAKETYPE_TARGET", 1},
			{"ANIMATION_SHAKETYPE_SCREEN", 2},

			// Equipment Types/Filters
			{"EQUIPMENT_ALL", 0},
			{"EQUIPMENT_WEAPON", 1},
			{"EQUIPMENT_SHIELD", 2},
			{"EQUIPMENT_ARMOR", 3},
			{"EQUIPMENT_HELMET", 4},
			{"EQUIPMENT_ACCESSORY", 5},

			// Battler Properties
			{"BATTLERSPEED_SLOW", 20},
			{"BATTLERSPEED_MID", 14},
			{"BATTLERSPEED_FAST", 8},

			{"BATTLERGRAPHIC_CHARACTER", 0},
			{"BATTLERGRAPHIC_ANIMATION", 1},

			{"BATTLERANIM_IDLE", 0},
			{"BATTLERANIM_ATTACKRIGHT", 1},
			{"BATTLERANIM_ATTACKLEFT", 2},
			{"BATTLERANIM_SKILLUSE", 3},
			{"BATTLERANIM_KNOCKOUT", 4},
			{"BATTLERANIM_DAMAGE", 5},
			{"BATTLERANIM_CONDITION", 6},
			{"BATTLERANIM_DEFEND", 7},
			{"BATTLERANIM_WALKLEFT", 8},
			{"BATTLERANIM_WALKRIGHT", 9},
			{"BATTLERANIM_VICTORY", 10},
			{"BATTLERANIM_ITEMUSE", 11},

			// MoveRoute Commands
			{"MC_STEPUP", 0},
			{"MC_STEPRIGHT", 1},
			{"MC_STEPDOWN", 2},
			{"MC_STEPLEFT", 3},
			{"MC_STEPRIGHTUP", 4},
			{"MC_STEPRIGHTDOWN", 5},
			{"MC_STEPLEFTDOWN", 6},
			{"MC_STEPLEFTUP", 7},
			{"MC_STEPRANDOM", 8},
			{"MC_STEPTOWARDHERO", 9},
			{"MC_STEPAWAYFROMHERO", 10},
			{"MC_STEPFORWARD", 11},

			{"MC_LOOKUP", 12},
			{"MC_LOOKRIGHT", 13},
			{"MC_LOOKDOWN", 14},
			{"MC_LOOKLEFT", 15},
			{"MC_TURNRIGHT", 16},
			{"MC_TURNLEFT", 17},
			{"MC_TURN180", 18},
			{"MC_RIGHTLEFTTURN", 19},
			{"MC_LOOKRANDOM", 20},
			{"MC_LOOKTOWARDHERO", 21},
			{"MC_LOOKAWAYFROMHERO", 22},

			{"MC_WAIT", 23},
			{"MC_STARTJUMPING", 24},
			{"MC_STOPJUMPING", 25},
			{"MC_FIXDIRECTION", 26},
			{"MC_RELEASEDIRECTION", 27},
			{"MC_INCREASESPEED", 28},
			{"MC_DECREASESPEED", 29},
			{"MC_INCREASEFREQUENCY", 30},
			{"MC_DECREASEFREQUENCY", 31},
			{"MC_SETSWITCHON", 32},
			{"MC_SETSWITCHOFF", 33},
			{"MC_SETCHAR", 34},
			{"MC_PLAYSOUND", 35},
			{"MC_STARTPHASING", 36},
			{"MC_STOPPHASING", 37},
			{"MC_FIXANIMATION", 38},
			{"MC_RELEASEANIMATION", 39},
			{"MC_INCREASETRANSPARENCY", 40},
			{"MC_DECREASETRANSPARENCY", 41},

			// MovePoint Types
			{"MOVEPOINT_DISPLAY", 0},
			{"MOVEPOINT_HIGHRES", 1},

			// Screen Brightness
			{"SCREEN_BLACK", 0},
			{"SCREEN_DARK", 50},
			{"SCREEN_NORMAL", 100},
			{"SCREEN_BRIGHT", 150},
			{"SCREEN_WHITE", 200},

			// Bool Values
			{"FALSE", 0},
			{"TRUE", 1},

			// File System Attributes
			{"FILE_ATTRIBUTE_NORMAL", 128},
			{"FILE_ATTRIBUTE_SYSTEM", 4},
			{"FILE_ATTRIBUTE_HIDDEN", 2},
			{"FILE_ATTRIBUTE_ARCHIVE", 32},
			{"FILE_ATTRIBUTE_DIRECTORY", 16},
			{"FILE_ATTRIBUTE_READONLY", 1},

			// File Access Types
			{"FILE_READ", 1},
			{"FILE_WRITE", 2},
			{"FILE_APPEND", 6},

			{"FILEPOS_BEGIN", 0},
			{"FILEPOS_CURRENT", 1},
			{"FILEPOS_END", 2},

			// DataTypes
			{"TYPE_VARIABLE", 1},
			{"TYPE_BOOL", 2},
			{"TYPE_SWITCH", 2},  // LEGACY
			{"TYPE_DWORD", 3},
			{"TYPE_FLOAT", 4},
			{"TYPE_DOUBLE", 4},  // LEGACY
			{"TYPE_STRING", 5},
			{"TYPE_BYTE", 6},
			{"TYPE_WORD", 7},

			// Vulnerability Levels
			{"EFFECT_A", 0},
			{"EFFECT_B", 1},
			{"EFFECT_C", 2},
			{"EFFECT_D", 3},
			{"EFFECT_E", 4},

			// Element/Attribute Properties
			{"ELEMENTTYPE_WEAPONATTRIBUTETYPE_WEAPON", 0},
			{"ELEMENTTYPE_MAGICATTRIBUTETYPE_MAGIC", 1},

			// Event Facing Directions
			{"DIR_UP", 0},
			{"DIR_RIGHT", 1},
			{"DIR_DOWN", 2},
			{"DIR_LEFT", 3},
			{"DIR_UP_RIGHT", 4},
			{"DIR_RIGHT_UP", 4},  // LEGACY
			{"DIR_DOWN_RIGHT", 5},
			{"DIR_RIGHT_DOWN", 5},  // LEGACY
			{"DIR_DOWN_LEFT", 6},
			{"DIR_LEFT_DOWN", 6},  // LEGACY
			{"DIR_UP_LEFT", 7},
			{"DIR_LEFT_UP", 7},  // LEGACY

			// Event Properties
			{"EVENT_MAPID", 0},
			{"EVENT_X", 1},
			{"EVENT_Y", 2},
			{"EVENT_DIRECTION", 3},
			{"EVENT_SCREENX", 4},
			{"EVENT_SCREENY", 5},

			// Event Step Graphics
			{"EVENTFRAME_LEFT", 0},
			{"EVENTFRAME_NORMAL", 1},
			{"EVENTFRAME_RIGHT", 2},
			{"EVENTFRAME_NORMAL2", 3},

			// Eventoid Special IDs
			{"PLAYER", 10001},
			{"HERO", 10001},  // LEGACY
			{"BOAT", 10002},
			{"SHIP", 10003},
			{"AIRSHIP", 10004},
			{"THIS", 10005},

			// Error Messages
			{"DESTINYERROR_UNKNOWN", -1},
			{"DESTINYERROR_SYNTAX", 1},
			{"DESTINYERROR_NOVALUE", 2},
			{"DESTINYERROR_UNKNOWNNAME", 3},
			{"DESTINYERROR_CONVERT", 4},
			{"DESTINYERROR_READONLY", 5},
			{"DESTINYERROR_ARRAYBOUND", 6},
			{"DESTINYERROR_RANGE", 7},
			{"DESTINYERROR_MEMORY", 8},
			{"DESTINYERROR_VALUE", 9},
			{"DESTINYERROR_BINARYFLOAT", 10},
			{"DESTINYERROR_CALCSWITCH", 11},
			{"DESTINYERROR_CALC_STRING", 12},
			{"DESTINYERROR_FLOATERROR", 13},
			{"DESTINYERROR_FLOATLENGTH", 14},
			{"DESTINYERROR_DIVISIONBYZERO", 15},
			{"DESTINYERROR_STRINGFORMAT", 16},
			{"DESTINYERROR_STRINGRANGE", 17},
			{"DESTINYERROR_PICTURE", 18},
			{"DESTINYERROR_PIXELRANGE", 19},
			{"DESTINYERROR_SAMEPICTURE", 20},
			{"DESTINYERROR_PALETTERANGE", 21},
			{"DESTINYERROR_SOCKETSTARTUP", 22},
			{"DESTINYERROR_NOFREESOCKET", 23},
			{"DESTINYERROR_CANTCREATESOCKET", 24},
			{"DESTINYERROR_SOCKETSTILLOPEN", 25},
			{"DESTINYERROR_SOCKETNOTOPEN", 26},
			{"DESTINYERROR_CANTCONNECT", 27},
			{"DESTINYERROR_SOCKETTYPE", 28},
			{"DESTINYERROR_SOCKETERROR", 29},
			{"DESTINYERROR_OOB", 30},
			{"DESTINYERROR_STRINGTOOLONG", 31},
			{"DESTINYERROR_NOFREEFILEHANDLE", 32},
			{"DESTINYERROR_CANTRESOLVEPATH", 33},
			{"DESTINYERROR_NOPERMISSION", 34},
			{"DESTINYERROR_CANTOPENFILE", 35},
			{"DESTINYERROR_FILENOTOPEN", 36},
			{"DESTINYERROR_CANTACCESSFILE", 37},
			{"DESTINYERROR_CANTCREATEDIR", 38},
			{"DESTINYERROR_CANTREMOVEDIR", 39},
			{"DESTINYERROR_CANTRENAMEFILE", 40},
			{"DESTINYERROR_CANTCOPYFILE", 41},
			{"DESTINYERROR_CANTDELETEFILE", 42},
			{"DESTINYERROR_CANTREADATTRIBUTES", 43},
			{"DESTINYERROR_CANTWRITEATTRIBUTES", 44},
			{"DESTINYERROR_SEARCHSTILLOPEN", 45},
			{"DESTINYERROR_CANTSTARTSEARCH", 46},
			{"DESTINYERROR_NOSEARCHSTARTED", 47},
			{"DESTINYERROR_PROTECTED", 48},
			{"DESTINYERROR_CANTEXECUTEFILE", 49},
			{"DESTINYERROR_NOTENOUGHPARAMETERS", 50},
			{"DESTINYERROR_NOTCOMPARABLE", 51},
			{"DESTINYERROR_INVALIDSIGN", 52},
			{"DESTINYERROR_SETNOTALLOWEDHERE", 53},
			{"DESTINYERROR_MISSINGENDIF", 54},
			{"DESTINYERROR_MISSINGLOOP", 55},
			{"DESTINYERROR_MISSINGCONDITION", 56},
			{"DESTINYERROR_MISSINGNEXT", 57},
			{"DESTINYERROR_MISSINGCASE", 58},
			{"DESTINYERROR_MISSINGENDSWITCH", 59},
			{"DESTINYERROR_BREAKNOTALLOWEDHERE", 60},
			{"DESTINYERROR_CONTINUENOTALLOWEDHERE", 61},
			{"DESTINYERROR_INVALIDHANDLE", 62},
			{"DESTINYERROR_OPERATOREXPECTED", 63},
			{"DESTINYERROR_CANTACCESSSAVESLOT", 64},
			{"DESTINYERROR_EXCEPTION", 65},
			{"DESTINYERROR_INVALIDLOOPOPERATIONLEVEL", 66},
			{"DESTINYERROR_CONTINUEDOESNTPOINTTOLOOP", 67},

			// GameKey Flags
			{"GKF_DOWN", 1},
			{"GKF_LEFT", 2},
			{"GKF_RIGHT", 4},
			{"GKF_UP", 8},
			{"GKF_DECISION", 16},
			{"GKF_CANCEL", 32},
			{"GKF_MOUSELEFT", 64},
			{"GKF_MOUSERIGHT", 128},
			{"GKF_SHIFT", 256},
			{"GKF_CTRL", 512},
			{"GKF_NUM0", 1024},
			{"GKF_NUM1", 2048},
			{"GKF_NUM2", 4096},
			{"GKF_NUM3", 8192},
			{"GKF_NUM4", 16384},
			{"GKF_NUM5", 32768},
			{"GKF_NUM6", 65536},
			{"GKF_NUM7", 131072},
			{"GKF_NUM8", 262144},
			{"GKF_NUM9", 524288},
			{"GKF_ADD", 1048576},
			{"GKF_SUB", 2097152},
			{"GKF_MUL", 4194304},
			{"GKF_DIV", 8388608},
			{"GKF_DOT", 16777216},
			{"GKF_MOUSEMIDDLE", 33554432},
			{"GKF_WHEELUP", 67108864},
			{"GKF_WHEELDOWN", 134217728},
			{"GKF_WHEELLEFT", 268435456},
			{"GKF_WHEELRIGHT", 536870912},

			// GameKey IDs
			{"GK_DOWN", 1},
			{"GK_LEFT", 9},
			{"GK_RIGHT", 17},
			{"GK_UP", 25},
			{"GK_DECISION", 33},
			{"GK_CANCEL", 41},
			{"GK_NUM0", 81},
			{"GK_NUM1", 89},
			{"GK_NUM2", 97},
			{"GK_NUM3", 105},
			{"GK_NUM4", 113},
			{"GK_NUM5", 121},
			{"GK_NUM6", 129},
			{"GK_NUM7", 137},
			{"GK_NUM8", 145},
			{"GK_NUM9", 153},
			{"GK_ADD", 161},
			{"GK_SUB", 169},
			{"GK_MUL", 177},
			{"GK_DIV", 185},
			{"GK_DOT", 193},

			// Gamepad Button Flags
			{"JOY_BUTTON01", 1},
			{"JOY_BUTTON02", 2},
			{"JOY_BUTTON03", 4},
			{"JOY_BUTTON04", 8},
			{"JOY_BUTTON05", 16},
			{"JOY_BUTTON06", 32},
			{"JOY_BUTTON07", 64},
			{"JOY_BUTTON08", 128},
			{"JOY_BUTTON09", 256},
			{"JOY_BUTTON10", 512},
			{"JOY_BUTTON11", 1024},
			{"JOY_BUTTON12", 2048},
			{"JOY_BUTTON13", 4096},
			{"JOY_BUTTON14", 8192},
			{"JOY_BUTTON15", 16384},
			{"JOY_BUTTON16", 32768},
			{"JOY_BUTTON17", 65536},
			{"JOY_BUTTON18", 131072},
			{"JOY_BUTTON19", 262144},
			{"JOY_BUTTON20", 524288},
			{"JOY_BUTTON21", 1048576},
			{"JOY_BUTTON22", 2097152},
			{"JOY_BUTTON23", 4194304},
			{"JOY_BUTTON24", 8388608},
			{"JOY_BUTTON25", 16777216},
			{"JOY_BUTTON26", 33554432},
			{"JOY_BUTTON27", 67108864},
			{"JOY_BUTTON28", 134217728},
			{"JOY_BUTTON29", 268435456},
			{"JOY_BUTTON30", 536870912},
			{"JOY_BUTTON31", 1073741824},
			{"JOY_BUTTON32", -2147483648},

			// Gamepad State
			{"JOYPAD_UNPLUGGED", 1},
			{"JOYPAD_PLUGGED", 0},
			{"JOYPAD_NODRIVER", -1},
			{"JOYPAD_INVALIDPARAM", -2},
			{"JOYPAD_BADDEVICE", -3},
			{"JOYPAD_INVALIDJOY", -4},

			// Gamepad DPad
			{"JOYPOV_CENTER", 65536},
			{"JOYPOV_UP", 0},
			{"JOYPOV_UP_RIGHT", 4500},
			{"JOYPOV_RIGHT_UP", 4500},  // LEGACY
			{"JOYPOV_RIGHT", 9000},
			{"JOYPOV_DOWN_RIGHT", 13500},
			{"JOYPOV_RIGHT_DOWN", 13500},  // LEGACY
			{"JOYPOV_DOWN", 18000},
			{"JOYPOV_DOWN_LEFT", 22500},
			{"JOYPOV_LEFT_DOWN", 22500},  // LEGACY
			{"JOYPOV_LEFT", 27000},
			{"JOYPOV_UP_LEFT", 31500},
			{"JOYPOV_LEFT_UP", 31500},  // LEGACY

			// Inventory and Equipment
			{"ITEM_OWNED", 0},
			{"ITEM_EQUIPPED", 1},

			// Item Properties
			{"ITEMSCOPE_SINGLEALLY", 0},
			{"ITEMSCOPE_ALLALLIES", 1},

			{"INFINITE_USES", 0},

			{"PROJECTILE_KNIFE", 0},
			{"PROJECTILE_BOOMERANG", 1},

			{"PROJECTILESPEED_FAST", 0},
			{"PROJECTILESPEED_MID", 1},
			{"PROJECTILESPEED_SLOW", 2},

			{"WEAPONRANGE_MELEE", 0},
			{"WEAPONRANGE_DISTANT", 1},

			{"WEAPONSCOPE_SINGLE", 0},
			{"WEAPONSCOPE_CENTER", 1},
			{"WEAPONSCOPE_SIMULTANEOUS", 2},
			{"WEAPONSCOPE_CONSECUTIVE", 3},

			// Item Categories
			{"ITEMTYPE_COMMON", 0},
			{"ITEMTYPE_WEAPON", 1},
			{"ITEMTYPE_SHIELD", 2},
			{"ITEMTYPE_ARMOR", 3},
			{"ITEMTYPE_HELMET", 4},
			{"ITEMTYPE_ACCESSORY", 5},
			{"ITEMTYPE_MEDICINE", 6},
			{"ITEMTYPE_BOOK", 7},
			{"ITEMTYPE_MATERIAL", 8},
			{"ITEMTYPE_UNIQUE", 9},
			{"ITEMTYPE_SWITCH", 10},

			// Troop Order
			{"TROOP_ALIGNMENT_MANUAL", 0},
			{"TROOP_ALIGNMENT_AUTOMATIC", 1},

			// Troop Event Page Flags
			{"BATTLECON_SWITCH1", 1},
			{"BATTLECON_SWITCH2", 2},
			{"BATTLECON_VARIABLE", 4},
			{"BATTLECON_TURNS", 8},
			{"BATTLECON_EXHAUSTION", 16},
			{"BATTLECON_ENEMYHP", 32},
			{"BATTLECON_ACTORHP", 64},
			{"BATTLECON_ENEMYTURN", 128},
			{"BATTLECON_ACTORTURN", 256},
			{"BATTLECON_COMMAND", 512},

			// Enemy Pattern
			{"ENEMY_PATTERNTYPE_NORMAL", 0},
			{"ENEMY_PATTERNTYPE_SWITCH", 1},
			{"ENEMY_PATTERNTYPE_ROUND", 2},
			{"ENEMY_PATTERNTYPE_TROOP", 3},
			{"ENEMY_PATTERNTYPE_HP", 4},
			{"ENEMY_PATTERNTYPE_MP", 5},
			{"ENEMY_PATTERNTYPE_PARTYLEVEL", 6},
			{"ENEMY_PATTERNTYPE_PARTYSTATE", 7},

			{"ENEMY_PATTERNCONTENT_ACTION", 0},
			{"ENEMY_PATTERNCONTENT_SKILL", 1},
			{"ENEMY_PATTERNCONTENT_MORPH", 2},

			{"ENEMY_ACTION_NORMALATTACK", 0},
			{"ENEMY_ACTION_DOUBLEATTACK", 1},
			{"ENEMY_ACTION_DEFEND", 2},
			{"ENEMY_ACTION_WATCH", 3},
			{"ENEMY_ACTION_POWERUP", 4},
			{"ENEMY_ACTION_SELFDESTRUCT", 5},
			{"ENEMY_ACTION_ESCAPE", 6},
			{"ENEMY_ACTION_DONOTHING", 7},

			// Actor Properties
			{"ACTOR_LEVEL", 0},
			{"ACTOR_EXP", 1},
			{"ACTOR_HP", 2},
			{"ACTOR_MP", 3},
			{"ACTOR_MAXHP", 4},
			{"ACTOR_MAXMP", 5},
			{"ACTOR_STRENGTHACTOR_ATTACK", 6},
			{"ACTOR_DEFENSE", 7},
			{"ACTOR_MIND", 8},
			{"ACTOR_SPEEDACTOR_AGILITY", 9},
			{"ACTOR_WEAPON", 10},
			{"ACTOR_SHIELD", 11},
			{"ACTOR_ARMOR", 12},
			{"ACTOR_HELMET", 13},
			{"ACTOR_ACCESSORY", 14},

			// Battle Settings
			{"BATTLELAYOUT_CLASSIC", 0},
			{"BATTLELAYOUT_MODIFIED", 1},
			{"BATTLELAYOUT_FACES", 2},

			{"BATTLEWINDOW_LARGE", 0},
			{"BATTLEWINDOW_SMALL", 1},

			{"WINDOWBG_NORMAL", 0},
			{"WINDOWBG_TRANSPARENT", 1},

			{"PARTYFORMATION_MANUAL", 0},
			{"PARTYFORMATION_AUTO", 1},

			{"DEFEAT_GAMEOVER", 0},
			{"DEFEAT_COMMONEVENT", 1},

			{"BATTLECOMMAND_ATTACK", 0},
			{"BATTLECOMMAND_ITEM", 1},
			{"BATTLECOMMAND_DEFEND", 2},
			{"BATTLECOMMAND_ESCAPE", 3},
			{"BATTLECOMMAND_SKILL", 4},
			{"BATTLECOMMAND_SPECIAL", 5},
			{"BATTLECOMMAND_EVENT", 6},

			// Map Configuration
			{"SCROLLTYPE_NORMAL", 0},
			{"SCROLLTYPE_VERTICAL", 1},
			{"SCROLLTYPE_HORIZONTAL", 2},
			{"SCROLLTYPE_BOTH", 3},

			{"CONFIG_TES_FROMPARENT", 0},
			{"CONFIG_TES_ALLOW", 1},
			{"CONFIG_TES_FORBID", 2},

			{"CONFIG_BM_FROMPARENT", 0},
			{"CONFIG_BM_DONTCHANGE", 1},
			{"CONFIG_BM_SPECIFY", 2},

			// MapTree Data
			{"TREE_ENTRY_FOLDER", 0},
			{"TREE_ENTRY_MAP", 1},
			{"TREE_ENTRY_AREA", 2},
			{"TREE_NO_PARENT", -1},

			// Messagebox Buttons (Windows)
			{"MB_IDOK", 1},
			{"MB_IDCANCEL", 2},
			{"MB_IDABORT", 3},
			{"MB_IDRETRY", 4},
			{"MB_IDIGNORE", 5},
			{"MB_IDYES", 6},
			{"MB_IDNO", 7},
			{"MB_IDTRYAGAIN", 10},
			{"MB_IDCONTINUE", 11},

			// MessageBox Flags (Windows)
			{"MB_OK", 0},
			{"MB_OKCANCEL", 1},
			{"MB_ABORTRETRYIGNORE", 2},
			{"MB_YESNOCANCEL", 3},
			{"MB_YESNO", 4},
			{"MB_RETRYCANCEL", 5},
			{"MB_CANCELTRYCONTINUE", 6},

			{"MB_ICONHAND", 16},
			{"MB_ICONERROR", 16},  // REPEAT
			{"MB_ICONSTOP", 16},   // REPEAT

			{"MB_ICONQUESTION", 32},
			{"MB_ICONEXCLAMATION", 48},
			{"MB_ICONWARNING", 48},  // REPEAT
			{"MB_ICONASTERISK", 64},
			{"MB_ICONFORMATION", 64},  // REPEAT

			{"MB_DEFBUTTON1", 0},
			{"MB_DEFBUTTON2", 256},
			{"MB_DEFBUTTON3", 512},
			{"MB_DEFBUTTON4", 768},
			{"MB_HELP", 16384},
			{"MB_APPLMODAL", 0},
			{"MB_SYSTEMMODAL", 4096},
			{"MB_TASKMODAL", 8192},
			{"MB_DEFAULT_DESKTOP_ONLY", 131072},
			{"MB_RIGHT", 524288},
			{"MB_RTLREADING", 1048576},
			{"MB_SETFOREGROUND", 65536},
			{"MB_TOPMOST", 262144},
			{"MB_SERVICE_NOTIFICATION", 2097152},

			// Months
			{"JANUARY", 1},
			{"FEBRUARY", 2},
			{"MARCH", 3},
			{"APRIL", 4},
			{"MAY", 5},
			{"JUNE", 6},
			{"JULY", 7},
			{"AUGUST", 8},
			{"SEPTEMBER", 9},
			{"OCTOBER", 10},
			{"NOVEMBER", 11},
			{"DECEMBER", 12},

			// Music Backups
			{"MUSICBACKUP_EVENT", 0},
			{"MUSICBACKUP_BATTLE_FIELD", 1},
			{"MUSICBACKUP_VEHICLE_WALK", 2},

			// Message Options
			{"MESSAGEFORMAT_NORMAL", 0},
			{"MESSAGEFORMAT_TRANSPARENT", 1},

			{"MESSAGEPOSITION_TOP", 0},
			{"MESSAGEPOSITION_MIDDLE", 1},
			{"MESSAGEPOSITION_BOTTOM", 2},

			{"FACEPOSITION_LEFT", 0},
			{"FACEPOSITION_RIGHT", 1},

			// EnterName Pages
			{"ENTRYPAGE_HIRAGANA", 0},
			{"ENTRYPAGE_KATAKANA", 1},

			// Picture Properties
			{"ACTION_NONE", 0},
			{"ACTION_ROTATION", 1},
			{"ACTION_WAVE", 2},
			{"ACTION_RIPPLE", 2},  // LEGACY
			{"ACTION_SETANGLE", 3},

			{"BLEND_MULTIPLY", 1},
			{"BLEND_ADD", 2},
			{"BLEND_OVERLAY", 3},

			{"PICTUREDELETE_TELEPORT", 1},
			{"PICTUREDELETE_BATTLEEND", 2},
			{"PICTUREAFFECT_TINT", 16},
			{"PICTUREAFFECT_FLASH", 32},
			{"PICTUREAFFECT_SHAKE", 64},

			{"PICTUREMIRROR_HORIZONTAL", 1},
			{"PICTUREMIRROR_VERTICAL", 2},
			{"PICTUREMIRROR_TURN180", 3},

			{"PICTURESHEET_ANIMLOOP", -1},
			{"PICTURESHEET_ANIMDELETE", -2},

			{"FIELDPICTURE_PANORAMA", 1},
			{"FIELDPICTURE_TILESBELOW", 2},
			{"FIELDPICTURE_EVENTSBELOW", 3},
			{"FIELDPICTURE_PASSSAME", 4},
			{"FIELDPICTURE_TILESABOVE", 5},
			{"FIELDPICTURE_EVENTSABOVE", 6},
			{"FIELDPICTURE_WEATHER", 7},
			{"FIELDPICTURE_ANIMATION", 8},
			{"FIELDPICTURE_MESSAGE", 9},
			{"FIELDPICTURE_TIMER", 10},

			{"BATTLEPICTURE_BACKDROP", 1},
			{"BATTLEPICTURE_ANIMATION", 2},
			{"BATTLEPICTURE_WEATHER", 3},
			{"BATTLEPICTURE_WINDOW", 4},
			{"BATTLEPICTURE_TIMER", 5},

			// Skill Properties
			{"SKILLSCOPE_SINGLEENEMY", 0},
			{"SKILLSCOPE_ALLENEMIES", 1},
			{"SKILLSCOPE_USER", 2},
			{"SKILLSCOPE_SELF", 2},  // LEGACY
			{"SKILLSCOPE_SINGLEALLY", 3},
			{"SKILLSCOPE_ALLALLIES", 4},

			{"SKILLMP_NORMAL", 0},
			{"SKILLMP_PERCENT", 1},

			{"ACTORMOVE_NONE", 0},
			{"ACTORMOVE_STEP", 1},
			{"ACTORMOVE_JUMP", 2},
			{"ACTORMOVE_DASH", 3},

			// Skill Categories
			{"SKILLTYPE_NORMAL", 0},
			{"SKILLTYPE_TELEPORT", 1},
			{"SKILLTYPE_ESCAPE", 2},
			{"SKILLTYPE_SWITCH", 3},

			// Socket Data
			{"SOCK_DESTINY", 0},
			{"SOCK_RAW", 1},
			{"NEXT_FREE_SOCKET", -1},
			{"SOCKETFLAG_NOTBLOCKING", 1},

			{"STATE_ERROR", -1},
			{"STATE_CLOSED", 0},
			{"STATE_CONNECTED", 1},
			{"STATE_LISTENING", 2},
			{"STATE_CLOSING", 3},
			{"STATE_CONNECTING", 4},

			// Daylight Savings
			{"DAYLIGHT_UNUSED", 0},
			{"DAYLIGHT_STANDARD", 1},
			{"DAYLIGHT_SUMMER", 2},

			// Game Engine Scenes
			{"SCENE_MAP", 0},
			{"SCENE_MENU", 1},
			{"SCENE_BATTLE", 2},
			{"SCENE_SHOP", 3},
			{"SCENE_NAME", 4},
			{"SCENE_FILE", 5},
			{"SCENE_TITLE", 6},
			{"SCENE_GAMEOVER", 7},
			{"SCENE_DEBUG", 8},
			{"SCENE_FREEZE", 9},

			// Destiny Text Languages
			{"LANGUAGE_GERMAN", 0},
			{"LANGUAGE_ENGLISH", 1},

			// Battler Parameters
			{"PARAMETER_MAXHP", 0},
			{"ABILITY_MAXHP", 0},  // LEGACY
			{"PARAMETER_MAXMP", 1},
			{"ABILITY_MAXMP", 1},  // LEGACY
			{"PARAMETER_STRENGTH", 2},
			{"ABILITY_ATTACK", 2},  // LEGACY
			{"PARAMETER_DEFENSE", 3},
			{"ABILITY_DEFENSE", 3},  // LEGACY
			{"PARAMETER_MIND", 4},
			{"ABILITY_MIND", 4},  // LEGACY
			{"PARAMETER_SPEED", 5},
			{"ABILITY_AGILITY", 5},  // LEGACY

			// Switch Operations
			{"OPERATION_ON", 0},
			{"OPERATION_OFF", 1},
			{"OPERATION_TOGGLE", 2},

			// SystemGrapic Properties
			{"DISPLAY_STRETCH", 0},
			{"DISPLAY_REPEAT", 1},

			{"FONT_RMG2000", 0},
			{"FONT_RM2000", 1},

			// System Music
			{"MUSIC_BATTLE", 0},
			{"MUSIC_VICTORY", 1},
			{"MUSIC_INN", 2},
			{"MUSIC_BOAT", 3},
			{"MUSIC_SHIP", 4},
			{"MUSIC_AIRSHIP", 5},
			{"MUSIC_GAMEOVER", 6},

			// System Sounds
			{"SOUND_CURSOR", 0},
			{"SOUND_DECISION", 1},
			{"SOUND_CANCEL", 2},
			{"SOUND_BUZZER", 3},
			{"SOUND_STARTBATTLE", 4},
			{"SOUND_ESCAPE", 5},
			{"SOUND_ENEMYATTACK", 6},
			{"SOUND_ENEMYDAMAGE", 7},
			{"SOUND_ACTORDAMAGE", 8},
			{"SOUND_ALLYDAMAGE", 8},  // LEGACY
			{"SOUND_EVASION", 9},
			{"SOUND_KNOCKOUT", 10},
			{"SOUND_ENEMYDIE", 10},  // LEGACY
			{"SOUND_ITEMUSE", 11},

			// Keyboard Types
			{"KB_UNKNOWN", 0},
			{"KB_IBM_PC_XT", 1},
			{"KB_OLIVETTI_ICO", 2},
			{"KB_IPM_PC_AT", 3},
			{"KB_IBM_ENHANCED", 4},
			{"KB_NOKIA_1050", 5},
			{"KB_NOKIA_9140", 6},
			{"KB_JAPANESE", 7},

			// Terrain Eventoid Transparency
			{"TERRAIN_EVENT_NORMAL", 0},
			{"TERRAIN_EVENT_THIRD", 1},
			{"TERRAIN_EVENT_HALF", 2},
			{"TERRAIN_EVENT_TRANSPARENT", 3},

			// Text Colors
			{"TEXTCOLOR_DEFAULT", 0},
			{"TEXTCOLOR_BLUE", 1},
			{"TEXTCOLOR_LIGHTORANGE", 2},
			{"TEXTCOLOR_BLACK", 3},
			{"TEXTCOLOR_YELLOW", 4},
			{"TEXTCOLOR_RED", 5},
			{"TEXTCOLOR_PURPLE", 6},
			{"TEXTCOLOR_MAGENTA", 7},
			{"TEXTCOLOR_ORANGE", 8},
			{"TEXTCOLOR_GREEN", 9},
			{"TEXTCOLOR_LIGHTBLUE", 10},
			{"TEXTCOLOR_LIGHTRED", 11},
			{"TEXTCOLOR_OCHER", 12},
			{"TEXTCOLOR_VIOLET", 13},
			{"TEXTCOLOR_GOLD", 14},
			{"TEXTCOLOR_SEAGREEN", 15},
			{"TEXTCOLOR_DARKPURPLE", 16},
			{"TEXTCOLOR_OCEANBLUE", 17},
			{"TEXTCOLOR_TEAL", 18},
			{"TEXTCOLOR_BROWN", 19},

			// Tileset Properties
			{"TILESET_SEQUENCE_BIDI", 0},
			{"TILESET_SEQUENCE_FORWARD", 1},

			{"TILESET_ANIMATION_SLOW", 0},
			{"TILESET_ANIMATION_FAST", 1},

			{"TILEPASS_BLOCKED", 0},
			{"TILEPASS_PASSABLE", 15},
			{"TILEPASS_BOTTOM", 1},
			{"TILEPASS_LEFT", 2},
			{"TILEPASS_RIGHT", 4},
			{"TILEPASS_TOP", 8},
			{"TILEPASS_ABOVE", 16},
			{"TILEPASS_TOPABOVE", 48},
			{"TILEPASS_BRIDGE", 64},

			// Transitions
			{"ERASE_FADEOUT", 0},
			{"ERASE_RANDOMBLOCKS", 1},
			{"ERASE_RANDOMBLOCKSUP", 2},
			{"ERASE_RANDOMBLOCKSDOWN", 3},
			{"ERASE_BLINDCLOSE", 4},
			{"ERASE_STRIPEUPDOWN", 5},
			{"ERASE_STRIPELEFTRIGHT", 6},
			{"ERASE_OUTSIDEINSIDE", 7},
			{"ERASE_INSIDEOUTSIDE", 8},
			{"ERASE_SCROLLUP", 9},
			{"ERASE_SCROLLDOWN", 10},
			{"ERASE_SCROLLLEFT", 11},
			{"ERASE_SCROLLRIGHT", 12},
			{"ERASE_DIVISIONUPDOWN", 13},
			{"ERASE_DIVISIONLEFTRIGHT", 14},
			{"ERASE_DIVISION", 15},
			{"ERASE_ZOOMIN", 16},
			{"ERASE_MOSAIC", 17},
			{"ERASE_ROSTERSCROLL", 18},
			{"ERASE_INSTANT", 19},
			{"ERASE_DONTERASE", 20},

			{"SHOW_FADEIN", 0},
			{"SHOW_RANDOMBLOCKS", 1},
			{"SHOW_RANDOMBLOCKSUP", 2},
			{"SHOW_RANDOMBLOCKSDOWN", 3},
			{"SHOW_BLINDOPEN", 4},
			{"SHOW_STRIPEUPDOWN", 5},
			{"SHOW_STRIPELEFTRIGHT", 6},
			{"SHOW_OUTSIDEINSIDE", 7},
			{"SHOW_INSIDEOUTSIDE", 8},
			{"SHOW_SCROLLDOWN", 9},
			{"SHOW_SCROLLUP", 10},
			{"SHOW_SCROLLRIGHT", 11},
			{"SHOW_SCROLLLEFT", 12},
			{"SHOW_COMBINEUPDOWN", 13},
			{"SHOW_COMBINELEFTRIGHT", 14},
			{"SHOW_COMBINE", 15},
			{"SHOW_ZOOMOUT", 16},
			{"SHOW_MOSAIC", 17},
			{"SHOW_ROSTERSCROLL", 18},
			{"SHOW_INSTANT", 19},

			// Transition Categories
			{"TRANSITION_TELEPORTERASE", 0},
			{"TRANSITION_TELEPORTSHOW", 1},
			{"TRANSITION_BATTLESTARTERASE", 2},
			{"TRANSITION_BATTLESTARTSHOW", 3},
			{"TRANSITION_BATTLEENDERASE", 4},
			{"TRANSITION_BATTLEENDSHOW", 5},

			// Variables Misc Operands
			{"MISC_MONEY", 0},
			{"MISC_TIMERSECONDSLEFT", 1},
			{"MISC_PARTYSIZE", 2},
			{"MISC_NUMBEROFSAVES", 3},
			{"MISC_NUMBEROFBATTLES", 4},
			{"MISC_NUMBEROFVICTORIES", 5},
			{"MISC_NUMBEROFDEFEATS", 6},
			{"MISC_NUMBEROFESCAPES", 7},
			{"MISC_MIDITICK", 8},

			// Variable Operations
			{"OPERATION_SET", 0},
			{"OPERATION_ADD", 1},
			{"OPERATION_SUBTRACT", 2},
			{"OPERATION_MULTIPLY", 3},
			{"OPERATION_DIVIDE", 4},
			{"OPERATION_MODULO", 5},

			// Variable Value Types
			{"VALUETYPE_NUMBER", 0},
			{"VALUETYPE_VARIABLE", 1},
			{"VALUETYPE_VARIABLEINDIRECT", 2},
			{"VALUETYPE_RANDOM", 3},
			{"VALUETYPE_ITEM", 4},
			{"VALUETYPE_ACTOR", 5},
			{"VALUETYPE_EVENT", 6},
			{"VALUETYPE_MISC", 7},

			// VirtualKey Codes
			{"VK_A", 65},
			{"VK_B", 66},
			{"VK_C", 67},
			{"VK_D", 68},
			{"VK_E", 69},
			{"VK_F", 70},
			{"VK_G", 71},
			{"VK_H", 72},
			{"VK_I", 73},
			{"VK_J", 74},
			{"VK_K", 75},
			{"VK_L", 76},
			{"VK_M", 77},
			{"VK_N", 78},
			{"VK_O", 79},
			{"VK_P", 80},
			{"VK_Q", 81},
			{"VK_R", 82},
			{"VK_S", 83},
			{"VK_T", 84},
			{"VK_U", 85},
			{"VK_V", 86},
			{"VK_W", 87},
			{"VK_X", 88},
			{"VK_Y", 89},
			{"VK_Z", 90},

			{"VK_0", 48},
			{"VK_1", 49},
			{"VK_2", 50},
			{"VK_3", 51},
			{"VK_4", 52},
			{"VK_5", 53},
			{"VK_6", 54},
			{"VK_7", 55},
			{"VK_8", 56},
			{"VK_9", 57},

			{"VK_F1", 112},
			{"VK_F2", 113},
			{"VK_F3", 114},
			{"VK_F4", 115},
			{"VK_F5", 116},
			{"VK_F6", 117},
			{"VK_F7", 118},
			{"VK_F8", 119},
			{"VK_F9", 120},
			{"VK_F10", 121},
			{"VK_F11", 122},
			{"VK_F12", 123},

			{"VK_LEFT", 37},
			{"VK_RIGHT", 39},
			{"VK_UP", 38},
			{"VK_DOWN", 40},

			{"VK_TAB", 9},
			{"VK_MENU", 18},
			{"VK_SHIFT", 16},
			{"VK_SPACE", 32},
			{"VK_CONTROL", 17},
			{"VK_RETURN", 13},
			{"VK_ESCAPE", 27},
			{"VK_PAUSE", 19},
			{"VK_PRINT", 42},
			{"VK_SCROLL", 145},
			{"VK_CAPITAL", 20},
			{"VK_BACK", 8},

			{"VK_HOME", 36},
			{"VK_END", 35},
			{"VK_PGUP", 33},
			{"VK_PGDOWN", 34},
			{"VK_INSERT", 45},
			{"VK_DELETE", 46},

			{"VK_LBUTTON", 1},
			{"VK_MBUTTON", 4},
			{"VK_RBUTTON", 2},

			{"VK_NUMLOCK", 144},
			{"VK_NUMPAD0", 96},
			{"VK_NUMPAD1", 97},
			{"VK_NUMPAD2", 98},
			{"VK_NUMPAD3", 99},
			{"VK_NUMPAD4", 100},
			{"VK_NUMPAD5", 101},
			{"VK_NUMPAD6", 102},
			{"VK_NUMPAD7", 103},
			{"VK_NUMPAD8", 104},
			{"VK_NUMPAD9", 105},
			{"VK_ADD", 107},
			{"VK_SUBTRACT", 109},
			{"VK_MULTIPLY", 106},
			{"VK_DIVIDE", 111},
			{"VK_DECIMAL", 110},

			// Vocab
			{"VOCAB_ENEMY_APPEARS", 1},
			{"VOCAB_FIRST_ATTACK", 2},
			{"VOCAB_RUN_SUCCESS", 3},
			{"VOCAB_RUN_FAIL", 4},
			{"VOCAB_BATTLE_WIN", 5},
			{"VOCAB_BATTLE_LOSE", 6},
			{"VOCAB_EXP_GET", 7},
			{"VOCAB_MONEY_GET1", 8},
			{"VOCAB_MONEY_GET2", 9},
			{"VOCAB_ITEM_GET", 10},
			{"VOCAB_ATTACKING", 11},
			{"VOCAB_ACTOR_CRITICAL", 12},
			{"VOCAB_ENEMY_CRITICAL", 13},
			{"VOCAB_DEFENDING", 14},
			{"VOCAB_ENEMY_WAIT", 15},
			{"VOCAB_ENEMY_POWERUP", 16},
			{"VOCAB_ENEMY_EXPLODE", 17},
			{"VOCAB_ENEMY_ESCAPE", 18},
			{"VOCAB_ENEMY_MORPH", 19},
			{"VOCAB_ENEMY_DAMAGE", 20},
			{"VOCAB_ENEMY_EVADE", 21},
			{"VOCAB_ACTOR_DAMAGE", 22},
			{"VOCAB_ACTOR_EVADE", 23},
			{"VOCAB_SKILL_FAIL1", 24},
			{"VOCAB_SKILL_FAIL2", 25},
			{"VOCAB_SKILL_FAIL3", 26},
			{"VOCAB_ATTACK_FAIL", 27},
			{"VOCAB_ITEM_USE", 28},
			{"VOCAB_PARAMETER_NORMALIZE", 29},
			{"VOCAB_PARAMETER_BUFF", 30},
			{"VOCAB_PARAMETER_DEBUFF", 31},
			{"VOCAB_ACTOR_ABSORB", 32},
			{"VOCAB_ENEMY_ABSORB", 33},
			{"VOCAB_ELEMENT_BUFF", 34},
			{"VOCAB_ELEMENT_DEBUFF", 35},
			{"VOCAB_LEVEL_UP", 36},
			{"VOCAB_SKILL_LEARN", 37},

			{"VOCAB_SHOP1_WELCOME1", 85},
			{"VOCAB_SHOP1_WELCOME2", 86},
			{"VOCAB_SHOP1_BUY", 87},
			{"VOCAB_SHOP1_SELL", 88},
			{"VOCAB_SHOP1_LEAVE", 89},
			{"VOCAB_SHOP1_BUY_ITEM", 90},
			{"VOCAB_SHOP1_BUY_AMOUNT", 91},
			{"VOCAB_SHOP1_BUY_DONE", 92},
			{"VOCAB_SHOP1_SELL_ITEM", 93},
			{"VOCAB_SHOP1_SELL_AMOUNT", 94},
			{"VOCAB_SHOP1_SELL_DONE", 95},

			{"VOCAB_SHOP2_WELCOME1", 98},
			{"VOCAB_SHOP2_WELCOME2", 99},
			{"VOCAB_SHOP2_BUY", 100},
			{"VOCAB_SHOP2_SELL", 101},
			{"VOCAB_SHOP2_LEAVE", 102},
			{"VOCAB_SHOP2_BUY_ITEM", 103},
			{"VOCAB_SHOP2_BUY_AMOUNT", 104},
			{"VOCAB_SHOP2_BUY_DONE", 105},
			{"VOCAB_SHOP2_SELL_ITEM", 106},
			{"VOCAB_SHOP2_SELL_AMOUNT", 107},
			{"VOCAB_SHOP2_SELL_DONE", 108},
			{"VOCAB_SHOP3_WELCOME1", 111},
			{"VOCAB_SHOP3_WELCOME2", 112},
			{"VOCAB_SHOP3_BUY", 113},
			{"VOCAB_SHOP3_SELL", 114},
			{"VOCAB_SHOP3_LEAVE", 115},
			{"VOCAB_SHOP3_BUY_ITEM", 116},
			{"VOCAB_SHOP3_BUY_AMOUNT", 117},
			{"VOCAB_SHOP3_BUY_DONE", 118},
			{"VOCAB_SHOP3_SELL_ITEM", 119},
			{"VOCAB_SHOP3_SELL_AMOUNT", 120},
			{"VOCAB_SHOP3_SELL_DONE", 121},
			{"VOCAB_INN1_COSTS1", 124},
			{"VOCAB_INN1_COSTS2", 125},
			{"VOCAB_INN1_QUESTION", 126},
			{"VOCAB_INN1_YES", 127},
			{"VOCAB_INN1_NO", 128},
			{"VOCAB_INN2_COSTS1", 131},
			{"VOCAB_INN2_COSTS2", 132},
			{"VOCAB_INN2_QUESTION", 133},
			{"VOCAB_INN2_YES", 134},
			{"VOCAB_INN2_NO", 135},
			{"VOCAB_LEVEL", 59},
			{"VOCAB_HP", 60},
			{"VOCAB_MP", 61},

			{"VOCAB_NORMAL", 62},
			{"VOCAB_MP_COSTS", 67},

			{"VOCAB_STRENGTH", 68},
			{"VOCAB_DEFENSE", 69},
			{"VOCAB_MIND", 70},
			{"VOCAB_SPEED", 71},

			{"VOCAB_SHORT_LEVEL", 64},
			{"VOCAB_SHORT_HP", 65},
			{"VOCAB_SHORT_MP", 66},
			{"VOCAB_SHORT_EXP", 63},

			{"VOCAB_WEAPON", 72},
			{"VOCAB_SHIELD", 73},
			{"VOCAB_ARMOR", 74},
			{"VOCAB_HELMET", 75},
			{"VOCAB_ACCESSORY", 76},

			{"VOCAB_BATTLECOMMAND_START", 46},
			{"VOCAB_BATTLECOMMAND_AUTO", 47},
			{"VOCAB_BATTLECOMMAND_ESCAPE", 48},
			{"VOCAB_ACTORCOMMAND_ATTACK", 49},
			{"VOCAB_ACTORCOMMAND_DEFEND", 50},

			{"VOCAB_PARTY_ITEMS", 51},
			{"VOCAB_PARTY_SKILLS", 52},
			{"VOCAB_PARTY_EQUIPMENT", 53},
			{"VOCAB_PARTY_SAVE", 54},
			{"VOCAB_PARTY_QUIT", 55},

			{"VOCAB_TITLE_NEWGAME", 56},
			{"VOCAB_TITLE_CONTINUE", 57},
			{"VOCAB_TITLE_SHUTDOWN", 58},

			{"VOCAB_FILESELECT_SAVE", 77},
			{"VOCAB_FILESELECT_LOAD", 78},
			{"VOCAB_EXIT_MESSAGE", 79},
			{"VOCAB_FILE", 80},
			{"VOCAB_EXIT_YES", 81},
			{"VOCAB_EXIT_NO", 82},

			{"VOCAB_ITEMS_INVENTORY", 43},
			{"VOCAB_ITEMS_EQUIPPED", 44},
			{"VOCAB_CURRENCY", 45},

			// Weather Properties
			{"WEATHER_NONE", 0},
			{"WEATHER_RAIN", 1},
			{"WEATHER_SNOW", 2},
			{"WEATHER_FOG", 3},
			{"WEATHER_SANDSTORM", 4},

			{"WEATHERSTRENGTH_WEAK", 0},
			{"WEATHERSTRENGTH_NORMAL", 1},
			{"WEATHERSTRENGTH_STRONG", 2},

			// Angle Formats
			{"DEG", 1},
			{"RAD", 2},
			{"GRAD", 3},
			{"RPG", 4},

			// Weekdays
			{"SUNDAY", 0},
			{"MONDAY", 1},
			{"TUESDAY", 2},
			{"WEDNESDAY", 3},
			{"THURSDAY", 4},
			{"FRIDAY", 5},
			{"SATURDAY", 6},

			// State Properties
			{"STATETYPE_COMBAT", 0},
			{"CONDITIONTYPE_COMBAT", 0},  // LEGACY
			{"STATETYPE_MOVEMENT", 1},
			{"CONDITIONTYPE_MOVEMENT", 1},  // LEGACY

			{"STATELIMITATION_NONE", 0},
			{"CONDITIONLIMITATION_NONE", 0},  // LEGACY
			{"STATELIMITATION_NOACTION", 1},
			{"CONDITIONLIMITATION_NOACTION", 1},  // LEGACY
			{"STATELIMITATION_ATTACKENEMY", 2},
			{"CONDITIONLIMITATION_ATTACKENEMY", 2},  // LEGACY
			{"STATELIMITATION_ATTACKALLY", 3},
			{"CONDITIONLIMITATION_ATTACKALLY", 3}  // LEGACY

		};
		EventCode = {
			{"END", 10},
			{"CallCommonEvent", 1005},
			{"ForceFlee", 1006},
			{"EnableCombo", 1007},
			{"ChangeClass", 1008},
			{"ChangeBattleCommands", 1009},
			{"OpenLoadMenu", 5001},
			{"ExitGame", 5002},
			{"ToggleAtbMode", 5003},
			{"ToggleFullscreen", 5004},
			{"OpenVideoOptions", 5005},
			{"ShowMessage", 10110},
			{"MessageOptions", 10120},
			{"ChangeFaceGraphic", 10130},
			{"ShowChoice", 10140},
			{"InputNumber", 10150},
			{"ControlSwitches", 10210},
			{"ControlVars", 10220},
			{"TimerOperation", 10230},
			{"ChangeGold", 10310},
			{"ChangeItems", 10320},
			{"ChangePartyMembers", 10330},
			{"ChangeExp", 10410},
			{"ChangeLevel", 10420},
			{"ChangeParameters", 10430},
			{"ChangeSkills", 10440},
			{"ChangeEquipment", 10450},
			{"ChangeHP", 10460},
			{"ChangeSP", 10470},
			{"ChangeCondition", 10480},
			{"FullHeal", 10490},
			{"SimulatedAttack", 10500},
			{"ChangeHeroName", 10610},
			{"ChangeHeroTitle", 10620},
			{"ChangeSpriteAssociation", 10630},
			{"ChangeActorFace", 10640},
			{"ChangeVehicleGraphic", 10650},
			{"ChangeSystemBGM", 10660},
			{"ChangeSystemSFX", 10670},
			{"ChangeSystemGraphics", 10680},
			{"ChangeScreenTransitions", 10690},
			{"EnemyEncounter", 10710},
			{"OpenShop", 10720},
			{"ShowInn", 10730},
			{"EnterHeroName", 10740},
			{"Teleport", 10810},
			{"MemorizeLocation", 10820},
			{"RecallToLocation", 10830},
			{"EnterExitVehicle", 10840},
			{"SetVehicleLocation", 10850},
			{"ChangeEventLocation", 10860},
			{"TradeEventLocations", 10870},
			{"StoreTerrainID", 10910},
			{"StoreEventID", 10920},
			{"EraseScreen", 11010},
			{"ShowScreen", 11020},
			{"TintScreen", 11030},
			{"FlashScreen", 11040},
			{"ShakeScreen", 11050},
			{"PanScreen", 11060},
			{"WeatherEffects", 11070},
			{"ShowPicture", 11110},
			{"MovePicture", 11120},
			{"ErasePicture", 11130},
			{"ShowBattleAnimation", 11210},
			{"PlayerVisibility", 11310},
			{"FlashSprite", 11320},
			{"MoveEvent", 11330},
			{"ProceedWithMovement", 11340},
			{"HaltAllMovement", 11350},
			{"Wait", 11410},
			{"PlayBGM", 11510},
			{"FadeOutBGM", 11520},
			{"MemorizeBGM", 11530},
			{"PlayMemorizedBGM", 11540},
			{"PlaySound", 11550},
			{"PlayMovie", 11560},
			{"KeyInputProc", 11610},
			{"ChangeMapTileset", 11710},
			{"ChangePBG", 11720},
			{"ChangeEncounterSteps", 11740},
			{"TileSubstitution", 11750},
			{"TeleportTargets", 11810},
			{"ChangeTeleportAccess", 11820},
			{"EscapeTarget", 11830},
			{"ChangeEscapeAccess", 11840},
			{"OpenSaveMenu", 11910},
			{"ChangeSaveAccess", 11930},
			{"OpenMainMenu", 11950},
			{"ChangeMainMenuAccess", 11960},
			{"ConditionalBranch", 12010},
			{"Label", 12110},
			{"JumpToLabel", 12120},
			{"Loop", 12210},
			{"BreakLoop", 12220},
			{"EndEventProcessing", 12310},
			{"EraseEvent", 12320},
			{"CallEvent", 12330},
			{"Comment", 12410},
			{"GameOver", 12420},
			{"ReturntoTitleScreen", 12510},
			{"ChangeMonsterHP", 13110},
			{"ChangeMonsterMP", 13120},
			{"ChangeMonsterCondition", 13130},
			{"ShowHiddenMonster", 13150},
			{"ChangeBattleBG", 13210},
			{"ShowBattleAnimation_B", 13260},
			{"ConditionalBranch_B", 13310},
			{"TerminateBattle", 13410},
			{"ShowMessage_2", 20110},
			{"ShowChoiceOption", 20140},
			{"ShowChoiceEnd", 20141},
			{"VictoryHandler", 20710},
			{"EscapeHandler", 20711},
			{"DefeatHandler", 20712},
			{"EndBattle", 20713},
			{"Transaction", 20720},
			{"NoTransaction", 20721},
			{"EndShop", 20722},
			{"Stay", 20730},
			{"NoStay", 20731},
			{"EndInn", 20732},
			{"ElseBranch", 22010},
			{"EndBranch", 22011},
			{"EndLoop", 22210},
			{"Comment_2", 22410},
			{"ElseBranch_B", 23310},
			{"EndBranch_B", 23311},
			{"Maniac_GetSaveInfo", 3001},
			{"Maniac_Save", 3002},
			{"Maniac_Load", 3003},
			{"Maniac_EndLoadProcess", 3004},
			{"Maniac_GetMousePosition", 3005},
			{"Maniac_SetMousePosition", 3006},
			{"Maniac_ShowStringPicture", 3007},
			{"Maniac_GetPictureInfo", 3008},
			{"Maniac_ControlBattle", 3009},
			{"Maniac_ControlAtbGauge", 3010},
			{"Maniac_ChangeBattleCommandEx", 3011},
			{"Maniac_GetBattleInfo", 3012},
			{"Maniac_ControlVarArray", 3013},
			{"Maniac_KeyInputProcEx", 3014},
			{"Maniac_RewriteMap", 3015},
			{"Maniac_ControlGlobalSave", 3016},
			{"Maniac_ChangePictureId", 3017},
			{"Maniac_SetGameOption", 3018},
			{"Maniac_CallCommand", 3019},
			{"Maniac_ControlStrings", 3020},
			{"Maniac_GetGameInfo", 3021},
			{"Maniac_EditPicture", 3025},
			{"Maniac_WritePicture", 3026},
			{"Maniac_AddMoveRoute", 3027},
			{"Maniac_EditTile", 3028},
			{"Maniac_ControlTextProcessing", 3029}
		};
	};

	// Function to return contents from either DestinyScript or EventCode
	std::unordered_map<std::string, int>& MapCollection(const std::string& collectionName) {
		if ( Utils::StrICmp(collectionName, "destinyscript") == 0 ) return DestinyScript;
		else if ( Utils::StrICmp(collectionName, "eventcode") == 0 ) return EventCode;

		else return EventCode;
	};

	// Function to retrieve a value from mapCollection ignoring case
	std::string get(const std::string& mapName, const std::string& key, bool convertMode = 0 ) {
		std::unordered_map<std::string, int>& selectedMap = MapCollection(mapName);

		if (convertMode == 0) // Name 2 ID
			for (auto it = selectedMap.begin(); it != selectedMap.end(); ++it)
				if ( Utils::StrICmp(it->first, key) == 0 ) return std::to_string(it->second);

		if (convertMode == 1) // ID 2 Name
			for (auto it = selectedMap.begin(); it != selectedMap.end(); ++it)
				if (Utils::StrICmp(std::to_string(it->second), key) == 0) return it->first;

		return key; // Match not found
	}
};