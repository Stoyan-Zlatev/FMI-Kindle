#include "User.h"
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

void User::saveToFile(std::ofstream& file) const
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
		size = readBooks.collection[i].getSize();
		file.write((const char*)&size, sizeof(size));
		file.write((const char*)readBooks.collection[i].c_str(), readBooks.collection[i].getSize());
	}

	file.write((const char*)&writtenBooks.count, sizeof(writtenBooks.count));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		size = writtenBooks.collection[i].getSize();
		file.write((const char*)&size, sizeof(size));
		file.write((const char*)writtenBooks.collection[i].c_str(), writtenBooks.collection[i].getSize());
	}
}

void User::readFromFile(std::ifstream& file)
{
	size_t size;
	file.read((char*)&size, sizeof(size));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	name = MyString(data);
	delete[] data;

	file.read((char*)&size, sizeof(size));
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	password = MyString(data);
	delete[] data;

	file.read((char*)&readBooks.count, sizeof(readBooks.count));
	for (size_t i = 0; i < readBooks.count; i++)
	{
		file.read((char*)&size, sizeof(size));
		data = new char[size + 1];
		file.read((char*)data, size);
		data[size] = '\0';
		readBooks.collection[i] = MyString(data);
		delete[] data;
	}

	file.read((char*)&writtenBooks.count, sizeof(writtenBooks.count));
	for (size_t i = 0; i < writtenBooks.count; i++)
	{
		file.read((char*)&size, sizeof(size));
		data = new char[size + 1];
		file.read((char*)data, size);
		data[size] = '\0';
		writtenBooks.collection[i] = MyString(data);
		delete[] data;
	}
}

void User::readBook(const MyString& title)
{
	readBooks.add(title);
}

void User::writeBook(const MyString& title)
{
	writtenBooks.add(title);
}

const MyString User::getName() const
{
	return name;
}

bool User::isPasswordCorrect(const MyString& customerPassword) const
{
	return password == customerPassword;
}

int User::getWrittenBookIndex(const MyString& title) const
{
	for (size_t i = 0; i < writtenBooks.getCount(); i++)
	{
		if (writtenBooks.collection[i] == title)
		{
			return i;
		}
	}

	return -1;
}

bool User::hasUserRead(const MyString& title) const
{
	for (size_t i = 0; i < readBooks.getCount(); i++)
	{
		if (readBooks.collection[i] == title)
		{
			return true;
		}
	}

	return false;
}

bool User::isUsersBook(const MyString& title) const
{
	return getWrittenBookIndex(title) >= 0;
}