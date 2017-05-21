#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <sstream>
#include <algorithm>    // std::find

using namespace std;

class Logger {
public:
	void logVector(string label, vector<string> * message) {
		cout << label << endl;
		for(unsigned int i=0; i < message->size(); i++) {
			cout << message->at(i) << endl;
		}
		cout << "\n\n" << endl;
	}

	void logInt(string label, int value) {
		cout << label << "=" << value << "\n" << endl;
	}

	void logString(string label, string value) {
		cout << label << "=" << value << "\n" << endl;
	}
};

class ReadFile { // Standard way of defining the class
public:
	// Dump the file into memory, then create a vector of lines.
	// Rteurn the vector updated.
	void read(const char * filename, vector<string> * list) {
	    //load the text file and put it into a single string:
	    std::ifstream in(filename);
	    std::stringstream buffer;
	    buffer << in.rdbuf();
	    std::string test = buffer.str();

	    //create variables that will act as "cursors". we'll take everything between them.
	    size_t pos1 = 0;
	    size_t pos2;

	    // make sure last character of the string is \0
	    size_t testSize = test.size();
	    test[testSize] = '\0';

	    //use the given vector 'list' to store the strings line by line.
	    while (test[pos1] != '\0' && pos1 < testSize){
	        pos2 = min(test.find("\n", pos1), testSize);              //search for the delimiter. pos2 will be where it was found.
	        // Normal case
	        if(pos2 > pos1) {
	        	list->push_back(test.substr(pos1, (pos2-pos1)));          //make a substring, which is nothing more
                                                                          //than a copy of a fragment of the big string.
	        }
	        // Case of empty line
	        else {
	        	list->push_back("");
	        }
	        pos1 = pos2+1; // sets pos1 to the next character after pos2.
	                       //so, it can start searching the next bar |.
	    }
	}
};

class WriteFile { // Standard way of defining the class
protected:
	std::ofstream file;
public:
	// Create an empty file .
	int create(string filename) {
		file.open(filename.c_str(), std::ios::out | std::ios::app);
		file.close();
		remove(filename.c_str());

		file.open(filename.c_str(), std::ios::out | std::ios::app);
		if (file.fail()) {
		    return 1;
		}
	    return 0;
	}

	// Close the file
	int close () {
		if(file != NULL) {
			file.flush();
			file.close();
		}
		return 0;
	}

	// Append to the file
	int append (vector<string> * text, unsigned int start, unsigned int end) {
		if(file != NULL) {
			string line;
			for (unsigned int i=start; i < end; i++) {
				line = text->at(i);
				file << line << std::endl;
			}
			return 0;
		}
		return 1;
	}
};

class Analyzer {
public:
	int extractPosition(vector<string> * infile, int initialPosition, string pattern, string pattern2) {
		unsigned int index;
		int match = 0;
		for(index=initialPosition; index<infile->size() && match==0; index++) {
			// The second check will be the same string for the footer position end.
			// If pattern 2 is equal to pattern, then the code is build to end at pattern.
			if(infile->at(index).find(pattern) != std::string::npos) {
				if(pattern.compare(pattern2) == 0) {
					match = 0;
					break;
				}
			}
		}
		return index;
	}

	void extractBody(vector<string> * infile, vector<string> * body, unsigned int start, unsigned int end) {
		for(unsigned int i = start; i <= end; i++) {
			body->push_back(infile->at(i));
		}
	}
};

class Processor {
public:
	void process(vector<string> * elementsNouveaux, vector<string> * body) {
		insertMissing(elementsNouveaux, body);
		sortElements(elementsNouveaux, body);
	}

protected:
	void insertMissing(vector<string> * elementsNouveaux, vector<string> * body) {
		 std::vector<string>::iterator it;

		 // Go through the elementsNouveaux list and insert into infile
	    // only if not already present
	    for(unsigned int i=0; i<elementsNouveaux->size(); i++) {
	    	it = find (body->begin(), body->end(), elementsNouveaux->at(i));
	    	if (it == body->end()) {
	    		body->push_back(elementsNouveaux->at(i));
	    	}
	    }
	}

	void sortElements(vector<string> * input, vector<string> * output) {
		std::sort(output->begin(), output->end());
	}
};


/**
 * Syntax: ReadWriteFileTest ["text to be printed"]
 * 		If no text passed then will print a default one.
 */
int main( int argc, char *argv[] )  {
	Logger logger;

	// Getting the message to be saved and printed
	if(argc != 4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
		cerr << "ERROR: Missing input files.\n"
			 << endl;
		cout << "Syntax: "
			 << argv[0]
			 << " new_elements_file origin_file destination_file\n"
			 << endl;
	}

	// Read the list of new elements and the input file
	ReadFile readFile;
	vector<string> elementsNouveaux ;
	readFile.read(argv[1], &elementsNouveaux);

	vector<string> infile;
	readFile.read(argv[2], &infile);

	// Analyze
	Analyzer analyzer;
	string pattern = "/* Data_Base_Variables */";
	string pattern2 = "/* Data_Base_Variables */";
	int headerEnd       = analyzer.extractPosition(&infile, 0, pattern, pattern2);
	headerEnd++;

	pattern = "/* C+--- Inserted */";
	pattern2 = "/* C+--- Inserted */";
	int footerEnd       = analyzer.extractPosition(&infile, headerEnd, pattern, pattern2);
	footerEnd--;

	vector<string> body ;
	analyzer.extractBody(&infile, &body, headerEnd, footerEnd);
	logger.logVector("body", &body);

	// Process
	Processor processor;
	processor.process(&elementsNouveaux, &body);
	logger.logVector("body after additions", &body);

	// Save the result file
	WriteFile writeFile;
	writeFile.create(argv[3]);
	writeFile.append(&infile, 0, headerEnd);
	writeFile.append(&body, 0, body.size());
	writeFile.append(&infile, footerEnd, infile.size());
	writeFile.close();

	// Returning 0 for no execution error
	return 0;
}

/*
 * UML Diagram of the call sequence
 * http://plantuml.com/sequence-diagram
@startuml
actor User
control UpdateModules
User -> UpdateModules : execute (list,\ninfile,\noutfile)
UpdateModules -> ReadFile : read (list)
ReadFile --> UpdateModules : vector elementsNouveaux
UpdateModules -> ReadFile : read (infile)
ReadFile --> UpdateModules : vector infile
UpdateModules -> Analyzer : extractPosition(infile, initialPosition, pattern, pattern2)
Analyzer --> UpdateModules : int positionHeader
UpdateModules -> Analyzer : extractPosition(infile, initialPosition, pattern, pattern2)
Analyzer --> UpdateModules : int positionFooter
UpdateModules -> Analyzer : extractBody(infile,body vector, positionHeader, positionFooter)

UpdateModules -> Processor : process(elementsNouveaux, infile)
activate Processor
Processor -> Processor : insertMissing(elementsNouveaux, infile)
Processor -> Processor : sort(fichierOriginal2)
deactivate Processor
UpdateModules -> WriteFile : create (outFile)
UpdateModules -> WriteFile : append (infile, start, end)
UpdateModules -> WriteFile : append (body, start, end)
UpdateModules -> WriteFile : append (infile, start, end)
UpdateModules -> WriteFile : close ()

@enduml

*/
