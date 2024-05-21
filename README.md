Allowed options:
  --help                          produce help message 
  -S [ --block size ] arg (=128)  hash block size in bytes: 128 bytes is 
                                  default
  -r [ --recursion ] arg (=1)     recursion: 1-include subdirs in search, 
                                  0-don't include
  -p [ --path ] arg               search path, repeat the option for every path
                                  to search at
  -x [ --exclude path ] arg       path, excluded from the search, repeat the 
                                  option for every path you want to exclude
  -m [ --mask ] arg               a mask for names of files to search, uses 
                                  Perl-compatible regular expressions  syntax 
                                  (https://perldoc.perl.org/perlre)
  -z [ --min file size ] arg (=1) min size in bytes of every single searched 
                                  file
  -H [ --hash algo ] arg (=0)     hash algo: 0-BOOST default, 1-md5, 2-crc32
