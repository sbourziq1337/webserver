SRC = server.cpp Request.cpp
cpp= c++

CFLAGS = -std=c++98

RM = rm -rf

OBJ = $(SRC:.cpp=.o)

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(cpp) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(cpp) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
fclean: clean
	$(RM) $(TARGET)
re: clean all

.PHONY: all re clean fclean 