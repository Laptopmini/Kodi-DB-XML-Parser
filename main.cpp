// $Id: main.cpp,v 1.3 2014-10-07 18:09:11-07 - - $

//////////////////////////////////
// Paul-Valentin Mini (pcamille)
// cmps104a - asg4
//////////////////////////////////

#include <string>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <wait.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "astree.h"
#include "auxlib.h"

astree* yyparse_astree;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:D:ly");
      if (option == EOF) break;
      switch (option) {
         case '@':
            //
            break;
         default:
            errprintf ("%:bad option (%c)\n", optopt);
            break;
      }
   }
   if (optind != (argc - 1)) {
      errprintf ("Usage: %s [-@Dly] [filename]\n", get_execname());
      exit (get_exitstatus());
   }
   new_parseroot();
}

int main (int argc, char** argv) {
   set_execname (argv[0]);
   scan_options (argc, argv);
   
   FILE *outputFile_ast;
   bool hasPeriod = true;
   char* filename = argv[optind];
   string real_filename = filename;
   string temp_string = filename;

   // Extract filename
   int filenameMark = real_filename.find_last_of("/");
   if (filenameMark != -1) {
      real_filename = real_filename.substr((filenameMark + 1), 
         (real_filename.length() - 1));
   }

   // Check if file is .xml
   int periodMark = real_filename.find_last_of(".");
   if (periodMark != -1){
      temp_string = real_filename.substr(periodMark, 
         (real_filename.length() - 1));
   } else {
      hasPeriod = false;
   }
   if (hasPeriod){
      if (temp_string.compare(".xml") != 0){
         errprintf ("Error: input file is not of type \".%s\".\n", 
            get_execname());
         exit (get_exitstatus());
      } else {
         real_filename = real_filename.substr(0, periodMark);
      } 
   } else {
      errprintf ("Error: input file extension missing/unrecognized.\n");
      exit (get_exitstatus());
   }

   // Generate .ast output file
   temp_string = real_filename + ".ast";

   // Open .ast output file
   outputFile_ast = fopen(temp_string.c_str(), "w");

   // Dump AST tree
   dump_astree(outputFile_ast, yyparse_astree);

   // Close & Clean up
   free_ast (yyparse_astree);

   // Close output files
   fclose(outputFile_ast);

   return get_exitstatus();
}

