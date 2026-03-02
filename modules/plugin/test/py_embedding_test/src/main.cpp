#include "py_embedding_test_plugin.hpp"

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
    std::cout << "Module path: " << module_path_str << std::endl;

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