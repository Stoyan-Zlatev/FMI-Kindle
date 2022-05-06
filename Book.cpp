#include "Book.h"
#include "Collection.hpp"
#include "Page.h"
#include "UserRating.h"
#include <iostream>
#pragma warning(disable:4996)

Book::Book() : Book("", "") {}

Book::Book(const MyString& title, const MyString& authorName)
{
	setTitle(title);
	setAuthorName(authorName);
	//ratings = Collection<UserRating>();
	//pages = Collection<Page>();
	//comments = Collection<Comment>();
}

void Book::setTitle(const MyString& title)
{
	//if (title == "")
	//{
	//	throw std::invalid_argument("Title should not be empty!");
	//}

	this->title = title;
}
void Book::setAuthorName(const MyString& authorName)
{
	/*if (authorName == "")
	{
		throw std::invalid_argument("Author name should not be empty!");
	}*/

	this->authorName = authorName;
}

Book& Book::operator=(const Book& book)
{
	pages = book.pages;
	comments = book.comments;
	ratings = book.ratings;
	title = book.title;
	authorName = book.authorName;
	return *this;
}


void Book::rate(const MyString& username, int rating)
{
	ratings.add(UserRating(username, rating));
}

const MyString Book::getTitle() const
{
	return title;
}
const MyString Book::getAuthorName() const
{
	return authorName;
}

double Book::sumRating() const
{
	double sum = 0;
	for (size_t i = 0; i < ratings.getCount(); i++)
	{
		sum += ratings.getElementByIndex(i).getRating();
	}

	return sum;
}


double Book::getRating() const
{
	double ratingsSum = sumRating();
	return ratingsSum / (double)ratings.getCount();
}

void Book::printComments() const
{
	for (size_t i = 0; i < comments.getCount(); i++)
	{
		std::cout << comments.getElementByIndex(i).getContent() << std::endl;
	}
}

void Book::printRatings() const
{
	for (size_t i = 0; i < ratings.getCount(); i++)
	{
		std::cout << ratings.collection[i].getUserName() << " rated " << ratings.collection[i].getRating() << " start!" << std::endl;
	}
}

size_t Book::getPagesCount() const
{
	return pages.count;
}

void Book::addComment(const MyString& username, const MyString& comment)
{
	comments.add(Comment(username, comment));
}

void Book::addPage(const MyString& content, size_t pageNumber)
{
	pages.add(Page(content, pageNumber));
}

void Book::removeLastPage()
{
	pages.remove();
}

void Book::readFromFile(std::fstream& file)
{
	//Title 
	size_t size;
	file.read((char*)&size, sizeof(size_t));
	char* data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	title = MyString(data);

	//Author 
	file.read((char*)&size, sizeof(size_t));
	delete[] data;
	data = new char[size + 1];
	file.read((char*)data, size);
	data[size] = '\0';
	authorName = MyString(data);

	delete[] data;

	//Pages 
	file.read((char*)&pages.count, sizeof(size_t));
	for (size_t i = 0; i < pages.count; i++)
	{
		pages.collection[i].readFromFile(file);
	}

	//Comments 
	file.read((char*)&comments.count, sizeof(size_t));
	for (size_t i = 0; i < comments.count; i++)
	{
		comments.collection[i].readFromFile(file);
	}

	//Ratings saved
	file.read((char*)&ratings.count, sizeof(size_t));
	for (size_t i = 0; i < ratings.count; i++)
	{
		ratings.collection[i].readFromFile(file);
	}
}

void Book::saveToFile(std::fstream& file)
{
	//Title saved
	size_t titleSize = title.getSize();
	file.write((const char*)&titleSize, sizeof(size_t));
	file.write((const char*)title.c_str(), title.getSize());
	
	//Author saved
	size_t authorSize = authorName.getSize();
	file.write((const char*)&authorSize, sizeof(size_t));
	file.write((const char*)authorName.c_str(), authorName.getSize());

	//Pages saved
	file.write((const char*)&pages.count, sizeof(size_t));
	for (size_t i = 0; i < pages.count; i++)
	{
		pages.collection[i].saveToFile(file);
	}

	//Comments saved
	file.write((const char*)&comments.count, sizeof(size_t));
	for (size_t i = 0; i < comments.count; i++)
	{
		comments.collection[i].saveToFile(file);
	}

	//Ratings saved
	file.write((const char*)&ratings.count, sizeof(size_t));
	for (size_t i = 0; i < ratings.count; i++)
	{
		ratings.collection[i].saveToFile(file);
	}
}

void Book::printPageByIndex(int index) const
{
	if (index < 0)
	{
		throw std::invalid_argument("This is the first page of the book!");

	}
	else if (index > (pages.getCount() - 1))
	{
		throw std::invalid_argument("This is the last page of the book!");
	}
	std::cout << pages.getElementByIndex(index).getPageContent()<<std::endl;
}

