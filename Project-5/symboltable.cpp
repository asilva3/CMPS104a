#include <bitset>
#include <iostream>
#include <string>
#include <unordered_map>

#include "symboltable.h"
#include "astree.h"
#include "lyutils.h"
#include "stringset.h"

vector<symbol_table*> symbol_stack;
vector<int> block_count;
int next_block = 1;
FILE* fsym;

void get_sym_file(FILE* out) {
   fsym = out;
}

symbol* newsymbol(astree* node){
   if(symbol_stack.empty() )
   {
      symbol_stack.push_back(new symbol_table);
      symbol_stack.push_back(nullptr);
   }
   if(block_count.empty() )
   {
      block_count.push_back(0);
   }
   symbol* a = new symbol();
   a->attributes = node->attributes;
   a->fields = nullptr;
   a->blocknr = block_count.back();
   a->filenr = node->filenr;
   a->linenr = node->linenr;
   a->offset = node->offset;
   a->parameters = nullptr;
   return a;
}


const char* check_attr(astree* node){
   string out = "";
   attr_bitset attr = node->attributes;
    if (attr[ATTR_field]) {
        out += "field ";
    }
    if (attr[ATTR_void] == 1) {
        out += "void ";
    }
    if (attr[ATTR_bool] == 1) {
        out += "bool ";
    }
    if (attr[ATTR_char] == 1) {
        out += "char ";
    }
    if (attr[ATTR_int] == 1) {
        out += "int ";
    }
    if (attr[ATTR_null] == 1) {
        out += "null ";
    }
    if (attr[ATTR_string] == 1) {
        out += "string ";
    }
    if (attr[ATTR_struct] == 1) {
        out += "struct ";
    }
    if (attr[ATTR_array] == 1) {
        out += "array ";
    }
    if (attr[ATTR_function] == 1) {
        out += "function ";
    }
    if (attr[ATTR_variable] == 1) {
        out += "variable ";
    }
    if (attr[ATTR_typeid] == 1) {
        out += "typeid ";
    }
    if (attr[ATTR_param] == 1) {
        out += "param ";
    }
    if (attr[ATTR_lval] == 1) {
        out += "lval ";
    }
    if (attr[ATTR_const] == 1) {
        out += "const ";
    }
    if (attr[ATTR_vreg] == 1) {
        out += "vreg ";
    }
    if (attr[ATTR_vaddr] == 1) {
        out += "vaddr ";
    }
   return out.c_str();
}

void check_array(astree* node){
   
   switch(node->children[0]->symbol){
      
      case TOK_CHAR:       node->attributes[ATTR_char] = true;
                           node->attributes[ATTR_lval] = true;
                           break;

      case TOK_INT:        node->attributes[ATTR_int] = true;  
                           node->children[0]-> 
                              attributes[ATTR_variable] = true;
                           node->children[0]-> 
                              attributes[ATTR_int] = true;
                           node->
                              attributes[ATTR_lval] = true;
                           break;
      case TOK_BOOL:       node->attributes[ATTR_bool]= true;
                           node->attributes[ATTR_const] = true;
                           node->attributes[ATTR_lval] = true;
                           break;
     } 
}

void set_attribute(astree* node){
   switch(node->symbol)
   {
      case TOK_VOID:       node->attributes[ATTR_void] = true;
                           if (node->children.size() > 0)
                           node->children[0]-> 
                              attributes[ATTR_void] = true;
                           break;
      case TOK_BOOL:       node->attributes[ATTR_bool]= true;
                           node->attributes[ATTR_const] = true;
                           node->attributes[ATTR_lval] = true;
                           if (node->children.size() > 0)
                           node->children[0]-> 
                                attributes[ATTR_bool] = true;
                           break;
      case TOK_CHAR:       node->
                                 attributes[ATTR_char] = true;
                           node->
                                 attributes[ATTR_lval] = true;
                           if (node->children.size() > 0)
                           node->children[0]-> 
                                 attributes[ATTR_char] = true;
                           break;
      case TOK_INT:        node->attributes[ATTR_int] = true; 
                           node->attributes[ATTR_lval] = true; 
                           if (node->children.size() > 0)
                           node->children[0]-> 
                                 attributes[ATTR_int] = true;
                           break;
      case TOK_NULL:       node->attributes[ATTR_null] = true;
                           node->attributes[ATTR_const] = true;
                           break;
      case TOK_STRING:     node->attributes[ATTR_string] = true;
                           node->attributes[ATTR_lval] = true;
                           if (node->children.size() > 0)
                           node->children[0]-> 
                                attributes[ATTR_string] = true;
                           break;
      case TOK_STRUCT:     node->attributes[ATTR_struct] = true;
                           node->attributes[ATTR_typeid] = true;
                           node->children[0]-> 
                           attributes[ATTR_struct] = true;
                           node->attributes[ATTR_variable] = false;
                           node->attributes[ATTR_lval] = false;
                        node->children[0]->
                              attributes[ATTR_variable] = false;
                        node->children[0]->
                                 attributes[ATTR_lval] = false;
                           break;
      case TOK_ARRAY:      node->attributes[ATTR_array] = true;
                           //check_array(node);
                           break;
      
      case TOK_FUNCTION:   node->attributes[ATTR_function] = true;
                        node->children[0]->children[0]->
                           attributes[ATTR_function] = true;
                           node->attributes[ATTR_variable] = false;
                           node->attributes[ATTR_lval] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_variable] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_lval] = false;
                           break;
      case TOK_PROTOTYPE:
                        node->children[0]->children[0]->
                           attributes[ATTR_variable] = false;
                        node->children[0]->children[0]->
                           attributes[ATTR_lval] = false;
                           break;
      case TOK_PARAMLIST:  for (size_t index = 0;
                                 index < node->children.size(); ++index)
                        node->children[index]->children[0]->
                                 attributes[ATTR_param] = true;
                           break;
      case TOK_STRINGCON:  node->attributes[ATTR_string] = true;
                           node->attributes[ATTR_const] = true;
                           node->attributes[ATTR_lval] = true;
                           break;
      case TOK_CHARCON:    node->attributes[ATTR_char] = true;
                           node->attributes[ATTR_const] = true;  
                           node->attributes[ATTR_lval] = true;
                           break;
      case TOK_INTCON:     node->attributes[ATTR_int] = true;
                           node->attributes[ATTR_const] = true;
                           node->attributes[ATTR_lval] = true;
                           break;
      case TOK_FIELD:      node->attributes[ATTR_field] = true;
                           node->attributes[ATTR_struct] = true;
                           break;
      case TOK_TYPEID:     node->attributes[ATTR_typeid] = true;
                           for(size_t index = 0; 
                              index < node->children.size(); ++index)
                           {
                      node->children[index]->
                               attributes[ATTR_typeid] = true;
                      node->children[index]->
                              attributes[ATTR_struct] = true;
                      break;
                      }
      case TOK_IDENT:      node->attributes[ATTR_lval] = true;
                           node->attributes[ATTR_variable] = true;
                           break;
      case TOK_VARDECL:    node->attributes[ATTR_variable] = true;
                           break;
      case TOK_DECLID:     node->attributes[ATTR_lval] = true;
                           node->attributes[ATTR_variable] = true;
                           break;
      default:           
                           break;
   }
}

void insert_symbol (symbol_table table, const string* key, 
                     symbol* sym, astree* node) {
   table[key] = sym;
   for (size_t size = 1; size < block_count.size(); ++size) {
      fprintf(fsym, "   ");
   }
   fprintf (fsym, "%s (%zu.%zu.%zu) {%zu} %s\n", key->c_str(), 
            sym->filenr, sym->linenr, sym->offset, sym->blocknr, 
            check_attr (node));
}

void make_struct_symbol (astree* root) {
   symbol* sym = newsymbol(root->children[0]);
   const string* key;
   symbol_table fields;
   
   sym->fields = &fields;
   key = root->children[0]->lexinfo;
   // insert struct symbol into global (block 0) symbol table
   insert_symbol(*symbol_stack[0], key, sym, root->children[0]);

   for (size_t index = 1; index < root->children.size(); ++index) {
      // turn fields into symbols
      astree* field = root->children[index]->children[0];
      sym = newsymbol(field);
      // turn fields into key
      key = field->lexinfo;
      // insert (key,symbol) into fields symbol table
      fprintf (fsym, "   ");
      insert_symbol(fields, key, sym, field);
   }
}

void make_block (astree* root) {
   // increment next_block
      block_count.push_back(next_block);
      next_block++;
   // push_back new symbol_table on stack
      symbol_stack[block_count.back()] = new symbol_table;
      symbol_stack.push_back(nullptr);
   // traversing(block node);
      traversing(root);
   // pop_back()   
      block_count.pop_back();
}

void make_func_symbol(astree* root){
   astree* function = root->children[0]->children[0];
   symbol* sym = newsymbol(function);
   const string* key;
   vector<symbol*> params;

   sym->parameters = &params;
   key = function->lexinfo;
   insert_symbol (*symbol_stack[0], key, sym, function);
   
   astree* paramlist = root->children[1];
   for (size_t index = 0; 
            index < paramlist->children.size(); ++index){
      astree* param = paramlist->children[index]->children[0];
      sym = newsymbol(param);
      key = param->lexinfo;
      ++sym->blocknr;
      params.push_back(sym);
      fprintf(fsym, "   ");
      insert_symbol (*symbol_stack[0], key, sym, param);
   }
   
   make_block(root->children[2]); 
}

void make_proto_symbol(astree* root){
   astree* proto = root->children[0]->children[0];
   symbol* sym = newsymbol(proto);
   const string* key;
   vector<symbol*> params;

   sym->parameters = &params;
   key = proto->lexinfo;
   insert_symbol (*symbol_stack[0], key, sym, proto);
   
   astree* paramlist = root->children[1];
   for (size_t index = 0; 
         index < paramlist->children.size(); ++index){
      astree* param = paramlist->
         children[index]->children[0];
      sym = newsymbol(param);
      ++sym->blocknr;
      params.push_back(sym);
   }
}

void make_vardecl(astree* root){
   astree* vardecl = root->children[0]->children[0];
   symbol* sym = newsymbol(vardecl);
   const string* key;
   key = vardecl->lexinfo;
   insert_symbol (*symbol_stack[block_count.back()], 
                  key, sym, vardecl);
   
}

void run_attr(astree* root){
   for( size_t index = 0; index < root->children.size(); ++index){
      run_attr(root->children[index]);
   }
   set_attribute(root);
}

void traversing(astree* root){
   for (size_t index = 0; index < root->children.size(); ++index) {
      
      int nodesymbol = root->children[index]->symbol;
      switch(nodesymbol){
         case TOK_STRUCT:  
            make_struct_symbol(root->children[index]);
            fprintf(fsym, "\n");
            break;
         case TOK_FUNCTION: 
              make_func_symbol(root->children[index]); 
            fprintf(fsym, "\n");
            break;
         case TOK_PROTOTYPE:
            make_proto_symbol(root->children[index]); 
            fprintf(fsym, "\n");
            break;
         case TOK_VARDECL:
            make_vardecl(root->children[index]);
            break;
         case TOK_IF:
            make_block(root->children[index]->children[1]);
            break;
         case TOK_IFELSE:
            make_block(root->children[index]->children[1]);
            make_block(root->children[index]->children[2]);
            break;
         default:
            break;
      }
   }  
}

      
