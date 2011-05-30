#ifndef DEAGI_H
#define DEAGI_H

#include "common/str.h"
#include "common/file.h"
#include "common/array.h"

#define MAX_SCRIPT_SIZE 65536

class Disassembler {
public:
	Disassembler(Common::Filename path);
	~Disassembler();

private:
	/**
	 * Decrypt the string section using the "Avis Durgan" key.
	 */
	void decryptStrings();
	
	/**
	 * Parse the string section into an array of strings.
	 */
	void parseStrings();

	uint8 *_script;
	int _scriptSize;

	Common::Array<Common::String> _strings;
};

#endif
