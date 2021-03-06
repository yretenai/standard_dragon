//
// Created by yretenai on 2020-10-03.
//

#pragma once

#include "Array.hpp"
#include "dragon.hpp"
#include <cassert>
#include <cstdint>
#include <cstdbool>
#include <map>
#include <memory>
#include <string>

#ifdef DRAGON_AKPKANSI
#define DRAGON_AKPK_CHAR_TYPE char
#define DRAGON_AKPK_STRING_TYPE std::string
#else
#define DRAGON_AKPK_CHAR_TYPE wchar_t
#define DRAGON_AKPK_STRING_TYPE std::wstring
#endif

namespace dragon {
    class WemAudioPack {
      public:
#pragma pack(push, 1)
        typedef struct AUDIO_PACK_HEADER {
            uint32_t fourcc = AKPK_FOURCC;
            uint32_t size = 0;
            [[maybe_unused]] uint32_t version = 1;
            uint32_t name_table_size = 0;
            uint32_t bank_table_size = 0;
            uint32_t stream_table_size = 0;
            [[maybe_unused]] uint32_t external_stream_table_size = 0;
        } AudioPackHeader;
        DRAGON_ASSERT(sizeof(AudioPackHeader) == 28, "Wem Audio Pack header has an invalid size");

        template<typename T>
        struct AUDIO_PACK_ENTRY {
            T id = 0;
            uint32_t alignment = 0;
            uint32_t size = 0;
            uint32_t offset = 0;
            uint32_t folder = 0;
        };

        typedef AUDIO_PACK_ENTRY<uint32_t> AudioPackEntry;
        DRAGON_ASSERT(sizeof(AudioPackEntry) == 20, "Wem Audio Pack Entry (32-bit) has an invalid size");
        typedef AUDIO_PACK_ENTRY<uint64_t> AudioPackEntry64;
        DRAGON_ASSERT(sizeof(AudioPackEntry64) == 24, "Wem Audio Pack Entry (64-bit) has an invalid size");

        typedef struct AUDIO_PACK_NAME {
            uint32_t offset;
            uint32_t id;
        } AudioPackName;
        DRAGON_ASSERT(sizeof(AudioPackName) == 8, "Wem Audio Pack Name Entry has an invalid size");
#pragma pack(pop)

        static constexpr uint32_t AKPK_FOURCC = DRAGON_MAGIC32('A', 'K', 'P', 'K');

        WemAudioPack(dragon::Array<uint8_t> buffer) {
            AudioPackHeader header = buffer.cast<AudioPackHeader>(0);
            if (header.fourcc != AKPK_FOURCC) {
                DRAGON_ELOG("Not a AKPK file");
                return;
            }

            base_stream = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size(), nullptr);

            size_t cursor = sizeof(AudioPackHeader);
            uint32_t count = buffer.cast<uint32_t>(cursor);
            for (const AudioPackName& name_entry : buffer.cast<AudioPackName>(cursor + 4, count)) {
                names[name_entry.id] = DRAGON_AKPK_STRING_TYPE(reinterpret_cast<DRAGON_AKPK_CHAR_TYPE*>(buffer.data() + cursor + name_entry.offset));
            }

            cursor += header.name_table_size;
            count = buffer.cast<uint32_t>(cursor);
            banks = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.bank_table_size;
            count = buffer.cast<uint32_t>(cursor);
            sound_streams = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.stream_table_size;
            count = buffer.cast<uint32_t>(cursor);
            external_sound_streams = buffer.cast<AudioPackEntry64>(cursor + 4, count);
        }

        ~WemAudioPack() = default;

        std::shared_ptr<dragon::Array<uint8_t>> base_stream;

        std::map<uint32_t, DRAGON_AKPK_STRING_TYPE> names;
        dragon::Array<AudioPackEntry> banks;
        dragon::Array<AudioPackEntry> sound_streams;
        dragon::Array<AudioPackEntry64> external_sound_streams;

        [[maybe_unused]] DRAGON_AKPK_STRING_TYPE get_name(uint32_t id) {
            if (!names.contains(id)) {
                return DRAGON_AKPK_STRING_TYPE();
            }
            return names[id];
        }

        [[maybe_unused]] dragon::Array<uint8_t> get_bank(AudioPackEntry entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        [[maybe_unused]] dragon::Array<uint8_t> get_sound_stream(AudioPackEntry entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        [[maybe_unused]] dragon::Array<uint8_t> get_external_sound_streams(AudioPackEntry64 entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }
    };
} // namespace dragon
