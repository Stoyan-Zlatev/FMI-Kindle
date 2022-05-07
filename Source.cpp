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

size_t getCommandLength(const char* text)
{
	size_t textLength = strlen(text);

	for (size_t i = 0; i < textLength; i++)
	{
		if (text[i] == ' ' || text[i] == '\t' || text[i] == '\r\n')
			return (i + 1);
	}
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

void main()
{
	std::fstream sourceFile("FMIKindle.dat", std::ios::in | std::ios::binary);

	if (!sourceFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
	}

	char command[MaxContentLength];
	Kindle fmiKindle;
	fmiKindle.load(sourceFile);
	sourceFile.close();
	std::cout << ">";
	std::cin.getline(command, MaxContentLength);
	while (true)
	{
		try
		{
			if (isPrefix(command, "exit") && fmiKindle.exit())
			{
				return;
			}
			
			char content1[MaxContentLength];
			char content2[MaxContentLength];
			size_t startIndex = getCommandLength(command);
			
			if (isPrefix(command, "login"))
			{
				std::cout << "Enter username: ";
				std::cin.getline(content1, MaxContentLength);
				
				std::cout << "Enter password: ";
				std::cin.getline(content2, MaxContentLength);
				
				fmiKindle.login(std::move(content1), std::move(content2));
				std::cout << "\tWelcome," << fmiKindle.getCurrentUserName() << std::endl;
			}
			else if (isPrefix(command, "signup"))
			{
				std::cout << "Enter username: ";
				std::cin.getline(content1, MaxContentLength);
			
				std::cout << "Enter password: ";
				std::cin.getline(content2, MaxContentLength);
				
				fmiKindle.signup(std::move(content1), std::move(content2));
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
			else if (isPrefix(command, "write"))
			{
				std::cout << "Enter title: ";
				std::cin.getline(content1, MaxContentLength);

				size_t pagesCount;
				std::cout << "Pages count: ";
				std::cin.getline(command, MaxContentLength);

				pagesCount = parseStringToInt(std::move(command));
				Book currentBook(content1, fmiKindle.getCurrentUserName());

				for (size_t i = 0; i < pagesCount; i++)
				{
					std::cout << "Page " << i + 1 << ": ";
					std::cin.getline(command, MaxContentLength);
					currentBook.addPage(std::move(command), i);
				}
				
				fmiKindle.addBook(currentBook);
			}
			else {
				getCommandData(startIndex, command, content1);
				
				if (isPrefix(command, "addPage"))
				{
					std::cout << "Enter page content: ";
					std::cin.getline(command, MaxContentLength);
					
					fmiKindle.addBookPage(std::move(content1), std::move(command));
				}
				else if (isPrefix(command, "removePage"))
				{
					fmiKindle.removeBookLastPage(std::move(content1));
				}
				else if (isPrefix(command, "rates"))
				{
					fmiKindle.printBookRating(std::move(content1));
				}
				else if (isPrefix(command, "comments"))
				{
					fmiKindle.printBookComments(std::move(content1));
				}
				else
				{
					getCommandData(startIndex, command, content1, content2);
					
					if (isPrefix(command, "read"))
					{
						int currentPageNumber;
						fmiKindle.readBook(content1);
					
						if (content2[0] == '\0')
						{
							currentPageNumber = 0;
						}
						else
						{
							currentPageNumber = (parseStringToInt(std::move(content2)) - 1);
						}

						char readCommand = '\0';
						while (true)
						{
							fmiKindle.printBookPage(content1, currentPageNumber);
							std::cin >> readCommand;
							
							if (readCommand == 'n')
							{
								currentPageNumber++;
							}
							else if (readCommand == 'p')
							{
								currentPageNumber--;
							}
							else if (readCommand == 'q')
							{
								break;
							}
							else
							{
								throw std::invalid_argument("Invalid command!");
							}
						}
					}
					else if (isPrefix(command, "rate"))
					{
						fmiKindle.rateBookByName(std::move(content1), parseStringToInt(std::move(content2)));
					}
					else if (isPrefix(command, "comment"))
					{
						size_t startIndex = 8;
						fmiKindle.addBookComment(std::move(content1), std::move(content2));
					}
					else if (isPrefix(command, "editPage"))
					{
						std::cin.getline(command, MaxContentLength);
						fmiKindle.editBookPage(std::move(content1), (parseStringToInt(std::move(content2)) - 1), command);
					}
					else if (isPrefix(command, "editRate"))
					{
						fmiKindle.editBookRating(std::move(content1), parseStringToInt(std::move(content2)));
					}
				}
			}
		}
		catch (std::invalid_argument& e)
		{
			std::cout << e.what() << std::endl;
		}

		std::cout << ">";
		std::cin.getline(command, MaxContentLength);
	}
}