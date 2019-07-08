/**
*Purpose of this assignment is to practice with STL sequential container classes such as list,vector,string and iterator.
*/
#ifndef _Command_INCLUDED
#define _Command_INCLUDED
#include<iostream>
using std::string;
#include<unordered_map>
using std::unordered_map;
#include <regex>
using std::cout;
#include<exception>
using std::exception;

/**
/*This class parses the command into different parts
*/
class Command {
public:
	/**
	*Constructor sets and parses the command string
	*@param command_line Command string 
	*/
	Command(const string& command_line);

	/**
	*Function that resets and parses the command string
	*@param command_line Command string 
	*/
	void parse(const string& command_line);

	/**
	*Function to get symbol of command
	*@return Symbol of command
	*/
	string getSymbol();

	/**
	*Function to get Address-1 of command
	*@return Address-1 of command
	*/
	string getAddress1();

	/**
	*Function to get Address-2 of command
	*@return Address-2 of command
	*/
	string getAddress2();

	/**
	*Function to get Status of command
	*@return Bool value for status of command 
	*/
	bool getStatus();

	/**
	*Function to status of command
	*@param status Status of command
	*/
	void setStatus(bool status);

	//The two functions below are made two remove redundant code
	/**
	*This function check whether any symbol  is present in command irrespective of validity.
	*@param ch Symbol in command
	*/
	bool isSymbolPresent(char ch);
	
	/**
	*This function tackle the exception of stoi during conversion of invalid string
	*@param stringToConvert String that is being converted
	*/
	bool checkingForException(string stringToConvert);
private:

	/**
	*Stores supplied command line
	*/
	string command_line;

	/**
	*Stores validity of current command
	*/
	bool status{false};

	/**
	*Stores the command symbol
	*/
	string symbol;

	/**
	*Stores address 1
	*/
	string address1;

	/**
	*Stores address 2
	*/
	string address2;

	
};
#endif
