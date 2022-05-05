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
#include <vector>

/**
 * Vector class
 * @tparam T
 */
template<class T> class Vector: public std::vector<T> {
public:
	Vector(): std::vector<T>() {}
	explicit Vector(size_t n): std::vector<T>(n) {}
	Vector(size_t n, T value): std::vector<T>(n, value) {}

	/**
	 * Normalize the vector and set the vector to 1 if it's the 0 vector (for the toyoda algorithm)
	 * @return a reference to this
	 */
	Vector& normalize() {
		// Because we are only interested in the rankings of the items according to pseudo-utility
		// we can ignore normalizing the vector U in the toyoda algorithm.
		if (std::all_of(this->begin(), this->end(), [](T x) { return x == 0; })) {
			for (auto& item : *this) { item = 1.0; }
		}

		//				T norm = 0.0;
		//				for (const auto& item : *this) { norm += item * item; }
		//				norm = std::sqrt(norm);
		//
		//				if (norm == 0.0) {
		//					for (auto& item : *this) { item = 1.0; }
		//				} else {
		//					for (auto& item : *this) { item /= norm; }
		//				}

		return *this;
	}
};

/**
 * Matrix class using the indexing trick for faster computation
 * @tparam T
 */
template<class T> class Matrix {
	size_t    n_tot;
	size_t    m_tot;
	Vector<T> items;

public:
	/**
	 * Construct the matrix
	 * @param n rows
	 * @param m columns
	 */
	Matrix(size_t n, size_t m): n_tot(n), m_tot(m), items(n * m) {}

	/**
	 * Index the matrix
	 * @param n
	 * @param m
	 * @return a reference
	 */
	T& operator()(size_t n, size_t m) { return items[n * m_tot + m]; }

	/**
	 * Index the matrix
	 * @param n
	 * @param m
	 * @return the value
	 */
	T operator()(size_t n, size_t m) const { return items[n * m_tot + m]; }

	/**
	 * Matrix vector multiplication M * C
	 * @param m matrix
	 * @param c vector
	 * @return
	 */
	friend Vector<T> operator*(const Matrix<T>& m, const Vector<T>& c) {
		Vector<T> r(m.n_tot, 0);

		for (size_t i = 0; i < m.n_tot; ++i)
			for (size_t j = 0; j < m.m_tot; ++j) r[i] += m(i, j) * c[j];

		return r;
	}

	/**
	 * Transposed Matrix vector multiplication M^T * C
	 * @tparam S
	 * @param c vector
	 * @param m matrix
	 * @return
	 */
	template<class S> friend Vector<T> operator*(const Vector<S>& c, const Matrix<T>& m) {
		Vector<T> r(m.m_tot, 0);

		for (size_t i = 0; i < m.n_tot; ++i)
			for (size_t j = 0; j < m.m_tot; ++j) r[j] += m(i, j) * c[i];

		return r;
	}
};

class Solution;
struct problem;

struct params {
	char* instance_file{};
	int   seed{};
	void (Solution::*CH)(const problem&){};
	void (Solution::*II)(const problem&, void (Solution::*CH)(const problem&)){};
	Solution (*SLA)(const problem&){};
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
