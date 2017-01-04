/* rpsrbt.cpp */

#include <rpsrbt.hpp>

SRun::SRun(string l, unsigned n, unsigned p) {
  _label = l;
  _num = n;
  _spos = p;
}

ostream& operator<<(ostream &strm, const SRun &run) {
  return strm << run._num << ": " << run._label << ", " << run._spos << endl;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, string l){
  _var = v;
  _rule = 0;
  _tn = tn;
  _label = l;
  _left = _right = NULL;
  _term = false;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, string l, bool t){
  _var = v;
  _rule = 0;
  _tn = tn;
  _label = l;
  _left = _right = NULL;
  _term = t;
}

void RPSRBTNode::initRootNode(unsigned v, int t) {
  _var = v;
  _rule = 0;
  _tn = t;
  _label = "root";
  _term = false;
  _left = _right = NULL;
}

void RPSRBTNode::initTerminalNode() {
  _var = 0;
  _rule = Rule::getNumberOfRule()+1;
  _tn = -1;
  _label = "terminal";
  _term = true;
  _left = _right = NULL;
}

RPSRBT::RPSRBT(list<Rule> &rulelist) {
  unsigned w = Rule::getLengthOfRule();
  //unsigned n = Rule::getNumberOfRule();
  vector<unsigned> ord = Rule::getVariableOrder();

  for (unsigned i = 0; i < w; ++i) {
    roots.push_back(new RPSRBTNode());
    roots[i]->initRootNode(ord[i], i);
  }

  for (auto r : rulelist) {
    SRun run = cutOutSingleRunFromRule(r);
    makeBackboneRPSRBT(run);
  }

  connectT4ToTerminalNode();
  //addPointers();
}

SRun RPSRBT::cutOutSingleRunFromRule(Rule &rule) {
  unsigned i = 1, start;
  string r = "";
  bool b = false;
  string s = rule.getRuleBitString();

  for (auto c : s) {
    if (c != '*') {
      if (!b)
	b = true, start = i;
      r += c;
    }
    ++i;
  }

  return SRun(r, rule.getRuleNumber(), start);
}

void RPSRBT::makeBackboneRPSRBT(SRun run) {
  string label = run.getLabel();
  unsigned l = label.size();
  unsigned s = run.getStartPosition()-1;
  vector<unsigned> ord = Rule::getVariableOrder();

  // cout << s << ": " << label << endl;
  RPSRBTNode* ptr = roots[s];

  for (unsigned i = 0; i < l-1; ++i) {
    if ('0' == label[i]) {
      if (NULL == ptr->getLeft()) {
	ptr->setLeft(new RPSRBTNode(ord[s+i+1],s,label.substr(0,i+1)));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getLeft();
    }
    else {
      if (NULL == ptr->getRight()) {
	ptr->setRight(new RPSRBTNode(ord[s+i+1],s,label.substr(0,i+1)));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getRight();
    }
  }
  //RPSRBTNode* tmp = ptr;

  /* make the last node of this run */
  unsigned zero = 0;
  if ('0' == label[l-1]) {
    if (NULL == ptr->getLeft()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setLeft(new RPSRBTNode(zero,s,label, true));
      }
      else { ptr->setLeft(new RPSRBTNode(ord[s+l],s,label)); }
      RPSRBT::incNumOfNode();
    }
    ptr = ptr->getLeft();
  }
  else {
    if (NULL == ptr->getRight()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setRight(new RPSRBTNode(zero,s,label, true));
      } else { ptr->setRight(new RPSRBTNode(ord[s+l],s,label)); }
      RPSRBT::incNumOfNode();
    }
    ptr = ptr->getRight();
  }
  ptr->addRule(run.getNum());
}

void RPSRBT::connectT4ToTerminalNode() {
  terminal = new RPSRBTNode();
  terminal->initTerminalNode();
  unsigned w = Rule::getLengthOfRule();
  if (NULL == roots[w-1]->getLeft()) { roots[w-1]->setLeft(terminal); }
  if (NULL == roots[w-1]->getRight()) { roots[w-1]->setRight(terminal); }
}

void RPSRBT::addPointers() {
  unsigned w = roots.size();
  for (int i = w-2; 0 <= i; --i) {
    cout << "=========================" << endl;
    traverseAndAddPointer(roots[i]);
  }
}

void RPSRBT::traverseAndAddPointer(RPSRBTNode* ptr) {
  if (NULL == ptr || ptr->isTerm()) { return ; }
  if (NULL == ptr->getLeft()) {
    if ("root" == ptr->getLabel()) {
      ptr->setLeft(roots[ptr->getTrieNumber()+1]);
    }
    else {
      lowTrieTraverseAndAddPointer(ptr, false);
    }
  }
  if (NULL == ptr->getRight()) {
    if ("root" == ptr->getLabel()) {
      ptr->setRight(roots[ptr->getTrieNumber()+1]);
    }
    else {
      lowTrieTraverseAndAddPointer(ptr, true);
    }
  }
  traverseAndAddPointer(ptr->getLeft());
  traverseAndAddPointer(ptr->getRight());
}

// if d = false then Left, d = true then Right
void RPSRBT::lowTrieTraverseAndAddPointer(RPSRBTNode* ptr, bool d) {
  cout << ptr->getTrieNumber()+1 << "," << ptr->getLabel() << " --> " << d << endl;
}

void RPSRBT::traverse() {
  for (auto ptr : roots) {
    cout << "=========================" << endl;
    preOrder(ptr);
  }
}

void RPSRBT::preOrder(RPSRBTNode* ptr) {
  if (NULL == ptr) { return ; }
  cout << "[" << ptr->getTrieNumber()+1 << "] " << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << ", term = " << ptr->isTerm() << endl;
  preOrder(ptr->getLeft());
  preOrder(ptr->getRight());
}
