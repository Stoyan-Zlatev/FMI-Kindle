#pragma once
#include "Page.h"
#include "Comment.h"
#include "GlobalConstants.h"
#include "MyString.h"
#include "Collection.hpp"
#include "UserRating.h"
#include <fstream>

class Book
{
	MyString title;
	MyString authorName;
	Collection<UserRating> ratings;
	Collection<Page> pages;
	Collection<Comment> comments;
	
	int hasRated(const MyString& username);
	double sumRating() const;
	void setTitle(const MyString& title);
	void setAuthorName(const MyString& authorName);
public:
	Book();
	Book(const MyString& title, const MyString& authorName);

	void readFromFile(std::fstream& file);
	void saveToFile(std::fstream& file);

	void addComment(const MyString& username, const MyString& comment);
	void addPage(const MyString& content, int pageNumber);

	void editPage(const MyString& content, int pageNumber);
	void editRate(const MyString& username, int rating); 
	
	void rate(const MyString& username, int rating);
	
	void removeLastPage();

	void printPageByIndex(int index) const;
	void printComments() const;
	void printRatings() const;
	
	size_t getPagesCount() const;
	double getRating() const;
	const MyString getTitle() const;
	const MyString getAuthorName() const;
};