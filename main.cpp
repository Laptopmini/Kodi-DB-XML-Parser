// $Id: main.cpp,v 1.3 2014-10-07 18:09:11-07 - - $

//////////////////////////////////
// Paul-Valentin Mini (pcamille)
//////////////////////////////////

#include <string>
using namespace std;

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include "astree.h"
#include "auxlib.h"
#include <curl/curl.h>
#include <fstream>

using line_itor = vector<string>::iterator;
enum data_types {
   NONE = 0,
   MOVIE = 1,
   TVSHOW = 2
};
int data_type = NONE;
astree* cursor = NULL;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "a");
      if (option == EOF) break;
      switch (option) {
         case 'a':
            //
            break;
         default:
            errprintf ("%:bad option (%c)\n", optopt);
            break;
      }
   }
   if (optind != (argc - 1)) {
      errprintf ("Usage: %s [-a] [filename]\n", get_execname());
      exit (get_exitstatus());
   }
   new_parseroot();
}

vector<string> inline StringSplit(const string &source, const char *delimiter = " ", bool keepEmpty = false) {
   vector<string> results;
   size_t prev = 0;
   size_t next = 0;

   while ((next = source.find_first_of(delimiter, prev)) != string::npos) {
      if (keepEmpty || (next - prev != 0)) {
         results.push_back(source.substr(prev, next - prev));
      }
      prev = next + 1;
   }
   if (prev < source.size()) {
      results.push_back(source.substr(prev));
   }
   return results;
}

void process_line(string type, vector<string> data){
   cout << "data[ " << data_type << " ]: ";
   cout << "type[ " << type.c_str() << " ]: ";
   for (string word : data){
      cout << word.c_str() << " ";
   }
   if (data.size() > 0) cout << endl;

}

int parse_file(char* xml_input) {
   ifstream xml_in(xml_input);
   string line;
   int line_number = 0;

   while (getline(xml_in, line)) {
      line_number++;
      vector<string> line_tokens = StringSplit(line, " <>");
      if (line_tokens.size() > 1) {
         if (data_type == NONE) continue;

         string type = line_tokens.front();
         string closing_type = line_tokens.back();

         closing_type.erase(0,1);
         if (type.compare(closing_type) != 0){
            // bracket is not closed on same line (none essential info)
            // cout << "line n(" << line_number << ") " << type.c_str() << ", " << closing_type.c_str() << endl;
         } else {
            line_tokens.erase(line_tokens.begin());
            line_tokens.pop_back();
            process_line(type, line_tokens);
         }
      } else {
         if (!line_tokens.empty()) {
            string bracket_type = line_tokens[0];
            if (bracket_type.compare("movie") == 0) {
               if (data_type == NONE) {
                  data_type = MOVIE;
                  cursor = new_astree();
               } else {
                  cout << "ERROR: Previous Bracket was not closed." << endl;
               }
            } else if (bracket_type.compare("tvshow") == 0) {
               if (data_type == NONE) {
                  data_type = TVSHOW;
                  cursor = new_astree();
               } else {
                  cout << "ERROR: Previous Bracket was not closed." << endl;
               }
            } else if (bracket_type.compare("/movie") == 0) {
               data_type = NONE;
               adopt1(movie_astree, cursor);
               cursor = NULL;
            } else if (bracket_type.compare("/tvshow") == 0) {
               data_type = NONE;
               adopt1(tvshow_astree, cursor);
               cursor = NULL;
            }
         }
      }
   }

   return 0;
}


int main (int argc, char** argv) {
   set_execname (argv[0]);
   scan_options (argc, argv);
   
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
      string extention_type = ".xml";
      if (temp_string.compare(extention_type) != 0){
         errprintf ("Error: input file is not of type \"%s\".\n", 
            extention_type.c_str());
         exit (get_exitstatus());
      } else {
         real_filename = real_filename.substr(0, periodMark);
      } 
   } else {
      errprintf ("Error: input file extension missing/unrecognized.\n");
      exit (get_exitstatus());
   }

   parse_file(filename);

   // Generate .ast output file
   // temp_string = real_filename + ".ast";

   // Open .ast output file
   // outputFile_ast = fopen(temp_string.c_str(), "w");

   // Dump AST tree
   // dump_astree(outputFile_ast, yyparse_astree);

   // Close & Clean up
   free_ast (movie_astree);
   free_ast (tvshow_astree);

   // Close output files
   // fclose(outputFile_ast);

   return get_exitstatus();
}

