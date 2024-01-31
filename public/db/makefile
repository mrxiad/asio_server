# Makefile to compile _mysql.cpp into a dynamic library

# Compiler settings
CXX = g++
CXXFLAGS = -fPIC -Wall -Wextra -O2
MYSQLINCL = -I/usr/include/mysql

# mysql库文件存放的目录。
MYSQLLIB = -L/usr/lib/x86_64-linux-gnu

# 需要链接的mysql库。
MYSQLLIBS = -lmysqlclient



# Output library name
LIB_NAME = lib_mysql.so

# Source and object files
SRC = _mysql.cpp
OBJ = $(SRC:.cpp=.o)

# Rule to make the object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(MYSQLINCL) -c $< -o $@

# Rule to make the dynamic library
$(LIB_NAME): $(OBJ)
	$(CXX) -shared $(OBJ) -o $(LIB_NAME) $(MYSQLLIB) $(MYSQLLIBS)

# Phony targets
.PHONY: all clean

all: $(LIB_NAME)

clean:
	rm -f $(OBJ) $(LIB_NAME)

