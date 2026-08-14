#ifndef APPMAIN_ORCHESTRATION_CATALOGCONTRACT_HPP
#define APPMAIN_ORCHESTRATION_CATALOGCONTRACT_HPP

#include <AppMain/orchestration/RunOrchestrator.hpp>
#include <optional>

namespace AppMain { namespace orchestration {

/** Presence-preserving view of the live tractor JSON contract. No defaults are
 * applied here: an orchestrated run must never silently use another tractor's
 * geometry when a catalog field is absent. */
struct RawTractorContract {
    std::string id;
    std::string label;
    std::optional<double> wheelbaseM;
    std::optional<int> maxForwardGear;
    std::optional<double> maxRateDegPerSec;
    std::optional<double> tauSec;
    std::optional<double> ratio;
    std::optional<double> maxFrontWheelAngleDeg;
    std::optional<double> frontWheelTauSec;
};

bool makeTractorCatalogItem(const RawTractorContract& raw, CatalogItem& out,
                            std::string& error);

/** The simulator selection contract represents "no implement" as the stable
 * app-facing id `none`, even though the live implement catalog omits it. */
void ensureNoneImplement(std::vector<CatalogItem>& implements);

/** Exact JSON body required by POST /api/simulator/implement. */
std::string implementSelectionBody(const std::string& id);

} }
#endif
