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

static blockDetails myBlockDetails = {block_is_main_table, 0, 0, 0};

static unsigned short my_lookupBlockDetails(unsigned short locationId, blockDetails* details)
{
	details->FlashAddress=myBlockDetails.FlashAddress;
	details->FlashPage = myBlockDetails.FlashPage;
	details->RAMAddress = myBlockDetails.RAMAddress;
	details->RAMPage = myBlockDetails.RAMPage;
	details->flags = myBlockDetails.flags;
	details->parent = myBlockDetails.parent;
	details->size = myBlockDetails.size;
	return 0;
}

TEST_F(tableLookupTest, SanityOfGtest) {
  EXPECT_EQ(0, 0);
}

TEST_F(tableLookupTest, lookupMainTableShouldReturn0When_block_is_main_table_BitIsNotSetInFlags)
{
	myBlockDetails.flags=0;
	EXPECT_EQ(0, lookupMainTable(0,0,0));
}

TEST_F(tableLookupTest, lookupMainTableShouldReturnLookedUpResultWithSpotOnValues)
{
	mainTable Table = {4,4,{2,4,6,8},{2,4,6,8},
			{10,20,30,40,
			 20,30,40,50,
			 30,40,50,60,
			 40,50,60,70
			}
	};
	myBlockDetails.flags = block_is_main_table;
	myBlockDetails.RAMAddress = &Table;
	EXPECT_EQ(10, lookupMainTable(2,2,0));
	EXPECT_EQ(30, lookupMainTable(4,4,0));
	EXPECT_EQ(50, lookupMainTable(6,6,0));
	EXPECT_EQ(70, lookupMainTable(8,8,0));
}

TEST_F(tableLookupTest, lookupMainTableShouldReturnInterpolatedResultWithInbetweenValues)
{
	mainTable Table = {4,4,{2,4,6,8},{2,4,6,8},
			{10,20,30,40,
			 20,30,40,50,
			 30,40,50,60,
			 40,50,60,70
			}
	};
	myBlockDetails.flags = block_is_main_table;
	myBlockDetails.RAMAddress = &Table;
	EXPECT_EQ(10, lookupMainTable(1,1,0));
	EXPECT_EQ(20, lookupMainTable(3,3,0));
	EXPECT_EQ(40, lookupMainTable(5,5,0));
	EXPECT_EQ(60, lookupMainTable(7,7,0));
	EXPECT_EQ(70, lookupMainTable(9,9,0));
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
	twoDTableUS Table = {{2,4,6,8,10,12,14,16,18,20,22,24,26,28,30,32},
			{100,200,300,400,500,0,0,0,0,0,0,0,0,0,0,600}};
	EXPECT_EQ(100, lookupTwoDTableUS(&Table, 1));
	EXPECT_EQ(150, lookupTwoDTableUS(&Table, 3));
	EXPECT_EQ(250, lookupTwoDTableUS(&Table, 5));
	EXPECT_EQ(350, lookupTwoDTableUS(&Table, 7));
	EXPECT_EQ(450, lookupTwoDTableUS(&Table, 9));
	EXPECT_EQ(600, lookupTwoDTableUS(&Table, 33));
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
