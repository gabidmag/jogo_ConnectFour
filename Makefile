# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = connect4

# Arquivos fonte (adicione só os que fazem parte do jogo)
SRC = Main.c Game.c utils.c AI.c
OBJ = $(SRC:.c=.o)

# Regra padrão (gera o executável)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Regras para gerar .o (compilação individual)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
