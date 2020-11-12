//
// Created by yretenai on 2020-10-04.
//

#include "../../WemAudioPack.hpp"

void process_file(const std::filesystem::path& output, const std::filesystem::path& path) {
    dragon::Array<uint8_t> buffer = dragon::read_file(path);
    dragon::WemAudioPack pack(buffer);

    for (auto& pair : pack.names) {
        std::filesystem::path tmp = output / pair.second;
        if (!std::filesystem::exists(tmp)) {
            std::filesystem::create_directories(tmp);
        }
    }

    for (auto entry : pack.banks) {
        dragon::write_file(output / pack.get_name(entry.folder) / (std::to_string(entry.id) + ".bnk"), pack.get_bank(entry));
    }

    for (auto entry : pack.sound_streams) {
        dragon::write_file(output / pack.get_name(entry.folder) / (std::to_string(entry.id) + ".wem"), pack.get_sound_stream(entry));
    }

    for (auto entry : pack.external_sound_streams) {
        dragon::write_file(output / pack.get_name(entry.folder) / (std::to_string(entry.id) + ".wem"),
                           pack.get_external_sound_streams(entry)); // usually .wav, but sometimes it's still a wem codec.
    }
}

void process_directory(const std::filesystem::path& output, const std::filesystem::path& base, const std::filesystem::path& path) {
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
        const std::filesystem::path& subpath = entry.path();
        if (std::filesystem::is_regular_file(subpath)) {
            process_file(output / (std::filesystem::relative(subpath, base).replace_extension()), subpath);
        } else if (std::filesystem::is_directory(subpath)) {
            process_directory(output, base, subpath);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        ELOG("USAGE: " << argv[0] << " output_dir file1.pck...");
        return 1;
    }

    std::filesystem::path output = argv[1];
    if (!std::filesystem::exists(output)) {
        std::filesystem::create_directories(output);
    }

    for (int i = 2; i < argc; ++i) {
        std::filesystem::path path = argv[i];
        if (!std::filesystem::exists(path))
            continue;
        if (std::filesystem::is_directory(path)) {
            process_directory(output, path, path);
        } else {
            process_file(output / path.filename().replace_extension(), path);
        }
    }
}
