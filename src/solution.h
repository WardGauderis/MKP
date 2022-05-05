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

	void remove_unchecked(size_t item, const problem& problem);

	friend bool explore_neighbourhood(Solution& solution, const problem& p, size_t offset, size_t k,
	                                  const int*                            shuffled,
	                                  const std::function<bool(Solution&)>& criterion);

	[[nodiscard]] unsigned int random_item() const;

	void repair(const problem& p);

	void mutate(const problem& p);

	friend Solution crossover(const Solution& a, const Solution& b, const problem& p);

public:
	explicit Solution(const problem& p, void (Solution::*CH)(const problem&));

	void random(const problem& p);

	void greedy(const problem& p);

	void toyoda(const problem& p);

	void first_improvement(const problem& p, void (Solution::*CH)(const problem&));

	void best_improvement(const problem& p, void (Solution::*CH)(const problem&));

	void variable_neighbourhood_descent(const problem& p, void (Solution::*CH)(const problem&));

	bool invalid(const problem& p) const;

	void validate(const problem& p) const;

	friend std::ostream& operator<<(std::ostream& os, const Solution& solution);

	friend Solution simulated_annealing(const problem& p);

	friend Solution memetic_algorithm(const problem& p);

	inline bool operator<(const Solution& s) const { return (value < s.value); }

	inline bool operator>(const Solution& s) const { return (value > s.value); }

	inline bool operator<=(const Solution& s) const { return (value <= s.value); }

	inline bool operator>=(const Solution& s) const { return (value >= s.value); }

	/**
	 * Check if two solutions are completely equal
	 * @param s
	 * @return
	 */
	inline bool operator==(const Solution& s) const {
		if (value != s.value) return false;
		return sol == s.sol;
	}
};

Solution simulated_annealing(const problem& p);

Solution memetic_algorithm(const problem& p);

#endif    // MKP_SOLUTION_H
