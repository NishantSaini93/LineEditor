/**
*Purpose of this assignment is to practice with STL sequential container classes such as list,vector,string and iterator. 
*/
#ifndef _LineEd_INCLUDED
#define _LineEd_INCLUDED
#include<iostream>
#include<list>
#include<vector>
#include "Command.h"
using std::list;
using std::vector;
using std::string;

/**
*Allows a user to open,edit and save new or existing file with commands
*/
class LineEd {
public:
	/**
	*Constructor to initialize variables and check that file exist or not
	*@param stringFileName Name of txt file
	*/
	LineEd(string stringFileName);
	/**
    *Function to run command
	*@param stringCommand Command string
	*/
	void runCommand(string stringCommand);
	/**
	*function to loop command input
	*/
	void run();
private:
	/**
	*List of lines
	*/
	list<string> buffer{};
	/**
	*vector contains temprary data
	*/
	vector<string> clipboard{};
	/**
	*Variable to save File name
	*/
	string fileName{};
	/**
	*Current line variable
	*/
	int currentLine{0};

	/**
	*bool value for File is already present or not
	*/
	bool flagFilePresent{};
	
	/**
	*Bool to quit the run() function's while to quit program
	*/
	bool quitFlag{};

	/**
	*Function to check symbol is valid or not
	*@param ch
	*/
	bool resultForCommandSymbol(char ch);

	/**
	*Appends lines at the end and sets current line to last line entered
	*/
	void aCommand();

	/**
	*Insert text starting from line at address 1 and sets current line to last line entered
	*@param Address1 :Address from where line is inserted
	*/
	void iCommand(int Address1);

	/**
	*Past the clipboard data from line Address1 and sets current line to last line pasted
	*@param Address1 :Line number starting from which we paste
	*/
	void vCommand(int Address1);

	/**
	*Function deletes in range from Address1 to Address2 and sets current line to next line present after Address2(if present)
	*Or previous to Address1 (if present) otherwise undefined
	*@param Address1 :Address of line 1 in range
	*@param Address2 :Address of last line in range
	*/
	void dCommand(int Address1, int Address2);

	/**
	*It copy data from range to clipboard and sets current line to next line present after Address2 (if present)
	*Or previous to Address1 (if present) otherwise undefined
	*@param Address1 :Address of line 1 in range
	*@param Address2 :Address of last line in range
	*/
	void xCommand(int Address1, int Address2);
	
	/**
	*It joins whole range to line at Address1
	*Sets current line to Address1
	*@param Address1 :Address of line 1 in range
	*@param Address2 :Address of last line in range
	*/
	void jCommand(int Address1, int Address2);

	/**
	*It display data in given range
	*@param Address1 :Address of line 1 in range
	*@param Address2 :Address of last line in range
	*/
	void pCommand(int Address1, int Address2);

	/**
	*It finds and change particular string in given range
	*@param Address1 :Address of line 1 in range
	*@param Address2 :Address of last line in range
	*/
	void cCommand(int Address1, int Address2);

	/**
	*It moves current line up
	*/
	void uCommand();
	/**
	*It moves current line down
	*/
	void nCommand();
	/**
	*It moves current line to given address
	*/
	void gCommand(int Address1);
	/**
	*It gives option to save
	*/
	void wCommand();
	/**
	*It allows to quit
	*/
	void qCommand();
	/**
	*It is resposible to call different commands
	*@param stringCommand Command string
	*@param command Command object
	*/
	void runningCommand(string stringCommand,Command command);

	/**
	*This function is to convert string to integer
	*@param stringToConvert String for conversion
	*/
	int convertStringToInteger(string stringToConver);
};
#endif
