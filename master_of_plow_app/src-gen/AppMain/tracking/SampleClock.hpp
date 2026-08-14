#ifndef APPMAIN_TRACKING_SAMPLECLOCK_HPP
#define APPMAIN_TRACKING_SAMPLECLOCK_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace AppMain { namespace tracking {

/**
 * Derives machine-time progression from an authoritative interface timestamp.
 * SDK timestamps are accepted as milliseconds only when their delta is
 * plausible relative to wall time. Sequence counters or unknown units fail
 * closed to wall time instead of silently changing controller gains.
 */
class SampleClock
{
public:
    enum class Fault { None, Regression, ImplausibleJump, SourceLoss };

    double advance(std::int64_t sampleId, double wallDtS)
    {
        wallDtS = std::clamp(wallDtS, 0.0, 0.5);
        sinceAdvanceWallS_ += wallDtS;
        if (sampleId >= 0 && haveId_ && sampleId < lastId_) {
            // A timestamp regression invalidates the learned time domain now,
            // rather than letting an old accelerated scale survive the fault.
            scale_ = 1.0;
            consistentSamples_ = 0;
            haveCandidate_ = false;
            calibrated_ = false;
            fault_ = Fault::Regression;
            sinceAdvanceWallS_ = 0.0;
            return wallDtS;
        }
        if (sampleId >= 0 && haveId_ && sampleId > lastId_ &&
                sinceAdvanceWallS_ > 0.0) {
            const double sensorDtS = static_cast<double>(sampleId - lastId_) / 1000.0;
            const double candidate = sensorDtS / sinceAdvanceWallS_;
            if (sensorDtS >= 0.02 && sensorDtS <= 16.0 &&
                    candidate >= MIN_SCALE && candidate <= MAX_SCALE) {
                const bool consistent = !haveCandidate_ ||
                        std::fabs(candidate - candidate_) <=
                        std::max(0.35, 0.35 * candidate_);
                candidate_ = candidate;
                haveCandidate_ = true;
                consistentSamples_ = consistent ? consistentSamples_ + 1 : 1;
                if (consistentSamples_ >= 2) {
                    scale_ = calibrated_
                            ? scale_ + FILTER_ALPHA * (candidate - scale_)
                            : candidate;
                    calibrated_ = true;
                    fault_ = Fault::None;
                }
            } else {
                scale_ = 1.0;
                consistentSamples_ = 0;
                haveCandidate_ = false;
                calibrated_ = false;
                fault_ = Fault::ImplausibleJump;
            }
            sinceAdvanceWallS_ = 0.0;
        }
        if (sampleId >= 0 && (!haveId_ || sampleId >= lastId_)) {
            if (!haveId_) {
                sinceAdvanceWallS_ = 0.0;
            }
            lastId_ = sampleId;
            haveId_ = true;
        }
        // A learned acceleration factor must not survive loss of its source.
        // Muxed interfaces may recur at roughly 1 Hz wall time, so allow one
        // skipped epoch but fail closed before a third can be missed.
        if (sinceAdvanceWallS_ > 2.5) {
            scale_ = 1.0;
            consistentSamples_ = 0;
            haveCandidate_ = false;
            calibrated_ = false;
            fault_ = Fault::SourceLoss;
        }
        return wallDtS * scale_;
    }

    double scale() const { return scale_; }
    bool calibrated() const { return calibrated_; }
    bool valid() const { return calibrated_ && fault_ == Fault::None; }
    bool sourceLost() const { return fault_ == Fault::SourceLoss; }
    bool regressed() const { return fault_ == Fault::Regression; }
    bool implausibleJump() const { return fault_ == Fault::ImplausibleJump; }
    const char* statusName() const
    {
        switch (fault_) {
        case Fault::Regression: return "regression";
        case Fault::ImplausibleJump: return "implausible_jump";
        case Fault::SourceLoss: return "source_loss";
        case Fault::None: return calibrated_ ? "valid" : "calibrating";
        }
        return "unknown";
    }
    void reset() { *this = SampleClock{}; }

private:
    static constexpr double MIN_SCALE = 0.5;
    static constexpr double MAX_SCALE = 8.0;
    static constexpr double FILTER_ALPHA = 0.5;
    std::int64_t lastId_ = -1;
    double scale_ = 1.0;
    double candidate_ = 1.0;
    int consistentSamples_ = 0;
    double sinceAdvanceWallS_ = 0.0;
    bool haveId_ = false;
    bool haveCandidate_ = false;
    bool calibrated_ = false;
    Fault fault_ = Fault::None;
};

}} // namespace AppMain::tracking
#endif
