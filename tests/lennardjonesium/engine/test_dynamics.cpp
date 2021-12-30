/**
 * Test for PeriodicBoundaryCondition
 */

#include <catch2/catch.hpp>
#include <Eigen/Dense>

#include <src/lennardjonesium/engine/dynamics.hpp>
#include <src/lennardjonesium/physics/system_state.hpp>
#include <src/lennardjonesium/tools/dimensions.hpp>

#include <src/lennardjonesium/engine/periodic_boundary_condition.hpp>

// In order to test some protected methods, it is useful to define a derived class which makes
// them public
class TestDynamics : public engine::Dynamics
{
    public:
        using engine::Dynamics::Dynamics;
        using engine::Dynamics::impose_boundary_conditions_;
        using engine::Dynamics::rebuild_cell_lists_;
        using engine::Dynamics::compute_forces_;
};

SCENARIO("Imposing boundary conditions")
{
    // System of 3 particles
    physics::SystemState state(3);

    // Set the positions
    state.positions = Eigen::Matrix4Xd{
        {1.7,   0,    0},
        {0,   3.2,    0},
        {0,     0, -4.3},
        {0,     0,    0}
    };

    // Set up a box of side length 1.5
    tools::Dimensions dimensions{1.5};

    /**
     * TODO: Can't write tests until we have defined more about PairwiseForce.
     * 
     * Paste in old PBC test, we will use the values
     */

    // Use a bounding box of side length 1.5
    engine::PeriodicBoundaryCondition boundary_condition(1.5);

    WHEN( "I impose the boundary conditions on the system" ) {
        state | boundary_condition;

        THEN( "The particle positions are set to the correct locations within the box" ) {
            REQUIRE( Approx(0.2) == state.positions(0, 0) );
            REQUIRE( Approx(0.2) == state.positions(1, 1) );
            REQUIRE( Approx(0.2) == state.positions(2, 2) );
        }
    }
}
