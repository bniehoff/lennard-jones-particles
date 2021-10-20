/**
 * Test Velocity Verlet Integrator
 */

#include <catch2/catch.hpp>
#include <Eigen/Dense>

#include <src/lennardjonesium/engine/velocity_verlet_integrator.hpp>
#include <src/lennardjonesium/engine/system_state.hpp>

using Eigen::Matrix4Xd;
using Eigen::Vector4d;

using engine::VelocityVerletIntegrator;
using engine::SystemState;

SCENARIO( "Inertial motion without forces" ) {
    SystemState state(2);

    // Set initial velocities
    state.velocities.col(0) = Vector4d{1.0, 0, 0, 0};
    state.velocities.col(1) = Vector4d{0, 1.0, 0, 0};

    // Configure integrator with time step 1
    VelocityVerletIntegrator integrator(1.0);

    WHEN( "I evolve the state by 4 time steps" ) {
        state | integrator | integrator | integrator | integrator;

        THEN( "The positions move in the expected way" ) {
            REQUIRE( Vector4d{4.0, 0, 0, 0} == state.positions.col(0) );
            REQUIRE( Vector4d{0, 4.0, 0, 0} == state.positions.col(1) );
        }

        THEN( "The displacements move in the same way" ) {
            REQUIRE( Vector4d{4.0, 0, 0, 0} == state.displacements.col(0) );
            REQUIRE( Vector4d{0, 4.0, 0, 0} == state.displacements.col(1) );
        }

        THEN( "The velocities are unchanged" ) {
            REQUIRE( Vector4d{1.0, 0, 0, 0} == state.velocities.col(0) );
            REQUIRE( Vector4d{0, 1.0, 0, 0} == state.velocities.col(1) );
        }
    }
}

SCENARIO( "Motion under a gravitational force" ) {
    SystemState state(2);

    // Set initial velocities
    state.velocities.col(0) = Vector4d{1.0, 0, 0, 0};
    state.velocities.col(1) = Vector4d{0, 1.0, 0, 0};

    // Set a gravitational force
    state.forces.col(0) = state.forces.col(1) = Vector4d{0, 0, -1.0, 0};

    // Configure integrator with time step 1
    VelocityVerletIntegrator integrator(1.0);

    WHEN( "I evolve the state by 4 time steps" ) {
        state | integrator | integrator | integrator | integrator;

        // Expected result from four iterations of Velocity Verlet with timestep 1.0
        const double expected_z_coordinate = -((1./2.) + (3./2.) + (5./2.) + (7./2.));

        THEN( "The positions move in the expected way" ) {
            REQUIRE( Vector4d{4.0, 0, expected_z_coordinate, 0} == state.positions.col(0) );
            REQUIRE( Vector4d{0, 4.0, expected_z_coordinate, 0} == state.positions.col(1) );
        }

        THEN( "The displacements move in the same way" ) {
            REQUIRE( Vector4d{4.0, 0, expected_z_coordinate, 0} == state.displacements.col(0) );
            REQUIRE( Vector4d{0, 4.0, expected_z_coordinate, 0} == state.displacements.col(1) );
        }

        THEN( "The velocities have accelerated downwards by the expected amount" ) {
            REQUIRE( Vector4d{1.0, 0, -4.0, 0} == state.velocities.col(0) );
            REQUIRE( Vector4d{0, 1.0, -4.0, 0} == state.velocities.col(1) );
        }
    }
}
