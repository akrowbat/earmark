#include <taglib/fileref.h>
#include <taglib/aifffile.h>
#include <taglib/apefile.h>
#include <taglib/asffile.h>
#include <taglib/dsdifffile.h>
#include <taglib/dsffile.h>
#include <taglib/flacfile.h>
#include <taglib/itfile.h>
#include <taglib/modfile.h>
#include <taglib/mp4file.h>
#include <taglib/mpcfile.h>
#include <taglib/mpegfile.h>
#include <taglib/oggfile.h>
#include <taglib/oggflacfile.h>
#include <taglib/opusfile.h>
#include <taglib/rifffile.h>
#include <taglib/s3mfile.h>
#include <taglib/shortenfile.h>
#include <taglib/speexfile.h>
#include <taglib/trueaudiofile.h>
#include <taglib/vorbisfile.h>
#include <taglib/wavfile.h>
#include <taglib/wavpackfile.h>
#include <taglib/xmfile.h>

#include <magic.h>

#include <string>
#include <string_view>
#include <unordered_map>
#include <iostream>
#include <fstream>


enum class MimeType
{
	AIFF,
	APE,
	ASF,
	DSDIFF,
	DSF,
	FLAC,
	IT,
	MOD,
	M4A,
	MP4,
	MP4_VIDEO,
	MPC,
	MPEG,
	OCTET,
	OGG,
	OGG_FLAC,
	OGG_OPUS,
	OGG_SPEEX,
	OGG_VORBIS,
	// RIFF,
	S3M,
	SHORTEN,
	TrueAudio,
	// WAV,
	WAVPACK,
	WMA,
	XM,
	UNKNOWN,
	// Ignore types
	Directory,
	Image,
};

MimeType mime_from_string(const std::string& mime) {
	static const std::unordered_map<std::string, MimeType> mime_map = {
		{"audio/aiff", MimeType::AIFF},
		{"audio/ape", MimeType::APE},
		{"video/x-ms-asf", MimeType::ASF},
		{"audio/x-dsd", MimeType::DSDIFF},
		{"audio/x-dsf", MimeType::DSF},
		{"audio/flac", MimeType::FLAC},
		{"audio/x-it", MimeType::IT},
		{"audio/x-mod", MimeType::MOD},
		{"audio/m4a", MimeType::M4A},
		{"audio/mp4", MimeType::MP4},
		{"video/mp4", MimeType::MP4_VIDEO},
		{"audio/x-musepack", MimeType::MPC},
		{"audio/mpeg", MimeType::MPEG},
		{"application/octet-stream", MimeType::OCTET},
		{"application/ogg", MimeType::OGG},
		{"audio/ogg", MimeType::OGG},
		{"audio/ogg; codecs=flac", MimeType::OGG_FLAC},
		{"audio/ogg; codecs=opus", MimeType::OGG_OPUS},
		{"audio/ogg; codecs=speex", MimeType::OGG_SPEEX},
		{"audio/ogg; codecs=vorbis", MimeType::OGG_VORBIS},
		// {"audio/x-riff", MimeType::RIFF},
		{"audio/x-s3m", MimeType::S3M},
		{"audio/x-shorten", MimeType::SHORTEN},
		/* Apparently application/octet-stream is just arbitrary binary data
			to be saved to disk. I need to add another function like the
			Ogg codec detection to figure out what these files are. */
		// {"application/octet-stream", MimeType::TTA},
		{"audio/x-tta", MimeType::TrueAudio},
		// {"audio/wav", MimeType::WAV},
		// {"audio/x-wav", MimeType::WAV},
		{"audio/x-wavpack", MimeType::WAVPACK},
		{"audio/x-ms-wma", MimeType::WMA},
		{"audio/x-xm", MimeType::XM},
		// Ignore-type section
		{"inode/directory", MimeType::Directory},
		{"image/avif", MimeType::Image},
		{"image/avif-sequence", MimeType::Image},
		{"image/bmp", MimeType::Image},
		{"image/cgm", MimeType::Image},
		{"image/g3fax", MimeType::Image},
		{"image/gif", MimeType::Image},
		{"image/heic", MimeType::Image},
		{"image/ief", MimeType::Image},
		{"image/jpeg", MimeType::Image},
		{"image/pjpeg", MimeType::Image},
		{"image/png", MimeType::Image},
		{"image/prs.btif", MimeType::Image},
		{"image/svg+xml", MimeType::Image},
		{"image/tiff", MimeType::Image},
		{"image/vnd.adobe.photoshop", MimeType::Image},
		{"image/vnd.djvu", MimeType::Image},
		{"image/vnd.dwg", MimeType::Image},
		{"image/vnd.dxf", MimeType::Image},
		{"image/vnd.fastbidsheet", MimeType::Image},
		{"image/vnd.fpx", MimeType::Image},
		{"image/vnd.fst", MimeType::Image},
		{"image/vnd.fujixerox.edmics-mmr", MimeType::Image},
		{"image/vnd.fujixerox.edmics-rlc", MimeType::Image},
		{"image/vnd.ms-modi", MimeType::Image},
		{"image/vnd.net-fpx", MimeType::Image},
		{"image/vnd.wap.wbmp", MimeType::Image},
		{"image/vnd.xiff", MimeType::Image},
		{"image/webp", MimeType::Image},
		{"image/x-adobe-dng", MimeType::Image},
		{"image/x-canon-cr2", MimeType::Image},
		{"image/x-canon-crw", MimeType::Image},
		{"image/x-cmu-raster", MimeType::Image},
		{"image/x-cmx", MimeType::Image},
		{"image/x-epson-erf", MimeType::Image},
		{"image/x-freehand", MimeType::Image},
		{"image/x-fuji-raf", MimeType::Image},
		{"image/x-icns", MimeType::Image},
		{"image/x-icon", MimeType::Image},
		{"image/x-kodak-dcr", MimeType::Image},
		{"image/x-kodak-k25", MimeType::Image},
		{"image/x-kodak-kdc", MimeType::Image},
		{"image/x-minolta-mrw", MimeType::Image},
		{"image/x-nikon-nef", MimeType::Image},
		{"image/x-olympus-orf", MimeType::Image},
		{"image/x-panasonic-raw", MimeType::Image},
		{"image/x-pcx", MimeType::Image},
		{"image/x-pentax-pef", MimeType::Image},
		{"image/x-pict", MimeType::Image},
		{"image/x-portable-anymap", MimeType::Image},
		{"image/x-portable-bitmap", MimeType::Image},
		{"image/x-portable-graymap", MimeType::Image},
		{"image/x-portable-pixmap", MimeType::Image},
		{"image/x-rgb", MimeType::Image},
		{"image/x-sigma-x3f", MimeType::Image},
		{"image/x-sony-arw", MimeType::Image},
		{"image/x-sony-sr2", MimeType::Image},
		{"image/x-sony-srf", MimeType::Image},
		{"image/x-xbitmap", MimeType::Image},
		{"image/x-xpixmap", MimeType::Image},
		{"image/x-xwindowdump", MimeType::Image},
	};

	auto mime_type = mime_map.find(mime);
	if (mime_type != mime_map.end())
		return mime_type->second;
	else
		return MimeType::UNKNOWN;
}

std::string detect_mime(const char* fileName)
{
	magic_t magic = magic_open(MAGIC_MIME_TYPE);
	if (!magic) return "";

	if (magic_load(magic, nullptr) != 0)
	{
		magic_close(magic);
		return "";
	}

	const char* mime = magic_file(magic, fileName);
	std::string result = mime ? mime : "";

	magic_close(magic);
	return result;
}

MimeType detect_ogg_codec(const char* fileName)
{
	std::ifstream input_file(fileName, std::ios::binary);
	if (!input_file) return MimeType::UNKNOWN;

	char header[4];
	input_file.read(header, sizeof(header));
	char buffer[54];
	input_file.read(buffer, sizeof(buffer));

	std::string sig(buffer, input_file.gcount());

	if (sig.find("vorbis") != std::string::npos) return MimeType::OGG_VORBIS;
	else if (sig.find("OpusHead") != std::string::npos) return MimeType::OGG_OPUS;
	else if (sig.find("fLaC") != std::string::npos) return MimeType::OGG_FLAC;
	else if (sig.find("Speex") != std::string::npos) return MimeType::OGG_SPEEX;
	else return MimeType::UNKNOWN;
}

MimeType detect_octet_codec(const char* fileName)
{
	std::ifstream input_file(fileName, std::ios::binary);
	if (!input_file) return MimeType::UNKNOWN;

	char header[4];
	input_file.read(header, sizeof(header));

	std::string sig(header, input_file.gcount());
	if (sig.find("wvpk") != std::string::npos) return MimeType::WAVPACK;
	else if (sig.find("TTA1") != std::string::npos) return MimeType::TrueAudio;
	/*
		Extra processing is needed to detect TTA files with metadata.
		This is a more gnarly problem to solve because it seems like
		the ID3 metadata block can push back the actual TTA1 signature
		back to varying degrees depending on how much metadata is written.
	*/
	else return MimeType::UNKNOWN;
}

class MimeResolver : public TagLib::FileRef::FileTypeResolver
{
	public:
		TagLib::File* createFile(const char* fileName,
									 bool readAudioProperties,
									 TagLib::AudioProperties::ReadStyle style) const {
		 std::string mime = detect_mime(fileName);
		switch (mime_from_string(mime))
		{
			case MimeType::APE:
				return new TagLib::APE::File(fileName, readAudioProperties, style);
			case MimeType::ASF:
				return new TagLib::ASF::File(fileName, readAudioProperties, style);
			case MimeType::DSDIFF:
				return new TagLib::DSDIFF::File(fileName, readAudioProperties, style);
			case MimeType::DSF:
				return new TagLib::DSF::File(fileName, readAudioProperties, style);
			case MimeType::FLAC:
				return new TagLib::FLAC::File(fileName, readAudioProperties, style);
			case MimeType::IT:
				return new TagLib::IT::File(fileName, readAudioProperties, style);
			case MimeType::MOD:
				return new TagLib::Mod::File(fileName, readAudioProperties, style);
			case MimeType::M4A:
				return new TagLib::MP4::File(fileName, readAudioProperties, style);
			case MimeType::MP4:
				return new TagLib::MP4::File(fileName, readAudioProperties, style);
			case MimeType::MP4_VIDEO:
				return new TagLib::MP4::File(fileName, readAudioProperties, style);
			case MimeType::MPC:
				return new TagLib::MPC::File(fileName, readAudioProperties, style);
			case MimeType::MPEG:
				return new TagLib::MPEG::File(fileName, readAudioProperties, style);
			case MimeType::OCTET:
				switch (detect_octet_codec(fileName))
				{
					case MimeType::WAVPACK:
						return new TagLib::WavPack::File(fileName, readAudioProperties, style);
					case MimeType::TrueAudio:
						return new TagLib::TrueAudio::File(fileName, readAudioProperties, style);
					default:
						std::cerr << "ERROR: Unknown MIME type: " << mime << std::endl;
						std::cerr << "File: " << fileName << std::endl;
						exit(1);
				}
			case MimeType::OGG:
				switch (detect_ogg_codec(fileName))
				{
					case MimeType::OGG_FLAC:
						return new TagLib::Ogg::FLAC::File(fileName, readAudioProperties, style);
					case MimeType::OGG_OPUS:
						return new TagLib::Ogg::Opus::File(fileName, readAudioProperties, style);
					case MimeType::OGG_SPEEX:
						return new TagLib::Ogg::Speex::File(fileName, readAudioProperties, style);
					case MimeType::OGG_VORBIS:
						return new TagLib::Vorbis::File(fileName, readAudioProperties, style);
					default:
						std::cerr << "ERROR: Unknown MIME type: " << mime << std::endl;
						std::cerr << "File: " << fileName << std::endl;
						exit(1);
				}
			case MimeType::OGG_FLAC:
				return new TagLib::Ogg::FLAC::File(fileName, readAudioProperties, style);
			case MimeType::OGG_OPUS:
				return new TagLib::Ogg::Opus::File(fileName, readAudioProperties, style);
			case MimeType::OGG_SPEEX:
				return new TagLib::Ogg::Speex::File(fileName, readAudioProperties, style);
			case MimeType::OGG_VORBIS:
				return new TagLib::Vorbis::File(fileName, readAudioProperties, style);
			case MimeType::S3M:
				return new TagLib::S3M::File(fileName, readAudioProperties, style);
			case MimeType::SHORTEN:
				return new TagLib::Shorten::File(fileName, readAudioProperties, style);
			case MimeType::TrueAudio:
				return new TagLib::TrueAudio::File(fileName, readAudioProperties, style);
			case MimeType::WAVPACK:
				return new TagLib::WavPack::File(fileName, readAudioProperties, style);
			case MimeType::WMA:
				return new TagLib::ASF::File(fileName, readAudioProperties, style);
			case MimeType::XM:
				return new TagLib::XM::File(fileName, readAudioProperties, style);
			case MimeType::Directory:
				return nullptr;
			case MimeType::Image:
				return nullptr;
			default:
				std::cerr << "ERROR: Unkown MIME type: " << mime << std::endl;
				std::cerr << "File: " << fileName << std::endl;
				exit(1);
		}
	}
};
