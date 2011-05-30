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

#define offsetTHS(track,head,sector) (512 * ((((track) * 2 + (head)) * 9) + (sector)))
#define offset(sector) offsetTHS(sector / 18, (sector % 18) / 9, (sector % 18) % 9)

#define LOGDIR		offset(90) + 5
#define LOGDIR_MAX	118

#define VIEWDIR		offset(96) + 5
#define VIEWDIR_MAX	180

#define PICDIR		offset(93) + 8
#define PICDIR_MAX	117

#define SNDDIR		offset(99) + 5
#define SNDDIR_MAX	29

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
	Common::File dir;

	if (outpath.empty()) {
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
	
	Common::File in0(inpath0, "rb");
	Common::File in1(inpath1, "rb");
	Common::File vol0(volpath0, "wb");
	Common::File vol1(volpath1, "wb");
	Common::File vol2(volpath2, "wb");

	if (in0.size() != IMAGE_SIZE)
		error("Invalid size for a supposed disk image (%s)", inpath0.getFullPath().c_str());
	if (in1.size() != IMAGE_SIZE)
		error("Invalid size for a supposed disk image (%s)", inpath1.getFullPath().c_str());

	int o0 = 0;
	int o1 = 0;
	int o2 = 0;

	// Extract LOGIC files
	dirpath.setFullName("logdir");
	dir.open(dirpath, "wb");
	print("Extracting LOGIC files... ");
	for (int i = 0; i <= LOGDIR_MAX; i++) {
		// Read directory entry from the disk image
		int sec, off, vol;
		in0.seek(LOGDIR + 3 * i, SEEK_SET);
		if (!readDirEntry(in0, &sec, &off, &vol)) {
			continue;
		}

		Common::File &in = (vol == 2) ? in1 : in0;
		Common::File &out = (vol == 0) ? vol0 : ((vol == 1) ? vol1 : vol2);
		int &o = (vol == 0) ? o0 : ((vol == 1) ? o1 : o2);
		in.seek(offset(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(dir, o, vol);
		o += extractFile(in, out, vol);
	}
	dir.close();
	print("done!\n");

	// Extract VIEW files
	dirpath.setFullName("viewdir");
	dir.open(dirpath, "wb");
	print("Extracting VIEW files... ");
	for (int i = 0; i <= VIEWDIR_MAX; i++) {
		// Read directory entry from the disk image
		int sec, off, vol;
		in0.seek(VIEWDIR + 3 * i, SEEK_SET);
		if (!readDirEntry(in0, &sec, &off, &vol)) {
			continue;
		}

		Common::File &in = (vol == 2) ? in1 : in0;
		Common::File &out = (vol == 0) ? vol0 : ((vol == 1) ? vol1 : vol2);
		int &o = (vol == 0) ? o0 : ((vol == 1) ? o1 : o2);
		in.seek(offset(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(dir, o, vol);
		o += extractFile(in, out, vol);
	}
	dir.close();
	print("done!\n");

	// Extract PICTURE files
	dirpath.setFullName("picdir");
	dir.open(dirpath, "wb");
	print("Extracting PICTURE files... ");
	for (int i = 0; i <= PICDIR_MAX; i++) {
		// Read directory entry from the disk image
		int sec, off, vol;
		in0.seek(PICDIR + 3 * i, SEEK_SET);
		if (!readDirEntry(in0, &sec, &off, &vol)) {
			continue;
		}

		Common::File &in = (vol == 2) ? in1 : in0;
		Common::File &out = (vol == 0) ? vol0 : ((vol == 1) ? vol1 : vol2);
		int &o = (vol == 0) ? o0 : ((vol == 1) ? o1 : o2);
		in.seek(offset(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(dir, o, vol);
		o += extractFile(in, out, vol);
	}
	dir.close();
	print("done!\n");

	// Extract SOUND files
	dirpath.setFullName("snddir");
	dir.open(dirpath, "wb");
	print("Extracting SOUND files... ");
	for (int i = 0; i <= SNDDIR_MAX; i++) {
		// Read directory entry from the disk image
		int sec, off, vol;
		in0.seek(SNDDIR + 3 * i, SEEK_SET);
		if (!readDirEntry(in0, &sec, &off, &vol)) {
			continue;
		}

		Common::File &in = (vol == 2) ? in1 : in0;
		Common::File &out = (vol == 0) ? vol0 : ((vol == 1) ? vol1 : vol2);
		int &o = (vol == 0) ? o0 : ((vol == 1) ? o1 : o2);
		in.seek(offset(sec) + off, SEEK_SET);

		// Write directory entry and extract file
		writeDirEntry(dir, o, vol);
		o += extractFile(in, out, vol);
	}
	dir.close();
	print("done!\n");
}

// Entry format: xxtttttt ssssssho oooooooo
bool ExtractBC::readDirEntry(Common::File &in, int *sec, int *off, int *vol) {
	int b0 = in.readByte();
	int b1 = in.readByte();
	int b2 = in.readByte();
	if (b0 == 0xFF && b1 == 0xFF && b2 == 0xFF)
		return false;

	*sec = (b0 & 0x3F) * 18 + ((b1 >> 1) & 0x1) * 9 + ((b1 >> 2) & 0x1F) - 1;
	*off = ((b1 & 0x1) << 8) | b2;
	*vol = (b0 & 0xC0) >> 6;
	return true;
}

void ExtractBC::writeDirEntry(Common::File &dir, int off, int vol) {
	if (off >= 0) {
		dir.writeByte(((off >> 16) & 0xF) | ((vol & 0xF) << 4));
		dir.writeByte((off >> 8) & 0xFF);
		dir.writeByte(off & 0xFF);
	} else {
		dir.writeByte(0xFF);
		dir.writeByte(0xFF);
		dir.writeByte(0xFF);
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
	char buf[512];
	unsigned int n = length;
	while (n > 0) {
		int s = n < sizeof(buf) ? n : sizeof(buf);
		in.read_noThrow(buf, s);
		out.write(buf, s);
		n -= s;
	}

	return length + 5;
}

#ifdef STANDALONE_MAIN
int main(int argc, char *argv[]) {
	ExtractBC bc(argv[0]);
	return bc.run(argc, argv);
}
#endif
