//
// Created by ward on 3/14/22.
//

#ifndef MKP_SOLUTION_H
#define MKP_SOLUTION_H

#include "mkpproblem.h"

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <optional>
#include <vector>

// Output style
extern bool verbose;

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
		T norm = 0.0;
		for (const auto& item : *this) { norm += item * item; }
		norm = std::sqrt(norm);

		if (norm == 0.0) {
			for (auto& item : *this) { item = 1.0; }
		} else {
			for (auto& item : *this) { item /= norm; }
		}

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

/**
 * Class containing a MKP solution
 */
class Solution {
	// Profit of the solution
	unsigned int value;
	// Amount of selected items;
	size_t size;
	// Indication of the selected items
	Vector<bool> sol;
	// Count of the used resources
	Vector<int> resources_used;

	[[nodiscard]] std::pair<Vector<size_t>, Vector<size_t>> representation() const;

	bool add(size_t item, const problem& problem);

	bool remove(size_t item, const problem& problem);

	std::optional<Solution> removed(size_t item, const problem& problem);

	friend bool explore_neighbourhood(Solution& solution, const problem& p, size_t offset, size_t k,
	                                  const int*                            shuffled,
	                                  const std::function<bool(Solution&)>& criterion);

	unsigned int random_item() const;

public:
	explicit Solution(const problem& p, void (Solution::*CH)(const problem&));

	void random(const problem& p);

	void greedy(const problem& p);

	void toyoda(const problem& p);

	void first_improvement(const problem& p, void (Solution::*CH)(const problem&));

	void best_improvement(const problem& p, void (Solution::*CH)(const problem&));

	void variable_neighbourhood_descent(const problem& p, void (Solution::*CH)(const problem&));

	void simulated_annealing(const problem& p);

	friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

	void verify(const problem& p) const;
};

#endif    // MKP_SOLUTION_H
