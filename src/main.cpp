#include <main.hpp>

int main(int argc, char* argv[])
{
  list<Rule> *rulelist = new list<Rule>;
  list<string> *packets = new list<string>;
  RPSRBT *rpsrbt;
  // bool classbench_flag = false;

  /* check arguments */
  if (!strcmp("-c",argv[1]) && argc == 4) {
    // classbench_flag = true;
    try {
      readClassBenchRulelist(argv[2],rulelist);
    } catch (string error_message) {
      cout << error_message; return 1;
    }
    try {
      readClassBenchPackets(argv[3],packets);
    } catch (string error_message) {
      cout << error_message; return 1;
    }

    /* make a Reduced Pointed Single-Run-Based Trie For ClassBench Format Rule */

  }  else if (argc == 3) {
    /* open and read a rulelist */
    try {
      readRulelist(argv[1],rulelist);
    } catch (string error_message) {
      cout << error_message; return 1;
    }
    /* delete included rules */
    deleteIncludedRules(rulelist);

    /* open and read packets */
    try {
      readPackets(argv[2],packets);
    } catch (string error_message) {
      cout << error_message; return 1;
    }

    /* make a Reduced Pointed Single-Run-Based Trie */
    rpsrbt = new RPSRBT(*rulelist);
    //
    //
    //
    //

  } else {
    printf("Usage: main <rule list> <pseudo packets>\n");
    return 1;
  }

  /* do the sequential search */
  // list< list<Result> > results;
  // list<Result>* resultOfSequential = new list<Result>;
  // if (true == classbench_flag) 
  //   classifyViaSequentialSearchClassBench(rulelist, packets, resultOfSequential);
  // else
  //   classifyViaSequentialSearch(rulelist, packets, resultOfSequential);
  // cout << "Sequential Search Time: " << Result::getLatencySequential() << endl;
  // cout << "Sequential Search Comp: " << Result::getCompareNumberOfSequential() << endl;
	

  // assert(0 == checkClassifyResult(resultOfSequential, results));

  /* delete dynamicaly allocated memories */
  delete packets;
  // delete resultOfSequential;

  return 0;
}
