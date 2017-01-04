/* classify.hpp */

#ifndef __RPSRBT_CLASSIFY_HPP__
#define __RPSRBT_CLASSIFY_HPP__

#ifndef __RPSRBT_OPENFILE_HPP__
#include <openfile.hpp>
#endif

#ifndef __RPSRBT_RPSRBT_HPP__
#include <rpsrbt.hpp>
#endif

#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

class Result {
private:
  static double _latency_of_sequential;
  static double _latency_of_rpsrbt;

  static unsigned long _compareNumberOfSequential;
  static unsigned long _compareNumberOfRPSRBT;

  string _packet;
  unsigned _retVal;
public:
  Result(string p, unsigned v) { _packet = p, _retVal = v; }
  string getPacket() { return _packet; }
  unsigned getValue() { return _retVal; }

  static void initCompareNumberOfSequential() { _compareNumberOfSequential = 0; }
  static void initCompareNumberOfRPSRBT() { _compareNumberOfRPSRBT = 0; }
  static void incCompareNumberOfSequential() { _compareNumberOfSequential++; }
  static void incCompareNumberOfRPSRBT() { _compareNumberOfRPSRBT++; }
  static void setLatencySequential(double t) { _latency_of_sequential = t; }
  static void setLatencyRPSRBT(double t) { _latency_of_rpsrbt = t; }

  static unsigned long getCompareNumberOfSequential() { return _compareNumberOfSequential; }
  static unsigned long getCompareNumberOfRPSRBT() { return _compareNumberOfRPSRBT; }

  static double getLatencySequential() { return _latency_of_sequential; }
  static double getLatencyRPSRBT() { return _latency_of_rpsrbt; }
};

void classifyViaSequentialSearch(list<Rule>*&, list<string>*&, list<Result> *);
void classifyViaRPSRBT(RPSRBT*, list<string>*&, list<Result>*);

bool compareRuleAndPacket(Rule&, string&);
unsigned sequentialSearch(list<Rule> *&, string&);
unsigned rpsrbtSearch(RPSRBT*, string&);

// bool compareRuleAndPacketForClassBench(ClassBenchRule&, string&);
// unsigned sequentialSearchForClassBench(list<ClassBenchRule>&, string&);
// void classifyViaSequentialSearchClassBench(list<Rule>*&, list<string>*&, list<Result> *);

#endif
