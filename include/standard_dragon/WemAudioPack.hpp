//
// Created by Lilith on 2020-10-03.
//

#pragma once

#include "Array.hpp"
#include "dragon.hpp"
#include <cassert>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

#ifdef DRAGON_AKPKANSI
#    define DRAGON_AKPK_CHAR_TYPE char
#    define DRAGON_AKPK_STRING_TYPE std::string
#else
#    define DRAGON_AKPK_CHAR_TYPE wchar_t
#    define DRAGON_AKPK_STRING_TYPE std::wstring
#endif

namespace dragon {
    class WemAudioPack {
    public:
#pragma pack(push, 1)
        using AudioPackHeader = struct AUDIO_PACK_HEADER {
            uint32_t fourcc                     = AKPK_FOURCC;
            uint32_t size                       = 0;
            uint32_t version                    = 1;
            uint32_t name_table_size            = 0;
            uint32_t bank_table_size            = 0;
            uint32_t stream_table_size          = 0;
            uint32_t external_stream_table_size = 0;
        };
        DRAGON_ASSERT(sizeof(AudioPackHeader) == 28, "AudioPackHeader size is not 28");

        template<typename T>
        struct AUDIO_PACK_ENTRY {
            T id               = 0;
            uint32_t alignment = 0;
            uint32_t size      = 0;
            uint32_t offset    = 0;
            uint32_t folder    = 0;
        };

        using AudioPackEntry = AUDIO_PACK_ENTRY<uint32_t>;
        DRAGON_ASSERT(sizeof(AudioPackEntry) == 20, "AudioPackEntry size is not 20");
        using AudioPackEntry64 = AUDIO_PACK_ENTRY<uint64_t>;
        DRAGON_ASSERT(sizeof(AudioPackEntry64) == 24, "AudioPackEntry64 size is not 24");

        using AudioPackName = struct AUDIO_PACK_NAME {
            uint32_t offset;
            uint32_t id;
        };
        DRAGON_ASSERT(sizeof(AudioPackName) == 8, "AudioPackName size is not 8");
#pragma pack(pop)

        static constexpr uint32_t AKPK_FOURCC = DRAGON_MAGIC32('A', 'K', 'P', 'K');

        explicit WemAudioPack(dragon::Array<uint8_t> buffer) {
            auto header = buffer.cast<AudioPackHeader>(0);
            if (header.fourcc != AKPK_FOURCC) {
                DRAGON_ELOG("Not a AKPK file");
                return;
            }

            base_stream = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size());

            size_t cursor = sizeof(AudioPackHeader);
            auto count    = buffer.cast<uint32_t>(cursor);
            for (const AudioPackName &name_entry : buffer.cast<AudioPackName>(cursor + 4, count)) {
                names[name_entry.id] = DRAGON_AKPK_STRING_TYPE(reinterpret_cast<DRAGON_AKPK_CHAR_TYPE *>(buffer.data() + cursor + name_entry.offset));
            }

            cursor += header.name_table_size;
            count = buffer.cast<uint32_t>(cursor);
            banks = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.bank_table_size;
            count         = buffer.cast<uint32_t>(cursor);
            sound_streams = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.stream_table_size;
            count                  = buffer.cast<uint32_t>(cursor);
            external_sound_streams = buffer.cast<AudioPackEntry64>(cursor + 4, count);
        }

        ~WemAudioPack() = default;

        std::shared_ptr<dragon::Array<uint8_t>> base_stream;

        std::map<uint32_t, DRAGON_AKPK_STRING_TYPE> names;
        dragon::Array<AudioPackEntry> banks;
        dragon::Array<AudioPackEntry> sound_streams;
        dragon::Array<AudioPackEntry64> external_sound_streams;

        DRAGON_AKPK_STRING_TYPE get_name(uint32_t id) {
            if (!names.contains(id)) {
                return DRAGON_AKPK_STRING_TYPE();
            }
            return names[id];
        }

        [[nodiscard]] dragon::Array<uint8_t> get_bank(AudioPackEntry entry) const {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        [[nodiscard]] dragon::Array<uint8_t> get_sound_stream(AudioPackEntry entry) const {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        [[nodiscard]] dragon::Array<uint8_t> get_external_sound_streams(AudioPackEntry64 entry) const {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }
    };
} // namespace dragon
