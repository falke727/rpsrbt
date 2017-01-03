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

RPSRBTNode::RPSRBTNode(unsigned &v, string l){
  _var = v;
  _rule = 0;
  _label = l;
  _left = _right = NULL;
  _term = false;
}

void RPSRBTNode::initRootNode(unsigned v) {
  _var = v;
  _rule = 0;
  _label = "root";
  _term = false;
  _left = _right = NULL;
}

void RPSRBTNode::initTerminalNode() {
  _var = 0;
  _rule = Rule::getNumberOfRule()+1;
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
    roots[i]->initRootNode(ord[i]);
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
  unsigned l = label.size();
  unsigned s = run.getStartPosition()-1;
  vector<unsigned> ord = Rule::getVariableOrder();

  // cout << s+1 << ": " << label << endl;
  RPSRBTNode* ptr = roots[s];
  
  for (unsigned i = s, j = 1; i < s+l-1; ++i, ++j) {
    if ('0' == label[i]) {
      if (NULL == ptr->getLeft()) {
	ptr->setLeft(new RPSRBTNode(ord[i+1],label.substr(0,j)));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getLeft();
    }
    else {
      if (NULL == ptr->getRight()) {
	ptr->setRight(new RPSRBTNode(ord[i+1],label.substr(0,j)));
	RPSRBT::incNumOfNode();
      }
      ptr = ptr->getRight();
    }
  }

  /* make the last node of this run */
  unsigned zero = 0;
  if ('0' == label[s+l-1]) {
    if (NULL == ptr->getLeft()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setLeft(new RPSRBTNode(zero,label));
      }
      else {
	ptr->setLeft(new RPSRBTNode(ord[l],label));
      }
      RPSRBT::incNumOfNode();
    }
    ptr = ptr->getLeft();
  }
  else {
    if (NULL == ptr->getRight()) {
      if (l == Rule::getLengthOfRule()-s) {
	ptr->setRight(new RPSRBTNode(zero,label));
      }
      else {
	ptr->setRight(new RPSRBTNode(ord[l],label));
      }
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
  for (auto ptr : roots)
    traverseAndAddPointer(ptr, 1);
}

void RPSRBT::traverseAndAddPointer(RPSRBTNode* ptr, unsigned i) {

  if (NULL == ptr) { return ; }

  if (NULL == ptr->getLeft())
    printf("hoge\n");
  if (NULL == ptr->getRight())
    printf("hoge\n");
}

void RPSRBT::traverse() {
  for (auto ptr : roots) {
    preOrder(ptr);
  }
}

void RPSRBT::preOrder(RPSRBTNode* ptr) {
  if (NULL == ptr) { return ; }
  cout << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << endl;
  preOrder(ptr->getLeft());
  preOrder(ptr->getRight());
}
