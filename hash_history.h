/// @brief This file contains hash history objects, which include algo and appropriate buffers
#pragma once
#include "program_options.h"
#include <boost/container_hash/hash.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/crc.hpp>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

/// @brief Hash history base class, for technical reasons - not pure virtual
class HashHistory
{
public:
    /// @brief Calculate a hash  from string and put it into hash history
    /// @param std::string - buffer
    /// @param std::size_t - buffer length
    virtual void add_hash(const std::string, const std::size_t) {};

    /// @brief Returns last block index for given path index in history hash
    /// @param size_t path_ind - index of path in found files vector
    /// @return size_t last block index
    virtual size_t last_index([[maybe_unused]] const std::size_t path_ind) { return 0; }

    /// @brief Return true if history for given path index is empty
    /// @param size_t path_ind - index of path in found files vector
    /// @return true if history for given path index is empty
    virtual bool is_empty([[maybe_unused]] const std::size_t path_ind) { return true; }

    /// @brief Equivalent of == operator for hashes saved in history and defined by token_ind, path_ind, block_ind
    /// @param token_ind - the index of the file, we are searching matches for
    /// @param match_ind - the index of the file to match
    /// @param block_ind - the index of the current block in both files
    /// @return
    virtual bool match_hashes([[maybe_unused]] const std::size_t token_ind,
                              [[maybe_unused]] const std::size_t match_ind,
                              [[maybe_unused]] const std::size_t block_ind) { return true; }

    /// @brief Add an empty track for new file into hash history
    /// @param path_ind size_t path_ind - index of path in found files vector
    virtual void add_path([[maybe_unused]] const size_t path_ind) {}
};

/// @brief  Template hash history class, realizing methods, common for different hashes (BOOST default, md5, crc)
/// @tparam T - a class, which contains one hash value, 'size_t' for example
template <typename T>
class base_hh : public HashHistory
{
protected:
    /// @brief A map, containing hashes of blocks once read,
    ///        grouped by index of a file in the vector of found paths
    std::unordered_map<std::size_t, std::vector<T>> hash_history;

public:
    virtual void add_hash([[maybe_unused]] const std::string s, [[maybe_unused]] const std::size_t path_ind) override {}

    size_t last_index(const std::size_t path_ind) override final
    {
        return hash_history[path_ind].size() - 1;
    };

    bool is_empty(const std::size_t path_ind) override final
    {
        return hash_history[path_ind].size() == 0;
    }

    virtual bool match_hashes(const std::size_t token_ind, const std::size_t match_ind, const std::size_t block_ind) override
    {
        return (hash_history[token_ind].at(block_ind) == hash_history[match_ind].at(block_ind));
    }
    void add_path(const size_t path_ind) override final
    {
        hash_history.emplace(path_ind, std::vector<T>());
    }
};

/// @brief A hash history obj based on Boost default hash
class boost_default_hh_t : public base_hh<std::size_t>
{
private:
    boost::hash<std::string> boost_default_string_hash;

public:
    void add_hash(const std::string buf, const std::size_t path_ind) override final
    {
        std::size_t _hash = boost_default_string_hash(buf); // () operator performs hashing
        hash_history[path_ind].emplace_back(_hash);
    }
};

/// @brief Nof integer items in md5 hash, named 'digest'
constexpr std::size_t items_in_digest = sizeof(boost::uuids::detail::md5::digest_type) / sizeof(unsigned int);

/// @brief A hash history obj based on md5, which uses boost::uuids::detail::md5::digest_type to save hash value
class md5_hh_t : public base_hh<boost::uuids::detail::md5::digest_type>
{
private:
    std::unordered_map<std::size_t, std::vector<std::array<unsigned int, items_in_digest>>> hash_history;

public:
    using digest_t = boost::uuids::detail::md5::digest_type;
    void add_hash(const std::string buf, const std::size_t path_ind) override final
    {
        boost::uuids::detail::md5 md5;
        hash_history[path_ind].emplace_back(std::array<unsigned int, items_in_digest>());

        // Add new hash to digest in md5
        md5.process_bytes(buf.c_str(), buf.size());

        // Emplace accumulated digest into last element of hash_history
        md5.get_digest(*reinterpret_cast<digest_t *>(&(hash_history[path_ind][hash_history[path_ind].size() - 1][0])));
    }

    bool match_hashes(const std::size_t token_ind, const std::size_t match_ind, const std::size_t block_ind) override final
    {

        for (size_t i = 0; i < items_in_digest; ++i)
            if (hash_history[token_ind][block_ind][i] != hash_history[match_ind][block_ind][i])
                return false;
        return true;
    }
};

/// @brief A hash history obj based on CRC
class crc_hh_t : public base_hh<std::size_t>
{
public:
    void add_hash(const std::string buf, const std::size_t path_ind) override final
    {
        boost::crc_32_type result;
        result.process_bytes(buf.data(), buf.size());
        std::size_t _hash = result.checksum();
        hash_history[path_ind].emplace_back(_hash);
    }
};
