//
// Created by yretenai on 2020-10-03.
//

#pragma once

#include "Array.hpp"
#include "dragon.hpp"
#include <cassert>
#include <cstdbool>
#include <map>
#include <memory>
#include <string>

namespace dragon {
    class WemAudioPack {
      public:
#pragma pack(push, 1)
        typedef struct AUDIO_PACK_HEADER {
            uint32_t fourcc = AKPK_FOURCC;
            uint32_t size = 0;
            uint32_t version = 1;
            uint32_t name_table_size = 0;
            uint32_t bank_table_size = 0;
            uint32_t stream_table_size = 0;
            uint32_t external_stream_table_size = 0;
        } AudioPackHeader;

        typedef struct AUDIO_PACK_ENTRY {
            uint32_t id = 0;
            uint32_t alignment = 0;
            uint32_t size = 0;
            uint32_t offset = 0;
            uint32_t folder = 0;
        } AudioPackEntry;

        typedef struct AUDIO_PACK_ENTRY_EXTERNAL {
            uint32_t id = 0;
            uint32_t bank_id = 0;
            uint32_t alignment = 0;
            uint32_t size = 0;
            uint32_t offset = 0;
            uint32_t folder = 0;
        } AudioPackEntryExternal;

        typedef struct AUDIO_PACK_NAME {
            uint32_t offset;
            uint32_t id;
        } AudioPackName;
#pragma pack(pop)

        static constexpr uint32_t AKPK_FOURCC = MAKEFOURCC('A', 'K', 'P', 'K');

        WemAudioPack(dragon::Array<uint8_t> buffer) {
            AudioPackHeader header = buffer.cast<AudioPackHeader>(0);
            if (header.fourcc != AKPK_FOURCC) {
                ELOG("Not a AKPK file");
                return;
            }

            base_stream = std::make_shared<dragon::Array<uint8_t>>(buffer.data(), buffer.byte_size(), nullptr);

            size_t cursor = sizeof(AudioPackHeader);
            uint32_t count = buffer.cast<uint32_t>(cursor);
            for (const AudioPackName& name_entry : buffer.cast<AudioPackName>(cursor + 4, count)) {
                names[name_entry.id] = std::wstring(reinterpret_cast<wchar_t*>(buffer.data() + cursor + name_entry.offset));
            }

            cursor += header.name_table_size;
            count = buffer.cast<uint32_t>(cursor);
            banks = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.bank_table_size;
            count = buffer.cast<uint32_t>(cursor);
            sound_streams = buffer.cast<AudioPackEntry>(cursor + 4, count);

            cursor += header.stream_table_size;
            count = buffer.cast<uint32_t>(cursor);
            external_sound_streams = buffer.cast<AudioPackEntryExternal>(cursor + 4, count);
        }

        ~WemAudioPack() = default;

        std::shared_ptr<dragon::Array<uint8_t>> base_stream;

        std::map<uint32_t, std::wstring> names;
        dragon::Array<AudioPackEntry> banks;
        dragon::Array<AudioPackEntry> sound_streams;
        dragon::Array<AudioPackEntryExternal> external_sound_streams;

        std::wstring get_name(uint32_t id) {
            if (!names.contains(id)) {
                return std::wstring();
            }
            return names[id];
        }

        dragon::Array<uint8_t> get_bank(AudioPackEntry entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        dragon::Array<uint8_t> get_sound_stream(AudioPackEntry entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }

        dragon::Array<uint8_t> get_external_sound_streams(AudioPackEntryExternal entry) {
            return dragon::Array<uint8_t>(base_stream, entry.offset * entry.alignment, entry.size);
        }
    };
} // namespace dragon
