/* openfile.cpp */

#include <openfile.hpp>

// if rule2 is included rule1 then return true else return false;
bool isIncluded(string rule1, string rule2)
{
  unsigned l = rule1.size();
  for (unsigned i = 0; i < l; ++i) {
    if (rule1[i] != rule2[i] && rule1[i] != '*')
      return false;
  }

  return true;
}

void deleteIncludedRules(list<Rule> *orulelist, list<Rule> *rulelist) 
{
  list<Rule>::iterator ruleIt = orulelist->begin();
  list<Rule>::iterator ruleIt2;
  list<Rule>::iterator ruleItEnd = orulelist->end();
  string rule1,rule2;
	
  vector<bool> A;
  unsigned i, j, n = orulelist->size();
  for (i = 0; i < n; ++i)
    A.push_back(true);

  while (ruleIt != ruleItEnd) {
    if (A[ruleIt->getRuleNumber()-1]) {
      ruleIt2 = ruleIt;
      ruleIt2++;
      rule1 = ruleIt->getRuleBitString();
      while (ruleIt2 != ruleItEnd) {
	rule2 = ruleIt2->getRuleBitString();
	if (isIncluded(rule1,rule2))
	  A[ruleIt2->getRuleNumber()-1] = false;
	++ruleIt2;
      }
    }
    ++ruleIt;
  }

  i = 0, j = 1;
  for (auto it : *orulelist) {
    if (A[i]) {
      Rule* r = new Rule(j, it.getRuleBitString());
      rulelist->push_back(*r);
      ++j;
    }
    ++i;
  }
  Rule::resetNumberOfRule(rulelist->size());
}

void readPackets(char *&filename, list<string>* packets) throw (string)
{
  ifstream ifs(filename);
	
  if (ifs.fail())
    throw "Error: Can't open an input packets file.\n";
	
  string tmp;
  while (getline(ifs, tmp)) {
    packets->push_back(tmp);
  }
  ifs.close();

  if (0 == packets->size())
    throw "Error: Input Packets file has no packt.\n";
}

void Rule::setVariableOrder(vector<unsigned> order) {
  _variable_order = order;
}

void readRulelist(char *&filename, list<Rule>* rulelist) throw (string)
{
  ifstream ifs(filename);

  if (ifs.fail())
    throw "Error: Can't open an input rulelist file.\n";

  unsigned rulenumber = 1;
  string tmp;
  if (getline(ifs,tmp)) {
    rulelist->push_back(Rule(rulenumber,tmp));
    ++rulenumber;
    Rule::setLengthOfRule(tmp.size());
  }

  while (getline(ifs, tmp)) {
    rulelist->push_back(Rule(rulenumber,tmp));
    ++rulenumber;
  }
  ifs.close();

  vector<unsigned> ord;
  for (unsigned i = 1; i <= Rule::getLengthOfRule(); ++i)
    ord.push_back(i);
  // ord.push_back(2);
  // ord.push_back(4);
  // ord.push_back(3);
  // ord.push_back(1);
  Rule::setVariableOrder(ord);

  if (0 == rulelist->size())
    throw "Error: Input Rulelist file has no rule.\n";
}


//

string deleteBlank(string s)
{
  list<string> splited;
  string dd = "";

  string::size_type pos = 0;

  while(pos != string::npos ) {
    string::size_type p = s.find(" ", pos);

    if(p == string::npos) {
      splited.push_back(s.substr(pos));
      break;
    }
    else {
      splited.push_back(s.substr(pos, p-pos));
    }

    pos = p + 1;
  }

  for (auto ss : splited)
    dd += ss;

  return dd;
}

void readClassBenchPackets(char *&filename, list<string>* packets) throw (string)
{
  ifstream ifs(filename);
	
  if (ifs.fail())
    throw "Error: Can't open an input packets file.\n";
	
  string tmp;
  while (getline(ifs, tmp)) {
    packets->push_back(deleteBlank(tmp));
  }
  ifs.close();

  if (0 == packets->size())
    throw "Error: Input Packets file has no packt.\n";
}

void readClassBenchRulelist(char *&filename, list<Rule>* rulelist) throw (string)
{
  ifstream ifs(filename);

  if (ifs.fail())
    throw "Error: Can't open an input rulelist file.\n";

  unsigned rulenumber = 1;
  string tmp;
  while (getline(ifs, tmp)) {
    rulelist->push_back(Rule(rulenumber,tmp));
    ++rulenumber;
  }
  ifs.close();

  if (0 == rulelist->size())
    throw "Error: Input Rulelist file has no rule.\n";
}
