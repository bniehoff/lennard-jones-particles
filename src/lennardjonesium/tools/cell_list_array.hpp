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

            /**
             * TODO: Functions related to iteration
             */

            std::generator<CellList&> cell_view();
            // const CellView& cell_view() const;

            // std::generator<NeighborPair&> neighbor_view();

            // NeighborView& neighbor_view();
            // const NeighborView& neighbor_view() const;

        protected:
            typedef boost::multi_array<CellList, 3> cell_list_array_type;
            typedef boost::array<cell_list_array_type::index, 3> multi_index_type;

            // We store the cell lists internally in a multidimensional array
            cell_list_array_type cell_lists_;
    };
} // namespace tools


#endif
