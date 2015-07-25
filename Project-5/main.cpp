// $Id: main.cpp,v 1.6 2014-12-02 06:08:57-08 - - $
// Name : Kevin Thai (kjthai@ucsc.edu)
// Name: Armando Silva (asilva3@ucsc.edu)
#include <cstdlib>
#include <bitset>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "stringset.h"
#include "astree.h"
#include "auxlib.h"
#include "lyutils.h"
#include "symboltable.h"
using namespace std;

// Intialize Variables
const string CPP = "/usr/bin/cpp";
const size_t LINESIZE = 1024;
string optD;

// Looks for the options 
void scan_options(int argc, char** argv){
   for(;;){
      int option = getopt(argc, argv, "@:lyD:");
      if(option == EOF) break;
      switch(option){
      case '@': set_debugflags(optarg);
                break;
      case 'l': yy_flex_debug = 1;
                break;
      case 'y': yydebug = 1;
                break;
      case 'D': optD = "-D";
                optD.append(optarg);
                optD.append(" ");
                break;
      default: cerr<<"Invalid Option"<< endl;
               set_exitstatus(1);
               break;
      }     
   }
}

int main (int argc, char** argv) {
   // Make sure there are inputs
   yy_flex_debug =0;
   set_execname(argv[0]);
   if(argc >  1){
      scan_options(argc, argv);
      // Using fopen to check if file exists
      char* file_check = argv[optind];
      FILE* cFile = fopen(file_check, "r");
      if(cFile != NULL){
         char filename_str[LINESIZE];
         char filename_tok[LINESIZE];
         char filename_ast[LINESIZE];
         char filename_sym[LINESIZE];
         for (int argi = optind ; argi < argc; ++argi) {
            char* filename = argv[argi];
            // To get the filename from a different directory
            char* ptr = strrchr(filename,'/');
            char* dot = filename + strlen(filename) -3;
            char* tok;
            char* ast;
            char* sym;
            // Checks for .oc file type
            if(strcmp(dot,".oc") != 0){
               cerr << "Invalid file type: "<< filename << endl;
               set_exitstatus(1);
               exit(get_exitstatus());
            }
               // Changes the file to .str file
            if(ptr == NULL){
               strcpy(filename_str,filename);
               dot = filename_str + strlen(filename_str) - 3;
               strcpy(dot,".str");
            }else{
               strcpy(filename_str,ptr+1);
               dot = filename_str + strlen(filename_str) - 3;
               strcpy(dot,".str");
            }
            // Change the file to .tok file 
            if(ptr == NULL){
               strcpy(filename_tok,filename);
               tok = filename_tok + strlen(filename_tok) - 3;
               strcpy(tok,".tok");
            }else{
               strcpy(filename_tok,ptr+1);
               tok = filename_tok + strlen(filename_tok) -3;
               strcpy(tok,".tok");
            }
            FILE * ftok = fopen(filename_tok,"w");
            if (ftok == NULL){
               cerr<<"Cannot open file: "<< filename_tok<<endl;
               set_exitstatus(1);
               exit(get_exitstatus());
            }
            // Change the file to .ast file 
            if(ptr == NULL){
               strcpy(filename_ast,filename);
               ast = filename_ast + strlen(filename_ast) - 3;
               strcpy(ast,".ast");
            }else{
               strcpy(filename_ast,ptr+1);
               ast = filename_ast + strlen(filename_ast) -3;
               strcpy(ast,".ast");
            }
            FILE * fast = fopen(filename_ast,"w");
            if (fast == NULL){
               cerr<<"Cannot open file: "<< filename_ast<<endl;
               set_exitstatus(1);
               exit(get_exitstatus());
            }
            // Change the file to .ast file 
            if(ptr == NULL){
               strcpy(filename_sym,filename);
               sym = filename_sym + strlen(filename_sym) - 3;
               strcpy(sym,".sym");
            }else{
               strcpy(filename_sym,ptr+1);
               sym = filename_sym + strlen(filename_sym) -3;
               strcpy(sym,".sym");
            }
            FILE * fsym = fopen(filename_sym,"w");
            if (fsym == NULL){
               cerr<<"Cannot open file: "<< filename_sym<<endl;
               set_exitstatus(1);
               exit(get_exitstatus());
            }
            //Open with C preprocessor
            get_file(ftok);
            get_sym_file(fsym);
            string command = CPP + " " + optD  + filename;
            yyin = popen(command.c_str(), "r");
            if(yyin == NULL){
               syserrprintf(command.c_str());
               exit(get_exitstatus());
            }else{
               yyparse();
               pclose(yyin);
            }
            //Prints to file
            ofstream outfile(filename_str);
            dump_stringset (outfile);
            run_attr(yyparse_astree);
            traversing(yyparse_astree);
            dump_astree(fast, yyparse_astree);
            outfile.close();
            fclose(cFile);
            fclose(ftok);
            fclose(fast);
            fclose(fsym);
         }
      }else{
         // Prints out the error message if there is no file
         cerr<<"File: "<< "'" <<file_check<<"'" 
         << " does not exist"<<endl;
         set_exitstatus(1);

      }
   }else{
      // Prints out the error message if there is no input
      cerr<<"Requires a .oc input file"<<endl;
      set_exitstatus(1);
   }
   return get_exitstatus();
}
