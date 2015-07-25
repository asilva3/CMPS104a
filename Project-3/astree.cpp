// Name : Kevin Thai (kjthai@ucsc.edu)
// Name: Armando Silva (asilva3@ucsc.edu)
#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"
#include "stringset.h"
#include "lyutils.h"

astree* new_astree (int symbol, int filenr, int linenr,
                    int offset, const char* lexinfo) {
   astree* tree = new astree();
   tree->symbol = symbol;
   tree->filenr = filenr;
   tree->linenr = linenr;
   tree->offset = offset;
   tree->lexinfo = intern_stringset (lexinfo);
   DEBUGF ('f', "astree %p->{%d:%d.%d: %s: \"%s\"}\n",
           tree, tree->filenr, tree->linenr, tree->offset,
           get_yytname (tree->symbol), tree->lexinfo->c_str());
   return tree;
}
// Creates a new function astree
astree* func_def (astree* first, astree* paramlist, 
                     astree* block,int symbol, int symbol2){
   if((*block->lexinfo).compare(";") == 0){
      astree* new_func = new_astree(symbol2, first->filenr, 
                                 first->linenr, first->offset, "");
      return adopt2(new_func, first, paramlist); 
   }else{
      astree* new_func = new_astree(symbol, first->filenr, 
                                 first->linenr, first->offset, "");
      new_func = adopt2(new_func, first, paramlist); 
      return adopt1(new_func, block);
   }

}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   DEBUGF ('a', "%p (%s) adopting %p (%s)\n",
           root, root->lexinfo->c_str(),
           child, child->lexinfo->c_str());
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt2sym (astree* root, astree* left, 
                   astree* right,int symbol) {
   adopt1 (root, left);
   adopt1 (root, right);
   root->symbol = symbol;
   return root;
}
//adopts 3 and changes the symbol
astree* adopt3sym (astree* root, astree* left, 
                   astree* right, astree* other, int symbol) {
   adopt1 (root, left);
   adopt1 (root, right);
   adopt1 (root, other);
   root->symbol = symbol;
   return root;
}
// changes the symbol of the left child
astree* adopt2csym (astree* root, astree* left, 
                   astree* right,int symbol) {
   adopt1 (root, left);
   adopt1 (root, right);
   left->symbol = symbol;
   return root;
}

astree* adopt1sym (astree* root, astree* child, int symbol) {
   root = adopt1 (root, child);
   root->symbol = symbol;
   return root;
}
//changes the symbol of the child 
astree* adopt1csym (astree* root, astree* child, int symbol) {
   root = adopt1 (root, child);
   child->symbol = symbol;
   return root;
}

// Changes the symbol of the root
astree* change_sym (astree* root, int symbol){
   root -> symbol = symbol;
   return root;
}

static void dump_node (FILE* outfile, astree* node) {
      // Get rid of TOK_
      char* tname= (char*)get_yytname(node->symbol);   
      if(strstr(tname,"TOK_") == tname){
         tname += 4;
      }
      fprintf(outfile, 
         "%s \"%s\" %zu.%zu.%zu\n",
         tname, (node->lexinfo)->c_str(), 
         node->filenr, node->linenr, node->offset);
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   for ( int index = 0; index < depth; index++){
  
      fprintf(outfile,"|\t");
   }
   dump_node (outfile, root);
   for (size_t child = 0; child < root->children.size();
        ++child) {
      dump_astree_rec (outfile, root->children[child],
                       depth + 1);
   }
}

void dump_astree (FILE* outfile, astree* root) {
   dump_astree_rec (outfile, root, 0);
   fflush (NULL);
}

void yyprint (FILE* outfile, unsigned short toknum,
              astree* yyvaluep) {
   if (is_defined_token (toknum)) {
      dump_node (outfile, yyvaluep);
   }else {
      fprintf (outfile, "%s(%d)\n",
               get_yytname (toknum), toknum);
   }
   fflush (NULL);
}


void free_ast (astree* root) {
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   DEBUGF ('f', "free [%p]-> %d:%d.%d: %s: \"%s\")\n",
           root, root->filenr, root->linenr, root->offset,
           get_yytname (root->symbol), root->lexinfo->c_str());
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}

RCSC("$Id: astree.cpp,v 1.3 2014-11-17 01:50:59-08 - - $")

