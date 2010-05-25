/* ScummVM Tools
* Copyright (C) 2010 The ScummVM project
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* $URL$
* $Id$
*
*/

#ifndef DEC_DISASSEMBLER_H
#define DEC_DISASSEMBLER_H

#include <vector>

#include "instruction.h"
#include "common/file.h"

class Disassembler {
protected:
	Common::File f; ///<Used to perform file I/O.

public:
	/**
	 * Open a file for disassembly.		
	 * @param filename 
	 */
	void open(char *filename);

	/**
	 * Disassembles a file.
	 */
	virtual std::vector<Instruction> disassemble() = 0;
};

#endif
