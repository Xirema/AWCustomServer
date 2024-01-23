#pragma once
#include<boost/json.hpp>

namespace jss {
	template<size_t N>
	struct FixedString {
		char buf[N + 1]{};
		constexpr FixedString(char const* str) {
			for (size_t i = 0; i != N; i++) {
				buf[i] = str[i];
			}
		}
		constexpr operator char const* () const {
			return buf;
		}
	};
	template<size_t N> FixedString(char const(&)[N])->FixedString<N - 1>;
	template<typename T, FixedString variableName>
	struct JSSerializer {
		static_assert("Type is not implemented as serializable");
	};

	template<FixedString variableName>
	struct JSSerializer<int64_t, variableName> {
		boost::json::value serialize(int64_t val) const {
			return boost::json::value_from(val);
		}
		int64_t deserialize(boost::json::value const* ptr) const {
			if (ptr && ptr->is_int64()) {
				return ptr->as_int64();
			}
			else {
				std::string ret;
				ret += "Expected '" + variableName + "' as integer, but received ";
				if (!ptr) {
					ret += "'" + ptr->kind() + "'";
				}
				else {
					ret += "nothing";
				}
				ret += ".";
			}
		}
	};

	template<FixedString variableName>
	struct JSSerializer<double, variableName> {
		boost::json::value serialize(double val) const {
			return boost::json::value_from(val);
		}
		double deserialize(boost::json::value const* ptr) const {
			if (ptr && ptr->is_double()) {
				return ptr->as_double();
			}
			else {
				std::string ret;
				ret += "Expected '" + variableName + "' as double, but received ";
				if (!ptr) {
					ret += "'" + ptr->kind() + "'";
				}
				else {
					ret += "nothing";
				}
				ret += ".";
			}
		}
	};

	template<FixedString variableName>
	struct JSSerializer<bool, variableName> {
		boost::json::value serialize(bool val) const {
			return boost::json::value_from(val);
		}
		bool deserialize(boost::json::value const* ptr) const {
			if (ptr && ptr->is_bool()) {
				return ptr->as_bool();
			}
			else {
				std::string ret;
				ret += "Expected '" + variableName + "' as bool, but received ";
				if (!ptr) {
					ret += "'" + ptr->kind() + "'";
				}
				else {
					ret += "nothing";
				}
				ret += ".";
			}
		}
	};

	template<FixedString variableName>
	struct JSSerializer<std::string, variableName> {
		boost::json::value serialize(std::string val) const {
			return boost::json::value_from(val);
		}
		std::string deserialize(boost::json::value const* ptr) const {
			if (ptr && ptr->is_string()) {
				return ptr->as_string();
			}
			else {
				std::string ret;
				ret += "Expected '" + variableName + "' as string, but received ";
				if (!ptr) {
					ret += "'" + ptr->kind() + "'";
				}
				else {
					ret += "nothing";
				}
				ret += ".";
			}
		}
	};


}