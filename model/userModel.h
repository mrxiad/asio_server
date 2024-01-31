#ifndef USER_MODEL_H
#define USER_MODEL_H

#include "./include/user.h"
#include "_mysql.h"
#include <iostream>
//对接数据库的User表的操作接口方法
class UserModel {
public:
    // 增加用户
    bool insert(User &user) {
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败: " << conn.m_cda.message << std::endl;
            return false;
        }

        sqlstatement stmt(&conn);
        stmt.prepare("INSERT INTO User (id, name, password, state) VALUES (:1, :2, :3, :4)");

        int id = user.getId();
        char name[50], password[50], state[10];
        std::strcpy(name, user.getName().c_str());
        std::strcpy(password, user.getPassword().c_str());
        std::strcpy(state, user.getState().c_str());

        stmt.bindin(1, &id);
        stmt.bindin(2, name, 50);
        stmt.bindin(3, password, 50);
        stmt.bindin(4, state, 10);

        if (stmt.execute() != 0) {
            std::cout << "插入操作失败: " << stmt.m_cda.message << std::endl;
            return false;
        }

        conn.commit();
        return true;
    }

    // 根据用户号码查询用户信息
    User query(int id) {
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败" << std::endl;
            return User(); // 返回一个空的User对象
        }

        sqlstatement stmt(&conn);
        stmt.prepare("SELECT id, name, password, state FROM User WHERE id = :1");
        stmt.bindin(1, &id);

        int userId;
        char name[50], password[50], state[10];
        stmt.bindout(1, &userId);
        stmt.bindout(2, name, 50);
        stmt.bindout(3, password, 50);
        stmt.bindout(4, state, 10);

        User user;
        if (stmt.execute() == 0 && stmt.next() == 0) {
            user.setId(userId);
            user.setName(name);
            user.setPassword(password);
            user.setState(state);
        } else {
            std::cout << "查询操作失败或未找到用户" << std::endl;
        }

        return user;
    }

    // 更新用户的状态信息
    bool updateState(User user) {
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败" << std::endl;
            return false;
        }

        sqlstatement stmt(&conn);
        char state[10];
        std::strcpy(state, user.getState().c_str());
        int id = user.getId();

        stmt.prepare("UPDATE User SET state = :1 WHERE id = :2");
        stmt.bindin(1, state, 10);
        stmt.bindin(2, &id);

        if (stmt.execute() != 0) {
            std::cout << "更新操作失败" << std::endl;
            return false;
        }

        conn.commit();
        return true;
    }

    // 重置用户的状态信息
    void resetState() {
        connection conn;
        if (conn.connecttodb("localhost,root,123456,gin,3306", "utf8") != 0) {
            std::cout << "数据库连接失败" << std::endl;
            return;
        }

        sqlstatement stmt(&conn);
        stmt.prepare("UPDATE User SET state = 'offline' WHERE state = 'online'");

        if (stmt.execute() != 0) {
            std::cout << "重置状态操作失败" << std::endl;
            return;
        }

        conn.commit();
    }
};
#endif