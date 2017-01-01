/* rpsrbt.hpp */

#ifndef __RPSRBT_RPSRBT_HPP__
#define __RPSRBT_RPSRBT_HPP__

#ifndef __RPSRBT_OPENFILE_HPP__
#include <openfile.hpp>
#endif

#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <unordered_set>

// // Single Run
// class SRun {
// private:
//   string _label;  // label on a Pointed Run-Based Trie
//   unsigned _num;  // rule number
//   bool term;      // mark whether terminal Rule 
//                   // e.g the node of the run of Rule 1 must be terminal node
// public:
//   SRun() { }
//   SRun(string l, unsigned n, bool t) {
//     _label = l;
//     _num = n;
//     _term = t;
//   }
//   ~SRun() { /* printf("call the Single Run deconstructor.\n"); */ }
//   string getLabel() { return _label; }
//   unsigned getNum() { return _num; }
// };

// a node for Reduced Pointed Single Run-Based Trie
class RPSRBT {
private:
  static long unsigned _number_of_node_of_rpsrbt;
  unsigned _var;  // var of the root node is 0
  bool _rule;     // if there is Rule(Run) i then _rule = i;
  bool _term;     // mark whether terminal Rule 
                  // e.g the node of the run of Rule 1 must be terminal node
  list<RPSRBT*> _parents;
  RPSRBT* _left;
  RPSRBT* _right;
public:
  RPSRBT() { }
  ~RPSRBT() { /* printf("call the RPSRBT deconstructor.\n"); */ }
  RPSRBT(unsigned v) {
    _var = v;
    _rule = 0;
    _left = _right = NULL;
    _term = false;
  }
  RPSRBT(unsigned v, unsigned r, bool t) {
    _var = v;
    _rule = r;
    _left = _right = NULL;
    _term = t;
  }
};

#endif
