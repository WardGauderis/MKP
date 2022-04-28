//
// Created by ward on 3/14/22.
//

#include "solution.h"
#include "util.h"

int main(int argc, char* argv[]) {
	params* pars = read_params(argc, argv);
	set_seed(pars->seed);

	problem* p = read_problem(pars->instance_file);
	if (verbose) print_problem(p);

	if (!pars->CH) {
		std::cout << "No constructive heuristic has been defined." << std::endl;
		return 1;
	}

	auto s = Solution(*p, pars->CH);
	if (verbose) std::cout << "After applying the constructive heuristic:" << std::endl;
	std::cout << s;

	if (pars->II) {
		(s.*pars->II)(*p, pars->CH);
		if (verbose) std::cout << "After applying the iterative improvement algorithm:";
		std::cout << std::endl << s;
	}

	destroy_problem(p);
	free(pars);

	return 0;
}
