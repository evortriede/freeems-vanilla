/*
 * utilTest.cpp
 *
 *  Created on: Jan 12, 2013
 *      Author: eric
 */

#include "gtest/gtest.h"
#define UNIT_TEST_CONFIG
#ifdef never_defined
#endif
#include "../../main/inc/containerTypes.h"
#include "../../main/inc/globalDefines.h"
#include "../../main/inc/memory.h"
#include "../../main/inc/9S12XDP512.h"

#undef FPAGE_FE
#undef FPAGE_F8

#define FPAGE_FE
#define FPAGE_F8

#include "../../main/inc/utils.h"

#undef RPAGE

unsigned char currentFuelRPage, currentTimeRPage, currentTuneRPage, RPAGE;

#include "../../main/utils.c"

class utilTest : public ::testing::Test {
 protected:

};

TEST_F(utilTest, SanityOfGtest) {
  EXPECT_EQ(0, 0);
}

TEST_F(utilTest, ensureSafeAddAddsSmallSums)
{
	EXPECT_EQ(SHORTMAX-1, safeAdd(SHORTMAX-2, 1));
	EXPECT_EQ(4, safeAdd(2,2));
}

TEST_F(utilTest, ensureSafeAddNeverExceedsOrWrapsAroundSHORTMAX)
{
	EXPECT_EQ(SHORTMAX, safeAdd(SHORTMAX-1, 2));
	EXPECT_EQ(SHORTMAX, safeAdd(2, SHORTMAX-1));
}

TEST_F(utilTest, ensureSafeTrimWithNegativeNumberPerformsSubtract)
{
	EXPECT_EQ(1, safeTrim(3, -2));
}

TEST_F(utilTest, ensureSafeTrimWillNotDecrementBelow0)
{
	EXPECT_EQ(0, safeTrim(2, -3));
}

TEST_F(utilTest, ensureSafeTrimWithPositiveNumbersPerformsAdd)
{
	EXPECT_EQ(4, safeTrim(2,2));
}

TEST_F(utilTest, ensureSafeTrimWithPositiveNumbersWillNotExceedSHORTMAX)
{
	EXPECT_EQ(SHORTMAX, safeTrim(SHORTMAX-1,2));
}

TEST_F(utilTest, ensureSafeTrimReturnsFirstTermWhenSecondTermIs0)
{
	EXPECT_EQ(1, safeTrim(1,0));
}

TEST_F(utilTest, ensureSafeScaleScalesDown)
{
	EXPECT_EQ(100, safeScale(200, 2, 4));
}

TEST_F(utilTest, ensureSafeScaleScalesUp)
{
	EXPECT_EQ(200, safeScale(100, 4, 2));
}

TEST_F(utilTest, ensureSafeScaleWillNotScalePastSHORTMAX)
{
	EXPECT_EQ(SHORTMAX, safeScale(SHORTMAX-1, 4, 2));
}

TEST_F(utilTest, ensureSetupPagedRAMTrueSelectsRPAGE_ONEValues)
{
	setupPagedRAM(true);

	EXPECT_EQ(RPAGE_FUEL_ONE, currentFuelRPage);
	EXPECT_EQ(RPAGE_TIME_ONE, currentTimeRPage);
	EXPECT_EQ(RPAGE_TUNE_ONE, currentTuneRPage);
	EXPECT_EQ(RPAGE_TUNE_ONE, RPAGE);
}

TEST_F(utilTest, ensureSetupPagedRAMFalseSelectsRPAGE_TWOValues)
{
	setupPagedRAM(false);

	EXPECT_EQ(RPAGE_FUEL_TWO, currentFuelRPage);
	EXPECT_EQ(RPAGE_TIME_TWO, currentTimeRPage);
	EXPECT_EQ(RPAGE_TUNE_TWO, currentTuneRPage);
	EXPECT_EQ(RPAGE_TUNE_TWO, RPAGE);
}

int utilTestMain(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


