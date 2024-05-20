/// @brief File listing header only module
#pragma once
#include "program_options.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <boost/regex.hpp>
#include <algorithm>
#include <iostream>
#include <exception>

namespace fs = boost::filesystem;

struct size_n_path_t
{
    size_t fsize;
    std::string s;
};

/// @brief Regex-check filename checks the file name by arg_mask
/// @param p - filename in fs::path format
/// @return - true, if p regex-matches arg_mask
bool check_file_name(const fs::path &p)
{
    bool res = true;
    boost::regex expr(arg_mask);
    boost::smatch xResults;
    res = boost::regex_match(p.string(), xResults, expr);
    return res;
}

/// @brief Determine that the path is excluded from search by cmd '-x' option
/// @param p - path to check
/// @return true if the path is in exclude_paths
bool excluded_path(std::string p)
{
    for (auto ep : exclude_paths)
        if (ep == p)
            return true;
    return false;
}

/// @brief Build the list of files, which must be compared, according to command line args
/// @param paths_found - the resulting vector of found paths
void list_files(std::vector<size_n_path_t> &paths_found)
{
    try
    {
        // Iterates by possibly several 'root' paths, established by several '-p' arguments of the command string
        for (auto p : search_paths)
        {
            // Miss exclude paths
            if (excluded_path(p))
                continue;

            // Iterator to the end: fs::recursive_directory_iterator points to past the end,
            // if no args are given
            const fs::recursive_directory_iterator end_it;

            //  Recursion over one path, given in the command string
            for (fs::recursive_directory_iterator it(p); it != end_it; ++it)
            {
                // If the path is a file
                if (is_regular_file(it->path()))
                {
                    // Check for file size
                    size_t fsize = file_size(it->path());
                    if (fsize < arg_min_file_size)
                        continue;

                    // Check for filename regex-mask, empty mask means every name is legal
                    if (arg_mask != "" && !check_file_name(it->path().filename()))
                        continue;

                    // The path passed all the filters and is worth to be saved
                    paths_found.emplace_back(fsize, it->path().string());
                }

                // If the path is a directory
                if (is_directory(it->path()))
                {
                    // If recurse cmd string arg == 1
                    if (arg_recursion)
                    {
                        // Preliminary enable in-depth recursion
                        it.disable_recursion_pending(false);

                        // But disable it if it's an excluded path
                        if (excluded_path(it->path().string()))
                            it.disable_recursion_pending(true);
                    }
                    // If recurse cmd string arg == 0, disable in-depth recursion
                    else
                        it.disable_recursion_pending(true);
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}