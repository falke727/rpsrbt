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
  //_parents = NULL;
  _left = _right = NULL;
  _term = false;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, unsigned c, string l, RPSRBTNode* p){
  _var = v;
  _rule = 0;
  _tn = tn;
  _candidate = c;
  _label = l;
  _parents.push_back(p);
  // if (NULL == _parents) {
  //   cout << "[" << tn << "] -- p: " << p->getLabel() << " c: " << _label << endl;
  //   _parents = new list<RPSRBTNode*>;
  // }
  // _parents->push_back(p);
  _left = _right = NULL;
  _term = false;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, string l, bool t){
  _var = v;
  _rule = 0;
  _tn = tn;
  _label = l;
  // _parents = NULL;
  _left = _right = NULL;
  _term = t;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, unsigned c, string l, bool t, RPSRBTNode* p){
  _var = v;
  _rule = 0;
  _tn = tn;
  _candidate = c;
  _label = l;
  _parents.push_back(p);
  // if (NULL == _parents) {
  //   cout << "[" << tn << "] -- p: " << p->getLabel() << " c: " << _label << endl;
  //   _parents = new list<RPSRBTNode*>;
  // }
  // _parents->push_back(p);
  _left = _right = NULL;
  _term = t;
}

void RPSRBTNode::initRootNode(unsigned v, int t) {
  _var = v;
  _rule = 0;
  _tn = t;
  _candidate = Rule::getNumberOfRule()+1;
  _label = "root";
  _term = false;
  // _parents = NULL;
  _left = _right = NULL;
}

void RPSRBTNode::initTerminalNode() {
  _var = 0;
  _rule = Rule::getNumberOfRule()+1;
  _tn = -1;
  _candidate = Rule::getNumberOfRule()+1;
  _label = "terminal";
  _term = true;
  // _parents = NULL;
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
  addPointers();
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
  unsigned candidate = run.getNum();
  unsigned l = label.size();
  unsigned s = run.getStartPosition()-1;
  vector<unsigned> ord = Rule::getVariableOrder();

  // cout << s << ": " << label << endl;
  RPSRBTNode* ptr = roots[s];

  for (unsigned i = 0; i < l-1; ++i) {
    if ('0' == label[i]) {
      if (NULL == ptr->getLeft()) {
	ptr->setLeft(new RPSRBTNode(ord[s+i+1],s,candidate,label.substr(0,i+1),ptr));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getLeft();
    }
    else {
      if (NULL == ptr->getRight()) {
	ptr->setRight(new RPSRBTNode(ord[s+i+1],s,candidate,label.substr(0,i+1),ptr));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getRight();
    }
  }

  /* make the last node of this run */
  unsigned zero = 0;
  if ('0' == label[l-1]) {
    if (NULL == ptr->getLeft()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setLeft(new RPSRBTNode(zero,s,candidate,label,true,ptr));
      }
      else { ptr->setLeft(new RPSRBTNode(ord[s+l],s,candidate,label,ptr)); }
      RPSRBT::incNumOfNode();
    }
    ptr = ptr->getLeft();
  }
  else {
    if (NULL == ptr->getRight()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setRight(new RPSRBTNode(zero,s,candidate,label,true,ptr));
      } else { ptr->setRight(new RPSRBTNode(ord[s+l],s,candidate,label,ptr)); }
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
    //cout << "=========================" << endl;
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
void RPSRBT::lowTrieTraverseAndAddPointer(RPSRBTNode* high, bool d) {
  //cout << high->getTrieNumber()+1 << "," << high->getLabel() << " --> " << d << endl;
  string label = high->getLabel();
  RPSRBTNode* low;
  unsigned j;
  for (int i = high->getTrieNumber(); i < (int)roots.size()-1; ++i) {
    low = roots[i+1];
    for (j = 1; j < label.size(); ++j) {
      if ('0' == label[j] && NULL != low->getLeft()) { low = low->getLeft(); }
      else if ('1' == label[j] && NULL != low->getRight()) { low = low->getRight(); }
      else { break ; }
    }
    if (j == label.size()) {
      if (d) { high->setRight(low->getRight()); }
      else { high->setLeft(low->getLeft()); }
      break;
    }
  }
}

void RPSRBT::traverse() {
  // preOrder(roots[0]);
  for (auto ptr : roots) {
    cout << "=========================" << endl;
    preOrder2(ptr, ptr->getTrieNumber());
  }
}

void RPSRBT::preOrder(RPSRBTNode* ptr) {
  if (NULL == ptr) { return ; }
  cout << "[" << ptr->getTrieNumber()+1 << "] " << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << ", term = " << ptr->isTerm() << ", candidate = " << ptr->getCandidate() << endl;
  preOrder(ptr->getLeft());
  preOrder(ptr->getRight());
}

void RPSRBT::preOrder2(RPSRBTNode* ptr, int i) {
  if (NULL == ptr) { return ; }
  cout << "[" << ptr->getTrieNumber()+1 << "] " << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << ", term = " << ptr->isTerm() << ", candidate = " << ptr->getCandidate() << endl;
  if (i != ptr->getTrieNumber()) { return ; }
  preOrder2(ptr->getLeft(), i);
  preOrder2(ptr->getRight(), i);
}
