/* extract_ddpg - Extract resources from the bootable floppy version of
 *                Donald Duck's Playground
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

#include <stdio.h>
#include "extract_ddpg.h"

#define SECTOR_OFFSET(s) ((s) * 512)

#define BASE_SECTOR	0x1C2

#define LOGDIR		SECTOR_OFFSET(171) + 5
#define LOGDIR_MAX	43

#define PICDIR		SECTOR_OFFSET(180) + 5
#define PICDIR_MAX	30

#define VIEWDIR		SECTOR_OFFSET(189) + 5
#define VIEWDIR_MAX	171

#define SNDDIR		SECTOR_OFFSET(198) + 5
#define SNDDIR_MAX	64

ExtractDDPG::ExtractDDPG(const std::string &name) : Tool(name, TOOLTYPE_EXTRACTION) {
	ToolInput input;
	input.format = "*.dsk";
	_inputPaths.push_back(input);

	_shorthelp = "Extract resources from the bootable floppy version of Donald Duck's Playground.";
	_helptext = "\nUsage: " + _name + " [-o <output dir> = out/] <disk image>\n\t" + _shorthelp + "\n";
}

void ExtractDDPG::execute() {
	Common::Filename inpath(_inputPaths[0].path);
	Common::Filename &outpath = _outputPath;
	Common::Filename dirpath;

	if (outpath.empty()) {
		outpath.setFullPath("out/");
		dirpath.setFullPath("out/");
	} else {
		dirpath.setFullPath(outpath.getFullPath());
	}

	if (!Common::isDirectory(dirpath.getFullPath().c_str()))
		error("Output directory (%s) does not exist", dirpath.getFullPath().c_str());

	outpath.setFullName("vol.0");

	_in.open(inpath, "rb");
	_out.open(outpath, "wb");

	if (_in.size() != 40 * 2 * 9 * 512) // tracks * sides * sectors * sector size = 368640
		error("This is not a disk image!");

	_volumeOffset = 0;

	// Extract LOGIC files
	dirpath.setFullName("logdir");
	_dir.open(dirpath, "wb");
	print("Extracting LOGIC files... ");
	extractDir(LOGDIR, LOGDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract PICTURE files
	dirpath.setFullName("picdir");
	_dir.open(dirpath, "wb");
	print("Extracting PICTURE files... ");
	extractDir(PICDIR, PICDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract VIEW files
	dirpath.setFullName("viewdir");
	_dir.open(dirpath, "wb");
	print("Extracting VIEW files... ");
	extractDir(VIEWDIR, VIEWDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract SOUND files
	dirpath.setFullName("snddir");
	_dir.open(dirpath, "wb");
	print("Extracting SOUND files... ");
	extractDir(SNDDIR, SNDDIR_MAX);
	_dir.close();
	print("done!\n");
}

// Entry format: ssss ssssssso oooooooo (s=sector, o=offset)
bool ExtractDDPG::readDirEntry(int *sec, int *off) {
	int b0 = _in.readByte();
	int b1 = _in.readByte();
	int b2 = _in.readByte();
	if (b0 == 0xFF && b1 == 0xFF && b2 == 0xFF)
		return false;

	*sec = (BASE_SECTOR + (((b0 & 0xF) << 8) | b1)) >> 1;
	*off = ((b1 & 0x1) << 8) | b2;
	return true;
}

void ExtractDDPG::writeDirEntry(int off) {
	if (off >= 0) {
		_dir.writeByte((off >> 16) & 0xF);
		_dir.writeByte((off >> 8) & 0xFF);
		_dir.writeByte(off & 0xFF);
	} else {
		_dir.writeByte(0xFF);
		_dir.writeByte(0xFF);
		_dir.writeByte(0xFF);
	}
}

int ExtractDDPG::extractFile() {
	// Check header from image
	int signature = _in.readUint16BE();
	_in.readByte();
	int length = _in.readUint16LE();

	if (signature != 0x1234)
		error("Invalid signature in a resource file");
	if (length < 2)
		error("Invalid length in a resource file");

	// Write header to VOL file
	_out.writeByte(0x12);
	_out.writeByte(0x34);
	_out.writeByte(0);
	_out.writeByte(length);
	_out.writeByte(length >> 8);
	
	// Extract data from image and write it to VOL file
	char buf[512];
	unsigned int n = length;
	while (n > 0) {
		int s = n < sizeof(buf) ? n : sizeof(buf);
		_in.read_noThrow(buf, s);
		_out.write(buf, s);
		n -= s;
	}

	return length + 5;
}

void ExtractDDPG::extractDir(int offset, int max) {
	for (int i = 0; i <= max; i++) {
		// Read directory entry from the disk image
		int sec, off;
		_in.seek(offset + 3 * i, SEEK_SET);
		if (!readDirEntry(&sec, &off)) {
			writeDirEntry(-1);
			continue;
		}
		_in.seek(SECTOR_OFFSET(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(_volumeOffset);
		_volumeOffset += extractFile();
	}
}

#ifdef STANDALONE_MAIN
int main(int argc, char *argv[]) {
	ExtractDDPG ddpg(argv[0]);
	return ddpg.run(argc, argv);
}
#endif
