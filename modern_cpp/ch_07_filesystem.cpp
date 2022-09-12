#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>

#include "util.h"

namespace { //=================================================================

namespace fs = std::filesystem;
using namespace std::string_literals;

fs::path homeDir() {
    auto* envHome = std::getenv("HOME");
    return fs::path{envHome};
}

void test_path()
{
    auto home = homeDir();

    PRINT_ATTR(home);

    auto project = home /= "project"s;
    project.concat("/dailycommit"s);
    PRINT_ATTR(project);

    auto file = project /= "dailycommit.pro"s;
    PRINT_ATTR(file);
    PRINT_FUNC(file.root_name());       // ""
    PRINT_FUNC(file.root_directory());  // "/"
    PRINT_FUNC(file.root_path());       // "/"
    PRINT_FUNC(file.parent_path());
    PRINT_FUNC(file.filename());        // "dailycommit.pro"
    PRINT_FUNC(file.stem());            // "dailycommit"
    PRINT_FUNC(file.extension());       // ".pro"
    PRINT_FUNC(file.is_absolute());     // true
    PRINT_FUNC(file.is_relative());     // false

    file.replace_filename(".gitignore"s);
    file.replace_extension(""s);
    PRINT_ATTR(file);

    for (const auto& part : file) {
        std::cout << part << "\n";
    }

    auto stdout = fs::path{"/dev/stdout"s};
    stdout.make_preferred();
    PRINT_ATTR(stdout);
}

//#define CLEAN_DIR

void test_file_system()
{
    auto home = homeDir();

    auto temp = home / "temp";
    PRINT_ATTR(temp);

    auto link = home / "temp_link";
#if !defined(CLEAN_DIR)
    auto success = fs::create_directory(temp);
    std::cout << "fs::create_directory(temp): " << success << "\n";

    auto source = home / "project"s / "dailycommit"s / "dbus";
    auto dest = home / "temp";

    auto error = std::error_code{};
    fs::copy(source, dest, fs::copy_options::recursive, error);

    if (error) {
        std::cout << error.message() << "\n";
    }
    try {
        fs::create_directory_symlink(temp, link);
    } catch (std::exception& e) {
        std::cout << e.what() << "\n";
    }
#else
    auto deleted = fs::remove_all(temp);
    std::cout << "remove_all(): " << deleted << " deleted.\n";

    auto success = fs::remove(link);
    std::cout << "remove(" << link << "): " << success << "\n";
#endif
}

std::string getenvstr(const char* env) {
    auto* value = std::getenv(env);
    if (value != nullptr) {
        return std::string{ std::getenv(env) };
    } else {
        return std::string{};
    }
}

void test_env()
{
    PRINT_FUNC(getenvstr("TMPDIR"));
    PRINT_FUNC(getenvstr("TMP"));
    PRINT_FUNC(getenvstr("TEMP"));
}

#define PRINT_PERM(p, mask, sig)    std::cout << ((p & mask) != fs::perms::none ? sig : "-")

void test_file_attribute(const fs::path& file)
{
    std::error_code ec;
    auto exists = fs::exists(file, ec);
    std::cout << "fs::exists(" << file << "): " << exists << "\n";
    if (ec.value()) {
        std::cout << ec.message() << "\n";
    }

    auto lwt = fs::last_write_time(file);

    auto printPermission = [](fs::perms p) {
        PRINT_PERM(p, fs::perms::owner_read, "r");
        PRINT_PERM(p, fs::perms::owner_write, "w");
        PRINT_PERM(p, fs::perms::owner_exec, "x");
        PRINT_PERM(p, fs::perms::group_read, "r");
        PRINT_PERM(p, fs::perms::group_write, "w");
        PRINT_PERM(p, fs::perms::group_exec, "x");
        PRINT_PERM(p, fs::perms::others_read, "r");
        PRINT_PERM(p, fs::perms::others_write, "w");
        PRINT_PERM(p, fs::perms::others_exec, "x");
        std::cout << "\n";
    };

    auto status = fs::status(file);
    printPermission(status.permissions());

    PRINT_FUNC(fs::is_regular_file(file));
    PRINT_FUNC(fs::is_directory(file));
    PRINT_FUNC(fs::is_character_file(file));
    PRINT_FUNC(fs::is_symlink(file));

    try {
        PRINT_FUNC(fs::file_size(file));
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

void test_file_attribute()
{
#if (0)
    auto home = homeDir();
    auto proj = home / "project" / "dailycommit";
    auto file = proj / "dailycommit.pro";
#else
    auto file = fs::path{"/dev/tty"};
#endif

    test_file_attribute(file);
}

//#define USE_RECULSIVE_ITERATOR

void test_dir(const fs::path& path, bool recursive, int step)
{
    if (fs::exists(path) && fs::is_directory(path)) {
#ifdef USE_RECULSIVE_ITERATOR
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            (void)recursive;
            (void)step;
#else
        for (const auto& entry : fs::directory_iterator(path)) {
            auto str = std::string(step*4, ' ');
            std::cout << str;
#endif
            auto filename = entry.path().filename();
            auto status = entry.status();

            if (fs::is_directory(status)) {
                std::cout << "[+] " << filename << "\n";
#if !defined(USE_RECULSIVE_ITERATOR)
                if (recursive) {
                    test_dir(entry.path(), recursive, step+1);
                }
#endif
            } else if (fs::is_symlink(status)) {
                std::cout << "[>] " << filename << "\n";
            } else if (fs::is_regular_file(status)) {
                std::cout << "    " << filename << "\n";
            } else {
                std::cout << "[?] " << filename << "\n";
            }
        }
    }
}

void test_dir()
{
    auto home = homeDir();
    auto proj = home / "project" / "dailycommit" / "dbus";

    test_dir(proj, true, 0);
}

std::vector<fs::path> find(const fs::path& dir, std::function<bool(const fs::path&)> filter)
{
    auto ret = std::vector<fs::path>{};
    for (const auto& entry : fs::recursive_directory_iterator(dir, fs::directory_options::follow_directory_symlink)) {
        if (fs::exists(entry)) {
            auto path = entry.path();
            auto status = entry.status();

            if (fs::is_regular_file(status) && filter(path)) {
                ret.push_back(path);
            }
        }
    }
    return ret;
}

void test_find()
{
    auto filter = [](const fs::path& path) -> bool {
        return path.extension() == ".cpp"s;
    };
    auto home = homeDir();
    auto proj = home / "project" / "dailycommit" / "modern_cpp";
    auto ret = find(proj, filter);

    std::copy(std::begin(ret), std::end(ret), std::ostream_iterator<fs::path>(std::cout, "\n"));
}

} //namespace =================================================================

void test_ch_07_filesystem()
{
    std::cout << std::boolalpha;
#if (0) // done
    test_path();
    test_file_system();
    test_env();
    test_file_attribute();
    test_dir();
#endif

    test_find();
}
