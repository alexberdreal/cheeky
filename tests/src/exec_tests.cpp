#include <assert.h>
#include <stdexcept>

#include <gtest/gtest.h>

#include <session/session.h>

using namespace cheeky;

class ExecTest : public testing::Test {
public:
    void SetUp() override { 
        spdlog::drop_all();
    }
};

TEST(ExecTest, SimpleBranchTest) {
    // TODO: path must start from the project root dir
    std::string path { PROJECT_ROOT_DIR };
    session::Session sesh(path + "/tests/data/branching.o");
    auto state = sesh.get_state();

    ASSERT_EQ(state->get_r_ref_32(0), 100) << "Returned value must be incremented up to 100";
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
