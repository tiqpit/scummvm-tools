/* extract_bc - Extract resources from the booter versions of Black Cauldron
 * Copyright (C) 2011  The ScummVM Team
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
 */

#ifndef EXTRACT_BC_H
#define EXTRACT_BC_H

#include "tool.h"

class ExtractBC : public Tool {
public:
	ExtractBC(const std::string &name = "extract_bc");
	virtual void execute();

	bool readDirEntry(Common::File &, int *, int *, int *);
	int extractFile(Common::File &, Common::File &, int);
	void writeDirEntry(Common::File &, int, int);
};

#endif
