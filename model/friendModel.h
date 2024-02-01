#ifndef FRIENDMODEL_H
#define FRIENDMODEL_H

//操作数据库的类


#include "./include/user.h"
#include <vector>
#include <iostream>
#include"../public/db/_mysql.h"
using namespace std;


class FriendModel
{
public:
    // 添加好友关系
    bool insert(int userid, int friendid){
        std::cout<<"添加好友关系"<<std::endl;
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败: " << conn.m_cda.message << std::endl;
            return false;
        }

        sqlstatement stmt(&conn);
        stmt.prepare("INSERT INTO Friend (userid, friendid) VALUES (:1, :2)");
        stmt.bindin(1, &userid);
        stmt.bindin(2, &friendid);
        if(stmt.execute() != 0){
            std::cout << "插入操作失败: " << stmt.m_cda.message << std::endl;
            return false;
        }
        conn.commit();
        return true;
    }

    // 返回用户好友列表
    vector<User> query(int userid){
        std::cout<<"查询id的好友列表"<<std::endl;
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败: " << conn.m_cda.message << std::endl;
            return vector<User>();// 返回一个空的User对象
        }

        sqlstatement stmt(&conn);
        stmt.prepare("\
        SELECT id, name, state,uuid FROM User WHERE id IN (SELECT friendid FROM Friend WHERE userid = :1)");
        stmt.bindin(1, &userid);
        if(stmt.execute() != 0){
            std::cout << "查询操作失败: " << stmt.m_cda.message << std::endl;
            return vector<User>();// 返回一个空的User对象
        }
        vector<User> vec;
        while(stmt.next() != 0){
            int id;
            char name[50], state[10],uuid[50];
            stmt.bindout(1, &id);
            stmt.bindout(2, name, 50);
            stmt.bindout(3, state, 10);
            stmt.bindout(4, uuid,50);
            vec.emplace_back(User{id, name, "", state,uuid});
        }
        return vec;
    }
};

#endif