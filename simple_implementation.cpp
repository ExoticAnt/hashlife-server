

/*
Simple implementation of the Conway's Game of Life.


*/

#include <vector>
#include <string>
#include <sstream>
#include <random>
#include <numeric>

#include "simple_implementation.h"

using namespace std;

namespace simple_implementation {

	bool Simple_Implementation::resize_at_bottom_test() {
		if (universe.size() > 0) {
			int sum = std::accumulate(universe[0].begin(), universe[0].end(), 0);
			if (sum > 0) return true;
		}
		return false;
	}

	bool Simple_Implementation::resize_at_left_test() {
		if (universe.size() > 0) {
			int sum = 0;
			for (auto row = 0; row < rows; row++)
				sum += universe[row][0];
			if (sum > 0) return true;
		}
		return false;
	}

	bool Simple_Implementation::resize_at_top_test() {
		if (universe.size() > 0) {
			auto& row = universe.back();
			int sum = std::accumulate(row.begin(), row.end(), 0);
			if (sum > 0) return true;
		}
		return false;
	}

	bool Simple_Implementation::resize_at_right_test() {
		if (universe.size() > 0) {
			int sum = 0;
			for (auto row = 0; row < rows; row++)
				sum += universe[row].back();
			if (sum > 0) return true;
		}
		return false;
	}

	Simple_Implementation::Simple_Implementation() :
		Simple_Implementation(8, 8) {};


	Simple_Implementation::Simple_Implementation(std::size_t cols, std::size_t rows) {
		if (cols < 2 || rows < 2) {
			throw(HLException("Las dimensiones deben ser mayores a 2"));
		}

		this->cols = cols;
		this->rows = rows;

		for (std::size_t j = 0; j < rows; j++) {
			universe.push_back(std::vector<T_cell>(cols));
		}

		//sets the initial default ruleset
		//cout << "Universe Size:" << universe.size() << "\n";
		ruleset();
	}

	Simple_Implementation::Simple_Implementation(std::string simulation_id, std::size_t cols, std::size_t rows)  :
				Simple_Implementation(cols, rows) {
		this->simulation_id = simulation_id;
	}

	void Simple_Implementation::ruleset(unsigned int survive_min, unsigned int survive_max, unsigned int born_min, unsigned int born_max) {
		this->survive_min = survive_min;
		this->survive_max = survive_max;
		this->born_min = born_min;
		this->born_max = born_max;

	}

	void Simple_Implementation::randomize(float ratio) {
		std::random_device rd;  // Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<float> dis(0.0, 1.0);
		for (auto& row : universe) {
			for (auto& cell : row) {
				if (dis(gen) <= ratio)
					cell = 1;
				else
					cell = 0;
			}
		}
	}
	void Simple_Implementation::run(int n) {
		if (n < 0) return; //in a reversible ruleset this can be a nice feature.
		for (auto i = 0; i < n; ++i) run_1();
	}

	void Simple_Implementation::run_1() {
		vector<vector<T_cell>> new_step;

		//This checks if there are active cells in the borders
		//If there are, automatically array is resized
		test_and_resize();

		//We only need to store two rows because in each step the velocity of propagation is 1.
		for (std::size_t j = 0; j < 2; j++) {
			new_step.push_back(std::vector<T_cell>(cols));
		}


		for (std::size_t row = 0; row < rows; row++) {

			int ant_ant = 0, ant_act = 0, ant_sig = 0;	//first part (ant, act, sig) is for row, second for column. Spanish names for previous, current and next.
			int act_ant = 0, act_act = 0, act_sig = 0;
			int sig_ant = 0, sig_act = 0, sig_sig = 0;

			if (row != 0) {
				ant_act = universe[row - 1][0];
				ant_sig = universe[row - 1][1];
			}

			act_act = universe[row][0];
			act_sig = universe[row][1];
			if (row < rows - 1) {
				sig_act = universe[row + 1][0];
				sig_sig = universe[row + 1][1];
			}
			else {
				sig_act = 0;
				sig_sig = 0;
			}


			for (std::size_t col = 0; col < cols; col++) {
				auto res = ant_ant + ant_act + ant_sig + act_ant + act_sig + sig_ant + sig_act + sig_sig;
				T_cell nv = 0;
				if (universe[row][col]) {
					if (res >= survive_min && res <= survive_max)
						nv = 1;
					else
						nv = 0;
				}
				else {
					if (res >= born_min && res <= born_max)
						nv = 1;
					else
						nv = 0;
				}
				new_step[1][col] = nv;

				ant_ant = ant_act;
				act_ant = act_act;
				sig_ant = sig_act;

				ant_act = ant_sig;
				act_act = act_sig;
				sig_act = sig_sig;

				if (col >= cols - 2) {
					ant_sig = 0;
					act_sig = 0;
					sig_sig = 0;
				}
				else {
					if (row > 0) {
						ant_sig = universe[row - 1][col + 2];
					}
					else {
						ant_sig = 0;
					}
					act_sig = universe[row][col + 2];
					if (row >= rows - 1) {
						sig_sig = 0;
					}
					else {
						sig_sig = universe[row + 1][col + 2];
					}
				}
			}

			if (row >= 1) {
				universe[row - 1] = new_step[0];
			}

			new_step[0] = new_step[1];

		}

		universe[rows - 1] = new_step[1];
	}


	void Simple_Implementation::set(std::size_t row, std::size_t col, int value) {
		if (row >= rows || col >= cols || row < 0 || col < 0) {
			throw(HLException("Invalid dimensions"));
		}
		universe[row][col] = value;
	}

	//Vectors are increased in 50% their size when a condition meets.
	void Simple_Implementation::test_and_resize() {

		//the easy case:
		if (resize_at_top_test()) {
		std:size_t new_rows = (std::size_t)rows * 1.5;

			universe.reserve(new_rows);
			universe.resize(new_rows, vector<T_cell>(cols));
			rows = new_rows;
		}

		//second easy case:
		if (resize_at_right_test()) {
			std::size_t new_cols = (std::size_t)cols * 1.5;
			for (auto& row : universe) {
				row.resize(new_cols, T_cell{ 0 });
			}
			cols = new_cols;
		}

		//not so easy:
		if (resize_at_bottom_test()) {
			std::size_t new_rows = (std::size_t)rows * 1.5;
			universe.reserve(new_rows);
			universe.resize(new_rows, vector<T_cell>(cols));

			//here an alternate solution can be the use of std:move on each row:
			std::copy_backward(&universe[0], &universe[rows - 1], &universe[new_rows - 1]);

			for (std::size_t i = 0; i < new_rows - rows; i++) {
				std::fill(universe[i].begin(), universe[i].end(), T_cell{ 0 });
			}
			rows = new_rows;
		}

		if (resize_at_left_test()) {
			std::size_t new_cols = (std::size_t)cols * 1.5;
			for (auto& row : universe) {
				row.resize(new_cols, T_cell{ 0 });
				std::copy_backward(&row[0], &row[cols - 1], &row[new_cols - 1]);
				std::fill(&row[0], &row[new_cols - cols], T_cell{ 0 });
			}
			cols = new_cols;
		}

	}

	string Simple_Implementation::to_s(void) const {
		ostringstream os;
		for (const auto row : universe) {
			for (const auto cell : row) {
				os << (int)cell;
			}
			os << "\n";
		}

		return os.str();
	}

	Simple_Implementation Simple_Implementation::operator()(std::size_t row, std::size_t col, std::size_t width, std::size_t height) const {
		//Returns a "slice" of the actual universe.
		//Warning: can generate an invalid universe
		//TODO: test
		
		//sanity check:
		if (row + height >= rows || col + width >= cols) {
			throw(HLException("Invalid Dimensions", 5));
		}

		auto new_universe = Simple_Implementation(width, height);

		//This can be optimized, but slice operator is designed to casual uses
		for (auto i = row; i < row + height; ++i)
			for (auto j = col; j < col + width; ++j)
				new_universe.set(i - row, j - col, universe[i][j]);
		
		return new_universe;
	}

	std::ostream& operator<<(std::ostream& os, const Simple_Implementation& obj)
	{
		// write obj to stream
		os << obj.to_s();
		return os;
	}

}