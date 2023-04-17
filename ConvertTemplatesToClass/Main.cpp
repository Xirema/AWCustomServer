#include<Utility/Generic/SimpleFile.h>
#include<string>
#include<iostream>
#include<sstream>
#include<boost/algorithm/string.hpp>
#include<future>
#include<boost/json.hpp>

std::string_view trim(std::string_view str) {
	auto i = str.begin();
	auto j = str.rbegin();
	while (*i == ' ' && i < j.base())
		++i;
	while (*j == ' ' && j.base() > i)
		++j;
	return { i, j.base() };
}

enum class CompoundType {
	BASIC, ARRAY, MAP, MAPMAP
};

std::string funcFilter(std::string func, bool optional, CompoundType type) {
	std::vector<std::string_view> lines;
	boost::split(
		lines,
		func,
		[](char c) {return c == '\n'; }
	);
	std::erase_if(
		lines,
		[type, optional](std::string_view line) {
			bool opt = (!optional && boost::contains(line, "[O]"))
				|| (optional && boost::contains(line, "[!O]"));
		return
			opt
			|| (type != CompoundType::BASIC && boost::contains(line, "[B]"))
			|| (type != CompoundType::ARRAY && boost::contains(line, "[AR]"))
			|| (type != CompoundType::MAP && boost::contains(line, "[M]"))
			|| (type != CompoundType::MAPMAP && boost::contains(line, "[MM]"))
			;
		}
	);
	std::stringstream ss;
	for (auto line : lines) {
		ss << line << "\n";
	}
	func = ss.str();
	boost::replace_all(func, "[O]", "   ");
	boost::replace_all(func, "[!O]", "    ");
	boost::replace_all(func, "[B]", "   ");
	boost::replace_all(func, "[AR]", "    ");
	boost::replace_all(func, "[M]", "   ");
	boost::replace_all(func, "[MM]", "    ");
	return func;
}

std::string typeFilter(std::string func, std::string_view type, std::string_view mType, std::string_view arType, std::string_view fullType) {
	boost::replace_all(func, "[TYPE]", type);
	boost::replace_all(func, "[ARTYPE]", arType);
	boost::replace_all(func, "[MTYPE]", mType);
	boost::replace_all(func, "[FULLTYPE]", fullType);
	return func;
}

int main() {
	auto openFuture = std::async([]
		{
			return util::show_file_dialog(
				util::dialog_type::OPEN,
				"Typescript Files with Templates",
				util::dialog_options::FILES | util::dialog_options::MULTI_SELECT
			);
		}
	);
	auto saveFuture = std::async([]
		{
			return util::show_file_dialog(
				util::dialog_type::SAVE,
				"File to save C++ Class Declaration"
			);
		}
	);

	auto templatePaths = openFuture.get();
	std::stringstream ss;
	std::optional<std::string_view> currentTemplateName;
	for (auto const& path : templatePaths) {
		auto fileContents = util::read_text_file(path);
		std::vector<std::string_view> lines;
		boost::split(
			lines,
			fileContents,
			[](char c) {return c == '\n'; }
		);
		std::stringstream sRead;
		std::stringstream sWrite;

		for (auto line : lines) {
			if (auto it = boost::find_first(line, "//"); !it.empty()) {
				line = std::string_view{ line.begin(), it.begin()};
			}
			if (!currentTemplateName) {
				std::string_view stringToFind = "export interface";
				if (auto token = boost::find_first(
					line,
					stringToFind
				); !token.empty()) {
					auto rightSide = boost::find_first(line, "{");
					std::string_view templateName{ token.end(), rightSide.begin()};
					templateName = trim(templateName);
					currentTemplateName = templateName;
					ss << "struct " << templateName << "{" << "\n";
					sRead = {};
					sWrite = {};
					sRead << "    " << "void readFrom(json::object const& obj) {\n";
					sWrite << "    " << "void writeTo(json::object & obj) const {\n";
				}
			}
			else {
				if (line[0] == '}') {
					currentTemplateName = {};
					sRead << "    " << "}\n";
					sWrite << "    " << "}\n";
					ss << "\n" << sRead.str();
					ss << "\n" << sWrite.str();
					ss << "};\n";
				}
				auto separator = boost::find_first(line, "?:");
				bool optional = true;
				if (separator.empty()) {
					optional = false;
					separator = boost::find_first(line, ":");
				}
				if (!separator.empty()) {
					std::string_view variableName{ line.begin(), separator.begin() };
					auto end = boost::find_last(line, ";");
					std::string_view variableType{ separator.end(), end.begin() };
					variableName = trim(variableName);
					variableType = trim(variableType);
					auto arrayIndicator = boost::find_first(variableType, "[]");
					bool array = !arrayIndicator.empty();
					if (array) {
						variableType = { variableType.begin(), variableType.end() - 2 };
					}
					ss << "    ";
					boost::json::object obj;
					for (auto const& entry : obj) {
						if (entry.value().if_bool()) {

						}
					}
					std::string readFunc =
R"Func(        if(auto ptr = obj.if_contains("[VARNAME]")) {
            if(auto tPtr = ptr->if_[TYPE]()) {
[B]             [VARNAME] = *tPtr;
[AR]			std::vector<[FULLTYPE]> arr;
[AR]            for(auto const& val : *tPtr) {
[AR]                if(auto vPtr = val.if_[ARTYPE]()) {
[AR]                    arr.push_back([FULLTYPE]{*vPtr});
[AR]                } else {
[AR]                    throw std::exception("Item in array '[VARNAME]' in [OBJNAME] was expected to be [ARTYPE], but was of wrong type.");
[AR]                }
[AR]            }
[AR]            [VARNAME] = std::move(arr);
[M]             std::map<std::string, [FULLTYPE]> map;
[M]             for(auto const& entry : *tPtr) {
[M]                 if(auto mPtr = entry.value().if_[MTYPE]()) {
[M]                     map[entry.key_c_str()] = [FULLTYPE]{*mPtr};
[M]                 } else {
[M]                     throw std::exception("Entry in map '[VARNAME]' in [OBJNAME] was expected to be [MTYPE], but was of wrong type.");
[M]                 }
[M]             }
[M]             [VARNAME] = std::move(map);
[MM]            std::map<std::string, std::map<std::string, [FULLTYPE]>> map;
[MM]            for(auto const& entry : *tPtr) {
[MM]                if(auto mPtr = entry.value().if_object()) {
[MM]                    std::map<std::string, [FULLTYPE]> subMap;
[MM]                    for(auto const& subEntry : *mPtr) {
[MM]                        if(auto mmPtr = subEntry.value().if_[MTYPE]()) {
[MM]                            subMap[subEntry.key_c_str()] = [FULLTYPE]{*mmPtr};
[MM]                        } else {
[MM]                            std::stringstream ss;
[MM]                            ss << "Entry in map '[VARNAME][";
[MM]                            ss << entry.key_c_str();
[MM]                            ss << "] in [OBJNAME] was expected to be [MTYPE], but was of wrong type.";
[MM]                            std::string exp = ss.str();
[MM]                            throw std::exception(exp.c_str());
[MM]                        }
[MM]                    }
[MM]                    map[entry.key_c_str()] = std::move(subMap);
[MM]                } else {
[MM]                    throw std::exception("Entry in map '[VARNAME]' in [OBJNAME] was expected to be object, but was of wrong type.");
[MM]                }
[MM]            }
[MM]            [VARNAME] = std::move(map);
            } else {
[B]             throw std::exception("Expected '[VARNAME]' as [FULLTYPE] in [OBJNAME], but was of wrong type.");
[AR]            throw std::exception("Expected '[VARNAME]' as array in [OBJNAME], but was of wrong type.");
[M]             throw std::exception("Expected '[VARNAME]' as object in [OBJNAME], but was of wrong type.");
[MM]            throw std::exception("Expected '[VARNAME]' as object in [OBJNAME], but was of wrong type.");
            }
        } 
[!O]    else {
[!O]        throw std::exception("'[VARNAME]' was missing in [OBJNAME]");
[!O]    })Func";
					std::string writeFunc =
R"Func([B][O]  if([VARNAME]) {
[B][O]      obj["[VARNAME]"] = *[VARNAME];
[B][O]  }
[B][!O] obj["[VARNAME]"] = [VARNAME];
[AR][O] if([VARNAME]) {
[AR][!O]{
[AR]        json::array arr;
[AR][O]     for(auto const& val : *[VARNAME]) {
[AR][!O]    for(auto const& val : [VARNAME]) {
[AR]            arr.push_back([FULLTYPE]{val});
[AR]        }
[AR]        obj["[VARNAME]"] = std::move(arr);
[AR]    }
[M][O]  if([VARNAME]) {
[M][!O] {
[M]         json::object m;
[M][O]      for(auto const& entry : *[VARNAME]) {
[M][!O]     for(auto const& entry : [VARNAME]) {
[M]             m[entry.first] = entry.second;
[M]         }
[M]         obj["[VARNAME]"] = std::move(m);
[M]     }
[MM][O] if([VARNAME]) {
[MM][!O]{
[MM]        json::object m;
[MM][O]     for(auto const& entry : *[VARNAME]) {
[MM][!O]    for(auto const& entry : [VARNAME]) {
[MM]            json::object mm;
[MM]            for(auto const& subEntry : entry.second) {
[MM]                mm[subEntry.first] = subEntry.second;
[MM]            }
[MM]            m[entry.first] = std::move(mm);
[MM]        }
[MM]        obj["[VARNAME]"] = std::move(m);
[MM]    })Func";
					boost::replace_all(
						readFunc,
						"[VARNAME]",
						variableName
					);
					boost::replace_all(
						readFunc,
						"[OBJNAME]",
						*currentTemplateName
					);
					boost::replace_all(
						writeFunc,
						"[VARNAME]",
						variableName
					);

					if (optional) {
						ss << "std::optional<";
					}
					if (array) {
						ss << "std::vector<";
					}
					if (variableType == "string") {
						ss << "std::string";
						readFunc = typeFilter(funcFilter(readFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), array ? "array" : "string", "string", "string", "std::string");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), "", "", "", "json::string");
					}
					else if (variableType == "number" || variableType == "bigint") {
						ss << "int64_t";
						readFunc = typeFilter(funcFilter(readFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), array ? "array" : "int64", "int64", "int64", "int64_t");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), "", "", "", "int64_t");
					}
					else if (variableType == "boolean") {
						ss << "bool";
						readFunc = typeFilter(funcFilter(readFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), array ? "array" : "bool", "bool", "bool", "bool");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array ? CompoundType::ARRAY : CompoundType::BASIC), "", "", "", "bool");
					}
					else if (variableType == "{[k:string]:number}") {
						ss << "std::map<std::string, int64_t>";
						readFunc = typeFilter(funcFilter(readFunc, optional, CompoundType::MAP), "object", "int64", "int64", "int64_t");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, CompoundType::MAP), "", "", "", "int64_t");
					}
					else if (variableType == "{[k:string]:{[k:string]:number}}") {
						ss << "std::map<std::string, std::map<std::string, int64_t>>";
						readFunc = typeFilter(funcFilter(readFunc, optional, CompoundType::MAPMAP), "object", "int64", "int64", "int64_t");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, CompoundType::MAPMAP), "", "", "", "int64_t");
					}
					else {
						ss << "[" << variableType << "] (UNKNOWN)";
					}
					if (array) {
						ss << ">";
					}
					if (optional) {
						ss << ">";
					}
					ss << " " << variableName << ";\n";
					sRead << readFunc;
					sWrite << writeFunc;
				}
				
			}
		}
	}

	auto savePath = saveFuture.get();
	if (savePath.size() > 0) {
		util::write_text_file(savePath.front(), ss.str());
	}
}