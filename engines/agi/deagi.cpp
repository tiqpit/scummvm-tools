#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deagi.h"
#include "common/endian.h"

Instruction insUnknown = { "unknown", 0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } };

Instruction insV1_cond[] = {
	{ "equaln",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 01
	{ "equalv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 02
	{ "lessn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 03
	{ "lessv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 04
	{ "greatern",			2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 05
	{ "greaterv",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 06
	{ "isset",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 07
	{ "issetv",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 08
	{ "...",				4, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL } }, // 09
	{ "posn",				5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 0A
	{ "...",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0B
	{ "...",				2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0C
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0D
	{ "have.key",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0E
	{ "...",				2, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0F
	{ "bit",				2, { A_NUM, A_VAR, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 10
	{ "...",				5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 11
	{ "...",				5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 12
};

Instruction insV1[] = {
	{ "return",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 00
	{ "increment",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 01
	{ "decrement",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 02
	{ "assignn",			2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 03
	{ "assignv",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 04
	{ "addn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 05
	{ "addv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 06
	{ "subn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 07
	{ "subv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 08
	{ "load.view",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 09
	{ "animate.obj",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0A
	{ "new.room",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0B
	{ "draw.pic",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0C
	{ "print",				1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0D
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0E
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0F
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 10
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 11
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 12
	{ "random",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 13
	{ "get",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 14
	{ "drop",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 15
	{ "draw",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 16
	{ "erase",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 17
	{ "position",			3, { A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } }, // 18
	{ "position.v",			3, { A_NUM, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } }, // 19
	{ "get.posn",			3, { A_NUM, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } }, // 1A
	{ "set.cel",			2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 1B
	{ "set.loop",			2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 1C
	{ "end.of.loop",		2, { A_OBJ, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 1D
	{ "reverse.loop",		2, { A_OBJ, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 1E
	{ "move.obj",			5, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 1F
	{ "set.view",			2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 20
	{ "follow.ego",			3, { A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } }, // 21
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 22
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 23
	{ "ignore.blocks",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 24
	{ "observe.blocks",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 25
	{ "wander",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 26
	{ "reposition",			3, { A_NUM, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } }, // 27
	{ "stop.motion",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 28
	{ "start.motion",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 29
	{ "stop.cycling",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2A
	{ "start.cycling",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2B
	{ "stop.update",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2C
	{ "start.update",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2D
	{ "program.control",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2E
	{ "player.control",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 2F
	{ "set.priority",		2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 30
	{ "release.priority",	1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 31
	{ "add.to.pic",			6, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL } }, // 32
	{ "set.horizon",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 33
	{ "ignore.horizon",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 34
	{ "observe.horizon",	1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 35
	{ "load.logics",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 36
	{ "object.on.water",	1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 37
	{ "load.pic",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 38
	{ "load.sound",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 39
	{ "sound",				2, { A_NUM, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3A
	{ "stop.sound",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3B
	{ "set",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3C
	{ "reset",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3D
	{ "...",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3E
	{ "new.room.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 3F
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 40
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 41
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 42
	{ "move.obj.v",			5, { A_OBJ, A_VAR, A_VAR, A_VAR, A_VAR, A_NUL, A_NUL } }, // 43
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 44
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 45
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 46
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 47
	{ "...",				2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 48 get.priority??
	{ "ignore.objs",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 49
	{ "observe.objs",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4A
	{ "distance",			3, { A_NUM, A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4B
	{ "object.on.land",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4C
	{ "...",				2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4D set.priority.v???
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4E
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 4F
	{ "display",			4, { A_NUM, A_NUM, A_NUM, A_STR, A_NUL, A_NUL, A_NUL } }, // 50
	{ "prevent.input???",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 51
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 52
	{ "...",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 53 ???
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 54 ???
	{ "stop.motion",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 55 or force.update??
	{ "discard.view",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 56
	{ "discard.pic",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 57
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 58
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 59
	{ "last.cel",			2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5A
	{ "set.cel.v",			2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5B
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5C
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5D
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5E
	{ "...",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 5F
	{ "setbit",				2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 60
};

Instruction insV2_cond[] = {
	{ "equaln",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 01
	{ "equalv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 02
	{ "lessn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 03
	{ "lessv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 04
	{ "greatern",			2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 05
	{ "greaterv",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 06
	{ "isset",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 07
	{ "issetv",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 08
	{ "has",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 09
	{ "obj.in.room",		2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0A
	{ "posn",				5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 0B
	{ "controller",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0C
	{ "have.key",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0D
	{ "said",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0E
	{ "compare.strings",	2, { A_STR, A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } }, // 0F
	{ "obj.in.box",			5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 10
	{ "center.posn",		5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 11
	{ "right.posn",			5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } }, // 12
};

Instruction insV2[] = {
	{ "return",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "increment",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "decrement",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "assignn",			2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "assignv",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "addn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "addv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "subn",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "subv",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "lindirectv",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "lindirect",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "lindirectn",			2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reset",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "toggle",				1, { A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.v",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reset.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "toggle.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "new.room",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "new.room.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.logics",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.logics.v",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "call",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "call.v",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.pic",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "draw.pic",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.pic",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "discard.pic",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "overlay.pic",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.pri.screen",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.view",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.view.v",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "discard.view",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "animate.obj",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "unanimate.all",		0, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "draw",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "erase",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "position",			3, { A_OBJ, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "position.v",			3, { A_OBJ, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.posn",			3, { A_OBJ, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reposition",			3, { A_OBJ, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.view",			2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.view.v",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.loop",			2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.loop.v",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "fix.loop",			1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "release.loop",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.cel",			2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.cel.v",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "last.cel",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "current.cel",		2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "current.loop",		2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "current.view",		2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "number.of.loops",	2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.priority",		2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.priority.v",		2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "release.priority",	1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.priority",		2, { A_OBJ, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "stop.update",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "start.update",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "force.update",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "ignore.horizon",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "observe.horizon",	1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.horizon",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "object.on.water",	1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "object.on.land",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "object.on.anything",	1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "ignore.objs",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "observe.objs",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "distance",			3, { A_OBJ, A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "stop.cycling",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "start.cycling",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "normal.cycle",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "end.of.loop",		2, { A_OBJ, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reverse.cycle",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reverse.loop",		2, { A_OBJ, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "cycle.time",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "stop.motion",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "start.motion",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "step.size",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "step.time",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "move.obj",			5, { A_OBJ, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } },
	{ "move.obj.v",			5, { A_OBJ, A_VAR, A_VAR, A_VAR, A_VAR, A_NUL, A_NUL } },
	{ "follow.ego",			3, { A_OBJ, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "wander",				1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "normal.motion",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.dir",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.dir",			2, { A_OBJ, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "ignore.blocks",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "observe.blocks",		1, { A_OBJ, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "block",				4, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL } },
	{ "unblock",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.v",				1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "drop",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "put",				2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "put.v",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.room.v",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "load.sound",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "sound",				2, { A_NUM, A_FLG, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "stop.sound",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "print",				1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "print.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "display",			3, { A_NUM, A_NUM, A_STR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "display.v",			3, { A_VAR, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "clear.lines",		3, { A_NUM, A_NUM, A_STR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "text.screen",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "graphics",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.cursor.char",	1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.text.attribute",	2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "shake.screen",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "configure.screen",	3, { A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "status.line.on",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "status.line.off",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.string",			2, { A_NUM, A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.string",			2, { A_NUM, A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "word.to.string",		2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "parse",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "get.num",			2, { A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "prevent.input",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "accept.input",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.key",			3, { A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "add.to.pic",			7, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUM } },
	{ "add.to.pic.v",		7, { A_VAR, A_VAR, A_VAR, A_VAR, A_VAR, A_VAR, A_VAR } },
	{ "status",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "save.game",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "restore.game",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "init.disk",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "restart.game",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.obj",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "random",				3, { A_NUM, A_NUM, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "program.control",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "player.control",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "obj.status.v",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "quit",				1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.mem",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "pause",				0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "echo.line",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "cancel.line",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "init.joy",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "toggle.monitor",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "version",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "script.size",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.game.id",		1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "log",				1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.scan.start",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reset.scan.start",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reposition.to",		3, { A_OBJ, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "reposition.to.v",	3, { A_OBJ, A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "trace.on",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "trace.info", 		3, { A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "print.at",			4, { A_STR, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL } },
	{ "print.at.v",			4, { A_VAR, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL } },
	{ "discard.view.v",		1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "clear.text.rect",	5, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL } },
	{ "set.upper.left",		2, { A_NUM, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.menu",			1, { A_STR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.menu.member",	2, { A_STR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "submit.menu",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "enable.member",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "disable.member",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "menu.input",			0, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.obj.v",			1, { A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "open.dialogue",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "close.dialogue",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "mul.n",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "mul.v",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "div.n",				2, { A_VAR, A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "div.v",				2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "close.window",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.simple",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "push.script",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "pop.script",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "hold.key",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "set.pri.base",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "discard.sound",		1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "hide.mouse",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "allow.menu",			1, { A_NUM, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "show.mouse",			0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "fence.mouse",		4, { A_NUM, A_NUM, A_NUM, A_NUM, A_NUL, A_NUL, A_NUL } },
	{ "mouse.posn",			2, { A_VAR, A_VAR, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "release.key",		0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
	{ "adj.ego.move.to.xy",	0, { A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL, A_NUL } },
};

Disassembler::Disassembler(Common::Filename path) {
	// Read the whole script file into memory
	Common::File in(path, "rb");
	_scriptSize = in.size();
	if (_scriptSize > MAX_SCRIPT_SIZE) {
		fprintf(stderr, "Invalid script (%s), file size exceeds 64KB\n", path.getFullPath().c_str());
		exit(1);
	}
	_script = new uint8[MAX_SCRIPT_SIZE];
	memset(_script, 0, MAX_SCRIPT_SIZE);
	in.read_noThrow(_script, _scriptSize);

	// Decrypt and parse the string section
	decryptStrings();
	parseStrings();

	// Setup opcode mapping
	setupV2();

	// Do further initialization
	_braceIndex = 0;
	_outputAddresses = true;
	_outputData = true;
}

Disassembler::~Disassembler() {
	delete[] _script;
}

void Disassembler::decryptStrings() {
	const char *avis = "Avis Durgan";
	int offset = READ_LE_UINT16(_script) + 2;
	int n = _script[offset];
	int endptr = READ_LE_UINT16(_script + offset + 1) + offset + 3;
	int i = offset + 3 + n * 2;
	int ai = 0;

	while (i < endptr)
		_script[i++] ^= avis[ai++ % 11];
}

void Disassembler::parseStrings() {
	int offset = READ_LE_UINT16(_script) + 2;
	int n = _script[offset];
	offset += 3;

	for (int i = 0; i < n; i++) {
		int pointer = READ_LE_UINT16(_script + offset + i * 2) - 2;
		if (pointer >= 0 && offset + pointer < _scriptSize)
			// Unencrypted strings each end with a zero
			_strings.push_back(Common::String((char*)(_script + offset + pointer)));
		else
			_strings.push_back(Common::String(""));
	}
}

void Disassembler::setVersion(int version) {
	if (version == 1)
		setupV1();
	else
		setupV2();
}

void Disassembler::setupV1() {
	for (int i = 0; i < 256; i++) {
		_insMapping[i] = &insUnknown;
		_insMapping_cond[i] = &insUnknown;
		_opMapping[i] = &Disassembler::doUnknown;
		_opMapping_cond[i] = &Disassembler::doUnknown;
	}
	for (int i = 0; i < (int)(sizeof(insV1) / sizeof(Instruction)); i++) {
		_insMapping[i] = &insV1[i];
		_opMapping[i] = &Disassembler::doOpcode;
	}
	for (int i = 0; i < (int)(sizeof(insV1_cond) / sizeof(Instruction)); i++) {
		_insMapping_cond[1 + i] = &insV1_cond[i];
		_opMapping_cond[1 + i] = &Disassembler::doCondition;
	}

	_opMapping[0xFE] = &Disassembler::doGoto;
	_opMapping[0xFF] = &Disassembler::doIf;
	
//	_opMapping_cond[0xF] = &Disassembler::doSaid;
}

void Disassembler::setupV2() {
	for (int i = 0; i < 256; i++) {
		_insMapping[i] = &insUnknown;
		_insMapping_cond[i] = &insUnknown;
		_opMapping[i] = &Disassembler::doUnknown;
		_opMapping_cond[i] = &Disassembler::doUnknown;
	}
	for (int i = 0; i < (int)(sizeof(insV2) / sizeof(Instruction)); i++) {
		_insMapping[i] = &insV2[i];
		_opMapping[i] = &Disassembler::doOpcode;
	}
	for (int i = 0; i < (int)(sizeof(insV2_cond) / sizeof(Instruction)); i++) {
		_insMapping_cond[1 + i] = &insV2_cond[i];
		_opMapping_cond[1 + i] = &Disassembler::doCondition;
	}

	_opMapping[0xFE] = &Disassembler::doGoto;
	_opMapping[0xFF] = &Disassembler::doIf;

	_opMapping_cond[0xF] = &Disassembler::doSaid;
}

void Disassembler::doOpcode() {
	int op = _script[_ip++];
	int n = _insMapping[op]->n;

	indent();
	printf("%s(", _insMapping[op]->name);
	for (int i = 0; i < n; i++) {
		switch (_insMapping[op]->type[i]) {
		case A_VAR:
			printf("v%d", _script[_ip++]);
			break;
		case A_STR:
			printf("\"%s\"", _strings[_script[_ip++] - 1].c_str());
			break;
		default:
			printf("%d", _script[_ip++]);
			break;
		}

		if (i < n - 1)
			printf(",");
	}
	printf(")");
}

void Disassembler::doUnknown() {
	int op = _script[_ip++];
	printf("Unknown opcode 0x%.2X\n", op);
	exit(1);
}

void Disassembler::doGoto() {
	_ip++;

	if (numberOfBraces() > 0 && getBrace() == _ip + 2) {
		// ELSE
		int offset = (int16)READ_LE_UINT16(_script + _ip) + _ip + 2;
		// We would need to do another pass through the code to correctly
		// make a difference between ELSE and GOTO. For now, we just print
		// the goto also in the case of a possible ELSE block.
		indent();
		printf("goto(0x%.4X)", offset);
		_ip += 2; printData(_ip - 3); _ip -= 2;
		printf("\n");
		popBrace();
		printAddress();
		indent();
		printf("} else {");
		pushBrace(offset);
	} else {
		// GOTO
		indent();
		printf("goto(0x%.4X)", (int16)READ_LE_UINT16(_script + _ip) + _ip + 2);
	}

	_ip += 2;
}

void Disassembler::doSaid() {
	int n = _script[++_ip];
	_ip++;
	printf("said(");
	for (int i = 0; i < n; i++) {
		_ip += 2;
		if (i > 0)
			printf(",");
		printf("word");
	}
	printf(")");
}

void Disassembler::doCondition() {
	int op = _script[_ip++];
	int n = _insMapping_cond[op]->n;

	printf("%s(", _insMapping_cond[op]->name);
	for (int i = 0; i < n; i++) {
		switch (_insMapping_cond[op]->type[i]) {
		case A_VAR:
			printf("v%d", _script[_ip++]);
			break;
		case A_STR:
			printf("\"%s\"", _strings[_script[_ip++] - 1].c_str());
			break;
		default:
			printf("%d", _script[_ip++]);
			break;
		}

		if (i < n - 1)
			printf(",");
	}
	printf(")");
}

void Disassembler::doIf() {
	bool parensOpen = false;
	bool first = true;
	int op, last_op;

	indent();
	printf("if (");
	_ip++;
	while ((op = _script[_ip]) != 0xFF) {
		if (!first)
			if (!(last_op == 0xFC || (parensOpen && op == 0xFC)))
				printf(" %s ", parensOpen ? "||" : "&&");
		last_op = op;

		if (op == 0xFC) {
			if (parensOpen) {
				parensOpen = false;
				printf(")");
			} else {
				parensOpen = true;
				printf("(");
			}
			_ip++;
			continue;
		}

		if (op == 0xFD) {
			printf("!");
			op = _script[++_ip];
		}

		(this->*_opMapping_cond[op])();
		first = false;
	}
	printf(") {");
	_ip++;
	pushBrace((int16)READ_LE_UINT16(_script + _ip) + _ip + 2);
	_ip += 2;
}

void Disassembler::indent() {
	for (int i = 0; i < numberOfBraces() * 4; i++)
		printf(" ");
}

void Disassembler::printAddress() {
	if (_outputAddresses)
		printf("%.4X  ", _ip);
}

void Disassembler::printData(int offset) {
	if (!_outputData)
		return;
	
	printf("  // ");
	while (offset < _ip)
		printf("%.2X", _script[offset++]);
}

void Disassembler::disassemble() {
	_ip = 2;
	_return = false;
	while (_return == false) {
		int op = _script[_ip];

		// Process next line
		printAddress();
		int previous = _ip;
		(this->*_opMapping[op])();
		printData(previous);
		printf("\n");

		// Are we done?
		if ((numberOfBraces() == 0) && (op == 0))
			_return = true;

		// Time for a closing brace or two?
		while (numberOfBraces() > 0 && getBrace() == _ip) {
			printAddress();
			popBrace();
			indent();
			printf("}\n");
		}
		
		// Skip braces in case of a false ELSE (GOTO).
		while (numberOfBraces() > 0 && getBrace() < _ip)
			popBrace();
	}
}

int main(int argc, char **argv) {
	if (argc <= 1)
		return 1;

	Common::Filename path(argv[1]);
	Disassembler d(path);
	d.setVersion(1);
	d.disassemble();

	return 0;
}
