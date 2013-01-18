/*
 * tableLookupTest.cpp
 *
 *  Created on: Jan 13, 2013
 *      Author: eric
 */

#include "gtest/gtest.h"
#define UNIT_TEST_CONFIG
#undef FPAGE_FE
#undef FPAGE_F8

#define FPAGE_FE
#define FPAGE_F8

#include "inc/generalTypes.h"
#include "inc/globalDefines.h"
#include "inc/tableTypes.h"
#include "inc/errorDefines.h"

#define lookupBlockDetails(x, y) my_lookupBlockDetails(x, y)

static unsigned short my_lookupBlockDetails(unsigned short, blockDetails*);

extern unsigned char RPAGE;

#include "tableLookup.c"


class tableLookupTest : public ::testing::Test {
 protected:

};

static unsigned short my_lookupBlockDetails(unsigned short locationId, blockDetails* details)
{
	return 0;
}

TEST_F(tableLookupTest, SanityOfGtest) {
  EXPECT_EQ(0, 0);
}

TEST_F(tableLookupTest, lookupTwoDTableUSShouldReturnValueCorrespondingToExactMatch)
{
	twoDTableUS Table = {{2,4,6,8,10},{100,200,300,400,500}};
	EXPECT_EQ(100, lookupTwoDTableUS(&Table, 2));
	EXPECT_EQ(200, lookupTwoDTableUS(&Table, 4));
	EXPECT_EQ(300, lookupTwoDTableUS(&Table, 6));
	EXPECT_EQ(400, lookupTwoDTableUS(&Table, 8));
	EXPECT_EQ(500, lookupTwoDTableUS(&Table, 10));
}

TEST_F(tableLookupTest, lookupTwoDTableUSShouldReturnInterpolatedValueForInbetweenInputValues)
{
	twoDTableUS Table = {{2,4,6,8,10},{100,200,300,400,500}};
	//EXPECT_EQ(50, lookupTwoDTableUS(&Table, 1));  TODO: This test case should work or at least not code dump
	// Here's a question: should the foregoing assume 0 as the implicit -1th table value or should we return the 0th value?
	EXPECT_EQ(150, lookupTwoDTableUS(&Table, 3));
	EXPECT_EQ(250, lookupTwoDTableUS(&Table, 5));
	EXPECT_EQ(350, lookupTwoDTableUS(&Table, 7));
	EXPECT_EQ(450, lookupTwoDTableUS(&Table, 9));
}

TEST_F(tableLookupTest, validateMainTableShouldFailWithInvalidRPMLength)
{
	mainTable Table;
	Table.RPMLength = MAINTABLE_MAX_RPM_LENGTH + 1;
	EXPECT_EQ(invalidMainTableRPMLength, validateMainTable(&Table));
}

TEST_F(tableLookupTest, validateMainTableShouldFailWithInvalidLoadLength)
{
	mainTable Table;
	Table.RPMLength = MAINTABLE_MAX_RPM_LENGTH;
	Table.LoadLength = MAINTABLE_MAX_LOAD_LENGTH + 1;
	EXPECT_EQ(invalidMainTableLoadLength, validateMainTable(&Table));
}

TEST_F(tableLookupTest, validateMainTableShouldFailWithInvalidRPMandLoadLengthCombination)
{
	mainTable Table;
	Table.RPMLength = MAINTABLE_MAX_RPM_LENGTH;
	Table.LoadLength = MAINTABLE_MAX_LOAD_LENGTH;
	EXPECT_EQ(invalidMainTableMainLength, validateMainTable(&Table));
}

TEST_F(tableLookupTest, validateMainTableShouldFailWhenRPMValuesAreOutOfOrder)
{
	mainTable Table1={4,4,{1,2,3,1},{1,2,3,1},{0}};
	mainTable Table2={4,4,{3,2,3,4},{3,2,3,4},{0}};
	mainTable Table3={4,4,{1,2,1,4},{1,2,1,4},{0}};
	EXPECT_EQ(invalidMainTableRPMOrder, validateMainTable(&Table1));
	EXPECT_EQ(invalidMainTableRPMOrder, validateMainTable(&Table2));
	EXPECT_EQ(invalidMainTableRPMOrder, validateMainTable(&Table3));
}

TEST_F(tableLookupTest, validateMainTableShouldFailWhenLoadValuesAreOutOfOrder)
{
	mainTable Table1={4,4,{1,2,3,4},{1,2,3,1},{0}};
	mainTable Table2={4,4,{1,2,3,4},{3,2,3,4},{0}};
	mainTable Table3={4,4,{1,2,3,4},{1,2,1,4},{0}};
	EXPECT_EQ(invalidMainTableLoadOrder, validateMainTable(&Table1));
	EXPECT_EQ(invalidMainTableLoadOrder, validateMainTable(&Table2));
	EXPECT_EQ(invalidMainTableLoadOrder, validateMainTable(&Table3));
}

TEST_F(tableLookupTest, validateMainTableShouldSucceedWhenEverythingIsInOrder)
{
	mainTable Table={4,4,{1,2,3,4},{1,2,3,4},{0}};
    EXPECT_EQ(0, validateMainTable(&Table));
}

TEST_F(tableLookupTest, validateTwoDTableShouldFailWhenAxisValuesNotInSequence)
{
	twoDTableUS Table1 = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,13},{0}};
	twoDTableUS Table2 = {{2,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0}};
	twoDTableUS Table3 = {{0,1,2,3,4,5,6,7,8,7,10,11,12,13,14,15},{0}};
	EXPECT_EQ(invalidTwoDTableAxisOrder, validateTwoDTable(&Table1));
	EXPECT_EQ(invalidTwoDTableAxisOrder, validateTwoDTable(&Table2));
	EXPECT_EQ(invalidTwoDTableAxisOrder, validateTwoDTable(&Table3));
}

TEST_F(tableLookupTest, validateTwoDTableShouldSucceedForProperlySequencedTable)
{
	twoDTableUS Table = {{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},{0}};
    EXPECT_EQ(0, validateTwoDTable(&Table));
}

int tableLookupTestMain(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
