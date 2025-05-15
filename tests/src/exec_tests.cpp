#include "assert.h"
#include <gtest/gtest.h>

#include <session/session.h>

using namespace cheeky;

TEST(ExecTest, SimpleBranchTest) {
    // TODO: path must start from the project root dir
    session::Session sesh("../tests/data/branching");
    auto state = sesh.get_state();

    ASSERT_EQ(state->get_r_ref_32(0), 100) << "Returned value must be incremented up to 100";
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
