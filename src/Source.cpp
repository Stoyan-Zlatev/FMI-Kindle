#include <iostream>
#include <fstream>
#include "Utils.h"
#include "Kindle.h"


void main()
{
	std::fstream sourceFile("FMIKindle.dat", std::ios::in | std::ios::binary);

	if (!sourceFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
	}

	MyString command;
	MyString field1;
	MyString field2;

	Kindle fmiKindle;
	fmiKindle.load(sourceFile);

	sourceFile.close();

	std::cout << ">";
	command.getline(std::cin);

	while (true)
	{
		try
		{
			if (isPrefix(command, "exit"))
			{
				if (fmiKindle.exit())
				{
					return;
				}
				else
				{
					std::cout << ">";
					command.getline(std::cin);
					continue;
				}
			}
			size_t startIndex = getCommandLength(command);

			if (isPrefix(command, "login"))
			{
				if (fmiKindle.getIsUsed())
				{
					throw std::invalid_argument("There is already logged user!");
				}

				std::cout << "Enter username: ";
				field1.getline(std::cin);

				std::cout << "Enter password: ";
				field2.getline(std::cin);

				fmiKindle.login(std::move(field1), std::move(field2));
				std::cout << "\tWelcome," << fmiKindle.getCurrentUserName() << std::endl;
			}
			else if (isPrefix(command, "signup"))
			{
				if (fmiKindle.getIsUsed())
				{
					throw std::invalid_argument("There is currently logged user!");
				}

				std::cout << "Enter username: ";
				field1.getline(std::cin);

				std::cout << "Enter password: ";
				field2.getline(std::cin);

				fmiKindle.signup(std::move(field1), std::move(field2));
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
				field1.getline(std::cin);

				if (fmiKindle.containsBook(field1))
				{
					throw std::invalid_argument("Book with this name already exists!");
				}

				size_t pagesCount;
				std::cout << "Pages count: ";
				command.getline(std::cin);

				pagesCount = parseStringToInt(std::move(command));
				Book currentBook(field1, fmiKindle.getCurrentUserName());

				for (size_t i = 0; i < pagesCount; i++)
				{
					std::cout << "Page " << i + 1 << ": ";
					command.getline(std::cin);
					currentBook.addPage(std::move(command), i);
				}

				fmiKindle.addBook(currentBook);
			}
			else {
				getField1Data(startIndex, command, field1);

				if (isPrefix(command, "addPage"))
				{
					std::cout << "Enter page content: ";
					command.getline(std::cin);
					fmiKindle.addBookPage(std::move(field1), std::move(command));
				}
				else if (isPrefix(command, "removePage"))
				{
					fmiKindle.removeBookLastPage(std::move(field1));
				}
				else if (isPrefix(command, "rates"))
				{
					fmiKindle.printBookRating(std::move(field1));
				}
				else if (isPrefix(command, "comments"))
				{
					fmiKindle.printBookComments(std::move(field1));
				}
				else if (isPrefix(command, "read"))
				{
					int currentPageNumber;
					fmiKindle.readBook(field1);

					if (command.getSize()==startIndex+field1.getSize())
					{
						currentPageNumber = 0;
					}
					else
					{
						getField2Data(startIndex, command, field1, field2);
						currentPageNumber = (parseStringToInt(std::move(field2)) - 1);
					}

					while (true)
					{
						fmiKindle.printBookPage(field1, currentPageNumber);
						command.getline(std::cin);

						if (command[0] == 'n')
						{
							currentPageNumber++;
						}
						else if (command[0] == 'p')
						{
							currentPageNumber--;
						}
						else if (command[0] == 'q')
						{
							break;
						}
						else
						{
							throw std::invalid_argument("Invalid command!");
						}
					}
				}
				else
				{
					getField2Data(startIndex, command, field1, field2);

					if (isPrefix(command, "rate"))
					{
						fmiKindle.rateBookByName(std::move(field1), parseStringToInt(std::move(field2)));
					}
					else if (isPrefix(command, "comment"))
					{
						size_t startIndex = 8;
						fmiKindle.addBookComment(std::move(field1), std::move(field2));
					}
					else if (isPrefix(command, "editPage"))
					{
						std::cout << "Enter the new page content:";
						command.getline(std::cin);
						fmiKindle.editBookPage(std::move(field1), (parseStringToInt(std::move(field2)) - 1), command);
					}
					else if (isPrefix(command, "editRate"))
					{
						fmiKindle.editBookRating(std::move(field1), parseStringToInt(std::move(field2)));
					}
					else
					{
						throw std::invalid_argument("Invalid command!");
					}
				}
			}
		}
		catch (std::invalid_argument& e)
		{
			std::cout << e.what() << std::endl;
		}

		std::cout << ">";
		command.getline(std::cin);
	}
}