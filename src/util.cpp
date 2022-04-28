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

#include "solution.h"
#include "util.h"

void set_seed(int seed) { srand(seed); }

int* create_shuffled(int n) {
	int i, *v = static_cast<int*>(malloc(n * sizeof(int)));
	for (i = 0; i < n; i++) v[i] = i;

	shuffle_int(v, n);

	return (v);
}

void shuffle_int(int* v, int n) {
	int i, j, tmp;
	for (i = n - 1; i >= 1; i--) {
		j    = rand() % i;
		tmp  = v[i];
		v[i] = v[j];
		v[j] = tmp;
	}
}

params* read_params(int argc, char* argv[]) {
	int i;

	// check in mkpdata.h what fields there are

	auto* pars = static_cast<params*>(malloc(sizeof(params)));

	pars->instance_file = argv[1];
	for (i = 2; i < argc; i++) {
		if (strcmp(argv[i], "--seed") == 0) {
			pars->seed = atoi(argv[++i]);
		} else if (strcmp(argv[i], "--verbose") == 0) {
			verbose = true;
		} else if (strcmp(argv[i], "--random") == 0) {
			pars->CH = &Solution::random;
		} else if (strcmp(argv[i], "--greedy") == 0) {
			pars->CH = &Solution::greedy;
		} else if (strcmp(argv[i], "--toyoda") == 0) {
			pars->CH = &Solution::toyoda;
		} else if (strcmp(argv[i], "--FI") == 0) {
			pars->II = &Solution::first_improvement;
		} else if (strcmp(argv[i], "--BI") == 0) {
			pars->II = &Solution::best_improvement;
		} else if (strcmp(argv[i], "--VND") == 0) {
			pars->II = &Solution::variable_neighbourhood_descent;
		} else if (strcmp(argv[i], "--SA") == 0) {
			pars->CH = &Solution::simulated_annealing;
		}
	}
	return (pars);
}
