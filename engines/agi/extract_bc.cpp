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

#include <stdio.h>
#include "extract_bc.h"

#define IMAGE_SIZE 368640 // = 40 * 2 * 9 * 512 = tracks * sides * sectors * sector size
#define SECTOR_OFFSET(s) ((s) * 512)

#define LOGDIR		SECTOR_OFFSET(90) + 5
#define LOGDIR_MAX	118
#define VIEWDIR		SECTOR_OFFSET(96) + 5
#define VIEWDIR_MAX	180
#define PICDIR		SECTOR_OFFSET(93) + 8
#define PICDIR_MAX	117
#define SNDDIR		SECTOR_OFFSET(99) + 5
#define SNDDIR_MAX	29
#define WORDS		0x26D
#define OBJECTS		0x1E6

ExtractBC::ExtractBC(const std::string &name) : Tool(name, TOOLTYPE_EXTRACTION) {
	ToolInput input0, input1;
	input0.format = "*.dsk";
	input1.format = "*.dsk";
	_inputPaths.push_back(input0);
	_inputPaths.push_back(input1);

	_shorthelp = "Extract resources from the booter versions of Black Cauldron.";
	_helptext = "\nUsage: " + _name + " [-o <output dir> = out/] <disk 1> <disk 2>\n\t" + _shorthelp + "\n";
}

void ExtractBC::execute() {
	Common::Filename inpath0(_inputPaths[0].path);
	Common::Filename inpath1(_inputPaths[1].path);
	Common::Filename &outpath = _outputPath;
	Common::Filename dirpath;
	Common::Filename volpath0, volpath1, volpath2;

	if (outpath.empty()) {
		outpath.setFullPath("out/");
		dirpath.setFullPath("out/");
		volpath0.setFullPath("out/");
		volpath1.setFullPath("out/");
		volpath2.setFullPath("out/");
	} else {
		dirpath.setFullPath(outpath.getFullPath());
		volpath0.setFullPath(outpath.getFullPath());
		volpath1.setFullPath(outpath.getFullPath());
		volpath2.setFullPath(outpath.getFullPath());
	}

	if (!Common::isDirectory(dirpath.getFullPath().c_str()))
		error("Output directory (%s) does not exist", dirpath.getFullPath().c_str());

	volpath0.setFullName("vol.0");
	volpath1.setFullName("vol.1");
	volpath2.setFullName("vol.2");
	
	_in0.open(inpath0, "rb");
	_in1.open(inpath1, "rb");
	_vol0.open(volpath0, "wb");
	_vol1.open(volpath1, "wb");
	_vol2.open(volpath2, "wb");

	if (_in0.size() != IMAGE_SIZE)
		error("Invalid size for a supposed disk image (%s)", inpath0.getFullPath().c_str());
	if (_in1.size() != IMAGE_SIZE)
		error("Invalid size for a supposed disk image (%s)", inpath1.getFullPath().c_str());

	_vol0Offset = 0;
	_vol1Offset = 0;
	_vol2Offset = 0;

	// Extract LOGIC files
	dirpath.setFullName("logdir");
	_dir.open(dirpath, "wb");
	print("Extracting LOGIC files... ");
	extractDir(LOGDIR, LOGDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract VIEW files
	dirpath.setFullName("viewdir");
	_dir.open(dirpath, "wb");
	print("Extracting VIEW files... ");
	extractDir(VIEWDIR, VIEWDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract PICTURE files
	dirpath.setFullName("picdir");
	_dir.open(dirpath, "wb");
	print("Extracting PICTURE files... ");
	extractDir(PICDIR, PICDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract SOUND files
	dirpath.setFullName("snddir");
	_dir.open(dirpath, "wb");
	print("Extracting SOUND files... ");
	extractDir(SNDDIR, SNDDIR_MAX);
	_dir.close();
	print("done!\n");

	// Extract words.tok
	Common::File out;
	outpath.setFullName("words.tok");
	out.open(outpath, "wb");
	print("Extracting words.tok... ");
	_in0.seek(SECTOR_OFFSET(WORDS), SEEK_SET);
	int signature = _in0.readUint16BE();
	_in0.readByte();
	int length = _in0.readUint16LE();
	if (signature != 0x1234)
		error("invalid signature");
	out.writeUint16LE(length);
	extractData(_in0, out, length);
	out.close();
	print("done!\n");

	// Extract object
	outpath.setFullName("object");
	out.open(outpath, "wb");
	print("Extracting object... ");
	_in0.seek(SECTOR_OFFSET(OBJECTS), SEEK_SET);
	signature = _in0.readUint16BE();
	_in0.readByte();
	length = _in0.readUint16LE();
	if (signature != 0x1234)
		error("invalid signature");
	out.writeUint16LE(length);
	extractData(_in0, out, length);
	out.close();
	print("done!\n");
}

// Entry format: xxtttttt ssssssho oooooooo
bool ExtractBC::readDirEntry(int *sec, int *off, int *vol) {
	int b0 = _in0.readByte();
	int b1 = _in0.readByte();
	int b2 = _in0.readByte();
	if (b0 == 0xFF && b1 == 0xFF && b2 == 0xFF)
		return false;

	*sec = (b0 & 0x3F) * 18 + ((b1 >> 1) & 0x1) * 9 + ((b1 >> 2) & 0x1F) - 1;
	*off = ((b1 & 0x1) << 8) | b2;
	*vol = (b0 & 0xC0) >> 6;
	return true;
}

void ExtractBC::writeDirEntry(int off, int vol) {
	if (off >= 0) {
		_dir.writeByte(((off >> 16) & 0xF) | ((vol & 0xF) << 4));
		_dir.writeByte((off >> 8) & 0xFF);
		_dir.writeByte(off & 0xFF);
	} else {
		_dir.writeByte(0xFF);
		_dir.writeByte(0xFF);
		_dir.writeByte(0xFF);
	}
}

void ExtractBC::extractData(Common::File &in, Common::File &out, int length) {
	char buf[512];
	unsigned int n = length;
	while (n > 0) {
		int s = n < sizeof(buf) ? n : sizeof(buf);
		in.read_noThrow(buf, s);
		out.write(buf, s);
		n -= s;
	}
}

int ExtractBC::extractFile(Common::File &in, Common::File &out, int vol) {
	// Check header from image
	int signature = in.readUint16BE();
	in.readByte();
	int length = in.readUint16LE();

	if (signature != 0x1234)
		error("Invalid signature in a resource file");
	if (length < 2)
		error("Invalid length in a resource file");

	// Write header to VOL file
	out.writeByte(0x12);
	out.writeByte(0x34);
	out.writeByte(vol);
	out.writeByte(length);
	out.writeByte(length >> 8);
	
	// Extract data from image and write it to VOL file
	extractData(in, out, length);

	return length + 5;
}

void ExtractBC::extractDir(int offset, int max) {
	for (int i = 0; i <= max; i++) {
		// Read directory entry from the disk image
		int sec, off, vol;
		_in0.seek(offset + 3 * i, SEEK_SET);
		if (!readDirEntry(&sec, &off, &vol)) {
			writeDirEntry(-1, -1);
			continue;
		}

		Common::File &in = (vol == 2) ? _in1 : _in0;
		Common::File &out = (vol == 0) ? _vol0 : ((vol == 1) ? _vol1 : _vol2);
		int &o = (vol == 0) ? _vol0Offset : ((vol == 1) ? _vol1Offset : _vol2Offset);
		in.seek(SECTOR_OFFSET(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(o, vol);
		o += extractFile(in, out, vol);
	}
}

#ifdef STANDALONE_MAIN
int main(int argc, char *argv[]) {
	ExtractBC bc(argv[0]);
	return bc.run(argc, argv);
}
#endif
