# Makefile para o Projeto de Árvores

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -g

# Diretórios
VP_DIR = arv_vermelha-preta
A23_DIR = arv2-3
A45_DIR = 4-5

# Alvos principais
all: programa_vp test_vp programa_23 test_23 test_45

programa_vp:
	$(CC) $(CFLAGS) $(VP_DIR)/main.c $(VP_DIR)/estruturas_vp.c -o $(VP_DIR)/programa.exe

test_vp:
	$(CC) $(CFLAGS) $(VP_DIR)/test_rb.c $(VP_DIR)/estruturas_vp.c -o $(VP_DIR)/test_rb.exe
	@echo "Executando Testes Automatizados Rubro-Negra..."
	@$(VP_DIR)\test_rb.exe

programa_23:
	$(CC) $(CFLAGS) $(A23_DIR)/main.c $(A23_DIR)/estruturas2-3.c -o $(A23_DIR)/programa.exe

test_23:
	$(CC) $(CFLAGS) $(A23_DIR)/test_23.c $(A23_DIR)/estruturas2-3.c -o $(A23_DIR)/test_23.exe
	@echo "Executando Testes Automatizados 2-3..."
	@$(A23_DIR)\test_23.exe

test_45:
	$(CC) $(CFLAGS) $(A45_DIR)/test_45.c $(A45_DIR)/estruturas4-5.c -o $(A45_DIR)/test_45.exe
	@echo "Executando Testes Automatizados 4-5..."
	@$(A45_DIR)\test_45.exe

clean:
	del /Q $(VP_DIR)\*.exe
	del /Q $(A23_DIR)\*.exe
	del /Q $(A45_DIR)\*.exe
