#include<boost/json.hpp>
#include<tuple>
#include<vector>
#include<string>
#include<boost/algorithm/string.hpp>
#include<Utility/Generic/SimpleFile.h>
#include<sstream>

struct TableEntry {
	int firstNumber;
	int secondNumber;
	std::string description;
};

struct TableStructure {
	std::vector<TableEntry> rows;
	std::string title;
};

TableStructure getTable() {
	auto paths = util::show_file_dialog(
		util::dialog_type::OPEN,
		"Table File"
	);
	if (paths.size() != 1)
		throw std::runtime_error("...");
	auto fileContents = util::read_text_file(paths[0]);
	std::stringstream ss{ fileContents };
	std::string line;
	std::getline(ss, line);
	TableStructure ret;
	std::vector<std::string_view> tokens;
	boost::split(tokens, line, [](char c) {return c == '|'; });
	ret.title = std::string{ tokens.at(2) } + " Damage";
	std::getline(ss, line);
	while (std::getline(ss, line)) {
		tokens = {};
		boost::split(tokens, line, [](char c) {return c == '|'; });
		std::vector<std::string_view> numbers;
		boost::split(numbers, tokens.at(1), [](char c) {return c == '-'; });
		TableEntry row;
		if (numbers.size() == 2) {//Range
			row.firstNumber = std::stoi(std::string(numbers[0]));
			row.secondNumber = std::stoi(std::string(numbers[1]));
		}
		else { //single number
			row.firstNumber = row.secondNumber = std::stoi(std::string(numbers[0]));
		}
		row.description = std::string(tokens.at(2));
		boost::replace_all(row.description, "**", "");
		ret.rows.push_back(std::move(row));
	}
	return ret;
}

boost::json::value convertToJson() {
	auto table = getTable();
	boost::json::object ret;
	ret["name"] = table.title;
	boost::json::array results;
	for (auto const& row : table.rows) {
		boost::json::object rowObj;
		rowObj["type"] = 0;
		rowObj["text"] = row.description;
		rowObj["weight"] = 1;
		boost::json::array range;
		range.push_back(row.firstNumber);
		range.push_back(row.secondNumber);
		rowObj["range"] = range;
		rowObj["drawn"] = false;
		rowObj["flags"] = boost::json::object{};
		rowObj["img"] = "icons/svg/d20-black.svg";
		rowObj["collection"] = nullptr;
		rowObj["resultId"] = nullptr;
		rowObj["rangeL"] = row.firstNumber;
		rowObj["rangeR"] = row.secondNumber;
		results.push_back(rowObj);
	}
	ret["results"] = results;
	ret["formula"] = "d20";
	ret["replacement"] = true;
	ret["displayRoll"] = true;
	return ret;
}

int main() {
	auto paths = util::show_file_dialog(
		util::dialog_type::SAVE,
		"JSON File Out"
	);
	if (paths.size() != 1)
		return 0;
	util::write_text_file(paths[0], boost::json::serialize(convertToJson()));
}