#include "User.h"
#include "Collection.hpp"
#include "Book.h"
#include <cstring>
#pragma warning(disable:4996)

User::User() : User("", "") {}

User::User(const MyString& name, const MyString& password)
{
	//readBooks = Collection<Book>();
	//writtenBooks = Collection<Book>();
	setName(name);
	setPassword(password);
}

void User::saveToFile(std::fstream& file)
{
	size_t nameSize = name.getSize();
	file.write((const char*)&nameSize, sizeof(size_t));
	file.write((const char*)name.c_str(), name.getSize());

	size_t passwordSize = password.getSize();
	file.write((const char*)&passwordSize, sizeof(size_t));
	file.write((const char*)password.c_str(), password.getSize());

	file.write((const char*)&readBooks.count, sizeof(size_t));
	for (size_t i = 0; i < readBooks.count; i++)
	{
		readBooks.collection[i].saveToFile(file);
	}

	file.write((const char*)&writtenBooks.count, sizeof(size_t));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		writtenBooks.collection[i].saveToFile(file);
	}
}

void User::readFromFile(std::fstream& file)
{
	size_t size;
	file.read((char*)&size, sizeof(size_t));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	name = MyString(data);

	file.read((char*)&size, sizeof(size_t));
	delete[] data;
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	password = MyString(data);
	delete[] data;

	file.read((char*)&readBooks.count, sizeof(size_t));
	for (size_t i = 0; i < readBooks.count; i++)
	{
		readBooks.collection[i].readFromFile(file);
	}

	file.read((char*)&writtenBooks.count, sizeof(size_t));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		writtenBooks.collection[i].readFromFile(file);
	}
}

void User::setName(const MyString& name)
{
	if (name.getSize() > MaxNameLength)
	{
		return;
	}

	this->name = name;
}

void User::setPassword(const MyString& password)
{
	if (password.getSize() > MaxNameLength)
	{
		return;
	}

	this->password = password;
}

void User::readBook(const Book& book)
{
	readBooks.add(book);
}

void User::writeBook(const Book& book)
{
	writtenBooks.add(book);
}


void User::readBookPage(const Book& book, size_t pageCount) const
{
	book.printPageByIndex(pageCount);
}

void User::readBookComments(const Book& book) const
{
	book.printComments();
}

void User::writeBookComment(Book& book, const MyString comment)
{
	book.addComment(name, comment);
}

void User::editWrittenBook(Book& book)
{
	if (isUsersBook(book.getTitle()))
	{
		writeBook(book);
	}
}

void User::rateBook(Book& book, size_t rating)
{
	book.rate(name, rating);
}

void User::editBookRating(Book& book, size_t newRating)
{
	rateBook(book, newRating);
}

const MyString User::getName() const
{
	return name;
}

const MyString User::getPassword() const
{
	return password;
}

bool User::isUsersBook(const MyString& bookTitle) const
{
	for (size_t i = 0; i < writtenBooks.getCount(); i++)
	{
		if (writtenBooks.getElementByIndex(i).getTitle() == bookTitle)
		{
			return true;
		}
	}

	return false;
}

bool User::hasUserRead(const MyString& bookTitle) const
{
	for (size_t i = 0; i < readBooks.getCount(); i++)
	{
		if (readBooks.getElementByIndex(i).getTitle() == bookTitle)
		{
			return true;
		}
	}

	return false;
}
