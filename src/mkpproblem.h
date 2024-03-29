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

#ifndef __MKPPROBLEM_H__
#define __MKPPROBLEM_H__

#include "mkpio.h"
#include "util.h"

struct problem {
	int            n;
	int            m;
	int            best_known;
	int*           profits;
	int**          constraints;
	int*           capacities;
	// The rescaled constraint matrix used in Toyoda
	Matrix<double> A;

	problem(int n, int m, int b, int* profits, int** constraints, int* capacities);

	[[nodiscard]] unsigned int runtime() const;
	[[nodiscard]] double       initial_temperature() const;
	[[nodiscard]] double       cooling_factor() const;
};

void destroy_problem(problem* p);

void print_problem(problem* p);

problem* read_problem(char* filename);

#endif
