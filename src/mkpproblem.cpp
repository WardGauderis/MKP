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

#include "mkpproblem.h"

/**
 * Create a problem and also initialize the rescaled constraint value matrix A used in Toyoda
 * @param n
 * @param m
 * @param b
 * @param profits
 * @param constraints
 * @param capacities
 */
problem::problem(int n, int m, int b, int* profits, int** constraints, int* capacities):
	n(n), m(m), best_known(b), profits(profits), constraints(constraints), capacities(capacities),
	A(n, m) {
	for (size_t i = 0; i < static_cast<size_t>(n); ++i)
		for (size_t j = 0; j < static_cast<size_t>(m); ++j)
			A(i, j) = static_cast<double>(constraints[i][j]) / static_cast<double>(capacities[j]);
}


void print_problem(problem* p) {
	int i, j;

	printf("Problem data:\n");
	printf("%d objects, %d knapsacks", p->n, p->m);
	if (p->best_known != 0) printf(", best known value: %d", p->best_known);
	printf("\n\n");

	printf("Object profits:\n");
	for (i = 0; i < p->n; i++) { printf("%d ", p->profits[i]); }
	printf("\n\n");

	printf("Constraint matrix:\n");
	for (i = 0; i < p->m; i++) {
		for (j = 0; j < p->n - 1; j++) { printf("%d * x%d + ", p->constraints[j][i], j); }
		printf("%d * x%d <= %d\n", p->constraints[p->n - 1][i], p->n - 1, p->capacities[i]);
	}
	printf("\n");
}

problem* read_problem(char* filename) {
	FILE* input_file = open_file(filename);

	int* problem_data = read_problem_data(input_file);
	int  n = problem_data[0], m = problem_data[1], b = problem_data[2];

	int*  profits     = read_profits(input_file, n);
	int** constraints = read_constraints(input_file, m, n);
	int*  capacities  = read_capacities(input_file, m);

	close_file(input_file);

	problem* p = new problem(n, m, b, profits, constraints, capacities);

	free(problem_data);

	return (p);
}

unsigned int problem::runtime() const { return n * m / 10; }

double problem::initial_temperature() const {
	auto min = profits[0];
	auto max = profits[0];
	for (int i = 1; i < n; i++) {
		if (profits[i] < min) min = profits[i];
		if (profits[i] > max) max = profits[i];
	}

	return (min - max) * 0.1 / std::log(0.99);
}

double problem::cooling_factor() const {
	return std::pow(std::log(0.99) / std::log(0.02), 1.0 / (runtime() * 1000));
}
