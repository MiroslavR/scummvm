/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#include "xeen/worldofxeen/clouds_cutscenes.h"
#include "xeen/worldofxeen/worldofxeen_resources.h"
#include "xeen/sound.h"

namespace Xeen {
namespace WorldOfXeen {

bool CloudsCutscenes::showCloudsTitle() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;

	// Initial logo display
	screen.loadPalette("intro1.pal");
	screen.loadBackground("logobak.raw");
	screen.saveBackground();
	screen.update();
	screen.fadeIn(128);

	SpriteResource logo[2] = {
		SpriteResource("logo.vga"), SpriteResource("logo1.vga")
	};
	sound.playFX(1);

	for (int idx = 0; idx < 80; ++idx) {
		screen.restoreBackground();
		logo[idx / 65].draw(0, idx % 65);
		screen.update();

		switch (idx) {
		case 37:
			sound.playFX(0);
			sound.playFX(53);
			sound.playSound("fire.voc");
			break;
		case 52:
		case 60:
			sound.playFX(3);
			break;
		case 64:
			sound.playFX(2);
			break;
		case 66:
			sound.playFX(52);
			sound.playSound("meangro&.voc");
			break;
		default:
			break;
		}

		WAIT(2);
	}

	screen.restoreBackground();
	screen.update();
	WAIT(30);

	screen.fadeOut(8);
	logo[0].clear();
	logo[1].clear();

	return true;
}

bool CloudsCutscenes::showCloudsIntro() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	Windows &windows = *_vm->_windows;

	SpriteResource stars("stars.vga"), intro1("intro1.vga"),
		lake("lake.vga"), xeen("xeen.vga"), wizTower("wiztower.vga"),
		wizTower2("wiztwer2.vga"), lake2("lake2.vga"), lake3("lake3.vga"),
		xeen1("xeen1.vga");
	_subtitles.load("special.bin", 0);
	_vm->_files->_isDarkCc = false;

	// Show the production splash screen
	sound.playSong("mm4theme.m");
	screen.loadBackground("jvc.raw");
	screen.loadPalette("mm4.pal");
	screen.update();
	screen.fadeIn();
	WAIT(30);
	screen.fadeOut(8);

	// Clouds of Xeen title with vertically scrolling background
	screen.loadPalette("intro.pal");
	screen.loadBackground("blank.raw");
	screen.saveBackground();
	stars.draw(0, 0);
	stars.draw(0, 1, Common::Point(160, 0));
	screen.loadPage(0);
	intro1.draw(0, 0);
	screen.loadPage(1);

	bool fadeFlag = true;
	for (int yCtr = SCREEN_HEIGHT, yScroll = 0, xeenCtr = -1; yCtr > 0; --yCtr, ++yScroll) {
		screen.vertMerge(yScroll);
		if (yCtr < 160) {
			xeen.draw(0, 0);
		} else if (yCtr < 100) {
			xeen.draw(0, 0);
			if (++xeenCtr < 14)
				xeen1.draw(0, xeenCtr);
		}
		screen.update();

		if (fadeFlag) {
			screen.fadeIn();
			fadeFlag = false;
		}

		WAIT(1);
	}

	// Remainder of vertical scrolling of background
	screen.restoreBackground();
	intro1.draw(0, 0);
	screen.loadPage(0);
	lake.draw(0, 0);
	screen.loadPage(1);

	bool drawFlag = false;
	for (int yCtr = SCREEN_HEIGHT - 1, yScroll = 1, lakeCtr = 0; yCtr > 0; --yCtr, ++yScroll) {
		screen.vertMerge(yScroll);

		if (yCtr < 95) {
			if (++lakeCtr >= 44 || drawFlag) {
				lakeCtr = 0;
				drawFlag = true;
			} else {
				lake3.draw(0, lakeCtr, Common::Point(0, yCtr));
			}
		}

		xeen.draw(0, 0);
		screen.update();
		WAIT(1);
	}

	screen.freePages();

	// Flying creatures moving horizontally
	lake.draw(0, 0);
	screen.saveBackground();

	int frameNum = 0;
	events.updateGameCounter();
	for (int idx = 0; idx < 100; ++idx) {
		frameNum = (frameNum + 1) % 43;
		screen.restoreBackground();
		lake2.draw(0, frameNum, Common::Point(0, 0), SPRFLAG_800);
		WAIT(1);
	}

	// Zoom in on a closeup of the wizardry tower
	const int XLIST1[16] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 44, 48, 52, 56, 60, 64, 68 };
	const int XLIST2[16] = { 160, 155, 150, 145, 140, 135, 130, 125, 120, 114, 108, 102, 96, 90, 84, 78 };
	const int YLIST[23] = { 0, 6, 12, 17, 20, 23, 26, 29, 32, 35, 38, 41, 44, 47, 50, 51 };

	for (int idx = 15; idx >= 0; --idx) {
		events.updateGameCounter();

		screen.restoreBackground();
		lake2.draw(0, frameNum, Common::Point(0, 0), SPRFLAG_800);
		frameNum = (frameNum + 1) % 43;
		wizTower.draw(0, 0, Common::Point(XLIST1[idx], YLIST[idx]), 0, idx);
		wizTower.draw(0, 1, Common::Point(XLIST2[idx], YLIST[idx]), 0, idx);
		screen.update();
		WAIT(1);
	}

	// Cloaked figure walks horizontally
	wizTower.draw(0, 0);
	wizTower.draw(0, 1, Common::Point(160, 0));
	screen.saveBackground();

	for (int idx = 0; idx < 39; ++idx) {
		screen.restoreBackground();
		wizTower2.draw(0, idx);
		screen.update();

		WAIT(2);
	}

	screen.fadeOut();
	lake2.clear();
	lake3.clear();
	xeen1.clear();

	// All the lines whilst the scroll is open
	SpriteResource groupo("groupo.vga"), group("group.vga"),
		crodo("crodo.vga"), box("box.vga");

	groupo.draw(0, 0);
	groupo.draw(0, 1, Common::Point(160, 0));
	crodo.draw(0, 0, Common::Point(0, -5));
	windows[0].writeString(Res.CLOUDS_INTRO1);

	// Unroll a scroll
	if (doScroll(false, true))
		return false;

	sound.setMusicVolume(75);
	screen.restoreBackground();
	screen.update();
	resetSubtitles(0, 1);

	// Loop through each spoken line
	int ctr1 = 0, ctr2 = 0, ctr3 = 0, ctr4 = 0, ctr5 = 0, totalCtr = 0;
	for (int lineCtr = 0; lineCtr < 14; ++lineCtr) {
		if (lineCtr != 6 && lineCtr != 7) {
			sound.playSound(_INTRO_VOCS[lineCtr]);
		}

		for (int frameCtr = 0, lookup = 0; sound.isPlaying() || _subtitleSize; ) {
			groupo.draw(0, 0);
			groupo.draw(0, 1, Common::Point(160, 0));

			switch (lineCtr) {
			case 2:
				ctr1 = (ctr1 + 1) % 5;
				group.draw(0, ctr1);
				ctr4 = (ctr4 + 1) % 9;
				break;

			case 4:
				ctr4 = (ctr4 + 1) % 9 + 9;
				break;

			case 8:
			case 12:
				ctr2 = (ctr2 + 1) % 3;
				ctr4 = (ctr4 + 1) % 9;
				ctr3 = (ctr3 + 1) % 6 + 3;
				break;

			case 9:
			case 13:
				ctr3 = (ctr3 + 1) % 3;
				group.draw(0, ctr3 + 43, Common::Point(178, 134));
				ctr4 = (ctr4 + 1) % 9;
				ctr2 = (ctr2 % 15) + 3;
				break;

			default:
				ctr4 = (ctr4 + 1) % 9;
				ctr2 = (ctr2 + 1) % 15 + 3;
				ctr3 = (ctr3 + 1) % 6 + 3;
				break;
			}

			group.draw(0, ctr4 + 5, Common::Point(0, 99));
			group.draw(0, ctr2 + 24, Common::Point(202, 12));
			if ((++totalCtr % 30) == 0)
				group.draw(0, 43, Common::Point(178, 134));

			switch (lineCtr) {
			case 2:
			case 4:
			case 8:
			case 9:
			case 12:
			case 13: {
				crodo.draw(0, 0, Common::Point(0, -5));
				windows[0].writeString(Res.CLOUDS_INTRO1);

				ctr5 = (ctr5 + 1) % 19;
				WAIT(1);
				showSubtitles();
				continue;
			}

			default:
				crodo.draw(0, frameCtr, Common::Point(0, -5));
				if (lookup > 30)
					lookup = 30;
				frameCtr = _INTRO_FRAMES_VALS[_INTRO_FRAMES_LOOKUP[lineCtr]][lookup];
				windows[0].writeString(Res.CLOUDS_INTRO1);

				ctr5 = (ctr5 + 1) % 19;
				WAIT(1);
				showSubtitles();
				break;
			}

			events.updateGameCounter();
			while (events.timeElapsed() < _INTRO_FRAMES_WAIT[_INTRO_FRAMES_LOOKUP[lineCtr]][lookup]
					&& sound.isPlaying()) {
				events.pollEventsAndWait();
				if (events.isKeyMousePressed())
					return false;
			}

			++lookup;
			if (!sound._soundOn && lookup > 30)
				lookup = 0;
		}

		if (!sound._soundOn)
			lineCtr = 20;

		if (lineCtr == 5)
			sound.playSound(_INTRO_VOCS[6]);
		else if (lineCtr == 6)
			sound.playSound(_INTRO_VOCS[7]);
	}

	// Roll up the scroll again
	sound.songCommand(50);
	doScroll(true, false);

	return true;
}

void CloudsCutscenes::showCloudsEnding(uint finalScore) {
	if (showCloudsEnding1())
		if (showCloudsEnding2())
			if (!showCloudsEnding3())
				if (!showCloudsEnding4(finalScore))
					showCloudsEnding5();
}

bool CloudsCutscenes::showCloudsEnding1() {
	EventsManager &events = *_vm->_events;
	FileManager &files = *_vm->_files;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;

	files._isDarkCc = false;
	files.setGameCc(0);

	// Show the castle with swirling clouds and lightning
	SpriteResource prec;
	prec.load("prec.end");
	screen.loadBackground("blank.raw");
	screen.loadPalette("mm4e.pal");

	loadScreen(Common::String::format("prec00%02u.frm", 1));
	prec.draw(0, 0);
	prec.draw(0, 1, Common::Point(160, 0));
	screen.update();
	screen.fadeIn();
	WAIT(15);

	sound.playFX(1);
	sound.playFX(34);

	// prec loop
	for (int idx = 1; idx < 42; ++idx) {
		// Load up the background frame of swirling clouds
		loadScreen(Common::String::format("prec00%02u.frm", idx));

		// Render castle in front of it
		prec.draw(0, 0, Common::Point(0, 0));
		prec.draw(0, 1, Common::Point(160, 0));
		screen.update();

		switch (idx) {
		case 8:
		case 18:
		case 21:
			sound.playFX(33);
			break;

		case 19:
		case 25:
			sound.playFX(34);
			break;

		default:
			break;
		}

		WAIT(3);
	}

	prec.clear();

	SpriteResource cast[16], darkLord[3];
	for (int idx = 1; idx < 7; ++idx)
		cast[idx - 1].load(Common::String::format("cast%02u.end", idx));
	for (int idx = 1; idx < 4; ++idx)
		darkLord[idx - 1].load(Common::String::format("darklrd%d.end", idx));

	// First vortex loop
	int cloudsCtr = 1;
	for (int idx = 1; idx < 16; ++idx) {
		loadScreen(Common::String::format("vort%02u.frm", cloudsCtr++));
		cast[0].draw(0, 0);
		cast[0].draw(0, 0, Common::Point(0, 100));
		WAIT(3);
	}

	screen.loadPalette("mm4.pal");
	screen.fadeIn(0x81);

	const byte COUNTS1[6] = { 9, 3, 2, 2, 3, 15 };
	bool flag = false;
	for (int idx1 = 1; idx1 < 7; ++idx1) {
		for (int idx2 = 0; idx2 < COUNTS1[idx1 - 1]; ++idx2) {
			loadScreen(Common::String::format("vort%02u.frm", cloudsCtr));
			if (cloudsCtr++ > 20)
				cloudsCtr = 1;

			if (flag && !sound.isPlaying()) {
				flag = false;
				sound.playFX(34);
			} else if (!flag && idx1 == 1 && idx2 == 6) {
				flag = true;
				sound.playSound("xeenlaff.voc");
			}

			switch (cloudsCtr) {
			case 0:
			case 1:
			case 5:
			case 9:
			case 15:
				sound.playFX(34);
				break;
			case 2:
			case 7:
			case 10:
			case 13:
				sound.playFX(33);
				break;
			default:
				break;
			}

			cast[idx1 - 1].draw(0, idx2, Common::Point(0, 0));
			cast[idx1 - 1].draw(0, idx2, Common::Point(0, 100));
			WAIT(3);
		}
	}

	for (int idx = 0; idx < 16; ++idx) {
		loadScreen(Common::String::format("vort%02u.frm", cloudsCtr));
		if (cloudsCtr++ > 20)
			cloudsCtr = 1;

		if (idx < 7)
			darkLord[0].draw(0, idx);
		else if (idx < 11)
			darkLord[1].draw(0, idx - 7);
		else
			darkLord[2].draw(0, idx - 11);

		switch (cloudsCtr - 1) {
		case 0:
		case 4:
		case 8:
		case 14:
			sound.playFX(34);
			break;
		case 1:
		case 6:
		case 9:
		case 12:
			sound.playFX(33);
			break;
		default:
			break;
		}

		WAIT(3);
	}
	sound.setMusicVolume(75);

	for (int idx = 0; idx < 3; ++idx) {
		switch (idx) {
		case 0:
			sound.playSound("dark1.voc");
			break;
		case 1:
			sound.playSound("dark2.voc");
			break;
		case 2:
			sound.playSound("dark3.voc");
			break;
		}

		do {
			loadScreen(Common::String::format("vort%02u.frm", cloudsCtr));
			if (cloudsCtr++ > 20)
				cloudsCtr = 1;

			darkLord[2].draw(0, getSpeakingFrame(2, 6));

			switch (cloudsCtr - 1) {
			case 0:
			case 4:
			case 8:
			case 14:
				sound.playFX(34);
				break;
			case 1:
			case 6:
			case 9:
			case 12:
				sound.playFX(33);
				break;
			default:
				break;
			}

			showSubtitles(0);
			WAIT(3);
		} while (sound.isPlaying() || _subtitleSize > 0);
	}

	sound.playSound("darklaff.voc");
	sound.setMusicVolume(95);

	for (int idx = 12; idx >= 0; --idx) {
		loadScreen(Common::String::format("vort%02u.frm", cloudsCtr));
		if (cloudsCtr++ > 20)
			cloudsCtr = 1;

		if (idx < 7)
			darkLord[0].draw(0, idx);
		else if (idx < 11)
			darkLord[1].draw(0, idx - 7);
		else
			darkLord[2].draw(0, idx - 11);

		switch (cloudsCtr - 1) {
		case 0:
		case 4:
		case 8:
		case 14:
			sound.playFX(34);
			break;
		case 1:
		case 6:
		case 9:
		case 12:
			sound.playFX(33);
			break;
		default:
			break;
		}

		WAIT(3);
	}

	sound.stopSound();
	screen.fadeOut();
	return true;
}

bool CloudsCutscenes::showCloudsEnding2() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;

	SpriteResource king("king.end"), room("room.end"), bigSky("bigsky.end"),
		mirror("mirror.end"), mirrBack("mirrback.end"), people("people.end"),
		crodo("crodo.end"), kingCord("kingcord.end");

	screen.loadPalette("endgame.pal");
	screen.loadBackground("later.raw");
	screen.fadeIn();
	WAIT(100);
	screen.fadeOut();

	screen.loadBackground("throne1.raw");
	screen.loadPage(0);
	screen.loadBackground("throne2.raw");
	screen.loadPage(1);

	int ctr1 = 0, ctr3 = -1, ctr4 = 0, ctr5 = 0;
	int xp2 = SCREEN_WIDTH;
	for (int ctr2 = SCREEN_WIDTH, xp1 = 117, xp3 = 0; ctr2 > 0; ) {
		events.updateGameCounter();
		screen.horizMerge(xp3);
		people.draw(0, 0, Common::Point(xp1, 68), SPRFLAG_800);
		if (xp3 > 250) {
			crodo.draw(0, 0, Common::Point(xp2, 68), SPRFLAG_800);
			xp2 -= ctr1 * 2;
		}

		events.pollEventsAndWait();
		if (_vm->shouldQuit())
			return false;

		if (!ctr4) {
			ctr1 = events.timeElapsed();
			if (!ctr1)
				ctr1 = 1;
			ctr4 = 1;
			ctr5 = ctr1 * 2;
		}

		xp3 += ctr1;
		ctr2 -= ctr1;
		xp1 -= ctr5;
		if (xp2 < 181)
			xp2 = 181;
		if (ctr3) {
			screen.fadeIn();
			ctr3 = 0;
		}
	}

	screen.horizMerge(SCREEN_WIDTH);
	crodo.draw(0, 0, Common::Point(xp2, 68), SPRFLAG_800);
	screen.freePages();
	WAIT(5);

	Graphics::ManagedSurface savedBg;
	savedBg.blitFrom(screen);

	const int XLIST1[13] = { 0, -5, -10, -15, -20, -25, -30, -33, -27, -22, -17 };
	const int XLIST2[13] = { 60, 145, 130, 115, 100, 85, 70, 57, 53, 48, 42, 39, 34 };
	const int YLIST[13] = { 42, 39, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	for (int idx = 12; idx >= 0; --idx) {
		screen.blitFrom(savedBg);
		king.draw(0, 0, Common::Point(XLIST1[idx], YLIST[idx]));
		king.draw(0, 1, Common::Point(XLIST2[idx], YLIST[idx]));
		WAIT(1);
	}

	const char *const VOC_NAMES[3] = { "king1.voc", "king2.voc", "king3.voc" };
	_subtitleSize = 0;
	for (int idx = 0; idx < 3; ++idx) {
		sound.playSound(VOC_NAMES[idx]);

		do {
			king.draw(0, 0, Common::Point(0, 0));
			king.draw(0, 1, Common::Point(160, 0));

			int frame = getSpeakingFrame(1, 6);
			if (frame > 1)
				king.draw(0, frame);

			showSubtitles();
			WAIT(3);
		} while (sound.isPlaying() || _subtitleSize);

		king.draw(0, 0, Common::Point(0, 0));
		king.draw(0, 1, Common::Point(160, 0));
		WAIT(1);
	}

	screen.fadeOut();
	screen.loadPalette("mirror.pal");
	screen.loadBackground("miror-s.raw");
	screen.loadPage(0);
	screen.loadPage(1);

	room.draw(0, 0, Common::Point(0, 0));
	room.draw(0, 1, Common::Point(160, 0));
	screen.fadeIn();

	for (int idx = 0; idx < 83; ++idx) {
		screen.horizMerge(idx);
		room.draw(0, 0, Common::Point(0, 0));
		room.draw(0, 1, Common::Point(160, 0));
		WAIT(1);
	}

	screen.freePages();
	savedBg.blitFrom(screen);

	const int XLIST3[9] = { 0, -5, -10, -15, -24, -30, -39, -50, -59 };
	const int YLIST3[9] = { 0, 12, 25, 37, 46, 52, 59, 64, 68 };
	for (int idx = 8; idx >= 0; --idx) {
		screen.blitFrom(savedBg);
		bigSky.draw(0, 0, Common::Point(XLIST3[idx], YLIST3[idx]), 0, idx);
		mirrBack.draw(0, 0, Common::Point(XLIST3[idx], YLIST3[idx]), 0, idx);
		WAIT(1);
	}

	int mergeX = 0;
	const int DELTA = 2;
	for (int idx = 0, xc1 = -115, yp = SCREEN_HEIGHT, xc2 = 335;
			idx < 115; idx += DELTA, xc1 += DELTA, yp -= DELTA, xc2 -= DELTA) {
		screen.horizMerge(mergeX);
		mergeX = (mergeX + 1) % SCREEN_WIDTH;

		mirrBack.draw(0, 0);
		mirror.draw(0, 0);
		kingCord.draw(0, 0, Common::Point(xc1, yp), SPRFLAG_800);
		kingCord.draw(0, 1, Common::Point(xc2, yp), SPRFLAG_800);
		WAIT(1);
	}

	screen.horizMerge(mergeX);
	mergeX = (mergeX + 1) % SCREEN_WIDTH;
	mirrBack.draw(0, 0);
	mirror.draw(0, 0);
	kingCord.draw(0, 0, Common::Point(0, 85), SPRFLAG_800);
	kingCord.draw(0, 1, Common::Point(220, 85), SPRFLAG_800);

	return true;
}

bool CloudsCutscenes::showCloudsEnding3() {
	SpriteResource mon, att;



	// TODO
	doScroll(true, false);

	return true;
}

bool CloudsCutscenes::showCloudsEnding4(uint finalScore) {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Windows &windows = *_vm->_windows;
	SpriteResource mirror("mirror.end"), mirrBack("mirrback.end"),
		endText("endtext.end");

	int mergeX = 298;
	screen.horizMerge(mergeX);
	mirrBack.draw(0, 0);
	mirror.draw(0, 0);
	doScroll(false, false);

	for (int idx = 0; idx < 19; ++idx) {
		screen.horizMerge(mergeX);
		mergeX = (mergeX + 1) % SCREEN_WIDTH;

		mirrBack.draw(0, 0);
		mirror.draw(0, 0);
		endText.draw(0, idx);
		WAIT(1);
	}

	int frames[10];
	const int FRAMEX[10] = { 64, 83, 102, 121, 140, 159, 178, 197, 216, 235 };
	for (int idx1 = 0; idx1 < 30; ++idx1) {
		for (int idx2 = 0; idx2 < 10; ++idx2)
			frames[idx2] = getSpeakingFrame(20, 29);

		screen.horizMerge(mergeX);
		mergeX = (mergeX + 1) % SCREEN_WIDTH;

		mirrBack.draw(0, 0);
		mirror.draw(0, 0);
		endText.draw(0, 19);
		for (int idx2 = 0; idx2 < 10; ++idx2)
			endText.draw(0, frames[idx2], Common::Point(FRAMEX[idx2], 73));

		WAIT(2);
	}

	Common::String scoreStr = Common::String::format("%.10u", finalScore);
	for (int idx1 = 0; idx1 < 10; ++idx1) {
		for (int idx2 = 0; idx2 < 10; ++idx2)
			frames[idx2] = getSpeakingFrame(20, 29);

		for (int idx2 = 0; idx2 <= idx1; ++idx2)
			frames[9 - idx2] = (byte)scoreStr[9 - idx2] - 28;

		screen.horizMerge(mergeX);
		mergeX = (mergeX + 1) % SCREEN_WIDTH;

		mirrBack.draw(0, 0);
		mirror.draw(0, 0);
		endText.draw(0, 19);

		for (int idx2 = 0; idx2 < 10; ++idx2)
			endText.draw(0, frames[idx2], Common::Point(FRAMEX[idx2], 73));

		WAIT(2);
	}

	// Move the score down
	for (int idx1 = 0; idx1 < 38; ++idx1) {
		screen.horizMerge(mergeX);
		mergeX = (mergeX + 1) % SCREEN_WIDTH;

		mirrBack.draw(0, 0);
		mirror.draw(0, 0);
		endText.draw(0, 19);

		for (int idx2 = 0; idx2 < 10; ++idx2)
			endText.draw(0, frames[idx2], Common::Point(FRAMEX[idx2], 73 + idx1));

		WAIT(1);
	}

	windows[28].setBounds(Common::Rect(63, 60, 254, 160));

	for (int idx = 1; idx <= 2; ++idx) {
		events.clearEvents();
		do {
			screen.horizMerge(mergeX);
			mergeX = (mergeX + 1) % SCREEN_WIDTH;

			mirrBack.draw(0, 0);
			mirror.draw(0, 0);
			endText.draw(0, 19);

			for (int idx2 = 0; idx2 < 10; ++idx2)
				endText.draw(0, frames[idx2], Common::Point(FRAMEX[idx2], 110));
			windows[28].writeString(idx == 1 ? Res.CLOUDS_CONGRATULATIONS1 :
				Res.CLOUDS_CONGRATULATIONS2);

			WAIT(1);
		} while (!events.isKeyMousePressed());
	}

	doScroll(true, false);
	screen.fadeOut();

	return true;
}

bool CloudsCutscenes::showCloudsEnding5() {
	EventsManager &events = *_vm->_events;
	Screen &screen = *_vm->_screen;
	Sound &sound = *_vm->_sound;
	SpriteResource king("king.end");

	king.draw(0, 0, Common::Point(0, 0));
	king.draw(0, 1, Common::Point(160, 0));
	screen.fadeIn();
	
	sound.playSound("king4.voc");
	do {
		king.draw(0, 0, Common::Point(0, 0));
		king.draw(0, 1, Common::Point(160, 0));
		int frame = getSpeakingFrame(1, 6);
		if (frame > 1)
			king.draw(0, frame);

		showSubtitles();
		WAIT(3);
	} while (sound.isPlaying() || _subtitleSize);

	king.draw(0, 0, Common::Point(0, 0));
	king.draw(0, 1, Common::Point(160, 0));
	WAIT(1);
	doScroll(true, false);

	return true;
}

void CloudsCutscenes::loadScreen(const Common::String &name) {
	Screen &screen = *_vm->_screen;
	File fSrc(name);
	byte *destP = (byte *)screen.getPixels();
	byte *destEndP = (byte *)destP + SCREEN_WIDTH * SCREEN_HEIGHT;

	// Setup reference arrays
	#define ARRAY_SIZE 314
	#define ARRAY_LAST1 ((ARRAY_SIZE - 1) * 2)
	#define ARRAY_LAST2 ((ARRAY_SIZE - 1) * 2 + 1)
	#define BUFFER_SIZE 0x1000
	uint array2[ARRAY_SIZE * 2], array3[ARRAY_SIZE * 2];
	uint array4[ARRAY_SIZE * 3];
	byte buffer[BUFFER_SIZE];

	for (int idx = 0; idx < ARRAY_SIZE; ++idx) {
		array3[idx] = 1;
		array4[idx + 627] = idx * 2;
		array2[idx] = idx * 2 + (ARRAY_SIZE * 4 - 2);
	}

	for (int ctr = 0, idx = 0, idx2 = ARRAY_SIZE; ctr < (ARRAY_SIZE - 1); ++ctr, idx += 2, ++idx2) {
		array3[idx2] = array3[idx] + array3[idx + 1];
		array2[idx2] = idx * 2;
		array4[idx] = array4[idx + 1] = idx2 * 2;
	}
	array4[ARRAY_LAST1] = 0;
	array3[ARRAY_LAST2] = (uint)-1;
	array2[ARRAY_LAST2] = 4036;
	uint16 bits = 0x8000;

	// Get the decompressed size and default buffer contents
	uint16 bytePair;
	fSrc.read((byte *)&bytePair, 2);
	Common::fill((uint16 *)buffer, (uint16 *)(buffer + BUFFER_SIZE),
		bytePair);

	int count = fSrc.readUint16BE();
	assert(count == (SCREEN_WIDTH * SCREEN_HEIGHT));

	for (int byteIdx = 0; byteIdx < count; ) {
		assert(fSrc.pos() < fSrc.size());
		int vMin = array2[(ARRAY_SIZE - 1) * 2];
		int vThreshold = ARRAY_SIZE * 4 - 2;
		while (vMin < vThreshold) {
			bool flag = (bits & 0x8000);
			bits <<= 1;

			if (!bits) {
				bits = fSrc.readUint16BE();
				flag = (bits & 0x8000);
				bits = (bits << 1) | 1;
			}

			vMin = array2[vMin / 2 + (flag ? 1 : 0)];
		}

		vMin -= vThreshold;

		if (array3[ARRAY_LAST1] == 0x8000) {
			for (int ctr = 0, ctr2 = 0; ctr < (ARRAY_SIZE * 2); ++ctr) {
				if (array2[ctr] >= (ARRAY_SIZE * 4 - 1)) {
					array3[ctr2] = (array3[ctr] + 1) / 2;
					array2[ctr2] = array2[ctr];
					++ctr2;
				}
			}

			for (int ctr = 0, ctr2 = ARRAY_SIZE; ctr < ARRAY_SIZE; ctr += 2, ++ctr2) {
				int currVal = array3[ctr] + array3[ctr + 1];
				array3[ctr2] = currVal;
				int ctr3 = ctr2;
				do {
					--ctr3;
				} while (array3[ctr3] >= array3[ctr2]);
				++ctr3;

				int diff = ctr2 - ctr3;
				uint *pDest = &array3[ctr2];
				Common::copy(pDest - 1, pDest - 1 + diff, pDest);
				array3[ctr3] = currVal;

				pDest = &array2[ctr2];
				Common::copy(pDest - 1, pDest - 1 + diff, pDest);
				array2[ctr3] = ctr * 2;
			}

			uint *arrEndP = &array4[ARRAY_SIZE * 2 - 1];
			for (int ctr = 0, val = 0; ctr < ARRAY_SIZE * 2; ++ctr, val += 2) {
				uint *arrP = &array4[array2[ctr] / 2];
				if (arrP < arrEndP)
					*arrP = val;
			}
		}

		for (int offset = array4[627 + vMin / 2]; offset; offset = array4[offset / 2]) {
			uint *arrP = &array3[offset / 2];
			uint threshold = ++arrP[0];
			if (threshold <= arrP[1])
				continue;

			uint *currP = arrP + 2;
			while (threshold > *currP)
				++currP;
			--currP;

			*arrP = *currP;
			*currP = threshold;
			int offset4 = array2[offset / 2];
			int newIndex = currP - array3;
			array4[offset4 / 2] = newIndex * 2;
			if (offset4 < (ARRAY_SIZE * 4 - 2))
				array4[offset4 / 2 + 1] = newIndex * 2;

			int newIndex2 = array2[newIndex] / 2;
			array2[newIndex] = offset4;
			array4[newIndex2] = offset;
			if ((newIndex2 * 2) <= (ARRAY_SIZE * 4 - 2))
				array4[newIndex2 + 1] = offset;

			array2[offset / 2] = newIndex2 * 2;
			offset = newIndex * 2;
		}

		vMin /= 2;
		if (vMin < 256) {
			// Single byte write to destination
			*destP++ = (byte)vMin;
			int buffOffset = array2[ARRAY_LAST2];
			array2[ARRAY_LAST2] = (buffOffset + 1) & 0xfff;
			buffer[buffOffset] = (byte)vMin;
			++byteIdx;
			continue;
		}

		uint16 bitsLow = bits, bitsHigh = 0;
		for (int ctr = 8; ctr > 0; --ctr) {
			bool highBit = bitsLow & 0x8000;
			bitsLow <<= 1;

			if (bitsLow) {
				bitsHigh = (bitsHigh << 1) | (highBit ? 1 : 0);
			} else {
				bitsLow = fSrc.readUint16BE();

				byte loBit = 1;
				do {
					bitsHigh = (bitsHigh << 1) | ((bitsLow & 0x8000) ? 1 : 0);
					bitsLow = (bitsLow << 1) | (loBit ? 1 : 0);
					loBit = 0;
				} while (--ctr > 0);
				break;
			}
		}
		bits = bitsLow;

		int t2Val = _DECODE_TABLE2[bitsHigh] << 6;
		int tCount = _DECODE_TABLE1[bitsHigh] - 2;

		for (int ctr = 0; ctr < tCount; ++ctr) {
			bool highBit = bits & 0x8000;
			bits <<= 1;
			if (!bits) {
				bits = fSrc.readUint16BE();
				highBit = bits & 0x8000;
				bits = (bits << 1) | 1;
			}

			bitsHigh = (bitsHigh << 1) | (highBit ? 1 : 0);
		}

		t2Val |= (bitsHigh & 0x3F);
		int buffOffset = array2[ARRAY_LAST2] - t2Val - 1;

		for (int ctr = 0; ctr < vMin - 253; ++ctr, ++buffOffset) {
			buffOffset &= 0xfff;
			byte b = buffer[buffOffset];
			*destP++ = b;

			uint &buffOffset2 = array2[ARRAY_LAST2];
			buffer[buffOffset2] = b;
			buffOffset2 = (buffOffset2 + 1) & 0xfff;
			++byteIdx;
		}
	}

	assert(destP == destEndP);
	screen.markAllDirty();
}

const char *const CloudsCutscenes::_INTRO_VOCS[14] = {
	"crodo1.voc", "crodo2.voc", "iamking.voc", "crodo3.voc",
	"ya1.voc", "crodo4a.voc", "crodo4b.voc", "crodo4c.voc",
	"xeenlaff.voc", "tiger2&.voc", "crodo5.voc", "crodo6.voc",
	"xeenlaff.voc", "tiger2&.voc"
};

const int CloudsCutscenes::_INTRO_FRAMES_LOOKUP[14] = {
	0, 1, 0, 2, 0, 3, 4, 5, 0, 0, 6, 7, 0, 0
};

const int CloudsCutscenes::_INTRO_FRAMES_VALS[8][32] = {
	{
		4, 2, 3, 0, 2, 3, 2, 0, 1, 1, 3, 4, 3, 2, 4, 2,
		3, 4, 3, 4, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}, {
		3, 2, 3, 2, 4, 3, 0, 3, 2, 2, 3, 1, 2, 3, 3, 3,
		2, 3, 2, 3, 2, 0, 3, 2, 0, 0, 0, 0, 0, 0, 2, 4
	}, {
		3, 1, 2, 3, 0, 3, 4, 3, 2, 3, 0, 3, 2, 3, 2, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 3
	}, {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 3
	}, {
		4, 2, 2, 3, 2, 3, 3, 4, 2, 4, 2, 0, 3, 2, 3, 2,
		3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 2, 3
	}, {
		2, 0, 2, 3, 2, 4, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 2, 3, 1
	}, {
		3, 2, 0, 2, 4, 2, 3, 2, 3, 2, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 4, 3, 4, 0, 2
	}, {
		3, 2, 4, 1, 2, 4, 3, 2, 3, 0, 2, 2, 0, 3, 2, 3,
		2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	}
};

const uint CloudsCutscenes::_INTRO_FRAMES_WAIT[8][32] = {
	{
		 2,  5,  6,  9, 10, 11, 12, 13, 14, 23, 25, 29, 31, 35, 38, 41,
		42, 45, 50, 52, 55, 56, 57,  0,  0,  0,  0,  0,  0,  0,  0,  0
	}, {
		 1,  4,  6,  8,  9, 11, 13, 15, 17, 18, 19, 22, 28, 29, 30, 31,
		 0, 39,  0, 44,  0, 50, 51,  0, 54,  0,  0,  0,  0,  0,  0,  4
	}, {
		 6,  9, 11, 13, 15, 19, 21, 23, 25, 27, 28, 31, 35, 39, 40,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  5,  7
	}, {
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  3,  4
	}, {
		 5,  9, 10, 11, 13, 15, 18, 23, 26, 31, 33, 36, 37, 41, 43, 45,
		48,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  0, 12
	}, {
		14, 17, 20, 23, 27, 29,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  2,  4,  8, 11, 13
	}, {
		15, 16, 17, 19, 21, 24, 24, 27, 34, 35,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  7, 10, 11, 13
	}, {
		17, 19, 22, 23, 26, 30, 32, 34, 40, 43, 47, 52, 53, 55, 57, 60,
		62,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
	}
};

const byte CloudsCutscenes::_DECODE_TABLE1[256] = {
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8
};

const byte CloudsCutscenes::_DECODE_TABLE2[256] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
	6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,
	10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
	12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 15,
	16, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 18, 19, 19, 19, 19,
	20, 20, 20, 20, 21, 21, 21, 21, 22, 22, 22, 22, 23, 23, 23, 23,
	24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 31,
	32, 32, 33, 33, 34, 34, 35, 35, 36, 36, 37, 37, 38, 38, 39, 39,
	40, 40, 41, 41, 42, 42, 43, 43, 44, 44, 45, 45, 46, 46, 47, 47,
	48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63
};

} // End of namespace WorldOfXeen
} // End of namespace Xeen
