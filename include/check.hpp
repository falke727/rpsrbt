/* check.hpp */

#ifndef __CLASIFICATION_CHECK_HPP__
#define __CLASIFICATION_CHECK_HPP__

#ifndef __CLASIFICATION_OPENFILE_HPP__
#include <openfile.hpp>
#endif

#ifndef __CLASIFICATION_CLASSIFY_HPP__
#include <classify.hpp>
#endif

#ifndef __CLASIFICATION_RBT_HPP__
#include <rbt.hpp>
#endif

#include <assert.h>

unsigned checkClassifyResult(list<Result> *&, list< list<Result> > &);

#endif
