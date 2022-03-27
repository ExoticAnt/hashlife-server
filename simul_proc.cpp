/*
Simulation start, run and report to server

*/

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "simul_proc.h"
#include "output_conversion.h"

#include <random>
#include <numeric>
#include <limits>

namespace simulation_manager {

	error_code SimulationManager::handle_request(http::string_body::value_type& body, const vector<string>& parameters) {
		auto p_num = parameters.size();
		
		//TODO: refactor as an expansible command processor not a chain of ifs
		if (p_num >= 1 && parameters[0] == "simulation") {
			if (p_num >= 2) {
				if (parameters[1] == "new") {
					std::size_t rows = 8;
					std::size_t cols = 8;
					constexpr std::size_t min_rows_cols = 4;

					if (p_num >= 3) { //rows
						rows = std::stoi(parameters[2]); //TODO: throws
						if (rows < min_rows_cols) return error_code::bad_arguments;
					}
					if (p_num >= 4) { //cols
						cols = std::stoi(parameters[3]); //TODO: throws
						if (cols < min_rows_cols) return error_code::bad_arguments;
					}

						auto simulation_id = make_simulation_number();
						try {
							auto u = universe_type(simulation_id, cols, rows);
							u.randomize();
							universes[simulation_id] = u;
						} catch (const simple_implementation::HLException& e) {
							if (universes.count(simulation_id))
								universes.erase(simulation_id);
							return error_code::error;
						}

						//if universe was created without problems, it is stored and displayed:
						//if (p_num >= 5) { //random
						//	if(parameters[4] == "random")
						//}

						//body.append("<html><head></head><body>");
						//body.append("<p>Simulación " + simulation_id + "</p>");
						body.append(JSONOutput(universes[simulation_id]));
						//body.append("</body></html>");
						return error_code::ok;
				}
				else if (parameters[1] == "delete") {
					return error_code::not_implemented;
				}
				else if (parameters[1] == "end") {
					//for ending async simulations
					return error_code::not_implemented;
				}
				else if (parameters[1] == "run") {

					if (p_num >= 3) {
						if (universes.count(parameters[2])) {
							std::size_t runs = 1;
							auto const& simulation_id = parameters[2];

							if (p_num >= 4) runs = std::stoi(parameters[3]); //TODO::throw
							universes[simulation_id].run(runs);
							
							//body.append("<html><head></head><body>");
							//body.append("<p>Simulación " + simulation_id + "</p>");
							//body.append(HTMLOutput(universes[simulation_id]));
							body.append(JSONOutput(universes[simulation_id]));
							//body.append("</body></html>");

							return error_code::ok;

						}
						else {
							return error_code::bad_arguments;
						}
					} else {
						return error_code::insuficient_parameters;
					}

					return error_code::not_implemented;
				}
				else {
					return error_code::unknown_command;
				}
			}
			else {
				return error_code::insuficient_parameters;
			}
		}
		else {
			//error: not a simulation request!
			return error_code::error;
		}

		return error_code::error;
	}

	/*make_simulation_number creates a new, unique simulation number. 
	Random number generator for simplicity is created and destroyed here.
	In a more performant implementation this can be optimized by creating a single instance and obtaining random numbers*/

	std::string SimulationManager::make_simulation_number() {
		std::random_device rd;  // Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		
		std::uniform_int_distribution<unsigned long> dis(0, ULONG_MAX);
		std::string number = std::to_string(dis(gen));
		
		//assures unique number for simulation
		while (universes.count(number) > 0) {
			number = std::to_string(dis(gen));
		}
		return number;
	}

}
