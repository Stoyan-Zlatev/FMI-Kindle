#include "User.h"
#include "Collection.hpp"
#include "Book.h"
#include <cstring>
#pragma warning(disable:4996)

User::User() : User("", "") {}

User::User(const MyString& name, const MyString& password)
{
	setName(name);
	setPassword(password);
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

void User::saveToFile(std::fstream& file)
{
	size_t size = name.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)name.c_str(), name.getSize());

	size = password.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)password.c_str(), password.getSize());

	file.write((const char*)&readBooks.count, sizeof(readBooks.count));
	for (size_t i = 0; i < readBooks.count; i++)
	{
		readBooks.collection[i].saveToFile(file);
	}

	file.write((const char*)&writtenBooks.count, sizeof(writtenBooks.count));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		writtenBooks.collection[i].saveToFile(file);
	}
}

void User::readFromFile(std::fstream& file)
{
	size_t size;
	file.read((char*)&size, sizeof(size));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	name = MyString(data);

	file.read((char*)&size, sizeof(size));
	delete[] data;
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	password = MyString(data);
	delete[] data;

	file.read((char*)&readBooks.count, sizeof(readBooks.count));
	for (size_t i = 0; i < readBooks.count; i++)
	{
		readBooks.collection[i].readFromFile(file);
	}

	file.read((char*)&writtenBooks.count, sizeof(writtenBooks.count));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		writtenBooks.collection[i].readFromFile(file);
	}
}

void User::readBook(const Book& book)
{
	readBooks.add(book);
}

void User::readBookPage(const Book& book, size_t pageCount) const
{
	book.printPageByIndex(pageCount);
}

void User::readBookComments(const Book& book) const
{
	book.printComments();
}

void User::writeBook(const Book& book)
{
	writtenBooks.add(book);
}

void User::writeBookComment(Book& book, const MyString comment)
{
	book.addComment(name, comment);
}

void User::addRating(const MyString& bookTitle, int rating)
{
	size_t bookIndex = getWrittenBookIndex(bookTitle);
	rateBook(writtenBooks.collection[bookIndex], rating);
}

void User::addComment(const MyString& bookTitle, const MyString& comment)
{
	size_t bookIndex = getWrittenBookIndex(bookTitle);
	writeBookComment(writtenBooks.collection[bookIndex], comment);
}

void User::addPage(const MyString& bookTitle, const MyString& content, int page)
{
	size_t bookIndex = getWrittenBookIndex(bookTitle);
	writtenBooks.collection[bookIndex].addPage(content, page);
}

void User::editWrittenBook(Book& book)
{
	if (isUsersBook(book.getTitle()))
	{
		writeBook(book);
	}
}

void User::editBookRating(Book& book, size_t newRating)
{
	rateBook(book, newRating);
}

void User::rateBook(Book& book, int rating)
{
	book.rate(name, rating);
}

const MyString User::getName() const
{
	return name;
}

const MyString User::getPassword() const
{
	return password;
}

int User::getWrittenBookIndex(const MyString& bookTitle) const
{
	for (size_t i = 0; i < writtenBooks.getCount(); i++)
	{
		if (writtenBooks.collection[i].getTitle() == bookTitle)
		{
			return i;
		}
	}

	return -1;
}

bool User::hasUserRead(const MyString& bookTitle) const
{
	for (size_t i = 0; i < readBooks.getCount(); i++)
	{
		if (readBooks.collection[i].getTitle() == bookTitle)
		{
			return true;
		}
	}

	return false;
}

bool User::isUsersBook(const MyString& bookTitle) const
{
	return getWrittenBookIndex(bookTitle) >= 0;
}