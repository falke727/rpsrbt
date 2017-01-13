/* main.hpp */

#ifndef __RPSRBT_OPENFILE_HPP__
#include <openfile.hpp>
#endif

#ifndef __RPSRBT_RPSRBT_HPP__
#include <rpsrbt.hpp>
#endif

#ifndef __RPSRBT_CLASSIFY_HPP__
#include <classify.hpp>
#endif

// #ifndef __CLASIFICATION_CHECK_HPP__
// #include <check.hpp>
// #endif

// #ifndef __CLASIFICATION_MEASURE_HPP__
// #include <measure.hpp>
// #endif

double RPSRBT::_construct_time_of_rpsrbt;
long unsigned Result::_compareNumberOfSequential = 0;
long unsigned Result::_compareNumberOfRPSRBT = 0;
double Result::_latency_of_sequential = 0;
double Result::_latency_of_rpsrbt = 0;
long unsigned RPSRBT::_number_of_node_of_rpsrbt = 0;
unsigned Rule::_number_of_rule = 0;
vector<unsigned> Rule::_variable_order;
unsigned Rule::_length_of_rule;
