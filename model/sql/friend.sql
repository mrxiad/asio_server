CREATE TABLE Friend (
    userid INT,
    friendid INT,
    PRIMARY KEY(userid, friendid),
    FOREIGN KEY (userid) REFERENCES User(id),
    FOREIGN KEY (friendid) REFERENCES User(id)
);