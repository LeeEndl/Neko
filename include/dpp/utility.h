/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#pragma once
#include <dpp/snowflake.h>
#include <dpp/misc-enum.h>
#include <random>
#include <thread>

#ifndef MAX_CND_IMAGE_SIZE
#define MAX_CDN_IMAGE_SIZE 4096
#endif
#ifndef MIN_CDN_IMAGE_SIZE
#define MIN_CDN_IMAGE_SIZE 16
#endif

 /**
  * @brief The main namespace for D++ functions, classes and types
  */
namespace dpp {
	enum sticker_format : uint8_t;

	/**
	 * @brief Utility helper functions, generally for logging, running programs, time/date manipulation, etc
	 */
	namespace utility {
		/**
		 * For internal use only. Helper function to easily create discord's cdn endpoint urls
		 * @see https://discord.com/developers/docs/reference#image-formatting-cdn-endpoints
		 * @param allowed_formats A vector of supported formats for the endpoint from the discord docs
		 * @param path_without_extension The path for the endpoint (without the extension e.g. `.png`)
		 * @param format the wished format to return. Must be one of the formats passed in `allowed_formats`, otherwise it returns an empty string
		 * @param size the image size which will be appended as a querystring to the url.
		 * It must be any power of two between 16 and 4096, otherwise no querystring will be appended (discord then returns the image as their default size)
		 * @param prefer_animated Whether the user prefers gif format. If true, it'll return gif format whenever the emoji is available as animated.
		 * In this case, the `format`-parameter is only used for non-animated images.
		 * @param is_animated Whether the image is actually animated or not
		 * @return std::string endpoint url or empty string
		 */
		std::string cdn_endpoint_url(const std::vector<image_type>& allowed_formats, const std::string& path_without_extension, const dpp::image_type format, uint16_t size, bool prefer_animated = false, bool is_animated = false);

		/**
		 * For internal use only. Helper function to easily create discord's cdn endpoint urls
		 * @see https://discord.com/developers/docs/reference#image-formatting-cdn-endpoints
		 * @param allowed_formats A vector of supported formats for the endpoint from the discord docs
		 * @param path_without_extension The path for the endpoint (without the extension e.g. `.png`)
		 * @param hash The hash (optional). If passed, it will be prefixed with `a_` for animated images (`is_animated`=true)
		 * @param format the wished format to return. Must be one of the formats passed in `allowed_formats`, otherwise it returns an empty string
		 * @param size the image size which will be appended as a querystring to the url.
		 * It must be any power of two between 16 and 4096, otherwise no querystring will be appended (discord then returns the image as their default size)
		 * @param prefer_animated Whether the user prefers gif format. If true, it'll return gif format whenever the emoji is available as animated.
		 * In this case, the `format`-parameter is only used for non-animated images.
		 * @param is_animated Whether the image is actually animated or not
		 * @return std::string endpoint url or empty string
		 */
		std::string cdn_endpoint_url_hash(const std::vector<image_type>& allowed_formats, const std::string& path_without_extension, const std::string& hash, const dpp::image_type format, uint16_t size, bool prefer_animated = false, bool is_animated = false);

		/**
		 * For internal use only. Helper function to easily create discord's cdn endpoint urls (specialised for stickers)
		 * @see https://discord.com/developers/docs/reference#image-formatting-cdn-endpoints
		 * @param sticker_id The sticker ID. Returns empty string if 0
		 * @param format The format type
		 * @return std::string endpoint url or empty string
		 */
		std::string cdn_endpoint_url_sticker(snowflake sticker_id, sticker_format format);

		/**
		 * @brief Timestamp formats for dpp::utility::timestamp()
		 *
		 * @note These values are the actual character values specified by the Discord API
		 * and should not be changed unless the Discord API changes the specification!
		 * They have been sorted into numerical order of their ASCII value to keep C++ happy.
		 */
		enum time_format : uint8_t {
			/// "20 April 2021" - Long Date
			tf_long_date = 'D',
			/// "Tuesday, 20 April 2021 16:20" - Long Date/Time
			tf_long_datetime = 'F',
			/// "2 months ago" - Relative Time
			tf_relative_time = 'R',
			/// "16:20:30" - Long Time
			tf_long_time = 'T',
			/// "20/04/2021" - Short Date
			tf_short_date = 'd',
			/// "20 April 2021 16:20" - Short Date/Time
			tf_short_datetime = 'f',
			/// "16:20" - Short Time
			tf_short_time = 't',
		};

		/**
		 * @brief The base URL for CDN content such as profile pictures and guild icons.
		 */
		const std::string cdn_host = "https://cdn.discordapp.com";

		/**
		 * @brief Callback for the results of a command executed via dpp::utility::exec
		 */
		typedef std::function<void(const std::string& output)> cmd_result_t;

		/**
		 * @brief Run a commandline program asynchronously. The command line program
		 * is spawned in a separate std::thread, and when complete, its output from
		 * stdout is passed to the callback function in its string parameter. For example
		 * ```
		 * dpp::utility::exec("/bin/ls", {"-al"}, [](const std::string& output) {
		 *     std::cout << "Output of 'ls -al': " << output << "\n";
		 * });
		 * ```
		 *
		 * @param cmd The command to run.
		 * @param parameters Command line parameters. Each will be escaped using `std::quoted`.
		 * @param callback The callback to call on completion.
		 */
		void DPP_EXPORT exec(const std::string& cmd, std::vector<std::string> parameters = {}, cmd_result_t callback = {});

		/* time manipulation function*/
		inline tm* mtm(time_t t)
		{
			long long _time = t;
			tm* types = localtime(&_time);
			return types;
		}
		/* time manipulation function*/
		time_t mt_t(tm* old_time, int second, int minute, int hour, int wday, int mday, int month_num) {
			old_time->tm_sec = second;
			old_time->tm_min = minute;
			old_time->tm_hour = hour;
			old_time->tm_wday = wday;
			old_time->tm_mday = mday;
			old_time->tm_mon = month_num;
			return mktime(old_time);
		}

		/**
		 * @brief Return a mentionable timestamp (used in a message). These timestamps will display the given timestamp in the user's timezone and locale.
		 *
		 * @param ts Time stamp to convert
		 * @param tf Format of timestamp using dpp::utility::time_format
		 * @return std::string The formatted timestamp
		 */
		std::string DPP_EXPORT timestamp(time_t ts, time_format tf = tf_short_datetime);

		/**
		 * @brief Returns current date and time
		 *
		 * @return std::string Current date and time in "Y-m-d H:M:S" format
		 */
		std::string DPP_EXPORT current_date_time();
		/**
		 * @brief Convert a dpp::loglevel enum value to a string
		 *
		 * @param in log level to convert
		 * @return std::string string form of log level
		 */
		std::string DPP_EXPORT loglevel(dpp::loglevel in);

		/**
		 * @brief Store a 128 bit icon hash (profile picture, server icon etc)
		 * as a 128 bit binary value made of two uint64_t.
		 * Has a constructor to build one from a string, and a method to fetch
		 * the value back in string form.
		 */
		struct DPP_EXPORT iconhash {
			uint64_t first;		//!< High 64 bits
			uint64_t second;	//!< Low 64 bits

			/**
			 * @brief Construct a new iconhash object
			 * @param _first Leftmost portion of the hash value
			 * @param _second Rightmost portion of the hash value
			 */
			iconhash(uint64_t _first = 0, uint64_t _second = 0);

			/**
			 * @brief Construct a new iconhash object
			 */
			iconhash(const iconhash&);

			/**
			 * @brief Destroy the iconhash object
			 */
			~iconhash();

			/**
			 * @brief Construct a new iconhash object
			 *
			 * @param hash String hash to construct from.
			 * Must contain a 32 character hex string.
			 *
			 * @throws std::length_error if the provided
			 * string is not exactly 32 characters long.
			 */
			iconhash(const std::string& hash);

			/**
			 * @brief Assign from std::string
			 *
			 * @param assignment string to assign from.
			 *
			 * @throws std::length_error if the provided
			 * string is not exactly 32 characters long.
			 */
			iconhash& operator=(const std::string& assignment);

			/**
			 * @brief Check if one iconhash is equal to another
			 *
			 * @param other other iconhash to compare
			 * @return True if the iconhash objects match
			 */
			bool operator==(const iconhash& other) const;

			/**
			 * @brief Change value of iconhash object
			 *
			 * @param hash String hash to change to.
			 * Must contain a 32 character hex string.
			 *
			 * @throws std::length_error if the provided
			 * string is not exactly 32 characters long.
			 */
			void set(const std::string& hash);

			/**
			 * @brief Convert iconhash back to 32 character
			 * string value.
			 *
			 * @return std::string Hash value
			 */
			std::string to_string() const;
		};

		/**
		 * @brief Return the current time with fractions of seconds.
		 * This is a unix epoch time with the fractional seconds part
		 * after the decimal place.
		 *
		 * @return double time with fractional seconds
		 */
		double DPP_EXPORT time_f();

		/**
		 * @brief Returns true if D++ was built with voice support
		 *
		 * @return bool True if voice support is compiled in (libsodium/libopus)
		 */
		bool DPP_EXPORT has_voice();

		/**
		 * @brief Convert a byte count to display value
		 *
		 * @param c number of bytes
		 * @return std::string display value suffixed with M, G, T where necessary
		 */
		std::string DPP_EXPORT bytes(uint64_t c);

		/**
		 * @brief A class used to represent an uptime in hours, minutes,
		 * seconds and days, with helper functions to convert from time_t
		 * and display as a string.
		 */
		struct DPP_EXPORT uptime {
			uint16_t days;	//!< Number of days
			uint8_t hours;	//!< Number of hours
			uint8_t mins;	//!< Number of minutes
			uint8_t secs;	//!< Number of seconds

			/**
			 * @brief Construct a new uptime object
			 */
			uptime();

			/**
			 * @brief Construct a new uptime object
			 *
			 * @param diff A time_t to initialise the object from
			 */
			uptime(time_t diff);

			/**
			 * @brief Construct a new uptime object
			 *
			 * @param diff A time_t to initialise the object from
			 */
			uptime(double diff);

			/**
			 * @brief Get uptime as string
			 *
			 * @return std::string Uptime as string
			 */
			std::string to_string() const;

			/**
			 * @brief Get uptime as seconds
			 *
			 * @return uint64_t Uptime as seconds
			 */
			uint64_t to_secs() const;

			/**
			 * @brief Get uptime as milliseconds
			 *
			 * @return uint64_t Uptime as milliseconds
			 */
			uint64_t to_msecs() const;
		};

		/**
		 * @brief Convert doubles to RGB for sending in embeds
		 *
		 * @param red red value, between 0 and 1 inclusive
		 * @param green green value, between 0 and 1 inclusive
		 * @param blue blue value, between 0 and 1 inclusive
		 * @return uint32_t returned integer colour value
		 */
		uint32_t DPP_EXPORT rgb(double red, double green, double blue);

		/**
		 * @brief Convert ints to RGB for sending in embeds
		 *
		 * @param red red value, between 0 and 255 inclusive
		 * @param green green value, between 0 and 255 inclusive
		 * @param blue blue value, between 0 and 255 inclusive
		 * @return uint32_t returned integer colour value
		 */
		uint32_t DPP_EXPORT rgb(int red, int green, int blue);

		/**
	 * @brief Convert doubles to CMYK for sending in embeds
	 *
	 * @param c cyan value, between 0 and 1 inclusive
	 * @param m magenta value, between 0 and 1 inclusive
	 * @param y yellow value, between 0 and 1 inclusive
	 * @param k key (black) value, between 0 and 1 inclusive
	 * @return uint32_t returned integer colour value
	 */
		uint32_t DPP_EXPORT cmyk(double c, double m, double y, double k);

		/**
		 * @brief Convert ints to CMYK for sending in embeds
		 *
		 * @param c cyan value, between 0 and 255 inclusive
		 * @param m magenta value, between 0 and 255 inclusive
		 * @param y yellow value, between 0 and 255 inclusive
		 * @param k key (black) value, between 0 and 255 inclusive
		 * @return uint32_t returned integer colour value
		 */
		uint32_t DPP_EXPORT cmyk(int c, int m, int y, int k);

		/**
		 * @brief Output hex values of a section of memory for debugging
		 *
		 * @param data The start of the data to display
		 * @param length The length of data to display
		 */
		std::string DPP_EXPORT debug_dump(uint8_t* data, size_t length);

		/**
		 * @brief Returns the length of a UTF-8 string in codepoints
		 *
		 * @param str string to count length of
		 * @return size_t length of string (0 for invalid utf8)
		 */
		size_t DPP_EXPORT utf8len(const std::string& str);

		/**
		 * @brief Return substring of a UTF-8 encoded string in codepoints
		 *
		 * @param str string to return substring from
		 * @param start start codepoint offset
		 * @param length length in codepoints
		 * @return std::string Substring in UTF-8 or empty string if invalid UTF-8 passed in
		 */
		std::string DPP_EXPORT utf8substr(const std::string& str, std::string::size_type start, std::string::size_type length);

		/**
		 * @brief Read a whole file into a std::string.
		 * Be sure you have enough memory to read the file, if you are reading a large file.
		 * @note Be aware this function can block! If you are regularly reading large files, consider caching them.
		 * @param filename The path to the file to read
		 * @return std::string The file contents
		 * @throw dpp::file_exception on failure to read the entire file
		 */
		std::string DPP_EXPORT read_file(const std::string& filename);

		/**
		 * @brief Validate a string value
		 * In the event the length of the string is less than _min, then an exception of type dpp:length_exception
		 * will be thrown. If the string is longer than _max UTF8 codepoints it will be truncated to fit.
		 *
		 * @param value The value to validate
		 * @param _min Minimum length
		 * @param _max Maximum length
		 * @param exception_message Exception message to throw if value length < _min
		 * @return std::string Validated string, truncated if necessary.
		 * @throw dpp::length_exception if value UTF8 length < _min
		 */
		std::string DPP_EXPORT validate(const std::string& value, size_t _min, size_t _max, const std::string& exception_message);

		/**
		 * @brief Get the url query parameter for the cdn endpoint. Internally used to build url getters.
		 *
		 * @param size size to generate url parameter for. Must be any power of two between 16 and 4096 (inclusive) or it'll return an empty string.
		 * @return std::string url query parameter e.g. `?size=128`, or an empty string
		 */
		std::string DPP_EXPORT avatar_size(uint32_t size);

		/**
		 * @brief Split (tokenize) a string into a vector, using the given separators
		 *
		 * @param in Input string
		 * @param sep Separator characters
		 * @return std::vector<std::string> Tokenized strings
		 */
		std::vector<std::string> DPP_EXPORT tokenize(std::string const& in, const char* sep = "\r\n");

		/**
		 * @brief Create a bot invite
		 *
		 * @param bot_id Bot ID
		 * @param permissions Permission bitmask of the bot to invite
		 * @param scopes Scopes to use
		 * @return Invite URL
		 */
		std::string DPP_EXPORT bot_invite_url(const snowflake bot_id, const uint64_t permissions = 0, const std::vector<std::string>& scopes = { "bot", "applications.commands" });

		/**
		 * @brief Escapes Discord's markdown sequences in a string
		 *
		 * @param text Text to escape
		 * @param escape_code_blocks If set to false, then code blocks are not escaped.
		 * This means that you can still use a code block, and the text within will be left as-is.
		 * If set to true, code blocks will also be escaped so that ` symbol may be used as a normal
		 * character.
		 * @return std::string The text with the markdown special characters escaped with a backslash
		 */
		std::string DPP_EXPORT markdown_escape(const std::string& text, bool escape_code_blocks = false);

		/**
		 * @brief Encodes a url parameter similar to [php urlencode()](https://www.php.net/manual/en/function.urlencode.php)
		 *
		 * @param value String to encode
		 * @return std::string URL encoded string
		 */
		std::string DPP_EXPORT url_encode(const std::string& value);

		/**
		 * @brief Create a mentionable slashcommand (used in a message).
		 * @param command_id The ID of the slashcommand
		 * @param command_name The command name
		 * @param subcommand Optional: The subcommand name (for mentioning a subcommand)
		 * @return std::string The formatted mention
		 */
		std::string DPP_EXPORT slashcommand_mention(snowflake command_id, const std::string& command_name, const std::string& subcommand = "");

		/**
		 * @brief Create a mentionable slashcommand (used in a message).
		 * @param command_id The ID of the slashcommand
		 * @param command_name The command name
		 * @param subcommand_group The subcommand group name
		 * @param subcommand The subcommand name
		 * @return std::string The formatted mention of the slashcommand with its subcommand
		 */
		std::string DPP_EXPORT slashcommand_mention(snowflake command_id, const std::string& command_name, const std::string& subcommand_group, const std::string& subcommand);

		/**
	 * @brief Create a mentionable user.
	 * @param id The ID of the user.
	 * @return std::string The formatted mention of the user.
	 */
		std::string DPP_EXPORT user_mention(const snowflake& id);

		/**
		* @brief Create a mentionable channel.
		* @param id The ID of the channel.
		* @return std::string The formatted mention of the channel.
		*/
		std::string DPP_EXPORT channel_mention(const snowflake& id);

		/**
		* @brief Create a mentionable emoji
		* @param name The name of the emoji.
		* @param id The ID of the emoji.
		* @param is_animated is emoji animated.
		* @return std::string The formatted mention of the emoji.
		*/
		std::string DPP_EXPORT emoji_mention(const std::string& name, const snowflake& id, bool is_animated = false);

		/**
		* @brief Create a mentionable role.
		* @param id The ID of the role.
		* @return std::string The formatted mention of the role.
		*/
		std::string DPP_EXPORT role_mention(const snowflake& id);

		/**
		 * @brief Returns the library's version string
		 *
		 * @return std::string version
		 */
		std::string DPP_EXPORT version();

		/**
		 * @brief Build a URL parameter string e.g. "?a=b&c=d&e=f" from a map of key/value pairs.
		 * Entries with empty key names or values are omitted.
		 *
		 * @param parameters parameters to create a url query string for
		 * @return std::string A correctly encoded url query string
		 */
		std::string DPP_EXPORT make_url_parameters(const std::map<std::string, std::string>& parameters);

		/**
		 * @brief Build a URL parameter string e.g. "?a=b&c=d&e=f" from a map of key/value pairs.
		 * Entries with empty key names or zero values are omitted.
		 *
		 * @param parameters parameters to create a url query string for
		 * @return std::string A correctly encoded url query string
		 */
		std::string DPP_EXPORT make_url_parameters(const std::map<std::string, uint64_t>& parameters);

		/**
		 * @brief Set the name of the current thread for debugging and statistical reporting
		 *
		 * @param name New name to set
		 */
		void DPP_EXPORT set_thread_name(const std::string& name);
	};
	std::vector<std::string> index(std::string source, const char& find)
	{
		std::string This = "";
		std::vector<std::string> i;
		for (auto c : source)
			if (c not_eq find) This += c;
			else if (c == find and This not_eq "") i.push_back(This), This = "";
		if (This not_eq "") i.push_back(This);
		return i;
	}
};

bool has_char(std::string str) {
	for (char c : str)
		if (c == 'q' or c == 'w' or c == 'e' or c == 'r' or c == 't' or c == 'y' or c == 'u' or c == 'i' or c == 'o' or c == 'p'
			or c == 'a' or c == 's' or c == 'd' or c == 'f' or c == 'g' or c == 'h' or c == 'j' or c == 'k' or c == 'l'
			or c == 'z' or c == 'x' or c == 'c' or c == 'v' or c == 'b' or c == 'n' or c == 'm') return true;
	return false;
}
std::string username(std::string str) {
	str.erase(remove(str.begin(), str.end(), '<'), str.end());
	str.erase(remove(str.begin(), str.end(), '>'), str.end());
	str.erase(remove(str.begin(), str.end(), '!'), str.end());
	str.erase(remove(str.begin(), str.end(), '@'), str.end());
	return str;
}
enum color { null, d_blue, d_green, d_cyan, d_red, d_purple, d_yellow, normal, gray, blue, green, cyan, red, purple, yellow, white };
enum option { newline, Inline };
class state {
public:
	option option = option::newline;
	color color = color::normal;
	bool freeze = false;
};
namespace neko { /* unqiue way of calling functions outside of std */
	template<typename ty> class print {
	public:
		print(ty out, std::function<void(ty)> in = nullptr, state s = state()) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), s.color);
			std::clog << out, s.option == option::newline ? std::clog << '\n' : std::clog << std::flush;
			if (s.freeze) while (true); // -> stop here
			if (in not_eq nullptr) {
				ty input = "";
				std::cin >> input;
				in(input);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color::normal);
		}
		print(std::map<ty, state> out, std::function<void(ty)> in = nullptr) {
			for (auto& it : out) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), it.second.color),
				std::clog << it.first, it.second.option == option::newline ? std::clog << '\n' : std::clog << std::flush;
			if (in not_eq nullptr) {
				ty input = "";
				std::cin >> input;
				in(input);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color::normal);
		}
		print(std::vector<ty> out, std::function<void(ty)> in = nullptr, state s = state()) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), s.color);
			for (auto& it : out) std::clog << it;
			s.option == option::newline ? std::clog << '\n' : std::clog << std::flush;
			if (s.freeze) while (true); // -> stop here
			if (in not_eq nullptr) {
				ty input = "";
				std::cin >> input;
				in(input);
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color::normal);
		}
		ty get_input() { return input; }
	private: ty input;
	};
}
class randomx {
public:
	randomx& i32(int32_t min, int32_t max) { std::random_device picker; std::uniform_int_distribution<int32_t> numbers(min, max); val32 = numbers(picker); return *this; }
	randomx& i64(int64_t min, int64_t max) { std::random_device picker; std::uniform_int_distribution<int64_t> numbers(min, max); val64 = numbers(picker); return *this; }
	int64_t val64 = 0;
	int32_t val32 = 0;
};
template <class T1, class T2, class Pred = std::greater<T1> >
struct first {
	bool operator()(const std::pair<T1, T2>& left, const std::pair<T1, T2>& right) {
		Pred p = Pred();
		return p(left.first, right.first);
	}
};

/**
* @brief get time type through user input
*/
class time_index {
public:
	/**
	 * @brief contains the time type. example: "s" = "seconds"
	*/
	std::string type;

	/**
	 * @brief checks if user provided valid time type
	*/
	bool valid(std::string str) {
		if (str.find_last_of("s") not_eq -1 and str.find_first_not_of("s") not_eq -1) type = "seconds";
		else if (str.find_last_of("m") not_eq -1 and str.find_first_not_of("m") not_eq -1) type = "minutes";
		else if (str.find_last_of("h") not_eq -1 and str.find_first_not_of("h") not_eq -1) type = "hours";
		else if (str.find_last_of("d") not_eq -1 and str.find_first_not_of("d") not_eq -1) type = "days";
		else return false;
		return true;
	}
	/**
	 * @brief convert stringed time into time_t. example: 5s = time(0) + 5
	*/
	time_t format(std::string str) { // -> didn't use tm* for simplicity
		time_t _time;
		std::cout << stoull(str) << std::endl;
		type == "seconds" ? _time = std::time(0) + stoi(str) : type == "minutes" ? _time = std::time(0) + (stoull(str) * 60) :
			type == "hours" ? _time = std::time(0) + ((stoull(str) * 60) * 60) : type == "days" ? _time = std::time(0) + ((stoull(str) * 60) * 60 * 24) : _time = time(0);
		return _time;
	}
};