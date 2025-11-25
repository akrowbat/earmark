# earmark
Earmark is a command line program to edit audio file metadata. I tried a few programs that were out there that do this, but I didn't like any of them. So I decided to write my own. Its main usefulness is if you're running a headless media server like Ampache, Jellyfin, or something. It is essentially a command line interface to access the functions from the [TagLib](http://taglib.org/) C++ library. Earmark is extremely barebones right now, and I don't recommend running it until I improve it substantially. It doesn't yet do any filetype verification, so if you have a file with a wrong extension, it may corrupt that file.

# Build
To build earmark, you'll need to [build TagLib](https://github.com/taglib/taglib/blob/master/INSTALL.md) and install it. TagLib depends on zlib, and utfcpp (the zlib1g-dev and libutfcpp-dev packages on Debian derivatives). You'll also need to install the [cxxopts](https://github.com/jarro2783/cxxopts) headers into your include directory.

After you clone the repository, run this to build the project: `cd earmark; mkdir build; cd build; cmake ..; make`

# Usage
Using earmark is very simple. It takes arguments for specific tag types like artist, genre, track, album, and so on, and treats the following argument as the value for that tag. For instance, `earmark --genre "Metal" *.mp3` will change all mp3 files in the working directory to have Metal for the genre tag. You can also use short options combined like `earmark -pa "Iron Maiden" "Aces High.flac"`. The -p means print out all the tag information after any file operations are done, and the -a means that earmark will change the artist value. Short options that require arguments cannot be combined, so if you're changing more than one value at a time, you have to separate them like so: `earmark -g Metal -a "Iron Maiden"`

# TODO
* Add logic to verify MIME type so files don't get corrupted if they have the wrong extensions
* Add cover stripping/extracting feature (and broader tag stripping features)
* Add filename/tag conversion features
* Come up with a more coherent scheme for printing information about what earmark is doing and verifying changes with the user before modifying files
* Add logging capability
* Add interactive mode to input different data for individual files
* Add config file defining standard operations used by individual users
