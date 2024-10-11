#include "MovementPath.h"

namespace ms {
    constexpr uint16_t FLUSH_INTERVAL = 500;
    constexpr uint16_t SHORT_FLUSH_INTERVAL = 200;

    void MovementPath::add_snapshot(const MovementSnapshot &snapshot) {
        path.emplace_back(snapshot);
    }

    bool MovementPath::is_time_for_flush() {
        if (path.size() >= 31) {
            return true;
        }
        auto now = clock::now();
        auto d = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_flush);
        auto d2 = std::chrono::milliseconds(short_update ? SHORT_FLUSH_INTERVAL : FLUSH_INTERVAL);
        return d >= d2;
    }

    void MovementPath::flush(MovementPacket& packet) {
        // Try to condense movements
        compact();

        // Send packet
        packet.write_movement(path);
        packet.dispatch();

        // Reset
        path.clear();
        last_flush = clock::now();
    }

    void MovementPath::compact() {

    }
}
