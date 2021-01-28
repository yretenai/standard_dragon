//
// Created by yretenai on 2020-11-12.
//

#include "../../WemSoundbank.hpp"
#include "../../Indent.hpp"

void process_file(const std::filesystem::path& path) {
    dragon::Array<uint8_t> buffer = dragon::read_file(path);
    dragon::WemSoundbank bank(buffer);
    std::filesystem::path full = path.parent_path() / path.filename().replace_extension();
    if(!std::filesystem::exists(full)) std::filesystem::create_directories(full);

    // process DIDX
    std::shared_ptr<dragon::bkhd::WemChunk> didx_chunk = bank.get_chunk_impl(dragon::WemSoundbank::DIDX_FOURCC);
    std::shared_ptr<dragon::bkhd::WemChunk> data_chunk = bank.get_chunk_impl(dragon::WemSoundbank::DATA_FOURCC);
    if(didx_chunk != nullptr && data_chunk != nullptr) {
        dragon::bkhd::WemDataIndex* didx = CAST_WEMBNK_CHUNK(dragon::bkhd::WemDataIndex, didx_chunk);
        dragon::bkhd::WemData* data = CAST_WEMBNK_CHUNK(dragon::bkhd::WemData, didx_chunk);
        for(auto entry : didx->streams) {
            dragon::write_file(full / (std::to_string(entry.first) + ".wem"), data->get_stream(entry.second));
        }
    }

    // process HIRC
    std::shared_ptr<dragon::bkhd::WemChunk> hirc_chunk = bank.get_chunk_impl(dragon::WemSoundbank::HIRC_FOURCC);
    if(hirc_chunk != nullptr) {
        // dragon::bkhd::WemHierarchy* hirc = CAST_WEMBNK_CHUNK(dragon::bkhd::WemHierarchy, didx_chunk);
        dragon::Indent indent(0);
    }
}

void process_directory(const std::filesystem::path& path) {
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
        const std::filesystem::path& subPath = entry.path();
        if (std::filesystem::is_regular_file(subPath)) {
            process_file(subPath);
        } else if (std::filesystem::is_directory(subPath)) {
            process_directory(subPath);
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        DRAGON_ELOG("USAGE: " << argv[0] << " file1.bnk...");
        return 1;
    }

    for (int i = 2; i < argc; ++i) {
        std::filesystem::path path = argv[i];
        if (!std::filesystem::exists(path))
            continue;
        if (std::filesystem::is_directory(path)) {
            process_directory(path);
        } else {
            process_file(path);
        }
    }
}
