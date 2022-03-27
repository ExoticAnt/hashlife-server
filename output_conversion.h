/*
* Function templates for conversion from Universes to output formats (JSON, HTML) to be served.

*/

//#include "simple_implementation.h"

#include <boost/json/src.hpp>
#include <string>
#include <algorithm>

namespace json = boost::json;

namespace simulation_manager {

	template<class R> void process_row_html(std::ostringstream& os, R& row) {
		//os << "<p>";
	std:for_each(row.cbegin(), row.cend(), [&os](const auto& val) { os << (int)val; });
		os << "<br>";
	}

	template<class U> std::string HTMLOutput(U& universe) {
		std::string output;
		ostringstream os;

		os << "<p>";
		std::for_each(universe.cbegin(), universe.cend(), [&os](const auto& row) { process_row_html(os, row);  });
		os << "</p>";

		return os.str();
	};

	template<class U> std::string JSONOutput(U& universe) {
		json::object obj;

		obj["simulation_id"] = universe.get_id();
		obj["rows"] = universe.get_rows();
		obj["cols"] = universe.get_cols();

		json::array array;
		//std::for_each(universe.cbegin(), universe.cend(), [array](const auto &r) {array.push_back(json::array(r.cbegin(), r.cend())); });
		for (auto r = universe.cbegin(); r != universe.cend(); ++r) {
			array.push_back(json::array(r->cbegin(), r->cend()));
		}

		obj["data"] = array;

		return json::serialize(obj);
	};

};