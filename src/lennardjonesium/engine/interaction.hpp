/**
 * interaction.hpp
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

#ifndef LJ_INTERACTION_HPP
#define LJ_INTERACTION_HPP

#include <lennardjonesium/physics/system_state.hpp>

namespace engine
{   
    class Interaction
    {
        /**
         * An Interaction acts on the SystemState to impose forces on the particles.  Forces may
         * be background forces (like gravity), long-range forces (like electrostatic forces), or
         * short-range forces (like the Lennard-Jones force).
         * 
         * The action of an Interaction object is executed by operator().  It should compute the
         * forces that result from the physical interaction it represents.
         */
        public:
            // Compute the forces resulting from this interaction
            virtual physics::SystemState& operator() (physics::SystemState&) const = 0;
            
    };
} // namespace physics

#endif