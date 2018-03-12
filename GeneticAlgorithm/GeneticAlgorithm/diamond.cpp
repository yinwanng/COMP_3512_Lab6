#include <iostream>
class A
{
	public:
	A() { std::cout << "A allocated" << std::endl; }
	~A() { std::cout << "A deallocated" << std::endl; }
};

class B : public A
{
	public:
	B() { std::cout << "B allocated" << std::endl; }
	~B() { std::cout << "B deallocated" << std::endl; }
};

class C : public A
{
	public:
	C() { std::cout << "C allocated" << std::endl; }
	~C() { std::cout << "C deallocated" << std::endl; }
};

class D : public B, public C
{
	public:
	D() { std::cout << "D allocated" << std::endl; }
	~D() { std::cout << "D deallocated" << std::endl; }
};


int main()
{
	D d;
	system("pause");
}