/**
 * system_state.hpp
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

#ifndef LJ_SYSTEM_STATE_HPP
#define LJ_SYSTEM_STATE_HPP

#include <Eigen/Dense>

namespace physics
{
    struct SystemState
    {
        /**
         * We use 4xN matrices so that each of the columns will be aligned for vectorization.
         * 
         * Eigen names the components of a 4-vector .x(), .y(), .z(), .w(), in that order.  So,
         * we will leave the 4th component unused (it must be set to zero in order for arithmetic
         * with += to work nicely).
         */

        // It will be useful to talk about functions acting on the SystemState
        typedef std::function<SystemState& (SystemState&)> Operator;

        // Kinematic quantities (properties of the motion itself)
        Eigen::Matrix4Xd positions;        // Position within the bounding box
        Eigen::Matrix4Xd velocities;       // Velocity
        Eigen::Matrix4Xd displacements;    // Total displacement ignoring boundary conditions
        double kinetic_energy;

        // Dynamic quantities (arising from the interactions between particles)
        Eigen::Matrix4Xd forces;           // Force or acceleration, since mass is normalized to 1
        double potential_energy;    // Potential energy from particle interactions
        double virial;              // Virial from pairwise forces

        /**
         * TODO: Consider defining kinetic energy tensor and virial tensor, which can be used
         * to compute shear stresses as well as pressure.
         */

        /**
         * The argument specifies the size of the system, not any of the data in it, so we use
         * explicit to make sure this constructor can't be used for implicit conversions from int.
         */
        explicit SystemState(int particle_count = 0);

        /**
         * This both sets the particle count AND initializes all array data to zero.
         */
        SystemState& set_particle_count(int particle_count);
    };

    /**
     * Let's also define the concept of an Operator that acts on the SystemState.
     * 
     * This definition is more generic than necessary, we will only use the SystemState version.
     */
    template <typename Op, typename S = SystemState>
    concept Operator = std::invocable<Op, S&> and std::is_invocable_r_v<S&, Op, S&>;

    /**
     * Operator that simply returns the state without change
     */
    inline auto identity_operator = [](SystemState& s) -> SystemState& {return s;};

    /**
     * Operators can act on SystemStates via the pipe syntax
     * 
     *      state | op1 | op2 | ...;
     */
    SystemState& operator| (SystemState& s, const Operator auto& op)
    {
        return op(s);
    }

    /**
     * Operators together in a pipeline can also be combined into a single operator
     * 
     *      combined_op = op1 | op2 | op3 | ...;
     */
    Operator auto operator| (const Operator auto& op1, const Operator auto& op2)
    {
        return [op1=std::move(op1), op2=std::move(op2)](SystemState& s) -> SystemState&
            {
                return op2(op1(s));
            };
    }
} // namespace physics

#endif