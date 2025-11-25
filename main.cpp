#include <iostream>
#include <filesystem>
#include <vector>

#include <cxxopts.hpp>

#include <taglib/fileref.h>
#include <taglib/tag.h>

/* Flag set by ‘--verbose’. */
static bool verbose_flag;
static bool print_flag;

void print_help_info()
{
	std::cout << "Usage: earmark [OPTION]... [FILE]...\n";
	std::cout << "Modify or print metadata from audio files\n\n";
	std::cout << "	-a, --artist   set artist tag for input files\n";
	std::cout << "	-g, --genre    set genre tag for input files\n";
	std::cout << "	-l, --album    set album tag for input files\n";
	std::cout << "	-k, --track    set track tag for input files\n";
	std::cout << "	-t, --title    set title tag for input files\n";
	std::cout << "	-h  --help     display this help and exit\n";

	std::cout << "Examples:\n";
	std::cout << "	earmark -pa \"Led Zeppelin\" \"Stairway to Heaven.mp3\"\n";
	std::cout << "	earmark --genre Rock *.flac\n";
}

void print_tags(const char* filename)
{
	TagLib::FileRef input_file(filename);
	std::cout << "File: " << filename << "\n";
	std::cout << "Album: " << input_file.tag()->album() << "\n";
	std::cout << "Artist: " << input_file.tag()->artist() << "\n";
	std::cout << "Genre: " << input_file.tag()->genre() << "\n";
	std::cout << "Title: " << input_file.tag()->title() << "\n";
	std::cout << "Track: " << input_file.tag()->track() << "\n";
	std::cout << "Year: " << input_file.tag()->year() << "\n";
}

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cerr << "earmark: missing file operand" << std::endl;
		std::cerr << "Try 'earmark --help' for more information." << std::endl;
		exit(1);
	}
	cxxopts::Options options("earmark", "Edit audio metadata");

	options.add_options()
		("a,artist",    "Param artist", cxxopts::value<std::string>())
		("l,album",     "Param album", cxxopts::value<std::string>())
		("g,genre",     "Param genre", cxxopts::value<std::string>())
		("t,title",     "Param title", cxxopts::value<std::string>())
		("k,track",     "Param track number", cxxopts::value<int>())
		("y,year",      "Param year", cxxopts::value<int>())
		("h,help",      "Print usage")
		("p,print",     "Print tags", cxxopts::value<bool>())
		("input_files", "Input files", cxxopts::value<std::vector<std::string>>())
	;

	options.parse_positional({"input_files"});
	auto result = options.parse(argc, argv);

	std::vector<std::string> input_files;
	if (result.count("input_files"))
		input_files = result["input_files"].as<std::vector<std::string>>();

	if (result.count("help"))
	{
		print_help_info();
		exit(0);
	}

	if (result.count("print"))
		print_flag = true;

	std::string album;
	std::string artist;
	std::string genre;
	std::string title;
	int track;
	bool track_set;
	int year;
	bool year_set;

	if (result.count("artist"))
	{
		artist = result["artist"].as<std::string>();
		std::cout << "Artist value: " << artist << std::endl;
	}
	
	if (result.count("album"))
	{
		album = result["album"].as<std::string>();
		std::cout << "Album value: " << album << std::endl;
	}
	
	if (result.count("genre"))
	{
		genre = result["genre"].as<std::string>();
		std::cout << "Genre value: " << genre << std::endl;
	}
	
	if (result.count("title"))
	{
		title = result["title"].as<std::string>();
		std::cout << "Title value: " << title << std::endl;
	}
	
	if (result.count("track"))
	{
		track_set = true;
		track = result["track"].as<int>();
		std::cout << "Track value: " << track << std::endl;
	}
	
	if (result.count("year"))
	{
		year_set = true;
		year = result["year"].as<int>();
		std::cout << "Year value: " << year << std::endl;
	}
	
	/* Check if all the files exist. */
	for (const auto& file : input_files)
	{
		if (std::filesystem::exists(file.c_str()) == 0)
		{
			std::cout << "File not found: " << file << "\n";
			exit(1);
		}
	}

	if (!artist.empty())
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setArtist(artist);
			current_file.save();
		}
	}
	if (!album.empty())
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setAlbum(album);
			current_file.save();
		}
	}
	if (!genre.empty())
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setGenre(genre);
			current_file.save();
		}
	}
 	if (!title.empty())
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setTitle(title);
			current_file.save();
		}
	}

	if (track_set)
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setTrack(track);
			current_file.save();
		}
	}

	if (year_set)
	{
		for (const auto &file : input_files)
		{
			TagLib::FileRef current_file(file.c_str());
			current_file.tag()->setYear(year);
			current_file.save();
		}
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
