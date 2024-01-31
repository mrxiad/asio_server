#ifndef USER_H
#define USER_H

#include <string>
using namespace std;


//对应数据库的表
class User{
    friend class UserModel;
private:
    int id;         //用户id,对应主键
    string name;
    string password;
    string state;    //online/offline
public: 
    User(int id=-1,string name="",string password="",string state="offline")
    {
        this->id=id;
        this->name=name;
        this->password=password;
        this->state=state;
    }

    void setId(int id)  {this->id=id;}
    void setName(string name) {this->name=name;}
    void setPassword(string password) {this->password=password;}
    void setState(string state) {this->state=state;}

    int getId() {return this->id;}
    string getName() {return this->name;}
    string getPassword() {return this->password;}
    string getState() {return this->state;}
};


#endif