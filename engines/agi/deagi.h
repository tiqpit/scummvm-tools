#ifndef DEAGI_H
#define DEAGI_H

#include "common/str.h"
#include "common/file.h"
#include "common/array.h"

#define MAX_SCRIPT_SIZE 65536

enum ArgumentType {
	A_NUM = 0,
	A_VAR,
	A_FLG,
	A_STR,
	A_OBJ,
	A_NUL
};

struct Instruction {
	const char *name; ///< Name of the instruction
	int n; ///< Number of arguments
	ArgumentType type[7]; ///< Types of arguments
};

class Disassembler {
public:
	Disassembler(Common::Filename path);
	~Disassembler();

	/**
	 * Set a flag that indicates if we want to output addresses to the left
	 * side of the code or not.
	 */
	void setOutputAddresses(bool v) { _outputAddresses = v; }

	/**
	 * Set a flag that indicates if we want to output data to the right side
	 * of the code or not.
	 */
	void setOutputData(bool v) { _outputData = v; }

	/**
	 * Perform disassembly.
	 */
	void disassemble();

private:
	/**
	 * Decrypt the string section using the "Avis Durgan" key.
	 */
	void decryptStrings();
	
	/**
	 * Parse the string section into an array of strings.
	 */
	void parseStrings();

	/**
	 * Setup AGI v1 instruction and opcode handler mappings.
	 */
	void setupV1();

	/**
	 * Setup AGI v2 instruction and opcode handler mappings.
	 */
	void setupV2();

	/**
	 * Push a position for a closing brace into stack.
	 */
	void pushBrace(int offset) { _braceStack[++_braceIndex] = offset; }
	
	/**
	 * Pop a position for a closing brace from stack.
	 */
	int popBrace() { return _braceStack[_braceIndex--]; }
	
	/**
	 * Return the top of the brace stack.
	 */
	int getBrace() { return _braceStack[_braceIndex]; }

	/**
	 * Return the number of elements in the brace stack.
	 */
	int numberOfBraces() { return _braceIndex; }

	/**
	 * Apply indentation.
	 */
	void indent();

	/**
	 * Output current address of the instruction pointer.
	 */
	void printAddress();

	/**
	 * Output data in hexadecimal up to the current address of the
	 * instruction pointer.
	 *
	 * @param offset  Pointer to data
	 */
	void printData(int offset);

	// Opcode handlers
	typedef void (Disassembler::*OpcodeHandler)();
	void doOpcode();
	void doCondition();
	void doIf();
	void doGoto();
	void doUnknown();

	uint8 *_script; ///< Script file in memory
	int _scriptSize; ///< Script file size

	int _braceStack[32]; ///< Brace stack for handling IF and ELSE blocks
	int _braceIndex; ///< Brace stack pointer

	Instruction *_insMapping[256]; ///< Instruction mapping in use
	Instruction *_insMapping_cond[256]; ///< Instruction mapping for conditional mode
	OpcodeHandler _opMapping[256]; ///< Opcode mapping in use
	OpcodeHandler _opMapping_cond[256]; ///< Opcode mapping for conditional mode

	int _ip; ///< Instruction pointer
	bool _return; ///< Are we done with the disassembly?

	Common::Array<Common::String> _strings; ///< Unencrypted strings
	
	bool _outputAddresses; ///< Do we want to output addresses to the left side of the code?
	bool _outputData; ///< Do we want to output data in hexadecimal to the right side of the code?
};

#endif
