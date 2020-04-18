#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cstdlib>

using namespace std;

enum FSMTransitions {
	Ignore = 0,
	Integer,
	Real,
	String,
	Operator,
	Unknown,
	Space
};


int stateTable[7][7] = { {0,  Integer, Real, String, Operator, Unknown, Space},
			   {Integer,   Integer, Real, Ignore, Ignore, Ignore, Ignore},
			   {Real,         Real, Unknown, Ignore, Ignore, Ignore, Ignore},
			   {String,     String, Ignore, String, String, Unknown, Ignore},
			   {Operator,   Ignore, Ignore, String, Ignore, Ignore, Ignore},
			   {Unknown,   Unknown, Unknown, Unknown, Unknown, Unknown, Ignore},
			   {Space,      Ignore, Ignore, Ignore, Ignore, Ignore, Ignore} };

char keywords[20][15] = { "int", "float", "bool", "true", "false", "if", "else", "then", "endif", "while",
				  "whileend", "do", "doend", "for", "forend", "input", "output", "and", "or" , "not" };


struct lexeme
{
	string lex; //String or char? Should be a string according to professor since we are taking in words from the text document
	int lexNumber = 0; //lexeme
	string token; //This will hold text coming from the file
};

lexeme *lexArr = new lexeme[1000000]; //Create a type lexeme array to store data from the text file 
int countWord; //Acts as the index for the lexArr array

class Analysis{
	Analysis() {}; //Default Contructor

	Analysis(string filename, string outputFile) {
		//Variables
		lexeme tool;
		int countWord = 0;
		char currentChar = ' ';
		int col = Ignore;
		int currentState = Ignore;
		int prevState = Ignore;
		string currentWord = "";

		//Notes: Group Keywords and Identifiers as strings in one loop then in a nested loop do a comparision with the strings to determine if they are 
		//keywords with the array. If there are any strings left that are not keywords then they will be labeled as idenitifiers.

		//This function determines the state of the character being read
		int getCharState(char currentC){
			//whitespace
			if (isspace(currentC)) { return Space; }

			//Checks for integers
			else if (isdigit(currentC)) { return Integer; }

			//Checks for real numbers
			else if (currentC == '.') { return Real; }

			//Checks for operators
			else if (ispunct(currentC)) { return Operator; }

			//Check for Strings then do a comparison to see if it is a keyword or identifier
			else if (isalpha(currentC)) { return String; }

			return Unknown;
		}

		string lexName(int lexeme) { //String function that uses a switch statement to return what a certain token is
			switch (lexeme) {
			case Integer:
				return "Integer";
				break;
			case Real:
				return "Real";
				break;
			case String:
				return "String";
				break;
			case Operator:
				return "Operator";
				break;
			case Unknown:
				return "Unknown";
				break;
			case Space:
				return "Space";
				break;
			default:
				return "Error";
				break;
			}
		}


		//File objects
		fstream file(filename, ios::in); //This will read in the file
		ofstream fileWriter; //Created so we can write the output to a separate file

		fileWriter.open(outputFile); //This will create a new file to write the output to
		fileWriter << "Token:		Lexeme:\n";

		if (!file.is_open()) {
			cout << "Cannot open file";
		}
		else {
			while (!file.eof()) {

				file >> lexArr[countWord].token; /*Stores each word and character as a string from the file
									 into the struct lexeme under the variable*/
				fileWriter << "\n";
				currentWord = lexArr[countWord].token;//Gets the word from the struct array and sets it to a string

				for (size_t i = 0; i < currentWord.length(); i++) {
					currentChar = currentWord[i]; //Grab each character from the word that came from the array
					col = getCharState(currentChar); //This will return the transition type for the current character

					currentState = stateTable[currentState][col]; //Get the current state from the current word

					if(currentState != Ignore) {
						tool.token = currentWord;

						if (prevState == Ignore) {
							tool.lexNumber = currentState;
						}
						else {
							tool.lexNumber = prevState;
						}

						tool.lex = lexName(tool.lexNumber);
						fileWriter << tool.token << "		" << tool.lex << "\n"; //Write the results to the text file
						string compareWord = tool.lex;
						//Have a series of if statements that use the functions from the syntax code 
						if (compareWord.compare("String")) {
							cout << Keywords();
						}

						currentWord = "";
					}
					else {
						currentWord += currentChar;
						i++;
					}
					revState = currentState;
				}
				countWord++;
			}
		}
		fileWriter.close();
		file.close();
	}
};