#include "py_embedding_test_plugin.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

int main()
{
    auto home_dir = std::getenv("HOME");
    if(home_dir == nullptr) {
        std::cerr << "Error: HOME environment variable is not set." << std::endl;
        return 1;
    }

    // Path where test_logic.py lives
    const char* module_path = std::getenv("PY_EMBED_PYDIR");
    if (!module_path) {
        std::cerr << "PY_EMBED_PYDIR is not set (getenv returned null). "
                << "If running with sudo, use sudo -E or set it explicitly.\n";
        return 1; // or throw
    }

    std::string module_path_str(module_path);
    std::filesystem::path module_path_fs(module_path_str);

    // Expand "~" manually (Python import path does not expand shell shortcuts).
    if (!module_path_str.empty() && module_path_str[0] == '~') {
        auto home = std::getenv("HOME");
        if (!home) {
            std::cerr << "HOME is not set, cannot expand '~' in PY_EMBED_PYDIR.\n";
            return 1;
        }
        if (module_path_str.size() == 1) {
            module_path_str = std::string(home);
        } else if (module_path_str[1] == '/') {
            module_path_str = std::string(home) + module_path_str.substr(1);
        }
        module_path_fs = std::filesystem::path(module_path_str);
    }

    // sys.path must contain a directory, not a .py file path.
    if (module_path_fs.has_extension() && module_path_fs.extension() == ".py") {
        module_path_fs = module_path_fs.parent_path();
    }

    module_path_str = module_path_fs.string();
    if (!std::filesystem::exists(module_path_fs) || !std::filesystem::is_directory(module_path_fs)) {
        std::cerr << "PY_EMBED_PYDIR must resolve to an existing directory. Got: "
                  << module_path_str << std::endl;
        return 1;
    }

    std::cout << "Python import directory: " << module_path_str << std::endl;

    PyEmbeddingTestPlugin plugin(
        "test_logic",
        module_path_str,
        "MyLogic",
        "tick_with_args",
        "py_embedding_test_plugin",
        "robot_01"
    );

    plugin.execute();
    
    return 0;
}
