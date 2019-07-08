#include "Command.h"
#include <unordered_map>
using std::unordered_map;

/**
*Constructor sets and parses the command string
*@param command_line Command string
*/
Command::Command(const string& command_line) {
	parse(command_line);
}

/**
*Function that resets and parses the command string
*@param command_line Command string
*/
void Command::parse(const string& command_line) {
	std::string temp{ command_line };
	//Remove white spaces from command
	temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
	//If command is blank
	if (temp.size() == 0) {
		symbol = "p";
		address1 = ".";
		address2 = ".";
		status = true;
	}
	//if only thing present in command is ","
	else if (temp == ",") {
		symbol = "p";
		address1 = ".";
		address2 = ".";
		status = true;
	}
	else {
		size_t found{ temp.find(",") };
		//if there is "," in command
		if (found != string::npos) {
			//if position of "," is at first :,y
			if (found == 0) {
				//trim ,y to y and store in st2
				string st2{ temp.substr(found + 1, temp.size() - 1) };
				//if y in ,y is "." or "$" or int
				if ((st2 == ".") || (st2 == "$") || checkingForException(st2)) {
					symbol = "p";
					address1 = ".";
					address2 = st2;
					status = true;
				}
				//if y in ,y is something gebrish
				else {
					symbol = "p";
					address1 = ".";
					address2 = st2;
					status = false;
					//bad address two
				}
			}
			else if (isSymbolPresent(tolower(temp[0])) && found == 1 && temp.size() == 2) {
				symbol = temp[0];
				address1 = ".";
				address2 = ".";
				status = true;
			}
			//if position of "," is at last:x,zx,
			else if (found == temp.size() - 1) {
				//zx, so here we are checking is there any command symbol at first place
				if (isSymbolPresent(tolower(temp[0]))) {

					//If yes command symbol is there there should be x also
					string st1{ temp.substr(1, found - 1) };

					//if x is "." or "$" or int
					if ((st1 == ".") || (st1 == "$") || checkingForException(st1)) {
						symbol = temp[0];
						address1 = st1;
						address2 = st1;
						status = true;
					}
					//if address format is not good
					else {
						symbol = temp[0];
						address1 = st1;
						address2 = st1;
						status = false;
						//bad address one
					}
				}
				//if there is no symbol:x,
				else {
					string st1{ temp.substr(0, found) };
					//-x,
					if (st1[0] == '-') {
						string st11{ st1.substr(1,found - 1) };
						if ((st11 == ".") || (st11 == "$") || checkingForException(st11)) {
							symbol = "p";
							address1 = st1;
							address2 = st11;
							status = true;
						}
						else {
							symbol = "p";
							address1 = st1;
							address2 = st11;
							status = false;
							//bad command symbol
						}
					}
					else {
						if ((st1 == ".") || (st1 == "$") || checkingForException(st1)) {
							symbol = "p";
							address1 = st1;
							address2 = st1;
							status = true;
						}
						else {
							symbol = "p";
							address1 = st1;
							address2 = st1;
							status = false;
							//bad command symbol
						}
					}

				}
			}
			//if position of "," is in between
			else {
				if (found == 1) {
					string st1{ temp.substr(0, found) };
					string st2{ temp.substr(found + 1, temp.size() - 1) };
					//if the thing is like symbol,y:z,y
					if (isSymbolPresent(tolower(st1[0]))) {
						if ((st2 == ".") || (st2 == "$") || checkingForException(st2)) {
							symbol = st1[0];
							address1 = ".";
							address2 = st2;
							status = true;
						}
						//if not converting to integer and not "." and not "$"
						else {
							symbol = st1[0];
							address1 = ".";
							address2 = st2;
							status = false;
							//bad address 2
						}
					}//x,y where x=. or $ or int
					else if ((st1 == ".") || (st1 == "$") || checkingForException(st1)) {
						//x,y where y is "." or "$" or int
						if ((st2 == "$") || (st2 == ".") || checkingForException(st2)) {

							symbol = "p";
							address1 = st1;
							address2 = st2;
							status = true;
						}
						//if not converting to integer
						else {
							symbol = "p";
							address1 = st1;
							address2 = st2;
							status = false;
							//bad address 2
						}
					}

					//xadas,y  //bad command
					else {
						symbol = "p";
						address1 = st1;
						address2 = st2;
						status = false;
						//bad command
					}
				}
				//if something like zx,y
				else {
					string st1{ temp.substr(0, found) };
					string st2{ temp.substr(found + 1, temp.size() - 1) };
					//suppose there is z:so symbol
					if (isSymbolPresent(tolower(st1[0]))) {
						string sttemp = temp.substr(1, found - 1);
						//z.,y or z$,y or zint,y
						if ((sttemp == ".") || (sttemp == "$") || checkingForException(sttemp)) {
							//z.,. or z.,$ or z.,int
							if ((st2 == ".") || (st2 == "$") || checkingForException(st2)) {
								symbol = st1[0];
								address1 = sttemp;
								address2 = st2;
								status = true;
							}
							else {
								symbol = st1[0];
								address1 = sttemp;
								address2 = st2;
								//bad address 2
								status = false;
							}
						}
						//zbad1,y
						else {
							symbol = st1[0];
							address1 = sttemp;
							address2 = st2;
							status = false;
							//bad address 1
						}
					}
					//No z simple x,y
					else {//.,y or $,y or int,y
						if ((st1 == ".") || (st1 == "$") || checkingForException(st1) || ((st1[0] == '-') && checkingForException(st1.substr(1, st1.size() - 1)))) {
							string tempStr = st1;
							if (tempStr[0] == '-') {
								tempStr = tempStr.substr(1, tempStr.size() - 1);
							}
							if ((st2 == ".") || (st2 == "$") || checkingForException(st2)) {
								symbol = "p";
								address1 = st1;
								address2 = st2;
								status = true;
							}
							else {
								symbol = "p";
								address1 = st1;
								address2 = st2;
								status = false;
							}
						}
						else {
							symbol = "p";
							address1 = st1;
							address2 = st2;
							status = false;
							//bad command symbol
						}
					}
				}
			}
		}


		//2 part when there is no "," in command
		//if there is no "," in command
		else {
			if (temp.size() == 1) {
				if (isSymbolPresent(tolower(temp[0]))) {
					symbol = temp[0];
					address1 = ".";
					address2 = ".";
					status = true;
				}
				else if (checkingForException(temp)) {
					symbol = "p";
					address1 = temp;
					address2 = temp;
					status = true;
				}
				else if (temp == ".") {
					symbol = "p";
					address1 = temp;
					address2 = temp;
					status = true;
				}
				else if (temp == "$") {
					symbol = "p";
					address1 = temp;
					address2 = temp;
					status = true;
				}
				else {
					//bad command
					symbol = temp;
					address1 = ".";
					address2 = ".";
					status = false;
				}
			}
			//if size is greater
			else {//zx

				if (isSymbolPresent(tolower(temp[0]))) {
					string strtemp{ temp.substr(1, temp.size() - 1) };
					if ((strtemp == ".") || (strtemp == "$") || checkingForException(strtemp)) {
						symbol = temp[0];
						address1 = strtemp;
						address2 = strtemp;
						status = true;
					}
					else {
						symbol = temp[0];
						address1 = strtemp;
						address2 = strtemp;
						status = false;
						//bad address 1
					}
				}
				//else if not symbol x only
				else {
					string tempStr = temp;
					if ((temp[0] == '-') && checkingForException(temp.substr(1, temp.size() - 1))) {
						temp = temp.substr(1, temp.size() - 1);
					}
					if (checkingForException(temp)) {
						symbol = "p";
						address1 = tempStr;
						address2 = tempStr;
						status = true;
					}
					else {
						symbol = "p";
						address1 = tempStr;
						address2 = tempStr;
						status = false;
					}
				}
			}
		}
	}

}

/**
*Function to get symbol of command
*@return Symbol of command
*/
string Command::getSymbol() {
	return symbol;
}

/**
*Function to get Address-1 of command
*@return Address-1 of command
*/
string Command::getAddress1() {
	return address1;
}

/**
*Function to get Address-2 of command
*@return Address-2 of command
*/
string Command::getAddress2() {
	return address2;
}

/**
*Function to get Status of command
*@return Bool value for status of command
*/
bool Command::getStatus() {
	return status;
}

/**
*Function to status of command
*@param status Status of command
*/
void Command::setStatus(bool boolStatus) {
	status = boolStatus;
}

/**
*This function check whether any symbol  is present in command irrespective of validity.
*@param ch Symbol in command
*/
bool Command::isSymbolPresent(char ch) {
	const char* forComparision = &ch;

	unordered_map<string, int> commandSymbols;
	commandSymbols["a"] = 1;
	commandSymbols["b"] = 1;
	commandSymbols["c"] = 1;
	commandSymbols["d"] = 1;
	commandSymbols["e"] = 1;
	commandSymbols["f"] = 1;
	commandSymbols["g"] = 1;
	commandSymbols["h"] = 1;
	commandSymbols["i"] = 1;
	commandSymbols["j"] = 1;
	commandSymbols["k"] = 1;
	commandSymbols["l"] = 1;
	commandSymbols["m"] = 1;
	commandSymbols["n"] = 1;
	commandSymbols["o"] = 1;
	commandSymbols["p"] = 1;
	commandSymbols["q"] = 1;
	commandSymbols["r"] = 1;
	commandSymbols["s"] = 1;
	commandSymbols["t"] = 1;
	commandSymbols["u"] = 1;
	commandSymbols["v"] = 1;
	commandSymbols["w"] = 1;
	commandSymbols["x"] = 1;
	commandSymbols["y"] = 1;
	commandSymbols["z"] = 1;
	bool exists = (commandSymbols.find(&ch) != commandSymbols.end());
	return exists;

}

/**
*This function tackle the exception of stoi during conversion of invalid string
*@param stringToConvert String that is being converted
*/
bool Command::checkingForException(string stringToConvert) {
	try {
		int flagForNegative{ 0 };
		size_t sizeOfString{ stringToConvert.size() };
		for (int i{ 0 }; i < sizeOfString; i++) {
			string temp{ stringToConvert[i] };
			if (temp == "-") {
				flagForNegative = 1;
			}
		}
		if (flagForNegative) {
			stringToConvert = stringToConvert.substr(1, stringToConvert.size() - 1);
		}
		if (std::to_string(stoi(stringToConvert)) == stringToConvert) {
			//size_t sizeOfString{ stringToConvert.size() };
			for (int i{ 0 }; i < sizeOfString; i++) {
				string temp{ stringToConvert[i] };
				string test{ std::to_string(stoi(temp)) };
			}

			return true;
		}
		else {
			return false;
		}
	}

	catch (exception e) {
		return false;
	}
}
