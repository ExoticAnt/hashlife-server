

#define BOOST_TEST_MODULE Simple_Implementation test

#include <string>
#include <iostream>
#include <vector>
#include <boost/test/included/unit_test.hpp>

//quick and dirty trick to test private members
#define private public

#include "simple_implementation.h"

using namespace std;

BOOST_AUTO_TEST_CASE(constructors_test)
{

	//auto universe = Simple_Implementation(cols, rows);
	//checks for minimum dimensions
	BOOST_CHECK_THROW(Simple_Implementation(1,1), HLException);
	BOOST_CHECK_THROW(Simple_Implementation(5, 1), HLException);
	BOOST_CHECK_THROW(Simple_Implementation(0, 10), HLException);

	//invalid access:
	auto universe = Simple_Implementation(4, 4);
	BOOST_CHECK_THROW(universe.set(5, 0, 1), HLException);
	BOOST_CHECK_THROW(universe.set(0, 5, 1), HLException);

	//simple set and to string
	universe = Simple_Implementation(4, 4);
	universe.set(2, 3, 1);
	BOOST_TEST(universe.to_s() == "0000\n0000\n0001\n0000\n");
}

BOOST_AUTO_TEST_CASE(life_test)
{
	//simple check of the ruleset and execution
	//an horizontal len 3 bar is set. In 1 step it changes to a vertical bar.
	//also is in a corner
	auto universe = Simple_Implementation(5, 5);
	universe.set(3, 2, 1);
	universe.set(3, 3, 1);
	universe.set(3, 4, 1);

	universe.run();

	BOOST_TEST(universe.to_s() == "00000\n00000\n00010\n00010\n00010\n");

	//a simple dot dissapears after 1 step.
	universe = Simple_Implementation(5, 5);
	universe.set(2, 2, 1);

	universe.run();

	BOOST_TEST(universe.to_s() == "00000\n00000\n00000\n00000\n00000\n");

	//a square persists in time.
	universe = Simple_Implementation(5, 5);
	universe.set(1, 1, 1);
	universe.set(1, 2, 1);
	universe.set(2, 1, 1);
	universe.set(2, 2, 1);

	universe.run();

	BOOST_TEST(universe.to_s() == "00000\n01100\n01100\n00000\n00000\n");

}

BOOST_AUTO_TEST_CASE(data_x_test)
{
	BOOST_TEST(1 == 1); 
	auto universe = Simple_Implementation(4, 4);

	BOOST_TEST(!universe.resize_at_bottom_test());
	BOOST_TEST(!universe.resize_at_top_test());
	BOOST_TEST(!universe.resize_at_left_test());
	BOOST_TEST(!universe.resize_at_right_test());

	universe.set(0, 1, 1);
	BOOST_TEST((universe.resize_at_bottom_test() && !universe.resize_at_top_test() && !universe.resize_at_left_test() && !universe.resize_at_right_test()));

	universe.set(0, 1, 0);
	universe.set(1, 0, 1);
	BOOST_TEST((!universe.resize_at_bottom_test() && !universe.resize_at_top_test() && universe.resize_at_left_test() && !universe.resize_at_right_test()));

	universe.set(1, 0, 0);
	universe.set(3, 1, 1);
	BOOST_TEST((!universe.resize_at_bottom_test() && universe.resize_at_top_test() && !universe.resize_at_left_test() && !universe.resize_at_right_test()));

	universe.set(3, 1, 0);
	universe.set(1, 3, 1);
	BOOST_TEST((!universe.resize_at_bottom_test() && !universe.resize_at_top_test() && !universe.resize_at_left_test() && universe.resize_at_right_test()));

	//resize at top:
	universe = Simple_Implementation(4, 4);
	universe.set(3, 1, 1);
	universe.test_and_resize();
	BOOST_TEST(universe.to_s() == "0000\n0000\n0000\n0100\n0000\n0000\n");

	//resise at right:
	universe = Simple_Implementation(4, 4);
	universe.set(1, 3, 1);
	universe.test_and_resize();
	BOOST_TEST(universe.to_s() == "000000\n000100\n000000\n000000\n");
	
	//reize at bottom
	universe = Simple_Implementation(4, 4);
	universe.set(0, 1, 1);
	universe.set(1, 1, 2);
	universe.set(2, 1, 3);
	universe.test_and_resize();
	BOOST_TEST(universe.to_s() == "0000\n0000\n0100\n0200\n0300\n0000\n");

	//reize at left
	universe = Simple_Implementation(4, 4);
	universe.set(1, 0, 1);
	universe.set(2, 0, 2);
	universe.test_and_resize();
	BOOST_TEST(universe.to_s() == "000000\n001000\n002000\n000000\n");

}
