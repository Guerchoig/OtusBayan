# Allowed options:
| option short | option long     | meaning                                                                                                                               |
| ------------ | --------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| --help       |                 | produce help message                                                                                                                  |
| -S           | --block size    | hash block size in bytes: 128 bytes is default                                                                                        |
| -r           | --recursion     | recursion: 1-include subdirs in search (default), 0-don't include                                                                     |
| -p           | --path          | search path, repeat the option for every search path                                                                                  |
| -x           | --exclude path  | path, excluded from the search, repeat the option for every path, you want to exclude                                                 |
| -m           | --mask          | a mask for names of files to search, uses Perl-compatible regular expressions  syntax               (https://perldoc.perl.org/perlre) |
| -z           | --min file size | min size in bytes of every single searched file, 1 byte is default                                                                    |
| -H           | --hash algo     | hash algo: 0-BOOST default (default algo), 1-md5, 2-crc32                                                                             |
