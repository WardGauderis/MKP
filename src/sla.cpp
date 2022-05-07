//
// Created by ward on 5/1/22.
//

#include "solution.h"
#include "util.h"

#include <chrono>
#include <csignal>
#include <queue>
#include <ranges>

using namespace std::chrono;

// Atomic variable to indicate when an SLA algorithm should stop
volatile std::sig_atomic_t stop = 0;

/**
 * Signal handler that sets the stop variable
 * @param signum
 */
void handle_stop(int signum __attribute__((unused))) { stop = 1; }

/**
 * Simulated annealing algorithm
 * @param p
 * @return
 */
Solution simulated_annealing(const problem& p) {
	// Set the signal handler and schedule an alarm
	std::signal(SIGALRM, handle_stop);
	alarm(p.runtime());

	// Construct an initial solution using the Random constructive heuristic
	auto solution = Solution(p, &Solution::random);

	// Set the geometric annealing schedule
	const auto init_T = p.initial_temperature();
	auto       T      = init_T;
	const auto alpha  = p.cooling_factor();

	auto begin = steady_clock::now();

	while (true) {
		// Stop when the alarm has run
		if (stop) {
			stop = 0;
			return solution;
		}

		// Do 20000 iterations at each temperature
		for (int i = 0; i < 20000; ++i) {
			// Create a neighbour in the 3-neighbourhood of the solution
			auto neighbour = solution;

			// Remove 3 random items
			std::array<unsigned int, 3> removed{};
			for (unsigned int& r : removed) {
				r = neighbour.random_item();
				neighbour.remove_unchecked(r, p);
			}

			// Apply Toyoda after removal
			// The bool indicating the selection of the item is temporally set to true so the
			// constructive heuristic won't consider it again
			for (unsigned int r : removed) { neighbour.sol[r] = true; }
			neighbour.toyoda(p);
			for (unsigned int r : removed) { neighbour.sol[r] = false; }

			// Metropolis condition
			if (neighbour >= solution) {
				// Accept any improving neighbour
				solution = neighbour;
			} else {
				// Accept a worsening neighbour with a probability depending on the temperature
				auto accept_chance =
					std::exp(-static_cast<double>(solution.value - neighbour.value) / T);
				if (accept_chance > static_cast<double>(std::rand()) / RAND_MAX) {
					solution = neighbour;
				}
			}
		}

		// Decrease the temperature using the schedule based on how many milliseconds have passed
		T = init_T *
		    std::pow(alpha, duration_cast<milliseconds>(steady_clock::now() - begin).count());
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Memetic/Evolutionary algorithm
 * @param p
 * @return
 */
Solution memetic_algorithm(const problem& p) {
	// Set the signal handler and schedule an alarm
	std::signal(SIGALRM, handle_stop);
	alarm(p.runtime());

	// Initialize the population with the random constructive heuristic
	auto                  N = 100;
	std::vector<Solution> population;
	for (int i = 0; i < N; ++i) { population.emplace_back(p, &Solution::random); }

	while (true) {
		// Stop when the alarm has run and return the best individual
		if (stop) {
			stop = 0;
			return *std::max_element(population.begin(), population.end());
		}

		// Apply binary tournament selection for both parents
		auto parent_1 = std::max(population[std::rand() % N], population[std::rand() % N]);
		auto parent_2 = std::max(population[std::rand() % N], population[std::rand() % N]);

		// Recombine them into an invalid child solution using crossover
		Solution child = crossover(parent_1, parent_2, p);
		// Apply the first improvement algorithm with Toyoda
		// This is disabled as it didn't improve the solution quality
		// child.repair(p
		// child.first_improvement(p, &Solution::toyoda);
		// Mutate the invalid child
		child.mutate(p);
		// Make the child valid again
		child.repair(p);
		// Apply the first improvement algorithm with Toyoda
		// This is disabled as it didn't improve the solution quality
		// child.first_improvement(p, &Solution::toyoda);

		// If the child already exists, don't add it
		if (std::find(population.begin(), population.end(), child) != population.end()) {
			continue;
		}

		// Replace the worst scoring individual with the child
		auto min = std::min_element(population.begin(), population.end());
//		if (child > *min) { *min = std::move(child); }
		*min = std::move(child);
	}
}

/**
 * Recombine two solutions using uniform crossover
 * @param a
 * @param b
 * @param p
 * @return an invalid solution
 */
Solution crossover(const Solution& a, const Solution& b, const problem& p) {
	auto child = a;

	// For every item choose if we copy its inclusion from a or b
	for (size_t item = 0; item < child.sol.size(); ++item) {
		// Only update the child with b if b is different
		if (a.sol[item] != b.sol[item]) {
			// With 50% chance copy it from b
			if (rand() & 1) {
				child.sol[item] = b.sol[item];

				// Update the value and resources when changed
				auto sign = child.sol[item] ? 1 : -1;

				child.value += sign * p.profits[item];
				for (size_t resource = 0; resource < child.resources_used.size(); ++resource) {
					child.resources_used[resource] += sign * p.constraints[item][resource];
				}
			}
		}
	}

	return child;
}

/**
 * Mutate the child by flipping two item's inclusion at random
 * @param p
 */
void Solution::mutate(const problem& p) {
	for (int i = 0; i < 2; ++i) {
		// Flip a random item's inclusion
		auto item = rand() % sol.size();
		sol[item].flip();

		// Update the value and resources
		auto sign = sol[item] ? 1 : -1;

		value += sign * p.profits[item];
		for (size_t resource = 0; resource < resources_used.size(); ++resource) {
			resources_used[resource] += sign * p.constraints[item][resource];
		}
	}
}

/**
 * Repair an invalid solution using the pseudo-utility of Toyoda
 * by first dropping items until no constraint is violated
 * and then adding as many items as possible as in Toyoda
 * but without updating the U and V vectors for speed
 * @param p
 */
void Solution::repair(const problem& p) {
	// Create the insert order
	Vector<size_t> indices(sol.size());
	std::iota(indices.begin(), indices.end(), 0);

	// Calculate U then V
	auto v = p.A * (sol * p.A).normalize();
	// Calculate the pseudo-utility
	for (size_t i = 0; i < v.size(); ++i) v[i] = static_cast<double>(p.profits[i]) / v[i];

	// Sort insert order by pseudo-utility
	std::sort(indices.begin(), indices.end(), [&v](size_t a, size_t b) { return v[a] > v[b]; });

	// Remove items in order until no constraint is violated
	if (invalid(p))
		for (const auto& index : std::ranges::reverse_view(indices))
			if (remove(index, p) && !invalid(p)) break;

	// Add as many items as possible in order
	for (const auto& index : indices) add(index, p);
}
