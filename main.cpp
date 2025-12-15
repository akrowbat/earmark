#include <iostream>
#include <filesystem>
#include <vector>

#include <cxxopts.hpp>

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/tpropertymap.h>

#include "modules/filetype.cpp"

static bool dry_run_flag = false;
static bool print_flag = false;
static bool verbose_flag = false;

void print_help_info()
{
	std::cout << "Usage: earmark [OPTION]... [FILE]..." << "\n";
	std::cout << "Modify or print metadata from audio files" << "\n" << "\n";
	std::cout << "	-a, --artist   set artist tag for input files" << "\n";
	std::cout << "	-g, --genre    set genre tag for input files" << "\n";
	std::cout << "	-l, --album    set album tag for input files" << "\n";
	std::cout << "	-k, --track    set track tag for input files" << "\n";
	std::cout << "	-t, --title    set title tag for input files" << "\n";
	std::cout << "	--autonumber   autonumber tracks from specified number" << "\n";
	std::cout << "	--strip        remove all tag data before processing" << "\n";
	std::cout << "	-p, --print    print out audio metadata after processing" << "\n";
	std::cout << "	-h  --help     display this help and exit" << "\n";

	std::cout << "\n" << "Examples:" << "\n";
	std::cout << "	earmark -pa \"Led Zeppelin\" \"Stairway to Heaven.mp3\"" << "\n";
	std::cout << "	earmark --genre Rock *.flac" << "\n";
}

void print_tags(const char* filename)
{
	TagLib::FileRef input_file(filename);
	TagLib::PropertyMap properties = input_file.properties();
	std::cout << "\n" << "File: " << filename << "\n";
	for (const auto& property : properties)
		std::cout << property.first << ": " << properties.value(property.first) << "\n";
}

int main(int argc, char **argv)
{

	struct {
		std::string album;
		std::string artist;
		std::string genre;
		std::string title;
		int track;
		int year;
		int autonumber;
		bool album_set = false;
		bool artist_set = false;
		bool genre_set = false;
		bool title_set = false;
		bool track_set = false;
		bool year_set = false;
		bool autonumber_set = false;
	} terminal_args;

	TagLib::FileRef::addFileTypeResolver(new MimeResolver());

	cxxopts::Options options("earmark", "Edit audio metadata");

	options.add_options()
		// Tag section
		("a,artist",    "Param artist", cxxopts::value<std::string>())
		("g,genre",     "Param genre", cxxopts::value<std::string>())
		("k,track",     "Param track number", cxxopts::value<int>())
		("l,album",     "Param album", cxxopts::value<std::string>())
		("t,title",     "Param title", cxxopts::value<std::string>())
		("y,year",      "Param year", cxxopts::value<int>())
		("autonumber",  "Number all tracks from starting value", cxxopts::value<int>())
		/* I'm not sure how I'm going to set up a property map argument
			that can separate well from the input_files vector */
		// ("m,property",  "Param property map", cxxopts::value<std::vector<std::string>>())
		// Flag section
		("strip",       "Remove all metadata", cxxopts::value<bool>())
		("p,print",     "Print tags", cxxopts::value<bool>())
		("dry-run",     "Simulate changes")
		("v,verbose",   "More output")
		("h,help",      "Print usage")
		//
		("input_files", "Input files", cxxopts::value<std::vector<std::string>>())
	;

	options.parse_positional({"input_files"});
	auto result = options.parse(argc, argv);
	
	if (result.count("help"))
	{
		// options.help(); // This is a built-in cxxopts thing. Not sure how it works.
		print_help_info();
		exit(0);
	}

	std::vector<std::string> input_files;
	if (result.count("input_files"))
		input_files = result["input_files"].as<std::vector<std::string>>();
	if (input_files.empty())
	{
		std::cerr << "earmark: missing file operand" << "\n";
		std::cerr << "Try 'earmark --help' for more information." << "\n";
		exit(1);
	}

	if (result.count("dry-run"))
		dry_run_flag = true;
	if (result.count("print"))
		print_flag = true;
	if (result.count("verbose"))
		verbose_flag = true;

	if (result.count("artist"))
	{
		terminal_args.artist_set = true;
		terminal_args.artist = result["artist"].as<std::string>();
	}
	
	if (result.count("album"))
	{
		terminal_args.album_set = true;
		terminal_args.album = result["album"].as<std::string>();
	}
	
	if (result.count("genre"))
	{
		terminal_args.genre_set = true;
		terminal_args.genre = result["genre"].as<std::string>();
	}
	
	if (result.count("title"))
	{
		terminal_args.title_set = true;
		terminal_args.title = result["title"].as<std::string>();
	}
	
	if (result.count("track"))
	{
		terminal_args.track_set = true;
		terminal_args.track = result["track"].as<int>();
	}
	
	if (result.count("year"))
	{
		terminal_args.year_set = true;
		terminal_args.year = result["year"].as<int>();
	}
	
	if (result.count("autonumber"))
	{
		if (terminal_args.track_set)
		{
			std::cerr << "ERROR: Track and autonumber cannot both be used at the same time." << "\n";
			exit(1);
		}
		terminal_args.autonumber_set = true;
		terminal_args.autonumber = result["autonumber"].as<int>();
	}
	
	// Check if all the files exist.
	bool file_error = false;
	for (const auto& file : input_files)
	{
		if (!std::filesystem::exists(file.c_str()))
		{
			std::cerr << "File not found: " << file << "\n";
			file_error = true;
		}
	}
	if (file_error) exit(1);

	// For loop that verifies MIME types before operations
	std::vector<TagLib::FileRef> verified_files;
	for (const auto &file : input_files)
		verified_files.push_back(TagLib::FileRef(file.c_str()));

	for (auto verified_file : verified_files)
	{
		bool modified = false;
		if (terminal_args.artist_set)
		{
			verified_file.tag()->setArtist(terminal_args.artist);
			modified = true;
		}
		if (terminal_args.album_set)
		{
			verified_file.tag()->setAlbum(terminal_args.album);
			modified = true;
		}
		if (terminal_args.genre_set)
		{
			verified_file.tag()->setGenre(terminal_args.genre);
			modified = true;
		}
		if (terminal_args.title_set)
		{
			verified_file.tag()->setTitle(terminal_args.title);
			modified = true;
		}
		if (terminal_args.track_set)
		{
			verified_file.tag()->setTrack(terminal_args.track);
			modified = true;
		}
		if (terminal_args.year_set)
		{	
			verified_file.tag()->setYear(terminal_args.year);
			modified = true;
		}
		if (terminal_args.autonumber_set)
		{
			verified_file.tag()->setTrack(terminal_args.autonumber);
			modified = true;
			terminal_args.autonumber++;
		}
		if (!dry_run_flag && modified)
		{
			if (!verified_file.save())
				std::cerr << "ERROR: Failed to save metadata for: " << verified_file.file()->name() << "\n";
		}
	}

	if (print_flag)
		for (const auto &file : input_files)
			print_tags(file.c_str());
	
	exit(0);
}
