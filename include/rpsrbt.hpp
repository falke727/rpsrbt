/* rpsrbt.hpp */

#ifndef __RPSRBT_RPSRBT_HPP__
#define __RPSRBT_RPSRBT_HPP__

#ifndef __RPSRBT_OPENFILE_HPP__
#include <openfile.hpp>
#endif

//#include <bits/stdc++.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include <unordered_map>

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
  int _tn;        // Trie number T[tn]
  unsigned _candidate; // higest candaite rule in the subtree
  string _label;
  bool _term;     // mark whether terminal Rule 
                  // e.g the node of the run of Rule 1 must be terminal node
  // list<RPSRBTNode*>* _parents;
  bool _reach;    // Whether is this node reachble from the root of RPSRBT[0]
  list<RPSRBTNode*> _parents;
  RPSRBTNode* _left;
  RPSRBTNode* _right;
public:
  RPSRBTNode() { }
  RPSRBTNode(unsigned &, int, string);
  RPSRBTNode(unsigned &, int, unsigned, string, RPSRBTNode*);
  RPSRBTNode(unsigned &, int, string, bool);
  RPSRBTNode(unsigned &, int, unsigned, string, bool, RPSRBTNode*);
  ~RPSRBTNode() { /* printf("call the RPSRBTNode deconstructor.\n"); */ }
  void initRootNode(unsigned, int);
  void initTerminalNode();
  void setTermTrue() { _term = true; }
  void setReachTrue() { _reach = true; }
  void setVar(unsigned v) { _var = v; }
  void setLeft(RPSRBTNode *l) { _left = l; }
  void setRight(RPSRBTNode *r) { _right = r; }
  void addRule(unsigned n) { _rule = n; }
  void addParent(RPSRBTNode *p) { _parents.push_back(p); }
  void updateCandidate(unsigned c) { _candidate = c; }
  void deleteParent(RPSRBTNode *p) { _parents.remove(p); }
  bool isTerm() { return _term; }
  bool isReachable() { return _reach; }
  unsigned getVar() { return _var; }
  string getLabel() { return _label; }
  unsigned getRule() { return _rule; }
  int getTrieNumber() { return _tn; }
  unsigned getCandidate() { return _candidate; }
  RPSRBTNode* getLeft() { return _left; }
  RPSRBTNode* getRight() { return _right; }
  list<RPSRBTNode*> getParents() { return _parents; }
};

struct LRPair {
  RPSRBTNode*l, *r;
  bool friend operator==(const LRPair &lhs, const LRPair &rhs) {
    return lhs.l == rhs.l && lhs.r == rhs.r;
  }
};

namespace std {
  template <>
  struct hash<LRPair> {
    size_t operator()(const LRPair &pair) const {
      size_t seed = 0;
      auto l_hash = hash<RPSRBTNode*>()(pair.l);
      auto r_hash = hash<RPSRBTNode*>()(pair.r);

      seed ^= l_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      seed ^= r_hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
      return seed;
    }
  };
}

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
  void traverseAndAddPointer(RPSRBTNode*);
  void lowTrieTraverseAndAddPointer(RPSRBTNode*, bool);
  void checkReachableAndUpdateCandidate();
  void traverseForCheckReachableAndUpdateCandidate(RPSRBTNode*, unordered_set<RPSRBTNode*>);
  void makeTerminalNodes();
  void traverseForMakeTerminalNodes(RPSRBTNode*, unordered_set<RPSRBTNode*>);
  void nodeShareReduction();
  void traverseForNodeShareReduction(RPSRBTNode*, unordered_map<LRPair, RPSRBTNode*>);
  void directConnectReduction(RPSRBTNode*);
  RPSRBTNode* getRoot() { return roots[0]; }
  static void incNumOfNode() { ++_number_of_node_of_rpsrbt; }
  /* functions for debug */
  void traverse();
  void preOrder(RPSRBTNode*);
  void preOrder2(RPSRBTNode*, int);

};

#endif
