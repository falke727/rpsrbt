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

// a node for Reduced Pointed Single Run-Based Trie

// // Single Run
class SRun {
private:
  string _label;  // label on a Pointed Run-Based Trie
  unsigned _num;  // rule number
  unsigned _spos; // start position of the run
  bool _term;     // mark whether terminal Rule 
                  // e.g the node of the run of Rule 1 must be terminal node
  friend ostream& operator<<(ostream&, const SRun&);
public:
  SRun() { }
  SRun(string, unsigned, unsigned);
  ~SRun() { /* printf("call the Single Run deconstructor.\n"); */ }
  string getLabel() { return _label; }
  unsigned getNum() { return _num; }
  void setTermTrue() { _term = true; }
};

class RPSRBT {
private:
  static long unsigned _number_of_node_of_rpsrbt;
  // friend ostream& operator<<(ostream&, const RPSRBT&);
public:
  RPSRBT() { } /* default constructor */
  RPSRBT(list<Rule> &);
  ~RPSRBT() { /* printf("call the RPSRBT deconstructor.\n"); */ }
  SRun cutOutSingleRunFromRule(Rule &);
};

class RPSRBTNode {
private:
  // friend ostream& operator<<(ostream&, const RPSRBTNode&);
  unsigned _var;  // var of the root node is 0
  bool _rule;     // if there is Rule(Run) i then _rule = i;
  bool _term;     // mark whether terminal Rule 
                  // e.g the node of the run of Rule 1 must be terminal node
  list<RPSRBTNode*> _parents;
  RPSRBTNode* _left;
  RPSRBTNode* _right;
public:
  RPSRBTNode(unsigned *&);
  RPSRBTNode(unsigned *&, unsigned *&, bool *&);
};

#endif
