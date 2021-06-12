//
// Created by Lilith on 2021-06-12.
//

#pragma once

namespace dragon::bkhd::hirc {
    using WemHierarchyType = enum class WEM_HIERARCHY_TYPE_ID : uint8_t {
        None            = 0x00,
        State           = 0x01,
        Sound           = 0x02,
        Action          = 0x03,
        Event           = 0x04,
        Container       = 0x05,
        SwitchContainer = 0x06,
        ActorMixer      = 0x07,
        AudioBus        = 0x08,
        LayerContainer  = 0x09,
        MusicSegment    = 0x0A,
        MusicTrack      = 0x0B,
        MusicSwitch     = 0x0C,
        MusicContainer  = 0x0D,
        Attenuation     = 0x0E,
        DialogueEvent   = 0x0F,
        FXShareSet      = 0x10,
        FXCustom        = 0x11,
        AuxiliaryBus    = 0x12,
        LFO             = 0x13,
        Envelope        = 0x14,
        AudioDevice     = 0x15,
        TimeMod         = 0x16,
    };
    DRAGON_ASSERT(sizeof(WemHierarchyType) == 1, "WemHierarchyTypeId size is not 1");

    class WemHierarchyChunk {
    public:
        const static WemHierarchyType type    = WemHierarchyType::None;
        virtual ~WemHierarchyChunk() noexcept = default;
    };
} // namespace dragon::bkhd::hirc
