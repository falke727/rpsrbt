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

RPSRBTNode::RPSRBTNode(unsigned*& v) {
  _var = *v;
  _rule = 0;
  _left = _right = NULL;
  _term = false;
}

RPSRBTNode::RPSRBTNode(unsigned*& v, unsigned*& r, bool*& t) {
  _var = *v;
  _rule = *r;
  _left = _right = NULL;
  _term = *t;
}

RPSRBT::RPSRBT(list<Rule> &rulelist) {
  for (auto r : rulelist) {
    cout << r.getRuleBitString() << "--> " << cutOutSingleRunFromRule(r) << endl;
  }
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
