#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deagi.h"
#include "common/endian.h"

Instruction insUnknown = { "unknown", "" };

Instruction insV1_cond[] = {
	{ "equaln",				"vn" },		// 01
	{ "equalv",				"vv" },		// 02
	{ "lessn",				"vn" },		// 03
	{ "lessv",				"vv" },		// 04
	{ "greatern",			"vn" },		// 05
	{ "greaterv",			"vv" },		// 06
	{ "isset",				"n" },		// 07
	{ "issetv",				"v" },		// 08
	{ "...",				"nnnn" },	// 09
	{ "posn",				"nnnnn" },	// 0A
	{ "controller",			"n" },		// 0B ???
	{ "...",				"nn" },		// 0C
	{ "...",				"" },		// 0D
	{ "have.key",			"" },		// 0E
	{ "...",				"nn" },		// 0F
	{ "bit",				"nv" },		// 10
	{ "...",				"nnnnn" },	// 11
	{ "...",				"nnnnn" },	// 12
};

Instruction insV1[] = {
	{ "return",				"" },		// 00
	{ "increment",			"v" },		// 01
	{ "decrement",			"v" },		// 02
	{ "assignn",			"vn" },		// 03
	{ "assignv",			"vv" },		// 04
	{ "addn",				"vn" },		// 05
	{ "addv",				"vv" },		// 06
	{ "subn",				"vn" },		// 07
	{ "subv",				"vv" },		// 08
	{ "load.view",			"n" },		// 09
	{ "animate.obj",		"n" },		// 0A
	{ "new.room",			"n" },		// 0B
	{ "draw.pic",			"v" },		// 0C
	{ "print",				"s" },		// 0D
	{ "status",				"" },		// 0E
	{ "save.game",			"" },		// 0F
	{ "restore.game",		"" },		// 10
	{ "...",				"" },		// 11
	{ "restart.game",		"" },		// 12
	{ "random",				"v" },		// 13
	{ "get",				"n" },		// 14
	{ "drop",				"n" },		// 15
	{ "draw",				"n" },		// 16
	{ "erase",				"n" },		// 17
	{ "position",			"nnn" },	// 18
	{ "position.v",			"nvv" },	// 19
	{ "get.posn",			"nvv" },	// 1A
	{ "set.cel",			"nn" },		// 1B
	{ "set.loop",			"nn" },		// 1C
	{ "end.of.loop",		"nn" },		// 1D
	{ "reverse.loop",		"nn" },		// 1E
	{ "move.obj",			"nnnnn" },	// 1F
	{ "set.view",			"nn" },		// 20
	{ "follow.ego",			"nnn" },	// 21
	{ "...",				"" },		// 22
	{ "...",				"" },		// 23
	{ "ignore.blocks",		"n" },		// 24
	{ "observe.blocks",		"n" },		// 25
	{ "wander",				"n" },		// 26
	{ "reposition",			"nvv" },	// 27
	{ "stop.motion",		"n" },		// 28
	{ "start.motion",		"n" },		// 29
	{ "stop.cycling",		"n" },		// 2A
	{ "start.cycling",		"n" },		// 2B
	{ "stop.update",		"n" },		// 2C
	{ "start.update",		"n" },		// 2D
	{ "program.control",	"" },		// 2E
	{ "player.control",		"" },		// 2F
	{ "set.priority",		"nn" },		// 30
	{ "release.priority",	"n" },		// 31
	{ "add.to.pic",			"nnnnnn" },	// 32
	{ "set.horizon",		"n" },		// 33
	{ "ignore.horizon",		"n" },		// 34
	{ "observe.horizon",	"n" },		// 35
	{ "load.logics",		"n" },		// 36
	{ "object.on.water",	"n" },		// 37
	{ "load.pic",			"v" },		// 38
	{ "load.sound",			"n" },		// 39
	{ "sound",				"nn" },		// 3A
	{ "stop.sound",			"" },		// 3B
	{ "set",				"n" },		// 3C
	{ "reset",				"n" },		// 3D
	{ "...",				"n" },		// 3E
	{ "new.room.v",			"v" },		// 3F
	{ "...",				"n" },		// 40
	{ "...",				"" },		// 41
	{ "...",				"v" },		// 42
	{ "move.obj.v",			"nvvvv" },	// 43
	{ "...",				"" },		// 44
	{ "...",				"" },		// 45
	{ "...",				"" },		// 46
	{ "...",				"" },		// 47
	{ "...",				"nv" },		// 48 get.priority??
	{ "ignore.objs",		"n" },		// 49
	{ "observe.objs",		"n" },		// 4A
	{ "distance",			"nnv" },	// 4B
	{ "object.on.land",		"n" },		// 4C
	{ "...",				"nv" },		// 4D set.priority.v???
	{ "...",				"" },		// 4E
	{ "...",				"n" },		// 4F
	{ "display",			"nnns" },	// 50
	{ "prevent.input???",	"" },		// 51
	{ "...",				"" },		// 52
	{ "...",				"n" },		// 53 ???
	{ "...",				"" },		// 54 ???
	{ "stop.motion",		"" },		// 55 or force.update??
	{ "discard.view",		"n" },		// 56
	{ "discard.pic",		"v" },		// 57
	{ "...",				"nn" },		// 58
	{ "...",				"" },		// 59
	{ "last.cel",			"nv" },		// 5A
	{ "set.cel.v",			"nv" },		// 5B
	{ "...",				"" },		// 5C
	{ "...",				"n" },		// 5D
	{ "...",				"" },		// 5E
	{ "...",				"" },		// 5F
	{ "setbit",				"nv" },		// 60
};

Instruction insV2_cond[] = {
	{ "equaln",				"vn" },		// 01
	{ "equalv",				"vv" },		// 02
	{ "lessn",				"vn" },		// 03
	{ "lessv",				"vv" },		// 04
	{ "greatern",			"vn" },		// 05
	{ "greaterv",			"vv" },		// 06
	{ "isset",				"n" },		// 07
	{ "issetv",				"v" },		// 08
	{ "has",				"n" },		// 09
	{ "obj.in.room",		"nv" },		// 0A
	{ "posn",				"nnnnn" },	// 0B
	{ "controller",			"n" },		// 0C
	{ "have.key",			"" },		// 0D
	{ "said",				"" },		// 0E
	{ "compare.strings",	"ss" },		// 0F
	{ "obj.in.box",			"nnnnn" },	// 10
	{ "center.posn",		"nnnnn" },	// 11
	{ "right.posn",			"nnnnn" },	// 12
};

Instruction insV2[] = {
	{ "return",				"" },
	{ "increment",			"v" },
	{ "decrement",			"v" },
	{ "assignn",			"vn" },
	{ "assignv",			"vv" },
	{ "addn",				"vn" },
	{ "addv",				"vv" },
	{ "subn",				"vn" },
	{ "subv",				"vv" },
	{ "lindirectv",			"vv" },
	{ "lindirect",			"vv" },
	{ "lindirectn",			"vn" },
	{ "set",				"n" },
	{ "reset",				"n" },
	{ "toggle",				"n" },
	{ "set.v",				"v" },
	{ "reset.v",			"v" },
	{ "toggle.v",			"v" },
	{ "new.room",			"n" },
	{ "new.room.v",			"v" },
	{ "load.logics",		"n" },
	{ "load.logics.v",		"v" },
	{ "call",				"n" },
	{ "call.v",				"v" },
	{ "load.pic",			"v" },
	{ "draw.pic",			"v" },
	{ "show.pic",			"" },
	{ "discard.pic",		"v" },
	{ "overlay.pic",		"v" },
	{ "show.pri.screen",	"" },
	{ "load.view",			"n" },
	{ "load.view.v",		"v" },
	{ "discard.view",		"n" },
	{ "animate.obj",		"n" },
	{ "unanimate.all",		"" },
	{ "draw",				"n" },
	{ "erase",				"n" },
	{ "position",			"nnn" },
	{ "position.v",			"nvv" },
	{ "get.posn",			"nvv" },
	{ "reposition",			"nvv" },
	{ "set.view",			"nn" },
	{ "set.view.v",			"nv" },
	{ "set.loop",			"nn" },
	{ "set.loop.v",			"nv" },
	{ "fix.loop",			"n" },
	{ "release.loop",		"n" },
	{ "set.cel",			"nn" },
	{ "set.cel.v",			"nv" },
	{ "last.cel",			"nv" },
	{ "current.cel",		"nv" },
	{ "current.loop",		"nv" },
	{ "current.view",		"nv" },
	{ "number.of.loops",	"nv" },
	{ "set.priority",		"nn" },
	{ "set.priority.v",		"nv" },
	{ "release.priority",	"n" },
	{ "get.priority",		"nn" },
	{ "stop.update",		"n" },
	{ "start.update",		"n" },
	{ "force.update",		"n" },
	{ "ignore.horizon",		"n" },
	{ "observe.horizon",	"n" },
	{ "set.horizon",		"n" },
	{ "object.on.water",	"n" },
	{ "object.on.land",		"n" },
	{ "object.on.anything",	"n" },
	{ "ignore.objs",		"n" },
	{ "observe.objs",		"n" },
	{ "distance",			"nnv" },
	{ "stop.cycling",		"n" },
	{ "start.cycling",		"n" },
	{ "normal.cycle",		"n" },
	{ "end.of.loop",		"nn" },
	{ "reverse.cycle",		"n" },
	{ "reverse.loop",		"nn" },
	{ "cycle.time",			"nv" },
	{ "stop.motion",		"n" },
	{ "start.motion",		"n" },
	{ "step.size",			"nv" },
	{ "step.time",			"nv" },
	{ "move.obj",			"nnnnn" },
	{ "move.obj.v",			"nvvvv" },
	{ "follow.ego",			"nnn" },
	{ "wander",				"n" },
	{ "normal.motion",		"n" },
	{ "set.dir",			"nv" },
	{ "get.dir",			"nv" },
	{ "ignore.blocks",		"n" },
	{ "observe.blocks",		"n" },
	{ "block",				"nnnn" },
	{ "unblock",			"" },
	{ "get",				"n" },
	{ "get.v",				"v" },
	{ "drop",				"n" },
	{ "put",				"nn" },
	{ "put.v",				"vv" },
	{ "get.room.v",			"vv" },
	{ "load.sound",			"n" },
	{ "sound",				"nn" },
	{ "stop.sound",			"" },
	{ "print",				"s" },
	{ "print.v",			"v" },
	{ "display",			"nns" },
	{ "display.v",			"vvv" },
	{ "clear.lines",		"nns" },
	{ "text.screen",		"" },
	{ "graphics",			"" },
	{ "set.cursor.char",	"s" },
	{ "set.text.attribute",	"nn" },
	{ "shake.screen",		"n" },
	{ "configure.screen",	"nnn" },
	{ "status.line.on",		"" },
	{ "status.line.off",	"" },
	{ "set.string",			"ns" },
	{ "get.string",			"ns" },
	{ "word.to.string",		"nn" },
	{ "parse",				"n" },
	{ "get.num",			"nv" },
	{ "prevent.input",		"" },
	{ "accept.input",		"" },
	{ "set.key",			"nnn" },
	{ "add.to.pic",			"nnnnnnn" },
	{ "add.to.pic.v",		"vvvvvvv" },
	{ "status",				"" },
	{ "save.game",			"" },
	{ "restore.game",		"" },
	{ "init.disk",			"" },
	{ "restart.game",		"" },
	{ "show.obj",			"n" },
	{ "random",				"nnv" },
	{ "program.control",	"" },
	{ "player.control",		"" },
	{ "obj.status.v",		"v" },
	{ "quit",				"n" },
	{ "show.mem",			"" },
	{ "pause",				"" },
	{ "echo.line",			"" },
	{ "cancel.line",		"" },
	{ "init.joy",			"" },
	{ "toggle.monitor",		"" },
	{ "version",			"" },
	{ "script.size",		"n" },
	{ "set.game.id",		"s" },
	{ "log",				"s" },
	{ "set.scan.start",		"" },
	{ "reset.scan.start",	"" },
	{ "reposition.to",		"nnn" },
	{ "reposition.to.v",	"nvv" },
	{ "trace.on",			"" },
	{ "trace.info", 		"nnn" },
	{ "print.at",			"snnn" },
	{ "print.at.v",			"vnnn" },
	{ "discard.view.v",		"v" },
	{ "clear.text.rect",	"nnnnn" },
	{ "set.upper.left",		"nn" },
	{ "set.menu",			"s" },
	{ "set.menu.member",	"sn" },
	{ "submit.menu",		"" },
	{ "enable.member",		"n" },
	{ "disable.member",		"n" },
	{ "menu.input",			"" },
	{ "show.obj.v",			"v" },
	{ "open.dialogue",		"" },
	{ "close.dialogue",		"" },
	{ "mul.n",				"vn" },
	{ "mul.v",				"vv" },
	{ "div.n",				"vn" },
	{ "div.v",				"vv" },
	{ "close.window",		"" },
	{ "set.simple",			"n" },
	{ "push.script",		"" },
	{ "pop.script",			"" },
	{ "hold.key",			"" },
	{ "set.pri.base",		"n" },
	{ "discard.sound",		"n" },
	{ "hide.mouse",			"" },
	{ "allow.menu",			"n" },
	{ "show.mouse",			"" },
	{ "fence.mouse",		"nnnn" },
	{ "mouse.posn",			"vv" },
	{ "release.key",		"" },
	{ "adj.ego.move.to.xy",	"" },
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
	int n = _insMapping[op]->getArgumentsLength();

	indent();
	printf("%s(", _insMapping[op]->name);
	for (int i = 0; i < n; i++) {
		switch (_insMapping[op]->args[i]) {
		case 'v':
			printf("v%d", _script[_ip++]);
			break;
		case 's':
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
	int n = _insMapping_cond[op]->getArgumentsLength();

	printf("%s(", _insMapping_cond[op]->name);
	for (int i = 0; i < n; i++) {
		switch (_insMapping_cond[op]->args[i]) {
		case 'v':
			printf("v%d", _script[_ip++]);
			break;
		case 's':
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
