#include <mpeg-isobase/boxes.h>
#include <gtest/gtest.h>
//#include <sstream>

using namespace mpeg::isobase;

TEST(Boxes, print) {
    boxes.print(std::cout);
}

