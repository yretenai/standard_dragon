//
// Created by Lilith on 2021-06-13.
//

#pragma once

#include "WemHierarchyChunk.hpp"

namespace dragon::bkhd::hirc {
#pragma pack(push, 1)
    template<typename T>
    struct ValueMod {
        T count {};
        T min {};
        T max {};
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
            plugin_type = buffer.lpcast<uint32_t>(offset);
            stream_type = buffer.lpcast<StreamType>(offset);
            source_id   = buffer.lpcast<uint32_t>(offset);
            size        = buffer.lpcast<uint32_t>(offset);
            flags       = buffer.lpcast<uint8_t>(offset);
        };

        uint32_t plugin_type   = 0;
        StreamType stream_type = StreamType::Data;
        uint32_t source_id     = 0;
        uint32_t size          = 0;
        uint8_t flags          = 0;
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
            points      = buffer.lpcast<ValueMod<uint32_t>>(offset, count);
        }

        uint32_t id         = 0;
        uint8_t type        = 0;
        uint8_t accumulator = 0;
        uint8_t param_id    = 0;
        int32_t curve_id    = 0;
        uint8_t scaling     = 0;

        dragon::Array<ValueMod<uint32_t>> points;
    };

    class NodeBaseParams {
    public:
        explicit NodeBaseParams() = default;
        explicit NodeBaseParams(dragon::Array<uint8_t> &buffer, uintptr_t &offset) {
            is_fx_override = buffer.lpcast<bool>(offset);
            fx             = buffer.lpcast<uint8_t>(offset);
            assert(fx == 0); // todo

            is_override_params = buffer.lpcast<bool>(offset);
            override_bus_id    = buffer.lpcast<uint32_t>(offset);
            direct_parent_id   = buffer.lpcast<uint32_t>(offset);
            flags              = buffer.lpcast<uint8_t>(offset);

            auto param_count = buffer.lpcast<uint8_t>(offset);
            auto keys        = buffer.lpcast<uint8_t>(offset, param_count);
            auto values      = buffer.lpcast<uint32_t>(offset, param_count);
            for (auto i = 0; i < param_count; ++i) {
                params[keys[i]] = values[i];
            }

            param_count = buffer.lpcast<uint8_t>(offset);
            keys        = buffer.lpcast<uint8_t>(offset, param_count);
            values      = buffer.lpcast<uint32_t>(offset, param_count * 2);
            for (auto i = 0; i < param_count; ++i) {
                range_params[keys[i]] = std::pair<uint32_t, uint32_t>(values[i * 2], values[i * 2 + 1]);
            }


            positioning = buffer.lpcast<uint8_t>(offset);
            if ((positioning & 1) == 1) {
                positioning_3d = buffer.lpcast<uint8_t>(offset);
            }
            aux            = buffer.lpcast<uint8_t>(offset);
            advanced_flags = buffer.lpcast<uint8_t>(offset);
            behavior       = buffer.lpcast<uint8_t>(offset);
            max_instances  = buffer.lpcast<uint16_t>(offset);
            below_behavior = buffer.lpcast<uint8_t>(offset);
            override_flags = buffer.lpcast<uint8_t>(offset);
            state_props    = buffer.lpcast<uint8_t>(offset);
            assert(state_props == 0); // todo
            state_groups = buffer.lpcast<uint8_t>(offset);
            assert(state_groups == 0); // todo

            param_count = buffer.lpcast<uint16_t>(offset);
            for (auto i = 0; i < param_count; ++i) {
                rtpc.emplace_back(RTPCManager(buffer, offset));
            }
        }

        bool is_fx_override = false;
        uint8_t fx          = 0;

        bool is_override_params   = false;
        uint32_t override_bus_id  = 0;
        uint32_t direct_parent_id = 0;
        uint8_t flags             = 0;

        std::map<uint8_t, uint32_t> params;
        std::map<uint8_t, std::pair<uint32_t, uint32_t>> range_params;

        uint8_t positioning    = 0;
        uint8_t positioning_3d = 0;
        uint8_t aux            = 0;
        uint8_t advanced_flags = 0;
        uint8_t behavior       = 0;
        uint16_t max_instances = 0;
        uint8_t below_behavior = 0;
        uint8_t override_flags = 0;
        uint8_t state_props    = 0;
        uint8_t state_groups   = 0;
        std::vector<RTPCManager> rtpc {};
    };
} // namespace dragon::bkhd::hirc
