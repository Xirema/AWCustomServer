#pragma once
#include<filesystem>
#include<iostream>
#include<iomanip>
#include<unordered_map>
#include<string>

namespace properties {
	class Properties {
		Properties();
	public:
		static Properties const& instance();
		static std::unique_ptr<Properties const> appProperties;

		std::string getString(std::string const& name, std::optional<std::string> defaultValue = {}) const;
		int64_t getInt(std::string const& name, std::optional<int64_t> defaultValue = {}) const;
		double getDouble(std::string const& name, std::optional<double> defaultValue = {}) const;
		bool getBool(std::string const& name, std::optional<bool> defaultValue = {}) const;

		std::unique_ptr<std::string[]> getStringArray(std::string const& name, std::optional<std::unique_ptr<std::string[]>> defaultValue = {}) const;
		std::unique_ptr<int64_t[]> getIntArray(std::string const& name, std::optional<std::unique_ptr<int64_t[]>> defaultValue = {}) const;
		std::unique_ptr<double[]> getDoubleArray(std::string const& name, std::optional<std::unique_ptr<double[]>> defaultValue = {}) const;
		std::unique_ptr<bool[]> getBoolArray(std::string const& name, std::optional<std::unique_ptr<bool[]>> defaultValue = {}) const;

		std::unordered_map<std::string, std::string> properties;
	};
}