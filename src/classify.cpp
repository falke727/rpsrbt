/* classify.cpp */

#include <classify.hpp>

void classifyViaRPSRBT(RPSRBT* rpsrbt, list<string>*& packets, list<Result>* results) {
  list<string>::iterator pIt, pEnd;
  pIt = packets->begin(), pEnd = packets->end();
  struct timeval startTime, endTime;
  double sec_timeOfDay;
  unsigned result;

  Result::initCompareNumberOfRPSRBT();
  gettimeofday(&startTime, NULL);

  while (pIt != pEnd) {
    result = rpsrbtSearch(rpsrbt, *pIt);
    Result r(*pIt, result);
    results->push_back(r);
    ++pIt;
  }

  gettimeofday(&endTime, NULL);
  sec_timeOfDay = (endTime.tv_sec - startTime.tv_sec)
    + (endTime.tv_usec - startTime.tv_usec) / 1000000.0;
  Result::setLatencyRPSRBT(sec_timeOfDay);
}

unsigned rpsrbtSearch(RPSRBT* rpsrbt, string& packet) {
  RPSRBTNode* ptr = rpsrbt->getRoot();
  unsigned candidate = Rule::getNumberOfRule()+1;

  while (!ptr->isTerm()) {
    if ('0' == packet[ptr->getVar()-1])
      ptr = ptr->getLeft(), Result::incCompareNumberOfRPSRBT();
    else
      ptr = ptr->getRight(), Result::incCompareNumberOfRPSRBT();
    if (ptr->getRule() > 0 && ptr->getRule() < candidate)
      candidate = ptr->getRule(), Result::incCompareNumberOfRPSRBT();
  }

  if (ptr->getRule() < candidate)
    candidate = ptr->getRule(), Result::incCompareNumberOfRPSRBT();

  cout << packet << " --> " << candidate << endl;

  return candidate;
}

void classifyViaSequentialSearch(list<Rule>*& rulelist, list<string>*& packets, list<Result>* results)
{
  struct timeval startTime, endTime;
  double sec_timeOfDay;
  unsigned result;

  Result::initCompareNumberOfSequential();
  gettimeofday(&startTime, NULL);

  for (auto it : *packets) {
    result = sequentialSearch(rulelist, it);
    Result r(it, result);
    results->push_back(r);
  }

  gettimeofday(&endTime, NULL);
  sec_timeOfDay = (endTime.tv_sec - startTime.tv_sec)
    + (endTime.tv_usec - startTime.tv_usec) / 1000000.0;
  Result::setLatencySequential(sec_timeOfDay);
}

bool compareRuleAndPacket(Rule& rule, string& packet)
{
  string ruleBitString = rule.getRuleBitString();
  unsigned l = ruleBitString.size();
  unsigned i = 0;

  while (i < l) {
    Result::incCompareNumberOfSequential();
    if (ruleBitString[i] != '*' && ruleBitString[i] != packet[i])
      return false;
    ++i;
  }
  return true;
}

unsigned sequentialSearch(list<Rule>*& rulelist, string& packet)
{
  for (auto it : *rulelist) {
    if (compareRuleAndPacket(it, packet)) {
      cout << packet << " --> " << it.getRuleNumber() << endl;
      return it.getRuleNumber();
    }
  }

  cout << packet << " --> " << rulelist->size()+1 << endl;
	
  return rulelist->size()+1;
}

// bool compareRuleAndPacketForClassBench(ClassBenchRule& rule, string& packet)
// {
//   string f[4] = { rule.getSA(), rule.getDA(), rule.getPROT(), rule.getFLAG() };
//   list<string> port[2] = { rule.getSP(), rule.getDP() };
//   unsigned fl[4] = { 32, 32, 8, 16 };
//   unsigned pl[2] = { 16, 16 };
//   unsigned fs[4] = { 0, 32, 96, 104 };
//   unsigned ps[2] = { 64, 80 };
//   // [0,31], [32,63], [64,79], [80,95], [96,103], [104,119]

//   for (unsigned i = 0; i < 4; ++i) {
//     string r = f[i];
//     for (unsigned j = 0, k = fs[i]; j < fl[i]; ++j)
//       if (packet[j+k] != r[j] && '*' != r[j])
// 	return false;
//   }

//   bool flag;
//   for (unsigned i = 0; i < 2; ++i) {
//     flag = true;
//     unsigned j,k;
//     for (auto r : port[i]) {
//       for (j = 0, k = ps[i]; j < pl[i]; ++j, ++k)
//   	if (packet[k] != r[j] && '*' != r[j])
//   	  break;
//       if (j == pl[i]) { flag = false; break; }
//     }
//     if (flag)
//       return false;
//   }

//   return true;
// }

// unsigned sequentialSearchForClassBench(list<ClassBenchRule>& rulelist, string& packet)
// {
//   for (auto r : rulelist) {
//     if (compareRuleAndPacketForClassBench(r, packet))
//       return r.getRuleNumber();
//   }

//   return rulelist.size()+1;
// }

// void classifyViaSequentialSearchClassBench(list<Rule>*& rulelist, list<string>*& packets, list<Result>* results)
// {
//   list<string>::iterator pIt, pEnd;
//   pIt = packets->begin(), pEnd = packets->end();
  
//   struct timeval startTime, endTime;
//   double sec_timeOfDay;
//   unsigned result;

//   list<ClassBenchRule> cbrulelist;
//   convertRulesToClassBenchRuleList(rulelist,cbrulelist);

//   // cout << cbrulelist.size() << endl;

//   Result::initCompareNumberOfSequential();
//   gettimeofday(&startTime, NULL);

//   while (pIt != pEnd) {
//     result = sequentialSearchForClassBench(cbrulelist, *pIt);
//     // cout << *pIt << " --> " << result << endl;
//     Result r(*pIt, result);
//     results->push_back(r);
//     ++pIt;
//   }

//   gettimeofday(&endTime, NULL);
//   sec_timeOfDay = (endTime.tv_sec - startTime.tv_sec)
//     + (endTime.tv_usec - startTime.tv_usec) / 1000000.0;
//   Result::setLatencySequential(sec_timeOfDay);
// }
