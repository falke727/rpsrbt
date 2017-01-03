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
  unsigned size() { return _label.size(); }
  unsigned getNum() { return _num; }
  unsigned getStartPosition() { return _spos; }
  void setTermTrue() { _term = true; }
};

class RPSRBTNode {
private:
  // friend ostream& operator<<(ostream&, const RPSRBTNode&);
  unsigned _var;  // var of the terminal node is 0
  unsigned _rule; // if a node has Rule(Run) i then _rule = i
  string _label;
  bool _term;     // mark whether terminal Rule 
                  // e.g the node of the run of Rule 1 must be terminal node
  list<RPSRBTNode*> _parents;
  RPSRBTNode* _left;
  RPSRBTNode* _right;
public:
  RPSRBTNode() { }
  RPSRBTNode(unsigned &, string);
  ~RPSRBTNode() { /* printf("call the RPSRBTNode deconstructor.\n"); */ }
  void initRootNode(unsigned);
  void initTerminalNode();
  void setTermTrue() { _term = true; }
  void setVar(unsigned v) { _var = v; }
  void setLeft(RPSRBTNode *l) { _left = l; }
  void setRight(RPSRBTNode *r) { _right = r; }
  void addRule(unsigned n) { _rule = n; }
  unsigned getVar() { return _var; }
  string getLabel() { return _label; }
  unsigned getRule() { return _rule; }
  RPSRBTNode* getLeft() { return _left; }
  RPSRBTNode* getRight() { return _right; }
};

class RPSRBT {
private:
  vector<RPSRBTNode*> roots;
  RPSRBTNode* terminal; // terminal node which var is n+1 
  static long unsigned _number_of_node_of_rpsrbt;
  static vector<unsigned> _variable_order;
  // friend ostream& operator<<(ostream&, const RPSRBT&);
public:
  RPSRBT() { } /* default constructor */
  RPSRBT(list<Rule> &);
  ~RPSRBT() { /* printf("call the RPSRBT deconstructor.\n"); */ }
  SRun cutOutSingleRunFromRule(Rule&);
  void makeBackboneRPSRBT(SRun);
  void connectT4ToTerminalNode();
  void addPointers();
  void traverseAndAddPointer(RPSRBTNode*, unsigned);
  void traverse();
  void preOrder(RPSRBTNode*);
  static void incNumOfNode() { ++_number_of_node_of_rpsrbt; }
};

#endif
