#include <iostream>
#include <filesystem>
#include <vector>

#include <cxxopts.hpp>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

#include "modules/filetype.cpp"

using namespace std;

static bool dry_run_flag;
static bool print_flag;
static bool verbose_flag;

void print_help_info()
{
	cout << "Usage: earmark [OPTION]... [FILE]..." << endl;
	cout << "Modify or print metadata from audio files" << endl << endl;
	cout << "	-a, --artist   set artist tag for input files" << endl;
	cout << "	-g, --genre    set genre tag for input files" << endl;
	cout << "	-l, --album    set album tag for input files" << endl;
	cout << "	-k, --track    set track tag for input files" << endl;
	cout << "	-t, --title    set title tag for input files" << endl;
	cout << "	-h  --help     display this help and exit" << endl;

	cout << endl << "Examples:" << endl;
	cout << "	earmark -pa \"Led Zeppelin\" \"Stairway to Heaven.mp3\"" << endl;
	cout << "	earmark --genre Rock *.flac" << endl;
}

void print_tags(const char* filename)
{
	TagLib::FileRef input_file(filename);
	TagLib::PropertyMap properties = input_file.properties();
	cout << endl << "File: " << filename << endl;
	for (const auto& property : properties)
		cout << property.first << ": " << properties.value(property.first) << endl;
}

int main(int argc, char **argv)
{
	TagLib::FileRef::addFileTypeResolver(new MimeResolver());

	cxxopts::Options options("earmark", "Edit audio metadata");

	options.add_options()
		("a,artist",    "Param artist", cxxopts::value<string>())
		("dry-run",     "Simulate changes")
		("g,genre",     "Param genre", cxxopts::value<string>())
		("h,help",      "Print usage")
		("k,track",     "Param track number", cxxopts::value<int>())
		("l,album",     "Param album", cxxopts::value<string>())
		("input_files", "Input files", cxxopts::value<vector<string>>())
		("p,print",     "Print tags", cxxopts::value<bool>())
		("t,title",     "Param title", cxxopts::value<string>())
		("v,verbose",   "More output")
		("y,year",      "Param year", cxxopts::value<int>())
	;

	options.parse_positional({"input_files"});
	auto result = options.parse(argc, argv);
	
	if (result.count("help"))
	{
		// options.help(); // This is a built-in cxxopts thing. Not sure how it works.
		print_help_info();
		exit(0);
	}

	vector<string> input_files;
	if (result.count("input_files"))
		input_files = result["input_files"].as<vector<string>>();
	if (input_files.empty())
	{
		cerr << "earmark: missing file operand" << endl;
		cerr << "Try 'earmark --help' for more information." << endl;
		exit(1);
	}

	if (result.count("print"))
		print_flag = true;
	if (result.count("verbose"))
		verbose_flag = true;

	string album;
	string artist;
	string genre;
	string title;
	int track;
	bool track_set;
	int year;
	bool year_set;

	if (result.count("artist"))
	{
		artist = result["artist"].as<string>();
		cout << "Artist value: " << artist << endl;
	}
	
	if (result.count("album"))
	{
		album = result["album"].as<string>();
		cout << "Album value: " << album << endl;
	}
	
	if (result.count("genre"))
	{
		genre = result["genre"].as<string>();
		cout << "Genre value: " << genre << endl;
	}
	
	if (result.count("title"))
	{
		title = result["title"].as<string>();
		cout << "Title value: " << title << endl;
	}
	
	if (result.count("track"))
	{
		track_set = true;
		track = result["track"].as<int>();
		cout << "Track value: " << track << endl;
	}
	
	if (result.count("year"))
	{
		year_set = true;
		year = result["year"].as<int>();
		cout << "Year value: " << year << endl;
	}
	
	/* Check if all the files exist. */
	for (const auto& file : input_files)
	{
		if (!filesystem::exists(file.c_str()))
		{
			cerr << "File not found: " << file << endl;
			exit(1);
		}
	}

	for (const auto &file: input_files)
	{
		TagLib::FileRef current_file(file.c_str());
		if (!artist.empty())
			current_file.tag()->setArtist(artist);
		if (!album.empty())
			current_file.tag()->setAlbum(album);
		if (!genre.empty())
			current_file.tag()->setGenre(genre);
		if (!title.empty())
			current_file.tag()->setTitle(title);
		if (track_set)
			current_file.tag()->setTrack(track);
		if (year_set)
			current_file.tag()->setYear(year);
		if (!dry_run_flag)
		{
			if (!current_file.save())
				cerr << "Error: Failed to save metadata for: " << file << endl;
		}
	}

	if (print_flag)
		for (const auto &file : input_files)
			print_tags(file.c_str());
	
	exit(0);
}
