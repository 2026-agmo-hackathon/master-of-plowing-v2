// TEMPORARY validation harness — replays the real recorded steering inputs
// (STEER_IO csv: tgt_deg, meas_deg, cmd, speed at 10 Hz) through
// SteeringController and compares the motor command against what the device
// actually produced. Purpose: prove the model of the controller is right before
// trusting any conclusion about a fix.
#include <gtest/gtest.h>

#include <AppMain/tracking/SteeringController.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace AppMain::tracking;

namespace {

constexpr double TICK_S = 0.1;
constexpr double STEERING_RATIO = 24.2;
constexpr double DEG_PER_RAD = 57.29577951308232;

struct Row
{
    double tgtDeg;
    double measDeg;
    double cmd;
    double speed;
};

std::vector<Row> load(const std::string& path)
{
    std::vector<Row> rows;
    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {
        for (char& c : line) {
            if (c == ',') {
                c = ' ';
            }
        }
        std::istringstream ss(line);
        Row r{};
        if (ss >> r.tgtDeg >> r.measDeg >> r.cmd >> r.speed) {
            rows.push_back(r);
        }
    }
    return rows;
}

struct Stats
{
    double medSwingWithinInterval = 0.0;
    double p90SwingWithinInterval = 0.0;
    double maxSwingWithinInterval = 0.0;
    int signReversals = 0;
    double pinnedFraction = 0.0;
    double medAbsCmdDiffVsDevice = 0.0;
};

double pct(std::vector<double> v, double p)
{
    if (v.empty()) {
        return 0.0;
    }
    std::sort(v.begin(), v.end());
    return v[static_cast<std::size_t>(p * static_cast<double>(v.size() - 1))];
}

Stats replay(const std::vector<Row>& rows)
{
    SteeringController ctrl;
    std::vector<int> cmds;
    cmds.reserve(rows.size());

    for (const Row& r : rows) {
        VehicleState st;
        st.speedMps = r.speed;
        st.yawRateValid = false;   // 융합을 끄면 fused = enc/RATIO 가 되어 기록된
                                   // 측정값을 정확히 재현할 수 있습니다.
                                   // Fusion off so fused == enc/RATIO, exactly
                                   // reproducing the recorded measurement.
        const double encoderDeg = r.measDeg * STEERING_RATIO;
        const SteeringController::Output out =
                ctrl.update(r.tgtDeg / DEG_PER_RAD, st, encoderDeg, true, TICK_S);
        cmds.push_back(out.motorCmd);
    }

    // 측정이 갱신되는 지점 = 기록된 meas 가 0.5도 넘게 튀는 지점
    // A measurement update = the recorded meas jumping by more than 0.5 deg.
    std::vector<std::size_t> upd;
    for (std::size_t i = 1; i < rows.size(); ++i) {
        if (std::fabs(rows[i].measDeg - rows[i - 1].measDeg) > 0.5) {
            upd.push_back(i);
        }
    }

    std::vector<double> swing;
    for (std::size_t k = 0; k + 1 < upd.size(); ++k) {
        int lo = cmds[upd[k]];
        int hi = cmds[upd[k]];
        for (std::size_t i = upd[k]; i < upd[k + 1]; ++i) {
            lo = std::min(lo, cmds[i]);
            hi = std::max(hi, cmds[i]);
        }
        swing.push_back(static_cast<double>(hi - lo));
    }

    Stats s;
    s.medSwingWithinInterval = pct(swing, 0.50);
    s.p90SwingWithinInterval = pct(swing, 0.90);
    s.maxSwingWithinInterval = pct(swing, 1.00);

    int pinned = 0;
    int rev = 0;
    std::vector<double> diff;
    for (std::size_t i = 0; i < cmds.size(); ++i) {
        if (std::abs(cmds[i]) >= 595) {
            ++pinned;
        }
        if (i > 0 && cmds[i] != 0 && cmds[i - 1] != 0
            && (cmds[i] > 0) != (cmds[i - 1] > 0)) {
            ++rev;
        }
        diff.push_back(std::fabs(static_cast<double>(cmds[i]) - rows[i].cmd));
    }
    s.signReversals = rev;
    s.pinnedFraction = static_cast<double>(pinned) / static_cast<double>(cmds.size());
    s.medAbsCmdDiffVsDevice = pct(diff, 0.50);
    return s;
}

}  // namespace

TEST(SteerReplayTest, DISABLED_ReplayRecordedInputs)
{
    const char* p = std::getenv("STEER_IO");
    if (p == nullptr) {
#ifdef GTEST_SKIP
        GTEST_SKIP() << "set STEER_IO to the recorded csv";
#else
        return;  // GoogleTest 1.8 on the target-compatible Buster toolchain.
#endif
    }
    const auto rows = load(p);
    ASSERT_GT(rows.size(), 100u);

    const Stats s = replay(rows);
    std::printf("rows=%zu\n", rows.size());
    std::printf("motor cmd swing WITHIN one measurement interval: med=%.0f p90=%.0f max=%.0f\n",
                s.medSwingWithinInterval, s.p90SwingWithinInterval,
                s.maxSwingWithinInterval);
    std::printf("sign reversals=%d   pinned=%.1f%%\n",
                s.signReversals, 100.0 * s.pinnedFraction);
    std::printf("median |cmd - device cmd| = %.0f\n", s.medAbsCmdDiffVsDevice);
    std::printf("DEVICE measured: swing p90=255 max=1200, reversals=21, pinned=46.6%%\n");
}
