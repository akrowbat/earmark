#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <iostream>
#include <filesystem>
#include <vector>

#include <taglib/fileref.h>
#include <taglib/tag.h>

/* Flag set by ‘--verbose’. */
static int verbose_flag;
static int print_flag;

void print_help_info()
{
	std::cout << "Usage: earmark [OPTION]... [FILE]...\n";
	std::cout << "Modify or print metadata from audio files\n\n";
	std::cout << "	-a, --artist   set artist tag for input files\n";
	std::cout << "	-g, --genre    set genre tag for input files\n";
	std::cout << "	-l, --album    set album tag for input files\n";
	std::cout << "	-k, --track    set track tag for input files\n";
	std::cout << "	-t, --title    set title tag for input files\n";
	std::cout << "	    --help     display this help and exit\n";

	std::cout << "Examples:\n	earmark -pa \"Led Zeppelin\" \"Stairway to Heaven.mp3\"	Change the artist tag of the mp3 file to Led Zeppelin and print all tags\n";
	std::cout << "	earmark --genre Rock *.flac	Change the genre tag of all flac files in the directory to Rock\n";
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
	int c;
	std::vector<const char*> input_files;

	const char* album = nullptr;
	const char* artist = nullptr;
	const char* genre = nullptr;
	const char* title = nullptr;
	int track;
	int year;
	bool file_changed = false;

	while (1)
	{
		static struct option long_options[] =
		{
			/* These options set a flag. */
			{"verbose", no_argument,       &verbose_flag, 1},
			{"brief",   no_argument,       &verbose_flag, 0},
			{"print",   no_argument,       &print_flag, 1},
			/* These options don’t set a flag.
			We distinguish them by their indices. */
			{"album",   required_argument, 0, 'l'},
			{"artist",  required_argument, 0, 'a'},
			{"extract", required_argument, 0, 'x'},
			{"genre",   required_argument, 0, 'g'},
			{"help",    no_argument,       0, 'h'},
			{"strip",   no_argument,       0, 's'},
			{"title",   required_argument, 0, 't'},
			{"track",   required_argument, 0, 'k'},
			{0, 0, 0, 0}
		};
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long (argc, argv, "psa:l:g:hk:t:",
						long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
			break;

		switch (c)
		{
			case 0:
			/* If this option set a flag, do nothing else now. */
				if (long_options[option_index].flag != 0)
					break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
					printf ("\n");
					break;

			case 's':
				puts ("option -s");
				break;

			case 'a':
				artist = optarg;
				break;

			case 'l':
				album = optarg;
				break;

			case 'g':
				genre = optarg;
				break;

			case 'h':
				print_help_info();
				exit(0);

			case 'k':
				// track = optarg;
				break;

			case 'p':
				print_flag = 1;
				break;

			case 't':
				title = optarg;
				break;

			case 'x':
				break;

			case '?':
				/* getopt_long already printed an error message. */
				std::cout << "Test case\n";
				break;

			default:
				std::cout << "earmark: missing file operand\n";
				std::cout << "Try 'earmark --help' for more information.\n";
				exit(1);
		}
	}

	/* Instead of reporting ‘--verbose’
	and ‘--brief’ as they are encountered,
	we report the final status resulting from them. */
	if (verbose_flag)
		puts ("verbose flag is set");

	/* Check if all the files exist. */
	for (int i = optind; i < argc; ++i )
	{
		if (std::filesystem::exists(argv[i]) == 0)
		{
			std::cout << "File not found: " << argv[i] << "\n";
			exit(1);
		}
		input_files.push_back(argv[i]);
	}

	if (artist != NULL)
	{
		for (const auto &arg : input_files)
		{
			TagLib::FileRef current_file(arg);
			current_file.tag()->setArtist(artist);
			current_file.save();
		}
	}
	if (album != NULL)
	{
		for (const auto &arg : input_files)
		{
			TagLib::FileRef current_file(arg);
			current_file.tag()->setAlbum(album);
			current_file.save();
		}
	}
	if (genre != NULL)
	{
		for (const auto &arg : input_files)
		{
			TagLib::FileRef current_file(arg);
			current_file.tag()->setGenre(genre);
			current_file.save();
		}
	}
 	if (title != NULL)
	{
		for (const auto &arg : input_files)
		{
			TagLib::FileRef current_file(arg);
			current_file.tag()->setTitle(title);
			current_file.save();
		}
	}
	if (print_flag)
	{
		for (const auto &arg : input_files)
		{
			// std::cout << arg << "\n";
			print_tags(arg);
		}
	}

	exit(0);
}
