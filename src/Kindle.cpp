#include "Kindle.h"
#include "Utils.h"
#include <exception>
#include <iostream>

const MyString FILENAME = "FMIKindle.dat";

Kindle::Kindle()
{
	isUsed = false;
}

void Kindle::login(const MyString& username, const MyString& password)
{
	if (!containsUser(username, password))
	{
		throw std::invalid_argument("User with this username and password does not exist!");
	}

	size_t userIndex = getUserIndexByName(username);
	this->currentUser = users.collection[userIndex];
	isUsed = true;
}

void Kindle::signup(const MyString& username, const MyString& password)
{
	if (containsUser(username))
	{
		throw std::invalid_argument("User with this username already exists!");
	}

	User user(username, password);
	users.add(user);
	this->currentUser = user;
	isUsed = true;
}

void Kindle::logout()
{
	if (!isUsed)
	{
		throw std::invalid_argument("You have not logged in yet!");
	}

	size_t userIndex = getUserIndexByName(currentUser.getName());
	users.collection[userIndex] = currentUser;
	isUsed = false;
}

bool Kindle::exit()
{
	if (isUsed)
	{
		std::cout << "You have to logout before you exit!" << std::endl;
		return false;
	}

	std::ofstream newFile(FILENAME.c_str(), std::ios::binary);

	if (!newFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
		return false;
	}

	saveToFile(newFile);
	newFile.close();
	return true;
}

void Kindle::load(std::ifstream& sourceFile)
{
	sourceFile.read((char*)&booksToRead.count, sizeof(booksToRead.count));
	for (size_t i = 0; i < booksToRead.count; i++)
	{
		booksToRead.collection[i].readFromFile(sourceFile);
	}

	sourceFile.read((char*)&users.count, sizeof(users.count));
	for (size_t i = 0; i < users.count; i++)
	{
		users.collection[i].readFromFile(sourceFile);
	}
}

void Kindle::saveToFile(std::ofstream& file) const
{
	file.write((const char*)&booksToRead.count, sizeof(booksToRead.count));
	for (size_t i = 0; i < booksToRead.count; i++)
	{
		booksToRead.collection[i].saveToFile(file);
	}

	file.write((const char*)&users.count, sizeof(users.count));
	for (size_t i = 0; i < users.count; i++)
	{
		users.collection[i].saveToFile(file);
	}
}

void Kindle::view() const
{
	if (!isUsed)
	{
		throw std::invalid_argument("You do not have access!");
	}

	if (booksToRead.getCount() == 0)
	{
		throw std::invalid_argument("The book list is empty!");
	}
	else
	{
		for (size_t i = 0; i < booksToRead.getCount(); i++)
		{
			std::cout << i + 1 << ". " << booksToRead.collection[i].getTitle() <<
				" by " << booksToRead.collection[i].getAuthorName() << std::endl;
		}
	}
}

void Kindle::rateBookByName(const MyString& bookTitle, int rating)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	if (!isUsed)
	{
		throw std::invalid_argument("You do not have access!");
	}
	if (currentUser.isUsersBook(bookTitle))
	{
		throw std::invalid_argument("You can't rate your book!");
	}
	if (!currentUser.hasUserRead(bookTitle))
	{
		throw std::invalid_argument("You have to read the book first!");
	}

	booksToRead.collection[bookIndex].rate(currentUser.getName(), rating);
}

void Kindle::addBook(const Book& book)
{
	booksToRead.add(book);
	currentUser.writeBook(book.getTitle());
}

void Kindle::addBookComment(const MyString& bookTitle, const MyString& comment)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	if (!currentUser.hasUserRead(bookTitle) && !currentUser.isUsersBook(bookTitle))
	{
		throw std::invalid_argument("You can comment only books you have read or written!");
	}

	booksToRead.collection[bookIndex].addComment(currentUser.getName(), comment);
}

void Kindle::addBookPage(const MyString& bookTitle, const MyString& pageContent)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	if (!currentUser.isUsersBook(bookTitle))
	{
		throw std::invalid_argument("You can edit only your books!");
	}

	booksToRead.collection[bookIndex].addPage(pageContent);
}

void Kindle::editBookPage(const MyString& bookTitle, int page, const MyString& pageContent)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	if (!currentUser.isUsersBook(bookTitle))
	{
		throw std::invalid_argument("You can edit only your books!");
	}

	if (page >= booksToRead.collection[bookIndex].getPagesCount())
	{
		throw std::invalid_argument("You can't edit unexisting page!");
	}

	booksToRead.collection[bookIndex].editPage(pageContent, page);
}

void Kindle::editBookRating(const MyString& bookTitle, int rate)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	booksToRead.collection[bookIndex].editRate(currentUser.getName(), rate);
}

void Kindle::removeBookLastPage(const MyString& bookTitle)
{
	size_t bookIndex = getBookIndexByName(bookTitle);

	if (!currentUser.isUsersBook(bookTitle))
	{
		throw std::invalid_argument("You can edit only your books!");
	}

	booksToRead.collection[bookIndex].removeLastPage();
}

void Kindle::readBook(const MyString& bookTitle)
{
	size_t bookIndex = getBookIndexByName(bookTitle);
	currentUser.readBook(booksToRead.collection[bookIndex].getTitle());
}

void Kindle::printBookComments(const MyString& bookTitle) const
{
	size_t bookIndex = getBookIndexByName(bookTitle);
	booksToRead.collection[bookIndex].printComments();
}

void Kindle::printBookPage(const MyString& bookTitle, int pageNumber) const
{
	size_t bookIndex = getBookIndexByName(bookTitle);
	if (booksToRead.collection[bookIndex].getPagesCount() == 0)
	{
		throw std::invalid_argument("This book is empty!");
	}
	else if (pageNumber < 0 || pageNumber >= booksToRead.collection[bookIndex].getPagesCount())
	{
		throw std::invalid_argument("The page you try to access does not exist!");
	}

	booksToRead.collection[bookIndex].printPageByIndex(pageNumber);
}

void Kindle::printBookRating(const MyString& bookTitle) const
{
	size_t bookIndex = getBookIndexByName(bookTitle);
	booksToRead.collection[bookIndex].printRatings();
}

bool Kindle::containsUser(const MyString& username) const
{
	for (size_t i = 0; i < users.getCount(); i++)
	{
		if (username == users.collection[i].getName())
		{
			return true;
		}
	}

	return false;
}

bool Kindle::containsBook(const MyString& bookTitle) const
{
	for (size_t i = 0; i < booksToRead.count; i++)
	{
		if (bookTitle == booksToRead.collection[i].getTitle())
		{
			return true;
		}
	}

	return false;
}

bool Kindle::getIsUsed() const
{
	return isUsed;
}

void Kindle::run()
{
	std::ifstream sourceFile(FILENAME.c_str(), std::ios::binary);

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
				fmiKindle.logout();
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
					currentBook.addPage(std::move(command));
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

					if (command.getSize() == startIndex + field1.getSize())
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

bool Kindle::containsUser(const MyString& username, const MyString& password) const
{
	for (size_t i = 0; i < users.getCount(); i++)
	{
		if (username == users.collection[i].getName() && users.collection[i].isPasswordCorrect(password))
		{
			return true;
		}
	}

	return false;
}

const MyString Kindle::getCurrentUserName() const
{
	return currentUser.getName();
}

int Kindle::getUserIndexByName(const MyString& name) const
{
	for (size_t i = 0; i < users.count; i++)
	{
		if (name == users.collection[i].getName())
		{
			return i;
		}
	}

	throw std::invalid_argument("This user does not exist!");
}

int Kindle::getBookIndexByName(const MyString& name) const
{
	for (size_t i = 0; i < booksToRead.count; i++)
	{
		if (name == booksToRead.collection[i].getTitle())
		{
			return i;
		}
	}

	throw std::invalid_argument("This book does not exist!");
}