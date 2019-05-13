#include "gtest/gtest.h"
#include "capture.h"

namespace {
    class TestCapture : public testing::Test
	{
	    void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

TEST_F(TestCapture, Test_Init)
{
    int8_t i_mode = ns_capture::em_read_capfile_mode;
    ASSERT_EQ(RET::SUC, capture::getinstance().init(i_mode));
}
