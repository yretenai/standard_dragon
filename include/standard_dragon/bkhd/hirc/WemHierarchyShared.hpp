//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"

namespace dragon::bkhd::hirc {
#pragma pack(push, 1)
    template<typename T>
    struct ValueMod {
        T count;
        T min;
        T max;
    };
#pragma pack(pop)

    using StreamType = enum class STREAM_TYPE : uint8_t {
        Data      = 0x00,
        Prefetch  = 0x01,
        Streaming = 0x02
    };

    class SourceInfo {
    public:
        explicit SourceInfo() = default;
        explicit SourceInfo(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            plugin_type    = buffer.lpcast<uint16_t>(offset);
            plugin_company = buffer.lpcast<uint16_t>(offset);
            stream_type    = buffer.lpcast<StreamType>(offset);
            source_id      = buffer.lpcast<uint32_t>(offset);
            media_size     = buffer.lpcast<uint32_t>(offset);
            flags          = buffer.lpcast<uint8_t>(offset);

            if (plugin_type == 2) {
                size = buffer.lpcast<uint32_t>(offset);
            }
        };

        uint16_t plugin_type    = 0;
        uint16_t plugin_company = 0;
        StreamType stream_type  = StreamType::Data;
        uint32_t source_id      = 0;
        uint32_t media_size     = 0;
        uint8_t flags           = 0;
        uint32_t size           = 0;
    };

    class RTPCManager {
    public:
        explicit RTPCManager() = default;
        explicit RTPCManager(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            id          = buffer.lpcast<uint32_t>(offset);
            type        = buffer.lpcast<uint8_t>(offset);
            accumulator = buffer.lpcast<uint8_t>(offset);
            param_id    = buffer.lpcast<uint8_t>(offset);
            curve_id    = buffer.lpcast<int32_t>(offset);
            scaling     = buffer.lpcast<uint8_t>(offset);
            auto count  = buffer.lpcast<uint16_t>(offset);
            if (count > 0) {
                points = buffer.lpcast<uint32_t>(offset, count * 3);
            }
        }

        uint32_t id         = 0;
        uint8_t type        = 0;
        uint8_t accumulator = 0;
        uint8_t param_id    = 0;
        int32_t curve_id    = 0;
        uint8_t scaling     = 0;

        dragon::Array<uint32_t> points;
    };

    class FXChunk {
    public:
        explicit FXChunk() = default;
        explicit FXChunk(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            index     = buffer.lpcast<uint8_t>(offset);
            id        = buffer.lpcast<uint32_t>(offset);
            is_share  = buffer.lpcast<bool>(offset);
            is_render = buffer.lpcast<bool>(offset);
        }

        uint8_t index  = 0;
        uint32_t id    = 0;
        bool is_share  = false;
        bool is_render = false;
    };

    class StateProp {
    public:
        explicit StateProp() = default;
        explicit StateProp(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            id         = buffer.lpcast<uint8_t>(offset);
            accum_type = buffer.lpcast<uint8_t>(offset);
            if (version > 125) {
                in_db = buffer.lpcast<uint8_t>(offset);
            }
        }

        uint8_t id         = 0;
        uint8_t accum_type = 0;
        uint8_t in_db      = 0;
    };

    class StateGroup {
    public:
        explicit StateGroup() = default;
        explicit StateGroup(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            id              = buffer.lpcast<uint32_t>(offset);
            state_sync_type = buffer.lpcast<uint8_t>(offset);
            auto count      = buffer.lpcast<uint8_t>(offset);
            if (count > 0) {
                states = buffer.lpcast<uint32_t>(offset, count * 2);
            }
        }

        uint32_t id             = 0;
        uint8_t state_sync_type = 0;
        dragon::Array<uint32_t> states;
    };

    class NodeBaseParams {
    public:
        explicit NodeBaseParams() = default;
        explicit NodeBaseParams(dragon::Array<uint8_t> &buffer, uintptr_t &offset, uint32_t &version) {
            is_fx_override       = buffer.lpcast<bool>(offset);
            uint32_t param_count = buffer.lpcast<uint8_t>(offset);
            if (param_count > 0) {
                fx_bypass = buffer.lpcast<uint8_t>(offset);
                fx.reserve(param_count);
                for (auto i = 0; i < param_count; ++i) {
                    fx.emplace_back(buffer, offset);
                }
            }

            is_override_params = buffer.lpcast<bool>(offset);
            override_bus_id    = buffer.lpcast<uint32_t>(offset);
            direct_parent_id   = buffer.lpcast<uint32_t>(offset);
            flags              = buffer.lpcast<uint8_t>(offset);

            param_count = buffer.lpcast<uint8_t>(offset);
            if (param_count > 0) {
                auto keys   = buffer.lpcast<uint8_t>(offset, param_count);
                auto values = buffer.lpcast<uint32_t>(offset, param_count);
                for (auto i = 0; i < param_count; ++i) {
                    params[keys[i]] = values[i];
                }
            }

            param_count = buffer.lpcast<uint8_t>(offset);
            if (param_count > 0) {
                auto keys   = buffer.lpcast<uint8_t>(offset, param_count);
                auto values = buffer.lpcast<uint32_t>(offset, param_count * 2);
                for (auto i = 0; i < param_count; ++i) {
                    range_params[keys[i]] = std::pair<uint32_t, uint32_t>(values[i * 2], values[i * 2 + 1]);
                }
            }


            positioning       = buffer.lpcast<uint8_t>(offset);
            bool has_override = ((positioning >> 0) & 1) == 1;
            bool has_3d       = false;
            if (has_override) {
                if (version <= 128) {
                    has_3d = ((positioning >> 4) & 1) == 1;
                } else {
                    has_3d = ((positioning >> 1) & 1) == 1;
                }
            }

            if (has_3d) {
                positioning_3d = buffer.lpcast<uint8_t>(offset);

                if (version <= 128) {
                    attenuation_id = buffer.lpcast<uint32_t>(offset);
                }
            }

            bool has_automation;
            if (version <= 125) {
                has_automation = ((positioning_3d >> 4) & 1) == 1;
            } else if (version <= 128) {
                has_automation = ((positioning_3d >> 6) & 1) == 1;
            } else {
                has_automation = ((positioning >> 5) & 3) != 0;
            }

            if (has_automation) {
                path_mode       = buffer.lpcast<uint8_t>(offset);
                transition_time = buffer.lpcast<int32_t>(offset);
                param_count     = buffer.lpcast<uint32_t>(offset);
                if (param_count > 0) {
                    position_vertices = buffer.lpcast<uint32_t>(offset, param_count * 4);
                }
                param_count = buffer.lpcast<uint32_t>(offset);
                if (param_count > 0) {
                    position_playlist = buffer.lpcast<uint32_t>(offset, param_count * 2);
                    position_params   = buffer.lpcast<float>(offset, param_count * 3);
                }
            }
            aux            = buffer.lpcast<uint8_t>(offset);
            advanced_flags = buffer.lpcast<uint8_t>(offset);
            behavior       = buffer.lpcast<uint8_t>(offset);
            max_instances  = buffer.lpcast<uint16_t>(offset);
            below_behavior = buffer.lpcast<uint8_t>(offset);
            override_flags = buffer.lpcast<uint8_t>(offset);

            param_count = buffer.lpcast<uint8_t>(offset);
            if (param_count > 0) {
                state_props.reserve(param_count);
                for (auto i = 0; i < param_count; ++i) {
                    state_props.emplace_back(buffer, offset, version);
                }
            }

            param_count = buffer.lpcast<uint8_t>(offset);
            if (param_count > 0) {
                state_groups.reserve(param_count);
                for (auto i = 0; i < param_count; ++i) {
                    state_groups.emplace_back(buffer, offset);
                }
            }

            param_count = buffer.lpcast<uint16_t>(offset);
            if (param_count > 0) {
                rtpc.reserve(param_count);
                for (auto i = 0; i < param_count; ++i) {
                    rtpc.emplace_back(RTPCManager(buffer, offset));
                }
            }
        }

        bool is_fx_override = false;
        uint8_t fx_bypass   = 0;
        std::vector<FXChunk> fx;

        bool is_override_params   = false;
        uint32_t override_bus_id  = 0;
        uint32_t direct_parent_id = 0;
        uint8_t flags             = 0;

        std::map<uint8_t, uint32_t> params;
        std::map<uint8_t, std::pair<uint32_t, uint32_t>> range_params;

        uint8_t positioning     = 0;
        uint32_t attenuation_id = 0;
        uint8_t positioning_3d  = 0;
        uint32_t path_mode      = 0;
        int32_t transition_time = 0;
        dragon::Array<uint32_t> position_vertices;
        dragon::Array<uint32_t> position_playlist;
        dragon::Array<float> position_params;


        uint8_t aux            = 0;
        uint8_t advanced_flags = 0;
        uint8_t behavior       = 0;
        uint16_t max_instances = 0;
        uint8_t below_behavior = 0;
        uint8_t override_flags = 0;

        std::vector<StateProp> state_props;
        std::vector<StateGroup> state_groups;
        std::vector<RTPCManager> rtpc;
    };
} // namespace dragon::bkhd::hirc
