#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <stdio.h>
#include <string.h>

using namespace std;

#ifndef __RPSRBT_OPENFILE_HPP__
#define __RPSRBT_OPENFILE_HPP__

class Rule {
private:
  static unsigned _number_of_rule;
  unsigned _ruleNumber;
  string _ruleBitString;
public:
  Rule() { }
  Rule(unsigned ruleNumber, string ruleBitString) {
    ++_number_of_rule;
    _ruleNumber = ruleNumber;
    _ruleBitString = ruleBitString;
  }
  ~Rule() {
    //	printf("call the Rule deconstructor.\n"); 
  }
  unsigned getRuleNumber() { return _ruleNumber; }
  string getRuleBitString() { return _ruleBitString; }
  static unsigned getNumberOfRule() { return _number_of_rule; }
  static void decNumberOfRule() { --_number_of_rule; }
  void setRuleNumber(unsigned i) { _ruleNumber = i; }
  unsigned size() { return _ruleBitString.size(); }
};

bool isIncluded(string, string);
void deleteIncludedRules(list<Rule>*);
void readPackets(char*&, list<string> *) throw (string);
void readRulelist(char*&, list<Rule> *) throw (string);

string deleteBlank(string);
void readClassBenchPackets(char*&, list<string> *) throw (string);
void readClassBenchRulelist(char*&, list<Rule> *) throw (string);
#endif
