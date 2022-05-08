#pragma once
#include "Book.h"
#include "Collection.hpp"
#include "GlobalConstants.h"
#include "MyString.h"
#include <fstream>

class User
{
	MyString name;
	MyString password;
	Collection<MyString> readBooks;
	Collection<MyString> writtenBooks;
	
	void setName(const MyString& name);
	void setPassword(const MyString& password);
public:
	User();
	User(const MyString& name, const MyString& password);

	void saveToFile(std::fstream& file);
	void readFromFile(std::fstream& file);

	void readBook(const MyString& title);
	void writeBook(const MyString& title);
	
	bool isUsersBook(const MyString& title) const;
	bool hasUserRead(const MyString& title) const;
	
	
	int getWrittenBookIndex(const MyString& title) const;
	const MyString getName() const;
	const MyString getPassword() const;
};