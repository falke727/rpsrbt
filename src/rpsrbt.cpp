/* rpsrbt.cpp */

#include <rpsrbt.hpp>

SRun::SRun(string l, unsigned n, unsigned p) {
  _label = l;
  _num = n;
  _spos = p;
  _term = false;
}

ostream& operator<<(ostream &strm, const SRun &run) {
  return strm << run._num << ": " << run._label << ", " << run._spos << endl;
}

RPSRBTNode::RPSRBTNode(unsigned &v, int tn, string l){
  _var = v;
  _rule = 0;
  _tn = tn;
  _label = l;
  _reach = false;
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
  _reach = false;
  _parents.insert(p);
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
  _reach = false;
  _parents.insert(p);
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
  _term = _reach = false;
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
  _reach = false;
  // _parents = NULL;
  _left = _right = NULL;
}

RPSRBT::RPSRBT(list<Rule> &rulelist) {

  // auto start = system_clock::now();
  unsigned w = Rule::getLengthOfRule();
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
  addPointersAndRules();
  checkReachableAndUpdateCandidate();
  makeTerminalNodes();
  // nodeShareReduction();
  directConnectReduction();
  // auto end = system_clock::now();
  // auto dur = end - start;
  // auto sec = duration_cast<std::chrono::microseconds>(dur).count();
  // cout << sec / (pow(10,6)) << endl;
}

SRun RPSRBT::cutOutSingleRunFromRule(Rule &rule) {
  unsigned i = 1, start = 1;
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
  if (NULL == roots[w-1]->getLeft()) {
    roots[w-1]->setLeft(terminal); 
    terminal->addParent(roots[w-1]);
  }
  if (NULL == roots[w-1]->getRight()) { 
    roots[w-1]->setRight(terminal);
    terminal->addParent(roots[w-1]);
  }
}

void RPSRBT::addPointersAndRules() {
  unsigned w = roots.size();

  for (unsigned i = 0; i < w-1; ++i)
    connectRootToRoot(roots[i]);

  for (int i = w-2; 0 <= i; --i) {
    // cout << "========== " << i << " ==========" << endl;
    traverseAndAddPointerAndRule(roots[i], roots[i]->getTrieNumber());
  }
}

void RPSRBT::connectRootToRoot(RPSRBTNode* ptr) {
  if (NULL == ptr->getLeft()) {
    ptr->setLeft(roots[ptr->getTrieNumber()+1]);
    roots[ptr->getTrieNumber()+1]->addParent(ptr);
  }
  if (NULL == ptr->getRight()) {
    ptr->setRight(roots[ptr->getTrieNumber()+1]);
    roots[ptr->getTrieNumber()+1]->addParent(ptr);
  }
}

void RPSRBT::traverseAndAddPointerAndRule(RPSRBTNode* ptr, int i) {
  if (NULL == ptr || ptr->isTerm()) { return ; }
  if (i != ptr->getTrieNumber()) { return ; }
  traverseAndAddPointerAndRule(ptr->getLeft(), i);
  traverseAndAddPointerAndRule(ptr->getRight(), i);

  lowTrieTraverseAndAddPointerAndRule(ptr);
}

void RPSRBT::lowTrieTraverseAndAddPointerAndRule(RPSRBTNode* high) {
  // cout << high->getTrieNumber()+1 << "," << high->getLabel() << endl;
  string label = high->getLabel();
  RPSRBTNode* low;
  unsigned j = 1, l;
  for (int i = high->getTrieNumber(); i < (int)roots.size()-1; ++i, ++j) {
    low = roots[i+1];
    for (l = j; l < label.size(); ++l) {
      if ('0' == label[l] && NULL != low->getLeft()) { low = low->getLeft(); }
      else if ('1' == label[l] && NULL != low->getRight()) { low = low->getRight(); }
      else { break ; }
    }
    if (l == label.size()) {
      if (!high->isTerm()) {
	if (NULL == high->getLeft()) {
	  // cout << "L : high[" << high->getTrieNumber()+1 << "] - " << high->getLabel() << endl;
	  // cout << "L : low[" << low->getTrieNumber()+1 << "] - " << low->getLabel() << endl;
	  high->setLeft(low->getLeft());
	  (low->getLeft())->addParent(high);
	}
	if (NULL == high->getRight()) {
	  // cout << "R : high[" << high->getTrieNumber()+1 << "] - " << high->getLabel() << endl;
	  // cout << "R : low[" << low->getTrieNumber()+1 << "] - " << low->getLabel() << endl;
	  high->setRight(low->getRight());
	  (low->getRight())->addParent(high);
	}
      }
      if (low->getRule() > 0 && 0 == high->getRule()) // Is this condition ok?
	high->addRule(low->getRule());
      break;
    }
  }
}

void RPSRBT::checkReachableAndUpdateCandidate() {
  for (auto ptr : roots)
    traverseForCheckReachableAndUpdateCandidate(ptr, ptr->getTrieNumber());
}

void RPSRBT::traverseForCheckReachableAndUpdateCandidate(RPSRBTNode* ptr, int i) {
  if (NULL == ptr) { return ; }

  if (NULL != ptr->getLeft())
    if ((ptr->getLeft())->getCandidate() < ptr->getCandidate())
      ptr->updateCandidate((ptr->getLeft())->getCandidate());
  if (NULL != ptr->getRight())
    if ((ptr->getRight())->getCandidate() < ptr->getCandidate())
      ptr->updateCandidate((ptr->getRight())->getCandidate());

  if (0 == ptr->getTrieNumber()) {
    ptr->setReachTrue();
  }

  if (ptr->isReachable() && !ptr->isTerm()) {
    (ptr->getLeft())->setReachTrue();
    (ptr->getRight())->setReachTrue();
  }

  if (i != ptr->getTrieNumber()) { return ; }

  traverseForCheckReachableAndUpdateCandidate(ptr->getLeft(), i);
  traverseForCheckReachableAndUpdateCandidate(ptr->getRight(), i);
}

void RPSRBT::makeTerminalNodes() {
  for (auto ptr : roots)
    traverseForMakeTerminalNodes(ptr, ptr->getTrieNumber());
}

void RPSRBT::traverseForMakeTerminalNodes(RPSRBTNode* ptr, int i) {
  if (NULL == ptr || ptr->isTerm()) { return ; }
  if (i != ptr->getTrieNumber()) { return ; }

  if (ptr->getRule() > 0) {
    if ((ptr->getLeft())->getCandidate() > ptr->getCandidate()) {
      if ((ptr->getRight())->getCandidate() > ptr->getCandidate()) {
	(ptr->getLeft()->deleteParent(ptr));
	(ptr->getRight()->deleteParent(ptr));
	ptr->setLeft(NULL), ptr->setRight(NULL);
	ptr->setTermTrue();
      }
    }
  }
  else {
    traverseForMakeTerminalNodes(ptr->getLeft(), i);
    traverseForMakeTerminalNodes(ptr->getRight(), i);
  }
}

void RPSRBT::nodeShareReduction() {
  unordered_map<LRPair,RPSRBTNode*> D;
  for (auto ptr : roots)
    traverseForNodeShareReduction(ptr, D, ptr->getTrieNumber());

}

// This subroutine is incomplete.
void RPSRBT::traverseForNodeShareReduction(RPSRBTNode* ptr, unordered_map<LRPair, RPSRBTNode*> D, int i) { 
  if (NULL == ptr || ptr->isTerm()) { return ; }
  if (i != ptr->getTrieNumber()) { return ; }

  LRPair left_right;
  left_right.l = ptr->getLeft();
  left_right.r = ptr->getRight();

  auto it = D.find(left_right);
  if(it != D.end()) { 
    cout << "Share!\n";
  }
  D[left_right] = ptr;  

  traverseForNodeShareReduction(ptr->getLeft(), D, i);
  traverseForNodeShareReduction(ptr->getRight(), D, i);
}

void RPSRBT::directConnectReduction() {
  for (auto ptr = roots.rbegin(); ptr != roots.rend(); ++ptr)
    traverseForDirectConnectReduction(roots[(*ptr)->getTrieNumber()], (*ptr)->getTrieNumber());
  // for (auto ptr : roots)
  //   traverseForDirectConnectReduction(roots[ptr->getTrieNumber()], ptr->getTrieNumber());
}

void RPSRBT::traverseForDirectConnectReduction(RPSRBTNode* ptr, int i) {
  if (NULL == ptr || ptr->isTerm()) { return ; }
  if (i != ptr->getTrieNumber()) { return ; }
  if (i == 0 && "root" == ptr->getLabel()) { return ; }
  if (ptr->getRule() > 0) { return ; }

  traverseForDirectConnectReduction(ptr->getLeft(), i);
  traverseForDirectConnectReduction(ptr->getRight(), i);

  RPSRBTNode *left, *right;
  left = ptr->getLeft(), right = ptr->getRight();
  
  if (left == right) {
    // cout << "Direct Connect!!\n";
    unordered_set<RPSRBTNode*> parents = ptr->getParents();
    for (auto p : parents) {
      if(p->getLeft() == ptr)
	p->setLeft(left);
      if(p->getRight() == ptr)
	p->setRight(right);
    }
    ptr->setReachFalse();
    //cout << "[" << ptr->getTrieNumber()+1 << "] " << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << ", term = " << ptr->isTerm() << ", candidate = " << ptr->getCandidate() << ", reachable = " << ptr->isReachable() << endl;
  }

}

void RPSRBT::traverse() {
  // preOrder(roots[0]);
  for (auto ptr : roots) {
    cout << "================================[" << ptr->getTrieNumber()+1 << "]=================================" << endl;
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
  if (i != ptr->getTrieNumber()) { return ; }

  if (ptr->isReachable())
    cout << "[" << ptr->getTrieNumber()+1 << "] " << ptr->getVar() << ", " << ptr->getLabel() << ", " << ptr->getRule() << ", term = " << ptr->isTerm() << ", candidate = " << ptr->getCandidate() << ", reachable = " << ptr->isReachable() << endl;
  preOrder2(ptr->getLeft(), i);
  preOrder2(ptr->getRight(), i);
}
