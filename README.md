Complex Network Dynamics Simulator

https://screenshot.png
Visualização 3D interativa de dinâmica em rede complexa
🌐 Visão Geral

O Complex Network Dynamics Simulator é uma ferramenta de visualização interativa que permite explorar dinâmicas em redes complexas usando modelos de Ising. Com uma interface 3D intuitiva, você pode observar como diferentes tipos de redes e modelos de atualização influenciam o comportamento do sistema.
✨ Recursos Principais

    🕸️ Dois tipos de rede:

        Redes aleatórias (Erdős-Rényi)

        Redes contínuas (espaço métrico com condições periódicas)

    🔄 Dois modelos de atualização:

        Modelo Majority Vote

        Modelo Vote Model

    🎨 Visualização 3D interativa:

        Rotação, zoom e panorâmica

        Código de cores para estados dos nós (+1 vermelho, -1 azul)

        Nós selecionados destacados em verde

    ⚙️ Personalização completa:

        Número de nós (10-1000)

        Magnetização inicial

        Grau médio

        Nível de ruído

        Nós selecionados por passo

    💾 Recursos avançados:

        Salvar configuração atual

        Reinicialização com último estado

        Captura de telas em sequência (formato PPM)

📋 Pré-requisitos

    Compilador C (GCC, Clang ou MinGW)

    Bibliotecas gráficas:

        OpenGL

        GLUT (FreeGLUT)

    Ferramenta make (recomendado)

🛠️ Instalação
Windows (MSYS2)
bash

# Instale o MSYS2: https://www.msys2.org/
# Abra o terminal MSYS2 MinGW 64-bit
pacman -Syu
pacman -S --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-freeglut
git clone https://github.com/seu-usuario/project-network.git
cd project-network
make
make run

Linux (Ubuntu/Debian)
bash

sudo apt update
sudo apt install build-essential freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev git
git clone https://github.com/seu-usuario/project-network.git
cd project-network
make
./bin/complex-network-sim

Linux (Fedora)
bash

sudo dnf groupinstall "Development Tools"
sudo dnf install freeglut-devel mesa-libGLU-devel git
git clone https://github.com/seu-usuario/project-network.git
cd project-network
make
./bin/complex-network-sim

macOS
bash

# Instale Homebrew (caso não tenha)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Instale dependências
brew install freeglut git

git clone https://github.com/seu-usuario/project-network.git
cd project-network
make
./bin/complex-network-sim

🧪 Compilação Manual (sem Makefile)
bash

gcc -Wall -Wextra -O3 -Iinclude src/main.c src/network.c src/graphics.c \
    -o bin/complex-network-sim \
    -lglut -lGLU -lGL -lm

🕹️ Como Usar
⚙️ Configuração Inicial

Ao iniciar o programa, configure os parâmetros:

    Número de nós (2-1000)

    Magnetização inicial (0.0-1.0)

    Grau médio (1 a N-1)

    Ruído (0.0-1.0)

    Nós selecionados por passo (1 a N)

    Tipo de rede (Aleatória ou Contínua)

    Modelo de atualização (Majority Vote ou Vote Model)

🎮 Controles durante a simulação
Tecla	Ação
Espaço	Reiniciar simulação
ESC	Sair do programa
+	Aumentar velocidade de simulação
-	Diminuir velocidade de simulação
W	Zoom in
S	Zoom out
P	Pausar/retomar simulação
R	Resetar câmera
I	Mostrar informações do sistema
Mouse	Arrastar para rotacionar
Scroll	Zoom
🔧 Personalização Avançada

Modifique valores padrão em src/network.c:
c

// Valores iniciais padrão
int NUM_NODES = 100;
float MAG_INIT = 0.55;
int K_AVG = 10;
float NOISE = 0.0;
int SELECT_NODES = 1;

🗂️ Estrutura do Projeto
text

project-network/
├── bin/               # Executável compilado
├── include/           # Cabeçalhos
│   ├── graphics.h
│   └── network.h
├── src/               # Código fonte
│   ├── graphics.c
│   ├── main.c
│   └── network.c
├── Makefile           # Script de compilação
└── README.md          # Documentação

🧩 Adicionando Novas Funcionalidades
Para novo tipo de rede:

    Implemente a função em network.c

    Adicione ao registro:

c

NetworkType networkTypes[] = {
    {"Aleatoria", RandomNetwork},
    {"Continua", continuosNetwork},
    {"Nova Rede", NovaRede}  // <= Adicione aqui
};

Para novo modelo de atualização:

    Implemente a função de atualização

    Adicione ao registro:

c

UpdateModelType updateModels[] = {
    {"Majority Vote", updateNetwork_MajorityVote},
    {"Vote Model", updateNetwork_VoteModel},
    {"Novo Modelo", NovoModelo}  // <= Adicione aqui
};

📸 Capturas de Tela

Para habilitar captura automática de frames:

    Edite src/graphics.c

    Descomente savePPM(frame); na função idle()

    Frames serão salvos em frr_save/ no formato PPM

📜 Licença

Este projeto está licenciado sob a Licença MIT.
🤝 Contribuição

Contribuições são bem-vindas! Siga estes passos:

    Faça um fork do projeto

    Crie sua branch (git checkout -b feature/nova-feature)

    Commit suas mudanças (git commit -m 'Adiciona nova feature')

    Push para a branch (git push origin feature/nova-feature)

    Abra um Pull Request

📧 Contato

Para dúvidas ou sugestões:

    Seu Nome

    Repositório GitHub

Explore a beleza das redes complexas! 🌐✨
New chat
AI-generated, for reference only