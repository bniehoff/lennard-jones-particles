/**
 * measurements.cpp
 * 
 * Copyright (c) 2021-2022 Benjamin E. Niehoff
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

#include <Eigen/Dense>

#include <lennardjonesium/physics/system_state.hpp>
#include <lennardjonesium/physics/measurements.hpp>

namespace physics
{
    /**
     * We define some various types of measurements that can be made on the SystemState.
     * These are simply functions, which return a value of the appropriate type
     */

    double kinetic_energy(const SystemState& state)
    {
        return (1./2.) * state.velocities.colwise().squaredNorm().sum();
    }

    Eigen::Vector4d total_momentum(const SystemState& state)
    {
        return state.velocities.rowwise().sum();
    }

    Eigen::Vector4d total_force(const SystemState& state)
    {
        return state.forces.rowwise().sum();
    }

    Eigen::Vector4d center_of_mass(const SystemState& state)
    {
        return state.positions.rowwise().sum() / static_cast<double>(state.particle_count());
    }

    // Eigen::Vector4d total_angular_momentum(const SystemState&);

    // Eigen::Matrix4d inertia_tensor(const SystemState&);
} // namespace physics