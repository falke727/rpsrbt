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

void deleteIncludedRules(list<Rule> *rulelist) 
{
  list<Rule>::iterator ruleIt = rulelist->begin();
  list<Rule>::iterator ruleIt2;
  list<Rule>::iterator ruleItEnd = rulelist->end();
  string rule1,rule2;
	
  vector<bool> A;
  unsigned i, j, n = rulelist->size();
  for (i = 0; i < n; ++i)
    A.push_back(false);

  i = 0;
  while (ruleIt != ruleItEnd) {
    ruleIt2 = ruleIt;
    ruleIt2++;
    j = i+1;
    rule1 = ruleIt->getRuleBitString();
    while (ruleIt2 != ruleItEnd) {
      rule2 = ruleIt2->getRuleBitString();
      if (isIncluded(rule1,rule2))
	A[j] = true;
      ++ruleIt2;
      ++j;
    }
    ++ruleIt;
    ++i;
  }

  ruleIt = rulelist->begin();
  i = 0;
  while (ruleIt != ruleItEnd) {
    if (A[i]) {
      // cout << ruleIt->getRuleBitString() << " !!!" << endl;
      ruleIt = rulelist->erase(ruleIt);
      Rule::decNumberOfRule();
      ++i;
      continue;
    }
    ++ruleIt;
    ++i;
  }
	
  ruleIt = rulelist->begin();
  i = 1;
  while (ruleIt != ruleItEnd) {
    ruleIt->setRuleNumber(i);
    // cout << ruleIt->getRuleNumber() << ' ' << ruleIt->getRuleBitString() << endl;
    ++ruleIt;
    ++i;
  }
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

void readRulelist(char *&filename, list<Rule>* rulelist) throw (string)
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
