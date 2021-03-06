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

#include "bladerunner/obstacles.h"

#include "bladerunner/bladerunner.h"

namespace BladeRunner {

Obstacles::Obstacles(BladeRunnerEngine *vm) {
	_vm = vm;
	_vertices       = new Vector2[150];
	clear();
}

Obstacles::~Obstacles() {
	delete[] _vertices;
}

void Obstacles::clear() {
	for (int i = 0; i < kPolygonCount; i++) {
		_polygons[i].isPresent = false;
		_polygons[i].verticeCount = 0;
		for (int j = 0; j < kVertexCount; j++) {
			_polygons[i].vertices[j].x = 0.0f;
			_polygons[i].vertices[j].y = 0.0f;
		}
	}
	_verticeCount = 0;
	_backup = false;
	_count = 0;
}

void Obstacles::add(float x0, float z0, float x1, float z1) {
}

bool Obstacles::find(const Vector3 &from, const Vector3 &to, Vector3 *next) const {
	//TODO
	*next = to;
	return true;
}

void Obstacles::backup() {
}

void Obstacles::restore() {}


} // End of namespace BladeRunner
