#include <gtest/gtest.h>
#include <AppMain/orchestration/CatalogContract.hpp>

using namespace AppMain::orchestration;

namespace {
RawTractorContract captured()
{
    RawTractorContract raw;
    raw.id="tractor-120hp";
    raw.label="120 HP";
    raw.wheelbaseM=2.45;
    raw.maxForwardGear=4;
    raw.maxRateDegPerSec=360.0;
    raw.tauSec=0.25;
    raw.ratio=20.0;
    raw.maxFrontWheelAngleDeg=32.0;
    raw.frontWheelTauSec=0.08;
    return raw;
}
}

TEST(CatalogContractTest, ParsesCapturedNestedSteeringContractWithoutDefaults)
{
    CatalogItem item; std::string error;
    ASSERT_TRUE(makeTractorCatalogItem(captured(),item,error)) << error;
    EXPECT_EQ(item.id,"tractor-120hp");
    EXPECT_DOUBLE_EQ(item.geometry.wheelbaseM,2.45);
    EXPECT_EQ(item.geometry.maxForwardGear,4);
    EXPECT_DOUBLE_EQ(item.geometry.steeringWheelMaxRateDegS,360.0);
    EXPECT_DOUBLE_EQ(item.geometry.steeringWheelTauS,0.25);
    EXPECT_DOUBLE_EQ(item.geometry.steeringRatio,20.0);
    EXPECT_NEAR(item.geometry.maxWheelAngleRad,32.0*3.141592653589793/180.0,1e-12);
}

TEST(CatalogContractTest, RejectsEveryMissingRequiredNestedSteeringField)
{
    auto assertRejected=[](const RawTractorContract& raw) {
        CatalogItem item; std::string error;
        EXPECT_FALSE(makeTractorCatalogItem(raw,item,error));
        EXPECT_FALSE(error.empty());
    };
    auto raw=captured(); raw.maxRateDegPerSec.reset(); assertRejected(raw);
    raw=captured(); raw.tauSec.reset(); assertRejected(raw);
    raw=captured(); raw.ratio.reset(); assertRejected(raw);
    raw=captured(); raw.maxFrontWheelAngleDeg.reset(); assertRejected(raw);
    raw=captured(); raw.wheelbaseM.reset(); assertRejected(raw);
    raw=captured(); raw.maxForwardGear.reset(); assertRejected(raw);
}

TEST(CatalogContractTest, RejectsMalformedGeometryInsteadOfFallingBack)
{
    auto raw=captured(); raw.ratio=0.0;
    CatalogItem item; std::string error;
    EXPECT_FALSE(makeTractorCatalogItem(raw,item,error));
}

TEST(CatalogContractTest, SynthesizesNoneImplementExactlyOnceAndFirst)
{
    CatalogItem plow; plow.id="plow-a"; plow.label="Plow A"; plow.widthM=2.2;
    std::vector<CatalogItem> items{plow};
    ensureNoneImplement(items);
    ASSERT_EQ(items.size(),2u);
    EXPECT_EQ(items.front().id,"none");
    EXPECT_EQ(items.front().label,"None");
    EXPECT_DOUBLE_EQ(items.front().widthM,0.0);
    ensureNoneImplement(items);
    EXPECT_EQ(items.size(),2u);
    EXPECT_EQ(std::count_if(items.begin(),items.end(),[](const CatalogItem& item){
        return item.id=="none";
    }),1);
}

TEST(CatalogContractTest, ImplementSelectionBodyUsesNullOnlyForNone)
{
    EXPECT_EQ(implementSelectionBody("none"),"{\"id\":null}");
    EXPECT_EQ(implementSelectionBody("plow-a"),"{\"id\":\"plow-a\"}");
}
