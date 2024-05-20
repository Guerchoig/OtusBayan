/// @brief Program options header only module
#pragma once

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <utility>
#include <string>

namespace po = boost::program_options;

constexpr std::size_t default_block_size = 128;

// Storage for command string args
inline po::variables_map vm;
inline std::size_t arg_block_size;
inline std::vector<std::string> search_paths;
inline std::vector<std::string> exclude_paths;
inline bool arg_recursion;
inline std::string arg_mask;
inline std::size_t arg_min_file_size;
inline int arg_hash_algo;

/// @brief Takes and parse cmd string options, then save it into global inline variables
/// @param argc
/// @param argv
/// @return false on "--help" option present in cmd string
bool save_program_options(int argc, char **argv)
{
    // Describe options
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")                                                                                                                                              //
        ("block size,S", po::value<std::size_t>(&arg_block_size)->default_value(default_block_size), "hash block size in bytes: 128 bytes is default")                                              //
        ("recursion,r", po::value<bool>(&arg_recursion)->default_value(1), "recursion: 1-include subdirs in search, 0-don't include")                                                               //
        ("path,p", po::value<std::vector<std::string>>(&search_paths)->multitoken(), "search path, repeat the option for every path to search at")                                                  //
        ("exclude path,x", po::value<std::vector<std::string>>(&exclude_paths)->multitoken(), "path, excluded from the search, repeat the option for every path you want to exclude")               //
        ("mask,m", po::value<std::string>(&arg_mask)->default_value(""), "a mask for names of files to search, uses Perl-compatible regular expressions  syntax (https://perldoc.perl.org/perlre)") //
        ("min file size,z", po::value<std::size_t>(&arg_min_file_size)->default_value(1), "min size in bytes of every single searched file")                                                        //
        ("hash algo,H", po::value<int>(&arg_hash_algo)->default_value(0), "hash algo: 0-BOOST default, 1-md5, 2-crc32");                                                                            //
    try
    {
        // Parse and store options
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // Throw exception on error
        po::notify(vm);
    }
    catch (po::error &e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl
                  << std::endl;
        std::cerr << desc << std::endl;
        return false;
    }

    // Print help on help option and return
    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return false;
    }
    return true;
}