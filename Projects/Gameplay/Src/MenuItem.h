#pragma once
#include <Component.h>
#include <string>
#include <vector>

class MenuItem : public Component
{
public:
	MenuItem();
	~MenuItem();

	std::string			documentName;
	bool				modal;
	std::vector<char*>	handlerNames;
	bool				initiallyVisible;
};

