#include <AppMain/orchestration/CatalogContract.hpp>
#include <algorithm>
#include <cmath>

namespace AppMain { namespace orchestration {

bool makeTractorCatalogItem(const RawTractorContract& raw, CatalogItem& out,
                            std::string& error)
{
    if (raw.id.empty() || !raw.wheelbaseM || !raw.maxForwardGear
        || !raw.maxRateDegPerSec || !raw.tauSec || !raw.ratio
        || !raw.maxFrontWheelAngleDeg) {
        error="tractor catalog is missing required geometry";
        return false;
    }
    CatalogItem candidate;
    candidate.id=raw.id;
    candidate.label=raw.label.empty()?raw.id:raw.label;
    candidate.geometry.wheelbaseM=*raw.wheelbaseM;
    candidate.geometry.maxForwardGear=*raw.maxForwardGear;
    candidate.geometry.steeringWheelMaxRateDegS=*raw.maxRateDegPerSec;
    candidate.geometry.steeringWheelTauS=*raw.tauSec;
    candidate.geometry.steeringRatio=*raw.ratio;
    candidate.geometry.maxWheelAngleRad=*raw.maxFrontWheelAngleDeg
        * 0.017453292519943295;
    if (raw.frontWheelTauSec) candidate.geometry.frontWheelTauS=*raw.frontWheelTauSec;
    if (!candidate.geometry.valid()) {
        error="tractor catalog geometry is outside supported bounds";
        return false;
    }
    out=std::move(candidate);
    return true;
}

void ensureNoneImplement(std::vector<CatalogItem>& implements)
{
    const auto found=std::find_if(implements.begin(),implements.end(),
        [](const CatalogItem& item){return item.id=="none";});
    if(found==implements.end()) {
        CatalogItem none;
        none.id="none";
        none.label="None";
        none.widthM=0.0;
        implements.insert(implements.begin(),std::move(none));
    }
}

std::string implementSelectionBody(const std::string& id)
{
    if(id=="none") return "{\"id\":null}";
    // Setup identifiers have already passed RunOrchestrator::idSafe, whose
    // accepted alphabet cannot require JSON escaping.
    return "{\"id\":\""+id+"\"}";
}

} }
