#include <iostream>
#include <filesystem>
#include <vector>

#include <cxxopts.hpp>

#include <taglib/fileref.h>
#include <taglib/tag.h>

using namespace std;

static bool verbose_flag;
static bool print_flag;

void print_help_info()
{
	cout << "Usage: earmark [OPTION]... [FILE]...\n";
	cout << "Modify or print metadata from audio files\n\n";
	cout << "	-a, --artist   set artist tag for input files\n";
	cout << "	-g, --genre    set genre tag for input files\n";
	cout << "	-l, --album    set album tag for input files\n";
	cout << "	-k, --track    set track tag for input files\n";
	cout << "	-t, --title    set title tag for input files\n";
	cout << "	-h  --help     display this help and exit\n";

	cout << "Examples:\n";
	cout << "	earmark -pa \"Led Zeppelin\" \"Stairway to Heaven.mp3\"\n";
	cout << "	earmark --genre Rock *.flac\n";
}

void print_tags(const char* filename)
{
	TagLib::FileRef input_file(filename);
	cout << endl << "File: " << filename << "\n";
	cout << "Album: " << input_file.tag()->album() << "\n";
	cout << "Artist: " << input_file.tag()->artist() << "\n";
	cout << "Genre: " << input_file.tag()->genre() << "\n";
	cout << "Title: " << input_file.tag()->title() << "\n";
	cout << "Track: " << input_file.tag()->track() << "\n";
	cout << "Year: " << input_file.tag()->year() << "\n";
}

int main(int argc, char **argv)
{
	cxxopts::Options options("earmark", "Edit audio metadata");

	options.add_options()
		("a,artist",    "Param artist", cxxopts::value<string>())
		("l,album",     "Param album", cxxopts::value<string>())
		("g,genre",     "Param genre", cxxopts::value<string>())
		("t,title",     "Param title", cxxopts::value<string>())
		("k,track",     "Param track number", cxxopts::value<int>())
		("y,year",      "Param year", cxxopts::value<int>())
		("h,help",      "Print usage")
		("p,print",     "Print tags", cxxopts::value<bool>())
		("input_files", "Input files", cxxopts::value<vector<string>>())
	;

	options.parse_positional({"input_files"});
	auto result = options.parse(argc, argv);

	vector<string> input_files;
	if (result.count("input_files"))
		input_files = result["input_files"].as<vector<string>>();
	if (input_files.empty())
	{
		cerr << "earmark: missing file operand" << endl;
		cerr << "Try 'earmark --help' for more information." << endl;
		exit(1);
	}

	if (result.count("help"))
	{
		print_help_info();
		exit(0);
	}

	if (result.count("print"))
		print_flag = true;

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
		current_file.save();
	}

	if (print_flag)
	{
		for (const auto &file : input_files)
		{
			print_tags(file.c_str());
		}
	}
	
	exit(0);
}
