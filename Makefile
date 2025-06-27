# Makefile para Simulação de Rede Complexa

# Configurações básicas
CC = gcc
CFLAGS = -Wall -Wextra -O3 -Iinclude
TARGET = bin/complex-network-sim
SOURCES = src/main.c src/network.c src/graphics.c
MKDIR = mkdir -p

# Detecção automática de SO
UNAME_S := $(shell uname -s)
OS_ARCH := $(shell uname -m)

# Configurações específicas por plataforma
ifeq ($(OS),Windows_NT)
    # Windows (MSYS2/MinGW)
    LIBS = -lfreeglut -lglu32 -lopengl32 -lm
    INSTALL_DEPS = pacman -S --noconfirm --needed mingw-w64-$(if $(findstring x86_64,$(OS_ARCH)),x86_64,i686)-toolchain mingw-w64-$(if $(findstring x86_64,$(OS_ARCH)),x86_64,i686)-freeglut
    TARGET = bin/complex-network-sim.exe
    RUN_CMD = .\\$(TARGET)
    MKDIR = mkdir
else ifeq ($(UNAME_S),Linux)
    # Linux
    LIBS = -lGL -lGLU -lglut -lm
    INSTALL_DEPS = sudo apt-get install -y freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev gcc
    RUN_CMD = ./$(TARGET)
else ifeq ($(UNAME_S),Darwin)
    # macOS
    LIBS = -framework OpenGL -framework GLUT -lm
    INSTALL_DEPS = if ! command -v brew &> /dev/null; then \
        /bin/bash -c "$$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"; \
    fi; \
    brew install freeglut
    RUN_CMD = ./$(TARGET)
else
    $(error "Sistema operacional não suportado: $(UNAME_S)")
endif

# Garante que os diretórios existem
$(shell $(MKDIR) bin)

all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo "Compilando para $(UNAME_S)..."
	@$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	@echo "Compilação concluída. "
	@echo "Execute com: make run"

install-deps:
	@echo "Instalando dependências para $(UNAME_S)..."
	@$(INSTALL_DEPS)
	@echo "Dependências instaladas com sucesso!"

run: $(TARGET)
	@echo "Iniciando simulação..."
	@$(RUN_CMD)

clean:
	@echo "Limpando arquivos gerados..."
	@rm -f $(TARGET)
	@rm -rf bin/* frr_save
	@echo "Limpeza concluída!"

clean-all: clean
	@rm -f *~

help:
	@echo "=== COMPLEX NETWORK SIMULATION ==="
	@echo "Comandos disponíveis:"
	@echo "  make           : Compila o programa"
	@echo "  make install-deps : Instala dependências do sistema"
	@echo "  make run       : Executa o programa"
	@echo "  make clean     : Remove arquivos gerados"
	@echo "  make clean-all : Remove todos os arquivos temporários"
	@echo "  make help      : Mostra esta ajuda"
	@echo ""
	@echo "Configurações detectadas:"
	@echo "  Sistema Operacional: $(UNAME_S)"
	@echo "  Arquitetura: $(OS_ARCH)"
	@echo "  Compilador: $(CC)"
	@echo "================================"

.PHONY: all install-deps run clean clean-all help
