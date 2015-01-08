#include <assert.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "astree.h"

astree* movie_astree;
astree* tvshow_astree;

astree* new_astree () {
  astree* tree = new astree();
  return tree;
}

astree* new_astree (int symbol, int filenr, int linenr,
                    int offset) {
   astree* tree = new astree();
   tree->symbol = symbol;
   tree->filenr = filenr;
   tree->linenr = linenr;
   tree->offset = offset;

    // String title;
    // int year;
    // int month;
    // int day;
    // String plot;
    // String runtime;
    // int    runtime_int;
    // String director;
    // String folder;
    // String filename;
    // String filenameandpath;
    // String trailer;
    // String poster;
    // String awards;
    // String imdbID;
    // Double imdbRating;
    // String height;
    // List   actorList;

   return tree;
}

void new_parseroot (void) {
  movie_astree = new_astree (0, 0, 0, 0);
  tvshow_astree = new_astree (0, 0, 0, 0);
  return;
}

int getFilenr(astree* node){
  return node->filenr;
}

int getLinenr(astree* node){
  return node->linenr;
}

int getOffset(astree* node){
  return node->offset;
}

astree* changeToken (astree* token, int newSymbol) {
  token->symbol = newSymbol;
  return token;
}

astree* adopt1 (astree* root, astree* child) {
   root->children.push_back (child);
   return root;
}

astree* adopt2 (astree* root, astree* left, astree* right) {
   adopt1 (root, left);
   adopt1 (root, right);
   return root;
}

astree* adopt3 (astree* root, astree* left, 
                          astree* right, astree* alpha) {
   adopt1 (root, left);
   adopt1 (root, right);
   adopt1 (root, alpha);
   return root;
}

astree* adopt1sym (astree* root, astree* child, int symbol) {
   root = adopt1 (root, child);
   root->symbol = symbol;
   return root;
}


static void dump_node (FILE* outfile, astree* node) {
  (void) outfile;
  (void) node;

   // fprintf (outfile, "%s \"%s\" %ld:%ld.%03ld",
   //          get_yytname (node->symbol), node->lexinfo->c_str(),
   //          node->filenr, node->linenr, node->offset);

   // Print symbol table related information
   // print_node(outfile, node);
}

static void dump_astree_rec (FILE* outfile, astree* root,
                             int depth) {
   if (root == NULL) return;
   for (int i = 0; i<depth; i++) fprintf(outfile, "|  ");
   dump_node (outfile, root);
   fprintf (outfile, "\n");
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


void free_ast (astree* root) {
  if (root == NULL) return;
   while (not root->children.empty()) {
      astree* child = root->children.back();
      root->children.pop_back();
      free_ast (child);
   }
   delete root;
}

void free_ast2 (astree* tree1, astree* tree2) {
   free_ast (tree1);
   free_ast (tree2);
}
