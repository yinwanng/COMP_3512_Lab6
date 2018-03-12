#pragma once
class City
{
public:
	City(char name);
	~City();
	void setName(char name);
	char getName() const;
	int get_X() const;
	int get_Y() const;

private:
	char name;
	int x;
	int y;
};
