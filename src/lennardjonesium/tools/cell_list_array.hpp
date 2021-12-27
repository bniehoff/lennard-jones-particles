/**
 * cell_list_array.hpp
 * 
 * Copyright (c) 2021 Benjamin E. Niehoff
 * 
 * This file is part of Lennard-Jonesium.
 * 
 * Lennard-Jonesium is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 * 
 * Lennard-Jonesium is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public
 * License along with Lennard-Jonesium.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef LJ_CELL_LIST_ARRAY_HPP
#define LJ_CELL_LIST_ARRAY_HPP

#include <vector>
#include <tuple>
#include <ranges>

#include <boost/multi_array.hpp>
#include <Eigen/Dense>

#include <lennardjonesium/draft_cpp23/generator.hpp>
#include <lennardjonesium/tools/dimensions.hpp>

namespace tools
{
    typedef std::vector<int> CellList;

    struct NeighborPair
    {
        const CellList& first;
        const CellList& second;

        // A vector of integers which indicates how the second cell should be offset, in multiples
        // of the total simulation box dimensions, in order to appear next to the first cell
        // (assuming periodic boundary conditions).
        Eigen::Vector4i offset;
    };

    class CellListArray
    {
        /**
         * A CellListArray stores the cell lists in a 3-dimensional array structure and implements
         * an interface for iterating over each unique pair of neighboring cells.
         * 
         * This is basically a wrapper around boost::multi_array which adds functionality.
         */

        public:
            // Construct the cell list array from a simulation box size, and a minimum cell size
            CellListArray(Dimensions dimensions, double cutoff_length);

            // Access a given element (cannot use operator[] with more than one parameter)
            CellList& operator() (int, int, int);
            const CellList& operator() (int, int, int) const;

            // Get the shape of the array (should not be needed for iteration usually; instead
            // use the generator methods below)
            // The type here is a raw pointer to a C-style array of size_t
            const boost::multi_array_types::size_type* shape();

            // Generators for iterating over cells and pairs of adjacent cells.
            // These are always used in a non-const context, so we do not write const versions.
            std::generator<CellList&> cell_view();
            std::generator<NeighborPair&&> neighbor_view();

        protected:
            typedef boost::multi_array<CellList, 3> cell_list_array_type;
            typedef boost::array<cell_list_array_type::index, 3> multi_index_type;

            // We store the cell lists internally in a multidimensional array
            cell_list_array_type cell_lists_;

            // Computes a NeighborPair from the first index and a displacement.
            NeighborPair get_neighbor_pair_
                (const multi_index_type index, const multi_index_type displacement);
    };
} // namespace tools


#endif