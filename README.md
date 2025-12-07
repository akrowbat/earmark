# earmark
Earmark is a command line program to edit audio file metadata. I tried a few programs that were out there that do this, but I didn't like any of them. So I decided to write my own. Its main usefulness is if you're running a headless media server like Ampache, Jellyfin, or something. It is essentially a command line interface to access the functions from the [TagLib](http://taglib.org/) C++ library.

Earmark is extremely barebones right now, and I don't recommend running it until I improve it substantially. I've implemented basic filetype verification with libmagic, so it shouldn't corrupt files with incorrect extensions. I make no promises. As of right now, it cannot handle multiple tags of the same type being assigned at once, so if you supply any tag, whether it be artist, title, track, whatever, all the input files will be set to those tags respectively.

# Build
Building earmark requires libmagic, TagLib, and [cxxopts](https://github.com/jarro2783/cxxopts).
If you're on a Debian based system, you can run `sudo apt update && sudo apt install zlib1g-dev libutfcpp-dev libmagic-dev` to install the basic dependencies. Then you'll need to [build TagLib](https://github.com/taglib/taglib/blob/master/INSTALL.md) and install it. You'll also need to install the cxxopts headers into your include directory (you just need the include/cxxopts.hpp file). The easiest way to install it is running `sudo wget -O /usr/local/include/cxxopts.hpp https://raw.githubusercontent.com/jarro2783/cxxopts/refs/heads/master/include/cxxopts.hpp`

Once all of that's done, you can build by running this in the repository directory: `mkdir build; cd build; cmake ..; make`

# Usage
Using earmark is very simple. It takes arguments for specific tag types like artist, genre, track, album, and so on, and treats the following argument as the value for that tag. For instance, `earmark --genre "Metal" *.mp3` will change all mp3 files in the working directory to have Metal for the genre tag. You can also use short options combined like `earmark -pa "Iron Maiden" "Aces High.flac"`. The -p means print out all the tag information after any file operations are done, and the -a means that earmark will change the artist value. Short options that require arguments cannot be combined, so if you're changing more than one value at a time, you have to separate them like so: `earmark -g Metal -a "Iron Maiden"`

# Known Issues
* Filetypes whose Mimetype is application/octet-stream can't be parsed yet

# TODO
* Add cover stripping/extracting feature (and broader tag stripping features)
* Add filename/tag conversion features
* Come up with a more coherent scheme for printing information about what earmark is doing and verifying changes with the user before modifying files
* Add logging capability
* Add interactive mode to input different data for individual files
* Add config file defining standard operations used by individual users
* Add the ability to import/export tag data to JSON or NFO
* Add autocompletion features
* Write a proper man page
