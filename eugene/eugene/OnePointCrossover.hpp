/* This file is part of Eugene
 * Copyright 2007-2012 David Robillard <http://drobilla.net>
 *
 * Eugene is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Eugene is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with Eugene.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EUGENE_ONEPOINTCROSSOVER_HPP
#define EUGENE_ONEPOINTCROSSOVER_HPP

#include <cassert>
#include <utility>

#include "eugene/Crossover.hpp"

namespace eugene {

template<typename G>
struct OnePointCrossover : public Crossover<G> {
	std::pair<G, G> crossover(Random& rng, const G& mom, const G& dad) {
		assert(mom.size() == dad.size());

		const size_t size = mom.size();

		G child_a(mom);
		G child_b(dad);

		const size_t chop_index = rng.natural(size);
		for (size_t i = chop_index; i < size; ++i) {
			child_a[i] = dad[i];
			child_b[i] = mom[i];
		}

		assert(child_a.size() == mom.size());
		assert(child_a.size() == child_b.size());

		return make_pair(child_a, child_b);
	}
};

} // namespace eugene

#endif // EUGENE_ONEPOINTCROSSOVER_HPP
