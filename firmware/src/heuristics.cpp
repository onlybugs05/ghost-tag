#include "heuristics.h"

PersistenceEngine::PersistenceEngine(uint32_t windowSizeMs, uint16_t thresholdHits, int minRssiThreshold)
    : m_windowSizeMs(windowSizeMs), m_thresholdHits(thresholdHits), m_minRssiThreshold(minRssiThreshold) {}

void PersistenceEngine::processScanResults(const std::vector<BeaconDetection>& detections) {
    uint32_t now = millis();

    for (const auto& dev : detections) {
        // Skip devices that are too weak to be on our person/immediate baggage
        if (dev.rssi < m_minRssiThreshold) continue;

        String key = dev.macAddress;
        auto it = m_targets.find(key);

        if (it == m_targets.end()) {
            TrackedTarget target;
            target.identifier = key;
            target.type = dev.type;
            target.typeName = dev.typeName;
            target.firstSeenMs = now;
            target.lastSeenMs = now;
            target.windowHitCount = 1;
            target.avgRssi = dev.rssi;
            target.maxRssi = dev.rssi;
            target.alertTriggered = false;
            m_targets[key] = target;
        } else {
            it->second.lastSeenMs = now;
            it->second.avgRssi = (it->second.avgRssi + dev.rssi) / 2;
            if (dev.rssi > it->second.maxRssi) {
                it->second.maxRssi = dev.rssi;
            }

            // Increment window count if enough time elapsed since first seen in previous window
            if ((now - it->second.firstSeenMs) >= (it->second.windowHitCount * m_windowSizeMs)) {
                it->second.windowHitCount++;
            }

            // Stalking threshold reached: seen across multiple consecutive windows with strong RSSI
            if (it->second.windowHitCount >= m_thresholdHits && it->second.avgRssi >= m_minRssiThreshold) {
                it->second.alertTriggered = true;
            }
        }
    }

    purgeOldTargets();
}

std::vector<TrackedTarget> PersistenceEngine::getActiveAlerts() {
    std::vector<TrackedTarget> alerts;
    for (const auto& pair : m_targets) {
        if (pair.second.alertTriggered) {
            alerts.push_back(pair.second);
        }
    }
    return alerts;
}

void PersistenceEngine::purgeOldTargets(uint32_t maxAgeMs) {
    uint32_t now = millis();
    for (auto it = m_targets.begin(); it != m_targets.end(); ) {
        if (now - it->second.lastSeenMs > maxAgeMs) {
            it = m_targets.erase(it);
        } else {
            ++it;
        }
    }
}
