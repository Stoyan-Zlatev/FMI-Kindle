#include "Comment.h"
#include "MyString.h"
#include <cstring>
#pragma warning(disable:4996)

Comment::Comment() : Comment("", "") {}

Comment::Comment(const MyString username, const MyString content)
{
	setUsername(username);
	setContent(content);
}

void Comment::setUsername(const MyString username)
{
	if (username.getSize() > MaxNameLength)
	{
		return;
	}

	this->username = username;
}

void Comment::setContent(const MyString content)
{
	if (content.getSize() > MaxContentLength)
	{
		return;
	}

	this->content = content;
}

void Comment::saveToFile(std::fstream& file)
{
	size_t size = username.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)username.c_str(), username.getSize());

	size = content.getSize();
	file.write((const char*)&size, sizeof(size));
	file.write((const char*)content.c_str(), content.getSize());
}

void Comment::readFromFile(std::fstream& file)
{
	size_t size;
	file.read((char*)&size, sizeof(size));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	username = MyString(data);

	file.read((char*)&size, sizeof(size));
	delete[] data;
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	content = MyString(data);

	delete[] data;
}

const MyString Comment::getUserName() const
{
	return username;
}

const MyString Comment::getContent() const
{
	return content;
}
