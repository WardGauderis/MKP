/***************************************************************************
 *   Copyright (C) 2018 Alberto Franzin                                    *
 *   alberto.franzin@ulb.ac.be                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef __MKPUTIL_H__
#define __MKPUTIL_H__

#include <cassert>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>

class Solution;
struct problem;

struct params {
	char* instance_file{};
	int   seed{};
	void (Solution::*CH)(const problem&){};
	void (Solution::*II)(const problem&, void (Solution::*CH)(const problem&)){};
	Solution (*VND)(const problem&){};
};

// set the random seed
void set_seed(int seed);

// create a vector of n shuffled integers (values from 0 to n-1)
int* create_shuffled(int n);

// shuffle vector of n integers
void shuffle_int(int* vector, int n);

// read command line parameters: TO BE EXTENDED
params* read_params(int argc, char* argv[]);

#endif
