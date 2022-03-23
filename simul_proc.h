
#include "simple_implementation.h"
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

namespace http = boost::beast::http;           // from <boost/beast/http.hpp>
using universe_type = simple_implementation::Simple_Implementation;


namespace simulation_manager {


	enum class error_code {
		ok,
		error,
		insuficient_parameters,
		unknown_command,
		bad_arguments,
		not_implemented
	};

	class SimulationManager {
	public:
		error_code handle_request(http::string_body::value_type& body, const vector<string>& parameters);

	private:
		map<std::string, universe_type> universes;

		std::string make_simulation_number();

		//error_code generate_view()

	};

}