
#include <fstream>
#include <stdexcept>
#include <vector>

namespace shinji // loader
{
	inline std::pair<char const*, size_t> load_shader_from_file(char const* shader, std::vector<char>& buf)
	{
		std::ifstream f(shader, std::ios::binary | std::ios::ate);
		if (!f.is_open()) {
			throw std::runtime_error("Could not find shader file.");
		}

		size_t buf_len = (size_t) f.tellg();
		buf.resize(buf_len);

		f.seekg(0);
		f.read(buf.data(), (std::streamsize) buf_len);
		return std::pair<char const*, size_t>(buf.data(), buf_len);
	}

	inline std::pair<char const*, size_t> load_shader_from_bundle(char const* shader)
	{
		return shinji::bundle::load_shader(shader);
	}
}
