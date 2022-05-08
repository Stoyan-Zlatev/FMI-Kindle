#include "Kindle.h"
#include "User.h"
#include "Book.h"
#include <exception>
#include <iostream>

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

void Kindle::logout(std::fstream& file)
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

	std::fstream newFile("FMIKindle.dat", std::ios::trunc | std::ios::binary | std::ios::out);

	if (!newFile.is_open())
	{
		std::cout << "Error while opening the file!" << std::endl;
		return false;
	}

	saveToFile(newFile);
	newFile.close();
	return true;
}

void Kindle::load(std::fstream& sourceFile)
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

void Kindle::saveToFile(std::fstream& file)
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
			Book currentBook = booksToRead.collection[i];
			std::cout << i + 1 << ". " << currentBook.getTitle() << " by " << currentBook.getAuthorName() << std::endl;
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
	currentUser.writeBook(book);
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

	booksToRead.collection[bookIndex].addPage(pageContent, booksToRead.collection[bookIndex].getPagesCount());
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
	currentUser.readBook(booksToRead.collection[bookIndex]);
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

bool Kindle::containsUser(const MyString& username, const MyString& password) const
{
	for (size_t i = 0; i < users.getCount(); i++)
	{
		if (username == users.collection[i].getName() && password == users.collection[i].getPassword())
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