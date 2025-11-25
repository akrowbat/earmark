# earmark
Earmark is a command line program to edit audio file metadata from the command line. I tried a few programs that were out there that do this, but I didn't like any of them. So I decided to write my own. Its main usefulness is if you're running a headless media server like Ampache, Jellyfin, or something. It is essentially a command line interface to access the functions from the [TagLib](http://taglib.org/) C++ library. Earmark is extremely barebones right now, and I don't recommend running it until I improve it substantially. It doesn't yet do any filetype verification, so if you have a file with a wrong extension, it may corrupt that file.

# Build
To build earmark, you'll need to [build TagLib](https://github.com/taglib/taglib/blob/master/INSTALL.md) and install it. You'll also need to install the [cxxopts](https://github.com/jarro2783/cxxopts) headers into your include directory. Usually that just involves installing cmake, zlib, and utfcpp (the zlib1g-dev and libutfcpp-dev packages on Debian derivatives). Then just make a build directory in the earmark directory and run `cmake ..` and then `make` in it.

# TODO
* Add logic to verify MIME type so files don't get corrupted if they have the wrong extensions
* Add cover stripping/extracting feature (and broader tag stripping features)
* Add filename/tag conversion features
* Come up with a more coherent scheme for printing information about what earmark is doing and verifying changes with the user before modifying files
