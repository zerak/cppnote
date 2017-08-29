#include <string>
#include <iostream>
#include <memory>
using namespace std;

//抽象类Tank
class Tank
{
public:
    virtual void shot()=0;
    virtual void run()=0;

public:
    virtual ~Tank()
    {
        cout<<"in the destructor of Tank"<<endl;
    }
};
//具体类 T50
class T50:public Tank
{
public:
    void shot()
    {
        cout<<"Tank T50 shot()"<<endl;
    }
    void run()
    {
        cout<<"Tank T50 run()"<<endl;
    }
public:
    virtual ~T50()
    {
        cout<<"In the destructor of T50"<<endl;
    }
};
//具体类T75
class T75:public Tank
{
public:
    void shot()
    {
        cout<<"Tank T75 shot()"<<endl;
    }
    void run()
    {
        cout<<"Tank T75 run()"<<endl;
    }
public:
    virtual ~T75()
    {
        cout<<"In the destructor of T75"<<endl;
    }
};
//具体类 T90
class T90:public Tank
{
public:
    void shot()
    {
        cout<<"Tank T90 shot()"<<endl;
    }
    void run()
    {
        cout<<"Tank T90 run()"<<endl;
    }
public:
    virtual ~T90()
    {
        cout<<"In the destructor of T90"<<endl;
    }
};

//抽象类，Decorator
class Decorator:public Tank
{
protected:
    auto_ptr<Tank> tank;
public:
    Decorator(auto_ptr<Tank> tank):tank(tank) {}  //具体的坦克的装饰类
    virtual ~Decorator()
    {
        cout<<"In the destructor of Decorator"<<endl;
    }
public:
    void shot()
    {
        tank->shot();
    }
    void run()
    {
        tank->run();
    }
};

class InfraredDecorator: public Decorator
{
private:
    string infrared;//这就是所谓的addAtrribute
public:
    InfraredDecorator(auto_ptr<Tank> tank):Decorator(tank) {}
    virtual ~InfraredDecorator()
    {
        cout<<"in the destructor of InfraredDecorator"<<endl;
    }
public:
    void set_Infrared(const string &infrared)
    {
        this->infrared=infrared;
    }
    string get_infrared() const
    {
        return infrared;
    }
    void run()
    {
        tank->run();
        set_Infrared("+Infrared");
        cout<<get_infrared()<<endl;
    }
    void shot()
    {
        tank->shot();
    }
};

class AmphibianDecorator:public Decorator
{
private:
    string amphibian;
public:
    AmphibianDecorator(auto_ptr<Tank> tank):Decorator(tank) {}
    ~AmphibianDecorator()
    {
        cout<<"in the destructor of AmphibianDecorator"<<endl;
    }
public:
    void set_amphibian(const string &hibian)
    {
        this->amphibian=amphibian;
    }
    string get_amphibian() const
    {
        return amphibian;
    }
public:
    void run()
    {
        tank->run();
        set_amphibian("+amphibian");
        cout<<get_amphibian()<<endl;
    }
    void shot()
    {
        tank->shot();
    }
};

class GPSDecorator:public Decorator
{
private:
    string gps;
public:
    GPSDecorator(auto_ptr<Tank> tank):Decorator(tank) {}
    ~GPSDecorator()
    {
        cout<<"in the destructor of GPSDecorator"<<endl;
    }
public:
    void set_gps(const string &gps)
    {
        this->gps=gps;
    }
    string get_gps() const
    {
        return gps;
    }
public:
    void run()
    {
        tank->run();
        set_gps("+gps");
        cout<<get_gps()<<endl;
    }
    void shot()
    {
        tank->shot();
    }
};

int main(int argc, char **argv)
{
    //给T50增加红外功能
    auto_ptr<Tank> tank1(new T50);
    auto_ptr<Tank> pid1(new InfraredDecorator(tank1));
    pid1->shot();
    cout<<endl;
    pid1->run();
    cout<<endl;
    cout<<endl<<"---------------"<<endl;
    //给t75增加红外、两栖功能
    auto_ptr<Tank> tank2(new T75);
    auto_ptr<Tank> pid2(new InfraredDecorator(tank2));
    auto_ptr<Tank> pad2(new AmphibianDecorator(pid2));
    pad2->shot();
    cout<<endl;
    pad2->run();
    cout<<endl;
    cout<<endl<<"--------------"<<endl;
    //给T75增加红外，两栖，定位功能
    auto_ptr<Tank> tank3(new T75);
    auto_ptr<Tank> pid3(new InfraredDecorator(tank3));
    auto_ptr<Tank> pad3(new AmphibianDecorator(pid3));
    auto_ptr<Tank> pgd3(new GPSDecorator(pad3));
    pgd3->shot();
    cout<<endl;
    pgd3->run();
    cout<<endl;



    return 0;
}