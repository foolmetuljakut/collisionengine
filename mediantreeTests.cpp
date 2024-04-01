#include <vector>

#include "mediantree.hpp"
#include <gtest/gtest.h>

class Vec2 {
public:
    float x,y;
};

class Obj {
public:
    Vec2 pos;
};

class MedianTreeTesting : public testing::TestWithParam<size_t> {
protected:
    std::map<size_t, std::vector<Obj>> pointMap;
public:
    MedianTreeTesting() {
        pointMap.emplace();
        pointMap[0].push_back({{0, 0}});
        pointMap[0].push_back({{3, 1}});
        pointMap[0].push_back({{1, 3}});
        pointMap[0].push_back({{4, 4}});
    }
};

TEST_P(MedianTreeTesting, BranchAndLeafTests) {
    int input = GetParam();
    std::vector<Obj> pointList = pointMap[input];
 
    MedianTree<Obj> mtree;
    mtree.add(std::move(pointList));

    ASSERT_EQ(input, input);
}

INSTANTIATE_TEST_SUITE_P(MedianTreeTests, MedianTreeTesting, testing::Values(0));

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}