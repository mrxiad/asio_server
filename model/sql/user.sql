CREATE TABLE User (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    password VARCHAR(50) NOT NULL,
    state VARCHAR(10) NOT NULL DEFAULT 'offline',
    uuid VARCHAR(50) DEFAULT NULL,
    UNIQUE KEY unique_name (name)
);
