// $Id: stringset.h,v 1.1 2014-11-03 14:35:55-08 - - $
// Kevin Thai (kjthai@ucsc.edu)
// Armando Silva (asilva3@ucsc.edu)
#ifndef __STRINGSET__
#define __STRINGSET__

#include <iostream>
#include <string>

const std::string* intern_stringset (const char*);

void dump_stringset (std::ostream&);

#endif

