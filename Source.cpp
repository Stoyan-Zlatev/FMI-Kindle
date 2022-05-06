#include <iostream>
#include <cstring>
#include <fstream>
#include "MyString.h"
#include "Kindle.h"
#include "Book.h"

bool isPrefix(const char* text, const char* prefix)
{
	int i = 0;
	while (prefix[i] != '\0')
	{
		if (text[i] == '\0' || text[i] != prefix[i])
			return false;
		i++;
	}
	return true;
}

bool isDigit(char element)
{
	return element >= '0' && element <= '9';
}

void getCommandData(size_t startIndex, const char* text, char* title, char* data)
{
	size_t textLength = strlen(text);
	size_t currentIndex;
	for (currentIndex = startIndex; currentIndex < textLength; currentIndex++)
	{
		if (isDigit(text[currentIndex]) || text[currentIndex] == '"')
		{
			startIndex++;
			break;
		}
		title[currentIndex - startIndex] = text[currentIndex];
	}
	title[currentIndex - startIndex] = '\0';
	for (size_t i = currentIndex; i < textLength; i++)
	{
		if (text[i] == '"')
		{
			if (i == currentIndex)
			{
				currentIndex++;
				continue;
			}
			currentIndex++;
			break;
		}

		data[i - currentIndex] = text[i];
	}
	data[textLength - currentIndex] = '\0';
}

void getCommandData(size_t startIndex, const char* text, char* title)
{
	size_t textLength = strlen(text);
	size_t currentIndex;
	for (currentIndex = startIndex; currentIndex < textLength; currentIndex++)
	{
		if (isDigit(text[currentIndex]) || text[currentIndex] == '"')
		{
			startIndex++;
			break;
		}
		title[currentIndex - startIndex] = text[currentIndex];
	}
	title[currentIndex - startIndex] = '\0';
}

int parseCharToInt(char element)
{
	return element - '0';
}

int parseStringToInt(const char* data)
{
	int number = 0;
	for (size_t i = 0; data[i] != '\0'; i++)
	{
		number += parseCharToInt(data[i]);
		number *= 10;
	}
	number /= 10;

	return number;
}

int main()
{
	//books should be added to users not only to currentUser
	std::fstream sourceFile("FMIKindle.dat", std::ios::in | std::ios::out | std::ios::app | std::ios::binary);

	if (!sourceFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
		return -1;
	}

	char command[MaxContentLength];
	Kindle fmiKindle;
	fmiKindle.load(sourceFile);
	sourceFile.close();
	std::cout << ">";
	std::cin.getline(command, MaxContentLength);
	while (!isPrefix(command, "exit"))
	{
		try
		{
			if (isPrefix(command, "login"))
			{
				MyString username;
				std::cout << "Enter username: ";
				std::cin >> username;
				MyString password;
				std::cout << "Enter password: ";
				std::cin >> password;
				fmiKindle.login(std::move(username), std::move(password));
				std::cout << "\tWelcome," << fmiKindle.getCurrentUserName() << std::endl;
			}
			else if (isPrefix(command, "signup"))
			{
				MyString username;
				std::cout << "Enter username: ";
				std::cin >> username;
				MyString password;
				std::cout << "Enter password: ";
				std::cin >> password;
				fmiKindle.signup(std::move(username), std::move(password));
				std::cout << "\tUser registered!" << std::endl;
			}
			else if (isPrefix(command, "logout"))
			{
				fmiKindle.logout(sourceFile);
			}
			else if (isPrefix(command, "view"))
			{
				fmiKindle.view();
			}
			else if (isPrefix(command, "read"))
			{
				size_t startIndex = 5;
				int currentPageNumber;
				char title[MaxContentLength];
				char data[MaxContentLength];
				getCommandData(startIndex, command, title, data);
				fmiKindle.readBook(title);
				if (data[0] == '\0')
				{
					currentPageNumber = 0;
				}
				else
				{
					currentPageNumber = parseStringToInt(data);
				}

				char readCommand = '\0';
				while (readCommand != 'q')
				{
					fmiKindle.printBookPage(title, currentPageNumber);
					std::cin >> readCommand;
					if (readCommand == 'n')
					{
						currentPageNumber++;
					}
					else if (readCommand == 'p')
					{
						currentPageNumber--;
					}
				}
			}
			else if (isPrefix(command, "rates"))
			{
				size_t startIndex = 6;
				char title[MaxContentLength];
				getCommandData(startIndex, command, title);
				fmiKindle.printBookRating(title);
			}
			else if (isPrefix(command, "rate"))
			{
				size_t startIndex = 5;
				char title[MaxContentLength];
				char data[MaxContentLength];
				getCommandData(startIndex, command, title, data);
				fmiKindle.rateBookByName(title, fmiKindle.getCurrentUserName(), parseStringToInt(data));
			}
			else if (isPrefix(command, "write"))
			{
				std::cout << "Enter title: ";
				
				char title[MaxContentLength];
				std::cin.getline(title, MaxContentLength);
				
				size_t pagesCount;
				std::cout << "Pages count: ";
				std::cin.getline(command, MaxContentLength);
				
				pagesCount = parseStringToInt(command);
				Book currentBook(title, fmiKindle.getCurrentUserName());
				
				for (size_t i = 0; i < pagesCount; i++)
				{
					std::cout << "Page " << i + 1 << ": ";
					std::cin.getline(command, MaxContentLength);
					currentBook.addPage(command, i);
				}
				fmiKindle.addBook(currentBook);
			}
			else if (isPrefix(command, "comments"))
			{
				size_t startIndex = 9;
				char title[MaxContentLength];
				getCommandData(startIndex, command, title);
				fmiKindle.printBookComments(title);
			}
			else if (isPrefix(command, "comment"))
			{
				size_t startIndex = 8;
				char title[MaxContentLength];
				char data[MaxContentLength];
				getCommandData(startIndex, command, title, data);
				fmiKindle.addBookComment(title, data);
			}
			else if (isPrefix(command, "addPage"))
			{
				size_t startIndex = 8;
				char title[MaxContentLength];
				getCommandData(startIndex, command, title);
				std::cin.getline(command, MaxContentLength);
				fmiKindle.addBookPage(title, command);
			}
			else if (isPrefix(command, "removePage"))
			{
				size_t startIndex = 11;
				char title[MaxContentLength];
				getCommandData(startIndex, command, title);
				fmiKindle.removeBookLastPage(title);
			}
		}
		catch (std::invalid_argument& e)
		{
			std::cout << e.what() << std::endl;
		}

		std::cout << ">";
		std::cin.getline(command, MaxContentLength);
	}
	
	std::fstream newFile("FMIKindle.dat", std::ios::trunc | std::ios::binary | std::ios::out | std::ios::ate);
	if (!newFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
		return -1;
	}
	newFile.seekp(0, std::ios::beg);
	fmiKindle.saveToFile(newFile);
	newFile.seekp(0, std::ios::beg);
	newFile.close();
}