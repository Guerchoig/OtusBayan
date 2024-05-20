/// @brief This module contain the main function, which
/// 1 remember program options
/// 2 produce calls to list_files - to obtain files list
/// 3 match_files - to compare files and to group matching files
#include "list_files.h"
#include "program_options.h"
#include "match_files.h"
#include <algorithm>

int main(int argc, char **argv)
{
    if (!save_program_options(argc, argv))
        // help option present
        return 0;

    // Paths of files to compare
   
    std::vector<size_n_path_t> res_paths;

    // Obtain paths of files to compare
    list_files(res_paths);

    // std::ofstream out("out.txt");
    // std::streambuf *coutbuf = std::cout.rdbuf();
    // std::cout.rdbuf(out.rdbuf()); // redirect std::cout to out.txt!

    // Compare all files from res_paths everyone with everyone,
    // hashing sequential blocks of files, and print them
    match_files(res_paths);

    // std::cout.rdbuf(coutbuf);
    return 0;
}
