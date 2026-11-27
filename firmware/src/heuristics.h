#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "ble_sniffer.h"
#include <map>
#include <vector>

struct TrackedTarget {
    String identifier;       // MAC or payload characteristic
    TrackerType type;
    String typeName;
    uint32_t firstSeenMs;
    uint32_t lastSeenMs;
    uint16_t windowHitCount; // Number of distinct observation windows seen
    int avgRssi;
    int maxRssi;
    bool alertTriggered;
};

class PersistenceEngine {
public:
    PersistenceEngine(uint32_t windowSizeMs = 60000, uint16_t thresholdHits = 3, int minRssiThreshold = -78);

    void processScanResults(const std::vector<BeaconDetection>& detections);
    std::vector<TrackedTarget> getActiveAlerts();
    void purgeOldTargets(uint32_t maxAgeMs = 1800000); // 30 minutes
    size_t getTrackedCount() const { return m_targets.size(); }

private:
    uint32_t m_windowSizeMs;
    uint16_t m_thresholdHits;
    int m_minRssiThreshold;
    std::map<String, TrackedTarget> m_targets;
};

#endif // HEURISTICS_H
