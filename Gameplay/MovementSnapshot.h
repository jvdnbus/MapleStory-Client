#pragma once

#include <chrono>
#include "../Gameplay/Physics/PhysicsObject.h"

namespace ms {
    struct MovementSnapshot {
        using clock = std::chrono::steady_clock;

        enum Command : int8_t {
            NORMAL_MOVE = 0,
            COMMAND_1 = 1,
            COMMAND_2 = 2,
            WALK = 3,
            COMMAND_TELEPORT = 4,
            COMMAND_5 = 5,
            FLASHJUMP = 6,
            ASSAULTER = 7,
            ASSASSINATE = 8,
            RUSH = 9,
            COMMAND_CHANGE_EQP = 10,
            CHAIR = 11,
            COMMAND_12 = 12,
            SHOT_RECOIL_JUMP = 13,
            COMMAND_FALL_DOWN = 14,
            COMMAND_JUMP_DOWN = 15,
            FLOATING_1 = 16,
            FLOATING_2 = 17,
            COMMAND_18 = 18,
            SPRING_MAP_OBJ = 19,
            ARAN_COMBAT_STEP = 20,
            ARAN_UNKNOWN = 21,
            COMMAND_22 = 22
        };

        static Command by_value(int8_t value) {
            return static_cast<Command>(value);
        }

        enum Type {
            ABSOLUTE,
            RELATIVE,
            FALL_DOWN,
            TELEPORT,
            CHANGE_EQUIP,
            JUMP_DOWN,
            ARAN
        };

        Type type;
        Command cmd; // nAttr
        int16_t position_x; // x
        int16_t position_y; // y
        int16_t velocity_x; // vx
        int16_t velocity_y; // vy
        uint16_t foothold; // fh
        uint16_t foothold_fall_start; // fhFallStart
        uint8_t state; // bMoveAction
        int16_t duration; // tElapse
//        uint8_t bStat;
//        uint16_t usRandCnt;
//        uint16_t usActualRandCnt;

        clock::time_point timestamp;

        static Type get_type(Command cmd) {
            Type type;
            switch (cmd) {
                case NORMAL_MOVE:
                case COMMAND_5:
                case FLOATING_2:
                    type = Type::ABSOLUTE;
                    break;
                case COMMAND_1:
                case COMMAND_2:
                case FLASHJUMP:
                case COMMAND_12:
                case SHOT_RECOIL_JUMP:
                case FLOATING_1:
                case COMMAND_18:
                case SPRING_MAP_OBJ:
                case ARAN_COMBAT_STEP:
                case COMMAND_22:
                default:
                    type = Type::RELATIVE;
                    break;
                case WALK:
                case COMMAND_TELEPORT:
                case ASSAULTER:
                case ASSASSINATE:
                case RUSH:
                case CHAIR:
                    type = Type::TELEPORT;
                    break;
                case COMMAND_CHANGE_EQP:
                    type = Type::CHANGE_EQUIP;
                    break;
                case COMMAND_FALL_DOWN:
                    type = Type::FALL_DOWN;
                    break;
                case COMMAND_JUMP_DOWN:
                    type = Type::JUMP_DOWN;
                    break;
                case ARAN_UNKNOWN:
                    type = Type::ARAN;
                    break;
            }
            return type;
        }

        MovementSnapshot() = default;

        MovementSnapshot(Command cmd, int16_t position_x, int16_t position_y, int16_t velocity_x, int16_t velocity_y,
                         uint16_t fh, uint16_t fh_fall_start, uint8_t state, int16_t duration, clock::time_point timestamp) :
            cmd(cmd), type(get_type(cmd)), position_x(position_x), position_y(position_y),
            velocity_x(velocity_x), velocity_y(velocity_y), foothold(fh), foothold_fall_start(fh_fall_start),
            state(state), duration(duration), timestamp(timestamp) {
        }

        MovementSnapshot(Command cmd, const PhysicsObject& p, int16_t velocity_x, int16_t velocity_y, uint8_t state) :
                cmd(cmd), type(get_type(cmd)), position_x(p.get_x()), position_y(p.get_y()), velocity_x(velocity_x),
                velocity_y(velocity_y), state(state), foothold(p.fh_id), foothold_fall_start(p.jumping_down_from_fh_id),
                duration(0), timestamp(clock::now()) {
        }

        bool has_moved(const MovementSnapshot& new_snapshot) const {
            if (new_snapshot.cmd != cmd || new_snapshot.state != state) {
                return true;
            }

            bool x_or_y_changed = new_snapshot.position_x != position_x || new_snapshot.position_y != position_y;
            if (x_or_y_changed) {
                constexpr int32_t sqrd_dist_threshold = 25 * 25; // 25px in any direction
                int16_t dx = new_snapshot.position_x - position_x;
                int16_t dy = new_snapshot.position_y - position_y;
                int32_t sqrd_dist = dx * dx + dy * dy;
                if (sqrd_dist >= sqrd_dist_threshold) {
                    return true;
                }

                // Makes it so we don't spam every little x,y change
                // Only when velocity vector changes significantly as well
                constexpr int16_t epsilon = 1;
                if (std::abs(new_snapshot.velocity_x - velocity_x) >= epsilon ||
                    std::abs(new_snapshot.velocity_y - velocity_y) >= epsilon) {
                    return true;
                }
            }
            return false;
        }
    };
}
