#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deagi.h"
#include "common/endian.h"

Disassembler::Disassembler(Common::Filename path) {
	// Read the whole script file into memory
	Common::File in(path, "rb");
	_scriptSize = in.size();
	if (_scriptSize > MAX_SCRIPT_SIZE) {
		fprintf(stderr, "Invalid script file (%s), file size exceeds 64KB\n", path.getFullPath().c_str());
		exit(1);
	}
	_script = new uint8[MAX_SCRIPT_SIZE];
	memset(_script, 0, MAX_SCRIPT_SIZE);
	in.read_noThrow(_script, _scriptSize);

	// Decrypt and parse the string section
	decryptStrings();
	parseStrings();
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
	}
	
//	for (int i = 0; i < _strings.size(); i++)
//		printf("%s\n", _strings[i].c_str());
}

int main(int argc, char **argv) {
	if (argc <= 1)
		return 1;

	Common::Filename path(argv[1]);
	Disassembler d(path);

	return 0;
}
