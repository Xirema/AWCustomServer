#include<Properties.h>
//#include<Utility/Generic/SimpleFile.h>
//#include<Utility/Generic/StringExt.h>
#include<fstream>
#include<boost/algorithm/string.hpp>

namespace properties {
	Properties::Properties() {
		std::filesystem::path propertiesPath = "app.properties";
		propertiesPath = std::filesystem::absolute(propertiesPath);
		std::cout << "Reading from " << propertiesPath.string() << std::endl;
		if (!std::filesystem::exists(propertiesPath) || !std::filesystem::is_regular_file(propertiesPath)) {
			std::ofstream out{ propertiesPath };
			std::cerr << "Properties file did not exist in " << propertiesPath.string() << std::endl;
			//util::write_text_file("app.properties", "");
		}
		std::string file;
		{
			std::ifstream in{ propertiesPath };
			in.seekg(0, std::ios::end);
			auto length = in.tellg();
			in.seekg(0, std::ios::beg);
			file.resize(length);
			in.read(file.data(), length);
		}
		std::vector<std::string_view> lines;
		boost::split(
			lines,
			file,
			[](char c) {return c == '\n'; }
		);
		for (auto const& line : lines) {
			if (line.starts_with("#")) {
				continue;
			}
			if (auto it = line.find("="); it != std::string::npos) {
				properties[std::string{boost::trim_copy(line.substr(0, it))}] = boost::trim_copy(line.substr(it + 1, line.size() - (it + 1)));
			}
		}
	}

	std::unique_ptr<Properties const> Properties::appProperties;

	Properties const& Properties::instance() {
		if (!appProperties) {
			appProperties.reset(new Properties{});
		}
		return *appProperties;
	}



	std::string Properties::getString(std::string const& name, std::optional<std::string> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			return it->second;
		}
		if (defaultValue) {
			return *defaultValue;
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	int64_t Properties::getInt(std::string const& name, std::optional<int64_t> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			try {
				int64_t value = std::stoll(it->second);
				return value;
			}
			catch (std::invalid_argument const&) {
				throw std::runtime_error("'" + it->second + "' could not be converted to an integer");
			}
			catch (std::out_of_range const&) {
				throw std::runtime_error("'" + it->second + "' was too large to be converted to an integer");
			}
		}
		if (defaultValue) {
			return *defaultValue;
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	double Properties::getDouble(std::string const& name, std::optional<double> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			try {
				double value = std::stod(it->second);
				return value;
			}
			catch (std::invalid_argument const&) {
				throw std::runtime_error("'" + it->second + "' could not be converted to a float");
			}
			catch (std::out_of_range const&) {
				throw std::runtime_error("'" + it->second + "' was too large to be converted to a float");
			}
		}
		if (defaultValue) {
			return *defaultValue;
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	bool Properties::getBool(std::string const& name, std::optional<bool> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			if (
				boost::iequals(it->second, "true")
				|| boost::iequals(it->second, "t")
				|| boost::iequals(it->second, "1")) {
				return true;
			}
			if (
				boost::iequals(it->second, "false")
				|| boost::iequals(it->second, "f")
				|| boost::iequals(it->second, "0")
				) {
				return false;
			}
			throw std::runtime_error("Unable to contextually convert '" + it->second + "' to a boolean value");
		}
		if (defaultValue) {
			return *defaultValue;
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}

	std::unique_ptr<std::string[]> Properties::getStringArray(std::string const& name, std::optional<std::unique_ptr<std::string[]>> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			std::vector<std::string> strings;
			boost::split(
				strings,
				it->second,
				[](char c) {return c == ','; }
			);
			auto ret = std::make_unique<std::string[]>(strings.size());
			std::copy(strings.begin(), strings.end(), ret.get());
			return ret;
		}
		if (defaultValue) {
			return std::move(*defaultValue);
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	std::unique_ptr<int64_t[]> Properties::getIntArray(std::string const& name, std::optional<std::unique_ptr<int64_t[]>> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			std::vector<std::string> strings;
			boost::split(
				strings,
				it->second,
				[](char c) {return c == ','; }
			);
			auto ret = std::make_unique<int64_t[]>(strings.size());
			for (size_t i = 0; i < strings.size(); i++) {
				try {
					ret[i] = std::stoll(strings[i]);
				}
				catch (std::invalid_argument const&) {
					throw std::runtime_error("'" + strings[i] + "' could not be converted to an integer");
				}
				catch (std::out_of_range const&) {
					throw std::runtime_error("'" + strings[i] + "' was too large to be converted to an integer");
				}
			}
			return ret;
		}
		if (defaultValue) {
			return std::move(*defaultValue);
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	std::unique_ptr<double[]> Properties::getDoubleArray(std::string const& name, std::optional<std::unique_ptr<double[]>> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			std::vector<std::string> strings;
			boost::split(
				strings,
				it->second,
				[](char c) {return c == ','; }
			);
			auto ret = std::make_unique<double[]>(strings.size());
			for (size_t i = 0; i < strings.size(); i++) {
				try {
					ret[i] = std::stod(strings[i]);
				}
				catch (std::invalid_argument const&) {
					throw std::runtime_error("'" + strings[i] + "' could not be converted to a float");
				}
				catch (std::out_of_range const&) {
					throw std::runtime_error("'" + strings[i] + "' was too large to be converted to a float");
				}
			}
			return ret;
		}
		if (defaultValue) {
			return std::move(*defaultValue);
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
	std::unique_ptr<bool[]> Properties::getBoolArray(std::string const& name, std::optional<std::unique_ptr<bool[]>> defaultValue) const {
		if (auto it = properties.find(name); it != properties.end()) {
			std::vector<std::string> strings;
			boost::split(
				strings,
				it->second,
				[](char c) {return c == ','; }
			);
			auto ret = std::make_unique<bool[]>(strings.size());
			for (size_t i = 0; i < strings.size(); i++) {
				if (
					boost::iequals(strings[i], "true")
					|| boost::iequals(strings[i], "t")
					|| boost::iequals(strings[i], "1")) {
					ret[i] = true;
				}
				if (
					boost::iequals(strings[i], "false")
					|| boost::iequals(strings[i], "f")
					|| boost::iequals(strings[i], "0")
					) {
					ret[i] = false;
				}
				throw std::runtime_error("Unable to contextually convert '" + strings[i] + "' to a boolean value");
			}
			return ret;
		}
		if (defaultValue) {
			return std::move(*defaultValue);
		}
		throw std::runtime_error("No Property found for '" + name + "' and no Default provided");
	}
}