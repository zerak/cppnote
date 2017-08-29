#include <iostream>
#include <string>
using namespace std;
class Person
{
private:
    string name;
public:
    Person()
    {
    }
    Person(string name)
    {
        this->name=name;
    }
    virtual void show()
    {
        cout<<" Decorator name "<<name<<endl;
    }
};

class Decorator:public Person
{
protected:
    Person* component;
public:
    void Deco(Person *component)
    {
        this->component=component;
    }
    void show()
    {
        component->show();
    }
};

class toufa:public Decorator
{
public:
    void show()
    {
        cout<<"头发"<<endl;
        component->show();
    }
};

class shoot:public Decorator
{
public:
    void show()
    {
        cout<<"鞋子"<<endl;
        component->show();
    }
};

int main()
{
    Person fuli("fuli");

    shoot xie;
    toufa tou;
    xie.Deco(&fuli);
    tou.Deco(&xie);
    tou.show();
    return 0;
}  