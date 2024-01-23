#include<string>
#include<iostream>
#include<sstream>
#include<boost/algorithm/string.hpp>
#include<future>
#include<filesystem>
#include<fstream>
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
	BASIC, MAP, MAPMAP, OBJECT
};

std::string funcFilter(std::string func, bool optional, bool array, CompoundType type) {
	std::vector<std::string_view> lines;
	boost::split(
		lines,
		func,
		[](char c) {return c == '\n'; }
	);
	std::erase_if(
		lines,
		[type, optional, array](std::string_view line) {
			bool opt = (!optional && boost::contains(line, "[O]"))
				|| (optional && boost::contains(line, "[!O]"));
			bool arr = (!array && boost::contains(line, "[AR]"))
				|| (array && boost::contains(line, "[!AR"));
		return
			opt
			|| arr
			|| (type != CompoundType::BASIC && boost::contains(line, "[B]"))
			|| (type != CompoundType::MAP && boost::contains(line, "[M]"))
			|| (type != CompoundType::MAPMAP && boost::contains(line, "[MM]"))
			|| (type != CompoundType::OBJECT && boost::contains(line, "[OB]"))
			|| (type == CompoundType::OBJECT && boost::contains(line, "[!OB"))
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
	boost::replace_all(func, "[!AR", "    ");
	boost::replace_all(func, "[M]", "   ");
	boost::replace_all(func, "[MM]", "    ");
	boost::replace_all(func, "[OB]", "    ");
	boost::replace_all(func, "[!OB", "    ");
	return func;
}

std::string typeFilter(std::string func, std::string_view type, std::string_view mType, std::string_view arType, std::string_view fullType, std::string_view objType) {
	boost::replace_all(func, "[TYPE]", type);
	boost::replace_all(func, "[ARTYPE]", arType);
	boost::replace_all(func, "[MTYPE]", mType);
	boost::replace_all(func, "[FULLTYPE]", fullType);
	boost::replace_all(func, "[OTYPE]", fullType);
	return func;
}

int main() {
	auto rootDirectory = std::filesystem::path("G:/AWCustom/temp");
	//auto output = rootDirectory / "out.h";
	auto templateDirectory = rootDirectory / "templates";

	std::vector<std::filesystem::path> templatePaths;
	for (auto path : std::filesystem::recursive_directory_iterator(templateDirectory)) {
		if (!path.is_regular_file()) {
			continue;
		}
		templatePaths.push_back(path.path());
	}
	std::optional<std::string_view> currentTemplateName;
	for (auto const& path : templatePaths) {
		std::stringstream ss;
		ss << R"RAW(#pragma once
#include<jss/jssinclude.h>

)RAW";
		auto parent = path.parent_path().stem();
		ss << "namespace " << parent.string() << "{\n";
		auto output = rootDirectory / path.stem();
		output += ".h";
		std::string fileContents;
		{
			std::stringstream ss;
			std::ifstream in{ path };
			std::string line;
			while (std::getline(in, line)) {
				ss << line << '\n';
			}
			fileContents = ss.str();
		}
		//auto fileContents = util::read_text_file(path);
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
					ss << "    struct " << templateName << "{" << "\n";
					sRead = {};
					sWrite = {};
					sRead << "        " << "void readFrom(json::object const& obj) {\n";
					sWrite << "        " << "void writeTo(json::object & obj) const {\n";
				}
			}
			else {
				if (line[0] == '}') {
					currentTemplateName = {};
					sRead << "        " << "}\n";
					sWrite << "        " << "}\n";
					ss << "\n" << sRead.str();
					ss << "\n" << sWrite.str();
					ss << "    };\n";
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
					ss << "        ";
					//boost::json::object obj;
					//for (auto const& entry : obj) {
					//	if (entry.value().if_bool()) {

					//	}
					//}
					std::string readFunc =
R"Func(            if(auto ptr = obj.if_contains("[VARNAME]")) {
	            if(auto tPtr = ptr->if_[TYPE]()) {
[B]	[!AR            [VARNAME] = *tPtr;
[AR]				std::vector<[FULLTYPE]> arr;
[AR]		        for(auto const& val : *tPtr) {
[AR]			        if(auto vPtr = val.if_[ARTYPE]()) {
[AR][!OB			        arr.push_back([FULLTYPE]{*vPtr});
[AR][OB]					[FULLTYPE] v;
[AR][OB]					v.readFrom(*vPtr);
[AR][OB]					arr.push_back(std::move(v));
[AR]				    } else {
[AR]				        throw std::runtime_error("Item in array '[VARNAME]' in [OBJNAME] was expected to be [ARTYPE], but was of wrong type.");
[AR]				    }
[AR]			    }
[AR]			    [VARNAME] = std::move(arr);
[M]				    std::map<std::string, [FULLTYPE]> map;
[M]				    for(auto const& entry : *tPtr) {
[M]				        if(auto mPtr = entry.value().if_[MTYPE]()) {
[M]				            map[entry.key_c_str()] = [FULLTYPE]{*mPtr};
[M]				        } else {
[M]				            throw std::runtime_error("Entry in map '[VARNAME]' in [OBJNAME] was expected to be [MTYPE], but was of wrong type.");
[M]				        }
[M]				    }
[M]				    [VARNAME] = std::move(map);
[MM]			    std::map<std::string, std::map<std::string, [FULLTYPE]>> map;
[MM]			    for(auto const& entry : *tPtr) {
[MM]			        if(auto mPtr = entry.value().if_object()) {
[MM]			            std::map<std::string, [FULLTYPE]> subMap;
[MM]			            for(auto const& subEntry : *mPtr) {
[MM]			                if(auto mmPtr = subEntry.value().if_[MTYPE]()) {
[MM]			                    subMap[subEntry.key_c_str()] = [FULLTYPE]{*mmPtr};
[MM]			                } else {
[MM]			                    std::stringstream ss;
[MM]			                    ss << "Entry in map '[VARNAME][";
[MM]			                    ss << entry.key_c_str();
[MM]			                    ss << "] in [OBJNAME] was expected to be [MTYPE], but was of wrong type.";
[MM]			                    std::string exp = ss.str();
[MM]			                    throw std::runtime_error(exp.c_str());
[MM]			                }
[MM]			            }
[MM]			            map[entry.key_c_str()] = std::move(subMap);
[MM]			        } else {
[MM]			            throw std::runtime_error("Entry in map '[VARNAME]' in [OBJNAME] was expected to be object, but was of wrong type.");
[MM]			        }
[MM]			    }
[MM]			    [VARNAME] = std::move(map);
[OB][!AR			[VARNAME].readFrom(*tPtr);
[!O]			} else {
[O]				} else if(!ptr->is_null()) {
[B][!AR			    throw std::runtime_error("Expected '[VARNAME]' as [FULLTYPE] in [OBJNAME], but was of type " + std::string(to_string(ptr->kind())) + ".");
[AR]			    throw std::runtime_error("Expected '[VARNAME]' as array in [OBJNAME], but was of type " + std::string(to_string(ptr->kind())) + ".");
[M]				    throw std::runtime_error("Expected '[VARNAME]' as object in [OBJNAME], but was of type " + std::string(to_string(ptr->kind())) + ".");
[MM]			    throw std::runtime_error("Expected '[VARNAME]' as object in [OBJNAME], but was of type " + std::string(to_string(ptr->kind())) + ".");
[OB][!AR			throw std::runtime_error("Expected '[VARNAME]' as [OTYPE] in [OBJNAME], but was of type " + std::string(to_string(ptr->kind())) + ".");
				}
			} 
[!O]	    else {
[!O]	        throw std::runtime_error("'[VARNAME]' was missing in [OBJNAME]");
[!O]	    })Func";
					std::string writeFunc =
R"Func([B][O][!AR  if([VARNAME]) {
[B][O][!AR	    obj["[VARNAME]"] = *[VARNAME];
[B][O][!AR	}
[B][!O][!AR	obj["[VARNAME]"] = [VARNAME];
[AR][O]		if([VARNAME]) {
[AR][!O]	{
[AR]		    json::array arr;
[AR][O]		    for(auto const& val : *[VARNAME]) {
[AR][!O]	    for(auto const& val : [VARNAME]) {
[AR][!OB	        arr.push_back([FULLTYPE]{val});
[AR][OB]			json::object obj;
[AR][OB]			val.writeTo(obj);
[AR][OB]			arr.push_back(std::move(obj));
[AR]		    }
[AR]		    obj["[VARNAME]"] = std::move(arr);
[AR]		}
[M][O]		if([VARNAME]) {
[M][!O]		{
[M]			    json::object m;
[M][O]		    for(auto const& entry : *[VARNAME]) {
[M][!O]		    for(auto const& entry : [VARNAME]) {
[M]			        m[entry.first] = entry.second;
[M]			    }
[M]			    obj["[VARNAME]"] = std::move(m);
[M]			}
[MM][O]		if([VARNAME]) {
[MM][!O]	{
[MM]		    json::object m;
[MM][O]		    for(auto const& entry : *[VARNAME]) {
[MM][!O]	    for(auto const& entry : [VARNAME]) {
[MM]		        json::object mm;
[MM]		        for(auto const& subEntry : entry.second) {
[MM]		            mm[subEntry.first] = subEntry.second;
[MM]		        }
[MM]		        m[entry.first] = std::move(mm);
[MM]		    }
[MM]		    obj["[VARNAME]"] = std::move(m);
[MM]		}
[!AR[OB][O] if([VARNAME]) {
[!AR[OB][!O]{
[!AR[OB]		json::object m;
[!AR[OB][O]		[VARNAME]->writeTo(m);
[!AR[OB][!O]	[VARNAME].writeTo(m);
[!AR[OB]		obj["[VARNAME]"] = std::move(m);
[!AR[OB]	})Func";
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
						readFunc = typeFilter(funcFilter(readFunc, optional, array, CompoundType::BASIC), array ? "array" : "string", "string", "string", "std::string", "string");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array, CompoundType::BASIC), "", "", "", "json::string", "string");
					}
					else if (variableType == "number" || variableType == "bigint") {
						ss << "int64_t";
						readFunc = typeFilter(funcFilter(readFunc, optional, array, CompoundType::BASIC), array ? "array" : "int64", "int64", "int64", "int64_t", "integer");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array, CompoundType::BASIC), "", "", "", "int64_t", "integer");
					}
					else if (variableType == "boolean") {
						ss << "bool";
						readFunc = typeFilter(funcFilter(readFunc, optional, array, CompoundType::BASIC), array ? "array" : "bool", "bool", "bool", "bool", "bool");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array, CompoundType::BASIC), "", "", "", "bool", "bool");
					}
					else if (variableType == "{[k:string]:number}") {
						ss << "std::map<std::string, int64_t>";
						readFunc = typeFilter(funcFilter(readFunc, optional, false, CompoundType::MAP), "object", "int64", "int64", "int64_t", "integer");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, false, CompoundType::MAP), "", "", "", "int64_t", "integer");
					}
					else if (variableType == "{[k:string]:{[k:string]:number}}") {
						ss << "std::map<std::string, std::map<std::string, int64_t>>";
						readFunc = typeFilter(funcFilter(readFunc, optional, false, CompoundType::MAPMAP), "object", "int64", "int64", "int64_t", "integer");
						writeFunc = typeFilter(funcFilter(writeFunc, optional, false, CompoundType::MAPMAP), "", "", "", "int64_t", "integer");
					}
					else {
						ss << variableType;
						readFunc = typeFilter(funcFilter(readFunc, optional, array, CompoundType::OBJECT), array ? "array" : "object", "object", "object", variableType, variableType);
						writeFunc = typeFilter(funcFilter(writeFunc, optional, array, CompoundType::OBJECT), "", "", "", "object", variableType);
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
		ss << "}\n";
		std::ofstream out{ output };
		out << ss.str();
	}
}