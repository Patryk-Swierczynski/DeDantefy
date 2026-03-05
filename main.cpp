#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <fstream>
using namespace std;
std::vector<string> dante_to_nuke = {
        "CMakeLists.txt",
        "makefile",
        "unit_helper_v2.c",
        "unit_helper_v2.h",
        "unit_test_v2.c",
        "tested_declarations.h",
        "rdebug.c",
        "rdebug.h"
};
std::vector<string> nuke_dante_includes{
"tested_declarations.h",
"rdebug.h"
};
int main(int argc,char* argv[]) {
    if (argc < 2) {
        cout << "Not enough arguments\n";
        return 1;
    }

    filesystem::path path = argv[1];
    //nuke every file that contains dante
    for (const auto &entry: filesystem::recursive_directory_iterator(path)) {
        auto cur_file = entry.path();
        for (const auto &dante: dante_to_nuke) {
            if (cur_file.filename() == dante) {
                filesystem::remove(entry.path());
            }
        }
    }
    //nuke dante includes
    for (const auto &entry: filesystem::recursive_directory_iterator(path)) {
        if (!entry.is_regular_file()) continue;
        auto cur_file = entry.path();
        std::string temp_file = cur_file.string() + ".tmp";
        ifstream in(cur_file);
        ofstream out(temp_file);

        std::string line;
        while (std::getline(in, line)) {
            bool nuke = false;
            for (const auto &dante: nuke_dante_includes) {
                if (line.find(dante) != std::string::npos) {
                    nuke = true;
                }
            }
            if (!nuke) {
                out << line << "\n";
            }
        }
        in.close();
        out.close();
        try {

            filesystem::rename(temp_file, cur_file);
        } catch (const filesystem::filesystem_error &) {
            filesystem::copy_file(temp_file, cur_file, filesystem::copy_options::overwrite_existing);
            filesystem::remove(temp_file);
        }
    }
    return 0;
}