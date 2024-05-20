/// @brief File matching header only module 
#pragma once
#include "program_options.h"
#include "hash_history.h"
#include <boost/container_hash/hash.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <fstream>
#include <string>
#include <utility>
#include <any>

/// @brief Hash histories for different hash algo's are ready
inline boost_default_hh_t boost_default_hh;
inline md5_hh_t md5_hh;
inline crc_hh_t crc_hh;

/// @brief All possible hash histories; only one of them is working during a program run
inline HashHistory *hashes[]{&boost_default_hh, &md5_hh, &crc_hh};

/// @brief A pointer to the current hash history
inline HashHistory *cur_hh;

/// @brief Check, if the block[block_ind] is already present in "hash history",
///        if not in history read a block from file
///        if in history - just "seekg" the file
/// @param file opened file to possibly read from
/// @param path_ind the index of the file in the "paths_found" list, see list_files.h
/// @param block_ind the block, needed to be present in "hashes_history"
void read_block_if_its_hash_is_absent(std::ifstream &file, const std::size_t path_ind,
                                      const std::size_t block_ind)
{
    try
    {
        // Read from file if block_ind is past last history index
        // or if the history for this path is still empty
        if (block_ind > cur_hh->last_index(path_ind) || cur_hh->is_empty(path_ind))
        {
            std::vector<char> buf(arg_block_size);
            char *beg = &buf[0];

            file.read(beg, arg_block_size);
            std::string s(buf.begin(), buf.end());

            // New hash is added to the history
            cur_hh->add_hash(s, path_ind);
        }
        else
            file.seekg(arg_block_size, std::ios_base::seekdir::_S_cur);
    }

    catch (std::ifstream::failure &e)
    {
        std::cerr << e.what() << '\n';
    }
}

void match_files(const std::vector<size_n_path_t> paths_found)
{
    /// @brief Once included in an equity group, the file is then and ever skipped;
    /// this is a set of files to skip
    std::unordered_set<std::size_t> paths_to_skip;
    try
    {
        cur_hh = hashes[arg_hash_algo];

        for (auto token = paths_found.begin(); token != paths_found.end(); ++token)
        {
            std::size_t token_ind = token - paths_found.begin();
            if (paths_to_skip.contains(token_ind))
                continue;
            paths_to_skip.emplace(token_ind);

            cur_hh->add_path(token_ind);

            bool token_isnt_printed = true;

            for (auto match = paths_found.begin(); match != paths_found.end(); ++match)
            {
                std::size_t match_ind = match - paths_found.begin();

                if (paths_to_skip.contains(match_ind))
                    continue;

                // Every file must be included in history
                cur_hh->add_path(match_ind);

                bool does_match = true;

                std::ifstream token_file;
                token_file.open(token->s, std::ios_base::binary);
                assert(token_file.is_open());

                std::ifstream match_file;
                match_file.open(match->s, std::ios_base::binary);
                assert(match_file.is_open());

                if (token->fsize == match->fsize)
                {
                    for (std::size_t block_ind = 0; !token_file.eof() && !match_file.eof(); ++block_ind)
                    {
                        read_block_if_its_hash_is_absent(token_file, token_ind, block_ind);
                        read_block_if_its_hash_is_absent(match_file, match_ind, block_ind);

                        if (!cur_hh->match_hashes(token_ind, match_ind, block_ind))
                        {
                            does_match = false;
                            break;
                        }
                    }
                    if (does_match)
                    {
                        if (token_isnt_printed)
                        {
                            std::cout << '\n'
                                      << token->s << '\n';
                            token_isnt_printed = false;
                        }
                        std::cout << match->s << '\n';
                        paths_to_skip.emplace(match_ind);
                    }
                }
                match_file.close();
                token_file.close();
            }
        }
    }

    catch (std::ifstream::failure &e)
    {
        std::cerr << e.what() << '\n';
    }
}
