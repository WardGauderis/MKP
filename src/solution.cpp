//
// Created by ward on 3/14/22.
//

#include "solution.h"
#include "util.h"

bool verbose = false;

/**
 * Add an item to the solution
 * @param item
 * @param problem
 * @return success
 */
bool Solution::add(const size_t item, const problem& problem) {
	// Avoid adding twice
	if (sol[item]) return false;

	// Check the constraints
	for (size_t i = 0; i < resources_used.size(); ++i) {
		if (resources_used[i] + problem.constraints[item][i] > problem.capacities[i]) {
			return false;
		}
	}

	// Add the item and update the resources
	sol[item] = true;
	value += problem.profits[item];
	for (size_t i = 0; i < resources_used.size(); ++i) {
		resources_used[i] += problem.constraints[item][i];
	}

	return true;
}

/**
 * Remove an item from the solution
 * @param item
 * @param problem
 * @return success
 */
bool Solution::remove(size_t item, const problem& problem) {
	// Avoid removing twice
	if (!sol[item]) return false;

	// Remove the item and update the resources
	sol[item] = false;
	value -= problem.profits[item];
	for (size_t i = 0; i < resources_used.size(); ++i) {
		resources_used[i] -= problem.constraints[item][i];
	}

	return true;
}

/**
 * Create a solution with the item removed
 * @param item
 * @param problem
 * @return A new solution if successful
 */
std::optional<Solution> Solution::removed(size_t item, const problem& problem) {
	// Avoid removing twice
	if (!sol[item]) return {};

	// Remove the item and update the resources in the new solution
	auto solution      = *this;
	solution.sol[item] = false;
	solution.value -= problem.profits[item];
	for (size_t i = 0; i < solution.resources_used.size(); ++i) {
		solution.resources_used[i] -= problem.constraints[item][i];
	}

	return solution;
}

/**
 * Create a solution
 * @param p
 * @param CH the constructive heuristic to use
 */
Solution::Solution(const problem& p, void (Solution::*CH)(const problem&)):
	value(0), sol(p.n, false), resources_used(p.m, 0) {
	(this->*CH)(p);
}

/**
 * Create an alternative representation of the solution
 * @return a vector of selected items and a vector of discarded items
 */
std::pair<Vector<size_t>, Vector<size_t>> Solution::representation() const {
	Vector<size_t> selected;
	Vector<size_t> discarded;

	for (size_t i = 0; i < sol.size(); ++i) {
		if (sol[i]) {
			selected.push_back(i);
		} else {
			discarded.push_back(i);
		}
	}

	return std::pair{ selected, discarded };
}

/**
 * Print the solution
 * @param os
 * @param solution
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Solution& solution) {
	if (!verbose) {
		os << solution.value;
		return os;
	}

	auto [selected, discarded] = solution.representation();
	if (selected.empty()) os << "No items selected in the solution!\n";
	else {
		os << "Items in solution:\n[ ";
		for (size_t i = 0; i < solution.sol.size(); i++) {
			if (solution.sol[i]) os << i << " ";
		}
		os << "]\nSolution value: " << solution.value << "\n";
	}

	os << "[ ";
	for (const auto item : selected) { os << item << " "; }
	os << "] :: [ ";
	for (const auto item : discarded) { os << item << " "; }
	os << "]\n\n";

	return os;
}

/**
 * Update the solution with the random constructive heuristic
 * @param p
 */
void Solution::random(const problem& p) {
	// Create the random insert order
	int* v = create_shuffled(sol.size());

	for (size_t i = 0; i < sol.size(); i++) add(v[i], p);

	free(v);
}

/**
 * Update the solution with the greedy constructive heuristic
 * @param p
 */
void Solution::greedy(const problem& p) {
	// Create the insert order sorted by profit
	Vector<size_t> indices(sol.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::sort(indices.begin(), indices.end(),
	          [&p](size_t a, size_t b) { return p.profits[a] > p.profits[b]; });

	for (const auto index : indices) add(index, p);
}

/**
 * Update the solution with the toyoda constructive heuristic
 * @param p
 */
void Solution::toyoda(const problem& p) {
	// Create the insert order
	Vector<size_t> indices(sol.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Calculate A = W / L
	Matrix<double> A(sol.size(), resources_used.size());
	for (size_t i = 0; i < sol.size(); ++i)
		for (size_t j = 0; j < resources_used.size(); ++j)
			A(i, j) =
				static_cast<double>(p.constraints[i][j]) / static_cast<double>(p.capacities[j]);

	// Keep adding items while possible
	bool added = true;
	while (added) {
		added = false;

		// Calculate U then V
		auto v = A * (sol * A).normalize();
		// Calculate the pseudo-utility
		for (size_t i = 0; i < v.size(); ++i) v[i] = static_cast<double>(p.profits[i]) / v[i];

		// Sort insert order by pseudo-utility
		auto sorted_indices = indices;
		std::sort(sorted_indices.begin(), sorted_indices.end(),
		          [&v](size_t a, size_t b) { return v[a] > v[b]; });

		// Insert the first item possible
		for (const auto index : sorted_indices)
			if (add(index, p)) {
				added = true;
				break;
			}
	}
}

/**
 * Update the solution with the first improvement iterative improvement algorithm
 * @param p
 * @param CH the constructive heuristic to use
 */
void Solution::first_improvement(const problem& p, void (Solution::*CH)(const problem&)) {
	// Keep improving until a local minimum is reached
	bool changed;
	do {
		changed = false;
		// Create the removal order
		auto shuffled = create_shuffled(sol.size());

		for (size_t i = 0; i < sol.size(); ++i) {
			// Try to remove an item
			if (auto solution = removed(shuffled[i], p)) {
				// Apply the constructive heuristic after removal
				// The bool indicating the selection of the item is temporally set to true so the
				// constructive heuristic won't consider it again

				solution->sol[shuffled[i]] = true;
				(*solution.*CH)(p);
				solution->sol[shuffled[i]] = false;

				// Accept the first improvement
				if (solution->value > value) {
					*this   = *solution;
					changed = true;
					break;
				}
			}
		}

		free(shuffled);
	} while (changed);
}

/**
 * Update the solution with the best improvement iterative improvement algorithm
 * @param p
 * @param CH the constructive heuristic to use
 */
void Solution::best_improvement(const problem& p, void (Solution::*CH)(const problem&)) {
	// Keep improving until a local minimum is reached
	bool changed;
	do {
		changed = false;
		// Create the removal order
		auto shuffled = create_shuffled(sol.size());
		// Initialize the best neighbour so far
		auto best = *this;

		for (size_t i = 0; i < sol.size(); ++i) {
			// Try to remove an item
			if (auto solution = removed(shuffled[i], p)) {
				// Apply the constructive heuristic after removal
				// The bool indicating the selection of the item is temporally set to true so the
				// constructive heuristic won't consider it again
				solution->sol[shuffled[i]] = true;
				(*solution.*CH)(p);
				solution->sol[shuffled[i]] = false;

				// Remember the best improvement
				if (solution->value > best.value) {
					best    = *solution;
					changed = true;
				}
			}
		}

		// Accept the best improvement
		*this = best;

		free(shuffled);
	} while (changed);
}

/**
 * Update the solution with the variable neighborhood descent algorithm based on first improvement
 * @param p
 * @param CH the constructive heuristic to use
 */
void Solution::variable_neighbourhood_descent(const problem& p,
                                              void (Solution::*CH)(const problem&)) {
	// Initialize the neighbourhood size
	size_t k = 1;
	// Keep improving until a local minimum is reached in every neighbourhood
	while (k < 4) {
		// Create the removal order
		auto shuffled = create_shuffled(sol.size());
		// Create the solution which will be transformed into a neighbour
		auto solution = *this;

		// Explore the current neighbourhood until the criterion is reached for the first time
		bool found = explore_neighbourhood(solution, p, 0, k, shuffled, [&](Solution& solution) {
			// Apply the constructive heuristic after removal of k items
			(solution.*CH)(p);
			// Check if it is an improvement
			return solution.value > value;
		});

		// If an improvement is found, return to the first neighbourhood, otherwise try the next one
		if (found) {
			*this = solution;
			k     = 1;
		} else {
			++k;
		}

		free(shuffled);
	}
}

/**
 * Explore the neighbourhood of the solution of size k until the criterion is met for the first
 * time. All possible combinations of k item removals are tried through recursion.
 * @param solution the solution to start from and that will be modified
 * @param p
 * @param offset the offset in the removal order for the next item to be removed
 * @param k the amount of items that need to be removed
 * @param shuffled the removal order
 * @param criterion the criterion for choosing the new solution in the neighbourhood
 * @return the success of finding a matching neighbour
 */
bool explore_neighbourhood(Solution& solution, const problem& p, size_t offset, const size_t k,
                           const int* shuffled, const std::function<bool(Solution&)>& criterion) {
	// No items are left to be removed
	if (k == 0) {
		// Apply the criterion and undo its changes to the solution if unsuccessful
		// Return the success
		auto orignal = solution;
		bool success = criterion(solution);
		if (!success) solution = orignal;
		return success;
	};

	// More items must be removed recursively in order starting at the offset
	for (; offset < solution.sol.size(); ++offset) {
		// Try to remove one item
		if (!solution.remove(shuffled[offset], p)) continue;

		// Recursively remove the next k - 1 items starting from offset + 1
		// The bool indicating the selection of the item is temporally set to true so the
		// constructive heuristic won't consider it again
		solution.sol[shuffled[offset]] = true;
		bool found = explore_neighbourhood(solution, p, offset + 1, k - 1, shuffled, criterion);
		solution.sol[shuffled[offset]] = false;

		// Accept the first matching neighbour
		if (found) return true;

		// Otherwise, undo the removal and try removing the next item
		solution.add(shuffled[offset], p);
	}

	return false;
}

/**
 * Verify whether the solution is a valid solution for the problem
 * This is only used for debugging purposes
 * @param p
 */
void Solution::verify(const problem& p) const {
	unsigned int v = 0;
	Vector<int>  r(resources_used.size(), 0);

	for (size_t item = 0; item < sol.size(); ++item) {
		if (!sol[item]) continue;

		v += p.profits[item];

		for (size_t i = 0; i < r.size(); ++i) { r[i] += p.constraints[item][i]; }
	}

	assert(v == value);

	for (size_t i = 0; i < r.size(); ++i) {
		assert(r[i] <= p.capacities[i]);
		assert(r[i] == resources_used[i]);
	}
}
