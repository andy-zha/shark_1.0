#include "gtest/gtest.h"
#include "recyclequeue.h"

namespace {
    class TestRecyclequeue : public testing::Test
	{
	    void SetUp() override
		{};

		void TearDown() override
		{};
	};
}

class demo
{
    public:
	    demo()
		{}

		~demo()
		{}

	public:
	    int i;
		std::string str;
};

TEST_F(TestRecyclequeue, Test_Cycle)
{
    recyclequeue<demo> queue;

	//test 缓冲区初始化
	ASSERT_EQ(RET::SUC, queue.init(12));

    //test push进缓冲区
	demo *de = new demo();
	de->i = 1;
	de->str = "hello world!";
	ASSERT_EQ(RET::SUC, queue.push(de));

    //test pop出缓冲区
	demo *out = queue.pop();
	ASSERT_EQ(1, out->i);
	ASSERT_STREQ("hello world!", out->str.c_str());
}
