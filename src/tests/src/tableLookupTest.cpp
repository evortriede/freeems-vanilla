/*
 * tableLookupTest.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: eric
 */


#include "gtest/gtest.h"
#define UNIT_TEST_CONFIG

class tableLookupTest : public ::testing::Test {
 protected:

};


TEST_F(tableLookupTest, SanityOfGtest) {
  EXPECT_EQ(0, 0);
}


int tableLookupTestMain(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
