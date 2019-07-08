#include "LineEd.h"
#include<fstream>
using std::endl;
/**
*Constructor to initialize variables and check that file exist or not
*@param stringFileName Name of txt file
*/
LineEd::LineEd(string stringFileName) {
	//to check file is already preasent or not
	std::fstream fileStream;
	fileStream.open(stringFileName);
	fileName = stringFileName;
	if (fileStream.fail()) {
		// file could not be opened
		flagFilePresent = false;
	}
	else {
		flagFilePresent = true;
		fileStream.close();
	}
	//if File is present
	if (flagFilePresent == true) {
		std::ifstream file(stringFileName);
		string str;
		while (std::getline(file, str))
		{
			buffer.push_back(str);
		}
		currentLine = buffer.size();

		cout << "\"" << fileName << " \"" << buffer.size() << " lines" << endl;
		cout << "Entering command mode." << endl << "?";
	}
	else {
		if (stringFileName.empty()) {
			cout << "\"?\"" << " " << "[New File]" << endl;
			cout << "Entering command mode." << endl << "?";
		}
		else {
			cout << "\"" << stringFileName << "\"" << " " << "[New File]" << endl;
			cout << "Entering command mode." << endl << "?";
		}

	}
}

/**
*function to loop command input
*/
void LineEd::run() {
	quitFlag = true;
	while (quitFlag) {
		string commandInput;
		getline(std::cin, commandInput);
		commandInput.erase(remove(commandInput.begin(), commandInput.end(), ' '), commandInput.end());
		runCommand(commandInput);
		if ((commandInput[0] != 'q'))
		{
			cout << "?";
		}

	}
}
/**
*Function to run command
*@param stringCommand Command string
*/
void LineEd::runCommand(string stringCommand) {
	string commandInput = stringCommand;
	Command command(commandInput);
	//if list is empty
	if (buffer.size() == 0) {
		if (tolower(command.getSymbol()[0]) == 'a' || tolower(command.getSymbol()[0]) == 'i' || tolower(command.getSymbol()[0]) == 'q' || (clipboard.size() > 0) && (tolower(command.getSymbol()[0]) == 'v')) {
			runningCommand(commandInput, command);
		}
		else {
			if (resultForCommandSymbol(tolower(command.getSymbol()[0]))) {
				if (clipboard.size() == 0) {
					cout << "File empty.Must use one of I,A,Q commands." << endl;
				}
				else {
					cout << "File empty.Must use one of I,A,Q,V commands." << endl;
				}

			}
			else {
				runningCommand(commandInput, command);
			}

		}

	}//if there is things present in buffer.
	else {
		runningCommand(commandInput, command);
	}
}

/**
*Function to check symbol is valid or not
*@param ch
*/
bool LineEd::resultForCommandSymbol(char ch) {
	const char* forComparision = &ch;
	unordered_map<string, int> commandSymbols;
	commandSymbols["a"] = 1;
	commandSymbols["i"] = 1;
	commandSymbols["v"] = 1;
	commandSymbols["d"] = 1;
	commandSymbols["x"] = 1;
	commandSymbols["j"] = 1;
	commandSymbols["p"] = 1;
	commandSymbols["c"] = 1;
	commandSymbols["u"] = 1;
	commandSymbols["n"] = 1;
	commandSymbols["g"] = 1;
	commandSymbols["w"] = 1;
	commandSymbols["q"] = 1;
	bool exists = (commandSymbols.find(&ch) != commandSymbols.end());
	return exists;

}

/**
*Appends lines at the end and sets current line to last line entered
*/
void LineEd::aCommand() {
	cout << endl << "Entering input mode." << endl;
	string inputLine;
	getline(std::cin, inputLine);
	int lineNumber{ 0 };
	while (!(inputLine == ".")) {
		buffer.push_back(inputLine);
		lineNumber++;
		getline(std::cin, inputLine);
	}
	currentLine = buffer.size();
	cout << "Entering command mode." << endl;
}

/**
*Insert text starting from line at address 1 and sets current line to last line entered
*@param Address1 :Address from where line is inserted
*/
void LineEd::iCommand(int Address1) {
	std::list<string>::iterator it1;
	string inputLine;
	if (buffer.size() == 0) {
		cout << endl << "Entering input mode." << endl;
		string inputLine;
		getline(std::cin, inputLine);
		int lineNumber{ 0 };
		while (!(inputLine == ".")) {
			buffer.push_back(inputLine);
			lineNumber++;
			getline(std::cin, inputLine);
		}
		currentLine = buffer.size();
		cout << "Entering command mode." << endl;
	}
	else {
		cout << "Entering input mode." << endl;
		it1 = buffer.begin();
		getline(std::cin, inputLine);
		int lineNumber{ 0 };
		int flagForBufferSize{ 0 };
		if (buffer.size() == 0) {
			flagForBufferSize = 1;
		}
		advance(it1, Address1 - 1);
		while (!(inputLine == ".")) {
			buffer.insert(it1, inputLine);
			lineNumber++;
			getline(std::cin, inputLine);
		}
		if (flagForBufferSize) {
			currentLine = buffer.size();
		}
		else {
			currentLine = Address1 + lineNumber - 1;
		}

		cout << "Entering command mode." << endl;

	}

}

/**
*Past the clipboard data from line Address1 and sets current line to last line pasted
*@param Address1 :Line number starting from which we paste
*/
void LineEd::vCommand(int Address1) {
	//buffer is empty but clipboard has content
	if (buffer.size() == 0) {
		int count{ 0 };
		for (auto element : clipboard) {
			buffer.push_back(element);
			count++;
		}
		currentLine = count;
		clipboard.clear();
	}
	else {
		std::list<string>::iterator it;
		it = buffer.begin();
		int count{ 0 };
		advance(it, Address1 - 1);
		std::reverse(clipboard.begin(), clipboard.end());
		for (auto element : clipboard) {
			it = buffer.insert(it, element);
			count++;
		}
		currentLine = Address1 + count - 1;
		clipboard.clear();
	}

}

/**
*Function deletes in range from Address1 to Address2 and sets current line to next line present after Address2(if present)
*Or previous to Address1 (if present) otherwise undefined
*@param Address1 :Address of line 1 in range
*@param Address2 :Address of last line in range
*/
void LineEd::dCommand(int Address1, int Address2) {
	std::list<string>::iterator it1;
	std::list<string>::iterator it2;
	it1 = buffer.begin();
	it2 = buffer.begin();
	size_t bufferSizeTemp{ buffer.size() };
	advance(it1, Address1 - 1);
	advance(it2, Address2);
	buffer.erase(it1, it2);
	if (bufferSizeTemp > Address2) {
		currentLine = Address1;
	}
	else if (buffer.size() == 0) {
		currentLine = 0;
	}
	else {
		currentLine = Address1 - 1;
	}
}

/**
*It copy data from range to clipboard and sets current line to next line present after Address2 (if present)
*Or previous to Address1 (if present) otherwise undefined
*@param Address1 :Address of line 1 in range
*@param Address2 :Address of last line in range
*/
void LineEd::xCommand(int Address1, int Address2) {
	std::list<string>::iterator it1;
	it1 = buffer.begin();
	advance(it1, Address1 - 1);
	size_t bufferSizeTemp{ buffer.size() };
	for (int i{ Address1 }; i <= Address2; i++) {
		clipboard.push_back(*it1);
		it1 = buffer.erase(it1);
	}
	if (bufferSizeTemp > Address2) {
		currentLine = Address1;
	}
	else if (buffer.size() == 0) {
		currentLine = 0;
	}
	else {
		currentLine = Address1 - 1;
	}
}

/**
*It joins whole range to line at Address1
*Sets current line to Address1
*@param Address1 :Address of line 1 in range
*@param Address2 :Address of last line in range
*/
void LineEd::jCommand(int Address1, int Address2) {
	std::list<string>::iterator it1;
	std::list<string>::iterator it2;
	it1 = buffer.begin();
	it2 = buffer.begin();
	std::list<string>::iterator it3;
	vector<char> temp{};
	int i{ 1 };
	for (auto element : buffer) {
		if (i >= Address1 && i <= Address2) {
			for (int j{ 0 }; j < (element).size(); j++) {
				temp.push_back((element)[j]);
			}
		}
		i++;

	}
	advance(it1, Address1 - 1);
	advance(it2, Address2);
	buffer.erase(it1, it2);
	string s1(temp.begin(), temp.end());
	it3 = buffer.begin();
	advance(it3, Address1 - 1);
	buffer.insert(it3, s1);
	currentLine = Address1;
}

/**
*It display data in given range
*@param Address1 :Address of line 1 in range
*@param Address2 :Address of last line in range
*/
void LineEd::pCommand(int Address1, int Address2) {
	int i{ 1 };
	for (auto element : buffer) {
		if (i >= Address1 && i <= Address2) {
			if (i == currentLine)
			{
				cout << i << "> " << element << endl;
			}
			else {
				cout << i << ": " << element << endl;
			}

		}
		i++;
	}
}

/**
*It finds and change particular string in given range
*@param Address1 :Address of line 1 in range
*@param Address2 :Address of last line in range
*/
void LineEd::cCommand(int Address1, int Address2) {
	std::list<string>::iterator it1;
	it1 = buffer.begin();
	string changeText{};
	string replaceString{};
	advance(it1, Address1 - 1);
	cout << "change?";
	std::getline(std::cin, changeText);
	cout << "    to?";
	std::getline(std::cin, replaceString);
	int i{ 1 };
	int changeFlag2{ 0 };
	for (auto element : buffer) {
		if (i >= Address1 && i <= Address2) {
			size_t pos = element.find(changeText);
			while (pos != std::string::npos)
			{
				changeFlag2 = i;
				// Replace this occurrence of Sub String
				element.replace(pos, changeText.size(), replaceString);
				// Get the next occurrence from the current position
				pos = element.find(changeText, pos + replaceString.size());
				advance(it1, 1);
				(*it1) = element;
				currentLine = i;
			}
		}

		i++;
	}
}

/**
*It moves current line up
*/
void LineEd::uCommand() {
	list<string>::iterator it;
	it = buffer.begin();
	if (buffer.size() == 1 && currentLine == 1) {
		cout << "BOF reached";
	}
	else {
		if (currentLine == 1) {
			cout << "BOF reached";
		}
		else {
			currentLine = currentLine - 1;
			advance(it, currentLine - 1);
			cout << currentLine << ">" << *it << endl;
		}
	}
}

/**
*It moves current line down
*/
void LineEd::nCommand() {
	list<string>::iterator it;
	it = buffer.begin();
	if (buffer.size() == 1 && currentLine == 1) {
		cout << "EOF reached" << endl;
	}
	else if (currentLine == buffer.size()) {
		cout << "EOF reached" << endl;
	}
	else {
		currentLine++;
		advance(it, currentLine - 1);
		cout << currentLine << ">" << *it << endl;
	}
}

/**
*It moves current line to given address
*/
void LineEd::gCommand(int Address1) {
	list<string>::iterator it;
	it = buffer.begin();
	currentLine = Address1;
	advance(it, Address1 - 1);
	cout << currentLine << ">" << *it << endl;
}

/**
*It gives option to save
*/
void LineEd::wCommand() {
	if (fileName.empty()) {
		string newFileName;
		cout << "Enter the name of file to write to:";
		getline(std::cin, newFileName);
		std::ofstream outfile(newFileName, std::ofstream::out | std::ofstream::trunc);
		if (!outfile.is_open()) {
			cout << "Unable to create file" << endl;
			return;
		}
		fileName = newFileName;
		int i{ 0 };
		for (auto element : buffer) {
			i++;
			if (i == buffer.size()) {
				outfile << element;
			}
			else {
				outfile << element << std::endl;
			}
		}
		outfile.close();

	}
	else {
		cout << "Save changes to file:" << fileName << "(y or n)?";
		string input;
		std::getline(std::cin, input);
		if (input == "y") {
			std::ofstream outfile(fileName, std::ofstream::out | std::ofstream::trunc);
			if (!outfile.is_open()) {
				cout << "Unable to create file" << endl;
				return;
			}
			int i{ 0 };
			for (auto element : buffer) {
				i++;
				if (i == buffer.size()) {
					outfile << element;
				}
				else {
					outfile << element << std::endl;
				}
			}
			outfile.close();
		}
		else {
			string newFileName;
			cout << "Enter the name of file to write to:";
			getline(std::cin, newFileName);
			std::ofstream outfile(newFileName, std::ofstream::out | std::ofstream::trunc);
			if (!outfile.is_open()) {
				cout << "Unable to create file" << endl;
				return;
			}
			fileName = newFileName;
			int i{ 0 };
			for (auto element : buffer) {
				i++;
				if (i == buffer.size()) {
					outfile << element;
				}
				else {
					outfile << element << std::endl;
				}
			}
			outfile.close();
		}
	}
	cout << buffer.size() << " lines written to file " << "\"" << fileName << "\"" << endl;
	currentLine = buffer.size();
}

/**
*It allows to quit
*/
void LineEd::qCommand() {
	string input;
	cout << "There are unsaved changes." << endl;
	cout << "Do you wish to save the changes(y or n)? ";
	getline(std::cin, input);
	if (input == "y") {
		if (fileName.empty()) {
			string newFileName;
			cout << "Enter the name of file to write to:";
			getline(std::cin, newFileName);
			std::ofstream outfile(newFileName, std::ofstream::out | std::ofstream::trunc);
			if (!outfile.is_open()) {
				cout << "Unable to create file" << endl;
				return;
			}
			int i{ 0 };
			for (auto element : buffer) {
				i++;
				if (i == buffer.size()) {
					outfile << element;
				}
				else {
					outfile << element << std::endl;
				}
				fileName = newFileName;

			}
			outfile.close();
		}
		else {
			cout << "Save changes to file:" << fileName << "(y or n)?";
			string input;
			std::getline(std::cin, input);
			if (input == "y") {
				std::ofstream outfile(fileName, std::ofstream::out | std::ofstream::trunc);
				if (!outfile.is_open()) {
					cout << "Unable to create file" << endl;
					return;
				}
				int i{ 0 };
				for (auto element : buffer) {
					i++;
					if (i == buffer.size()) {
						outfile << element;
					}
					else {
						outfile << element << std::endl;
					}


				}
				outfile.close();
			}
			else {
				string newFileName;
				cout << "Enter the name of file to write to:";
				getline(std::cin, newFileName);
				std::ofstream outfile(newFileName, std::ofstream::out | std::ofstream::trunc);
				if (!outfile.is_open()) {
					cout << "Unable to create file" << endl;
					return;
				}
				int i{ 0 };
				fileName = newFileName;
				for (auto element : buffer) {
					i++;
					if (i == buffer.size()) {
						outfile << element;
					}
					else {
						outfile << element << std::endl;
					}
				}
				outfile.close();
			}
		}
		cout << buffer.size() << " lines written to file " << "\"" << fileName << "\"" << endl;
		cout << "bye" << endl;
		buffer.clear();
	}
	quitFlag = false;
}

/**
*It is resposible to call different commands
*@param stringCommand Command string
*@param command Command object
*/
void LineEd::runningCommand(string stringCommand, Command command) {
	char symbolOfCommand = tolower(command.getSymbol()[0]);
	int addressFirst{};
	int addressSecond{};
	if (resultForCommandSymbol(symbolOfCommand) && command.getStatus()) {
		if ((command.getAddress1() == ".") || (command.getAddress1() == "$")) {
			if ((command.getAddress2() == "$") || (command.getAddress2() == ".")) {
				//address1 and address2
				if (command.getAddress1() == ".") {
					addressFirst = currentLine;
				}
				if (command.getAddress1() == "$") {
					addressFirst = buffer.size();
				}
				if (command.getAddress2() == ".") {
					addressSecond = currentLine;
				}
				if (command.getAddress2() == "$") {
					addressSecond = buffer.size();
				}
				
			}
			else {
				if (command.getAddress1() == ".") {
					addressFirst = currentLine;
				}
				if (command.getAddress1() == "$") {
					addressFirst = buffer.size();
				}
				//address1
				addressSecond = convertStringToInteger(command.getAddress2());
			}

		}
		else if ((command.getAddress2() == ".") || (command.getAddress2() == "$")) {
			addressFirst = convertStringToInteger(command.getAddress1());
			if (command.getAddress2() == ".") {
				addressSecond = currentLine;
			}
			if (command.getAddress2() == "$") {
				addressSecond = buffer.size();
			}
			//address2
		}
		else {
			addressFirst = convertStringToInteger(command.getAddress1());
			addressSecond = convertStringToInteger(command.getAddress2());

		}

		if (addressFirst <= 0) {
			addressFirst = 1;
		}
		if (addressFirst > buffer.size()) {
			addressFirst = buffer.size();
		}

		if (addressSecond <= 0) {
			addressSecond = 1;
		}
		if (addressFirst > buffer.size()) {
			addressFirst = buffer.size();
		}
		//this is to swap if address1 is greater then address 2
		if (addressFirst > addressSecond) {
			int temp{ addressFirst };
			addressFirst = addressSecond;
			addressSecond = temp;
		}
		switch (symbolOfCommand) {
		case 'a':aCommand(); break;
		case 'i':iCommand(addressFirst); break;
		case 'v':vCommand(addressFirst); break;
		case 'd':dCommand(addressFirst, addressSecond); break;
		case 'x':xCommand(addressFirst, addressSecond); break;
		case 'j':jCommand(addressFirst, addressSecond); break;
		case 'p':pCommand(addressFirst, addressSecond); break;
		case 'c':cCommand(addressFirst, addressSecond); break;
		case 'u':uCommand(); break;
		case 'n':nCommand(); break;
		case 'g':gCommand(addressFirst); break;
		case 'w':wCommand(); break;
		case 'q':qCommand(); break;
		}

	}
	else {
		if (!(resultForCommandSymbol(symbolOfCommand))) {
			cout << "bad command:" << command.getSymbol() << endl;
		}
		else if (command.getAddress1() == ".") {
			cout << "Bad Address 2:" << command.getAddress2() << endl;
		}
		else if (command.getAddress1() == "$") {
			cout << "Bad Address 2:" << command.getAddress2() << endl;
		}
		else if (!(command.checkingForException(command.getAddress1()))) {
			cout << "Bad address 1:" << command.getAddress1() << endl;
		}
		else {
			cout << "Bad Address 2:" << command.getAddress2() << endl;
		}
	}
}

/**
*This function is to convert string to integer
*@param stringToConvert String for conversion
*/
int LineEd::convertStringToInteger(string stringToConvert) {
	int flagForNegative{ 0 };
	size_t sizeOfString{ stringToConvert.size() };
	for (int i{ 0 }; i < sizeOfString; i++) {
		string temp{ stringToConvert[i] };
		if (temp == "-") {
			flagForNegative = 1;
		}
	}
	if (flagForNegative) {
		return 0;
	}
	int result{ stoi(stringToConvert) };
	return result;
}