/*
Simple implementation of the Conway's Game of Life.


*/
#include <string>
#include <sstream>
#include <vector>

using namespace std;

namespace simple_implementation {

	typedef unsigned char T_cell;

	class HLException {
	private:
		string desc;
		int number;

	public:
		HLException(string desc = "", int number = 0) {
			this->desc = desc;
			this->number = number;
		}

		string to_string() {
			ostringstream os;
			os << number << ": " << desc;
			return os.str();
		}
	};


	class Simple_Implementation
	{
	private:
		vector<vector<T_cell>> universe; //se crea un nuevo objeto, vacío en principio

		std::size_t cols, rows;
		int survive_min, survive_max, born_min, born_max;
		std::string simulation_id;
		void run_1();



	public:
		//Test if resize is required at row 0
		bool resize_at_bottom_test();
		bool resize_at_left_test();
		bool resize_at_top_test();
		bool resize_at_right_test();

		Simple_Implementation();
		Simple_Implementation(std::size_t cols, std::size_t rows);
		Simple_Implementation(std::string simulation_id, std::size_t cols, std::size_t rows);
		~Simple_Implementation() {};

		void ruleset(unsigned int survive_min = 2, unsigned int survive_max = 3, unsigned int born_min = 3, unsigned int born_max = 3);
		void randomize(float ratio = 0.25);
		void run(int n=1);
		void set(std::size_t row, std::size_t col, int value);
		void test_and_resize();
		Simple_Implementation operator()(std::size_t row, std::size_t col, std::size_t width=0, std::size_t height=0) const;
		string to_s(void) const;

		//returns iterators for rows. For each row, a column iterator must be implemented
		auto cbegin() { return universe.cbegin(); };
		auto cend() { return universe.cend(); };

		std::string get_id() const { return std::string(simulation_id); };
		std::size_t get_rows() const { return rows; };
		std::size_t get_cols() const { return cols; };
	};

	std::ostream& operator<<(std::ostream& os, const Simple_Implementation& obj);

}