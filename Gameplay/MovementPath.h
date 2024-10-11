#pragma once

#include <chrono>
#include <vector>
#include "MovementSnapshot.h"
#include "../Net/Packets/MovementPacket.h"
#include "../Template/Optional.h"

namespace ms {
    class MovementPath {
        using clock = std::chrono::steady_clock;
    public:
        MovementPath() : short_update(false) {};

        void add_snapshot(const MovementSnapshot& snapshot);
        bool is_time_for_flush();
        void flush(MovementPacket& packet);

        void set_short_update(bool value) {
            short_update = value;
        }

        bool empty() {
            return path.empty();
        }

        const std::vector<MovementSnapshot>& get_path() const {
            return path;
        }

        Optional<MovementSnapshot> first() {
            return path.empty() ? Optional<MovementSnapshot>() : Optional<MovementSnapshot>(path.front());
        }

        Optional<MovementSnapshot> last() {
            return path.empty() ? Optional<MovementSnapshot>() : Optional<MovementSnapshot>(path.back());
        }
    private:
        void compact();

        std::vector<MovementSnapshot> path;
        clock::time_point last_flush;
        bool short_update;
    };
}
