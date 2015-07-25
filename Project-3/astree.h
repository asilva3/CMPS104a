// Name : Kevin Thai (kjthai@ucsc.edu)
// Name: Armando Silva (asilva3@ucsc.edu)
#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <string>
#include <vector>
using namespace std;

#include "auxlib.h"

struct astree {
   int symbol;               // token code
   size_t filenr;            // index into filename stack
   size_t linenr;            // line number from source code
   size_t offset;            // offset of token with current line
   const string* lexinfo;    // pointer to lexical information
   vector<astree*> children; // children of this n-way node
};


astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo);
astree* func_def (astree* identdecl, astree* paramlist, 
                     astree* block,int symbol, int symbol2);
//astree* proto_def (astree* identdecl, astree* paramlist, int symbol);
astree* adopt1 (astree* root, astree* child);
astree* adopt2 (astree* root, astree* left, astree* right);
astree* adopt2csym (astree* root, astree* left, 
                    astree* right,int symbol);
astree* adopt1csym (astree* root, astree* child, int symbol);
astree* adopt1sym (astree* root, astree* child, int symbol);
astree* adopt2sym (astree* root , astree* left, 
                   astree* right, int symbol);
astree* adopt3sym (astree* root , astree* left, 
                   astree* right, astree* other,  int symbol);
astree* change_sym ( astree* root, int symbol);
void dump_astree(FILE* outfile, astree* root);
void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep);
void free_ast (astree* tree);
void free_ast2 (astree* tree1, astree* tree2);

RCSH("$Id: astree.h,v 1.3 2014-11-16 23:03:30-08 - - $")
#endif
