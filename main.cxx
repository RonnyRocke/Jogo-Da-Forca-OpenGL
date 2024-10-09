#include <iostream>
#include <string>
#include <set>
#include <GL/glut.h>

using namespace std;

enum GameState { MENU, GAME, GAME_OVER };
GameState estadoAtual = MENU;

string palavraSecreta;
string palavraOculta;
set<char> letrasDigitadas;
int erros = 0;
const int maxErros = 6;
bool hoverIniciar = false;
bool hoverSair = false;

void inicializa();
void desenhaMenu();
void desenhaJogo();
void desenhaBoneco();
void desenhaTexto(void* font, const char* text, float x, float y);
void mouseMenu(int button, int state, int x, int y);
void movimentoMouse(int x, int y);
void teclado(unsigned char key, int x, int y);
void reiniciarJogo();

// Função para desenhar o bonequinho
void desenhaBoneco() {
    // Desenha a forca
    glBegin(GL_LINES);
    glVertex2f(-0.5f, 0.8f);
    glVertex2f(-0.5f, -0.8f);
    glVertex2f(-0.5f, 0.8f);
    glVertex2f(0.1f, 0.8f);
    glVertex2f(0.1f, 0.8f);
    glVertex2f(0.1f, 0.6f);
    glEnd();

    // Desenho do boneco baseado no número de erros
    if (erros >= 1) { // Cabeça
        glBegin(GL_LINE_LOOP);
        glVertex2f(0.1f, 0.55f);
        glVertex2f(0.1f, 0.45f);
        glVertex2f(0.15f, 0.45f);
        glVertex2f(0.15f, 0.55f);
        glEnd();
    }
    if (erros >= 2) { // Corpo
        glBegin(GL_LINES);
        glVertex2f(0.125f, 0.45f);
        glVertex2f(0.125f, 0.25f);
        glEnd();
    }
    if (erros >= 3) { // Braço esquerdo
        glBegin(GL_LINES);
        glVertex2f(0.125f, 0.4f);
        glVertex2f(0.075f, 0.35f);
        glEnd();
    }
    if (erros >= 4) { // Braço direito
        glBegin(GL_LINES);
        glVertex2f(0.125f, 0.4f);
        glVertex2f(0.175f, 0.35f);
        glEnd();
    }
    if (erros >= 5) { // Perna esquerda
        glBegin(GL_LINES);
        glVertex2f(0.125f, 0.25f);
        glVertex2f(0.075f, 0.15f);
        glEnd();
    }
    if (erros >= 6) { // Perna direita
        glBegin(GL_LINES);
        glVertex2f(0.125f, 0.25f);
        glVertex2f(0.175f, 0.15f);
        glEnd();
    }
}

// Função para desenhar texto
void desenhaTexto(void* font, const char* text, float x, float y) {
    glRasterPos2f(x, y);
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

// Função para desenhar o menu principal
void desenhaMenu() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0); // Cor branca
    desenhaTexto(GLUT_BITMAP_TIMES_ROMAN_24, "JOGO DA FORCA", -0.3f, 0.5f);
    desenhaTexto(GLUT_BITMAP_HELVETICA_18, "AUTOR: Ronny Rocke", -0.25f, 0.4f);

    // Desenha as opções de menu
    if (hoverIniciar) {
        glColor3f(0.0, 1.0, 0.0); // Verde se estiver com o mouse sobre "Iniciar"
    } else {
        glColor3f(1.0, 1.0, 1.0); // Branco
    }
    desenhaTexto(GLUT_BITMAP_HELVETICA_18, "Iniciar", -0.1f, 0.1f);

    if (hoverSair) {
        glColor3f(0.0, 1.0, 0.0); // Verde se estiver com o mouse sobre "Sair"
    } else {
        glColor3f(1.0, 1.0, 1.0); // Branco
    }
    desenhaTexto(GLUT_BITMAP_HELVETICA_18, "Sair", -0.1f, -0.1f);

    glFlush();
}

// Função para desenhar a tela do jogo
void desenhaJogo() {
    glClear(GL_COLOR_BUFFER_BIT);
    desenhaBoneco();

    // Desenha a palavra oculta com espaços
    glColor3f(1.0, 1.0, 1.0);
    for (size_t i = 0; i < palavraOculta.length(); i++) {
        desenhaTexto(GLUT_BITMAP_HELVETICA_18, (string(1, palavraOculta[i]) + " ").c_str(), -0.7f + i * 0.1f, -0.9f);
    }

    // Exibe letras já digitadas
    string letras;
    for (char letra : letrasDigitadas) {
        letras += letra;
        letras += ' ';
    }
    desenhaTexto(GLUT_BITMAP_HELVETICA_18, ("Letras digitadas: " + letras).c_str(), -0.7f, -0.8f);

    // Exibe mensagens de vitória ou derrota
    if (estadoAtual == GAME_OVER) {
        if (erros >= maxErros) {
            desenhaTexto(GLUT_BITMAP_HELVETICA_18, ("Você perdeu! A palavra era: " + palavraSecreta).c_str(), -0.7f, 0.0f);
        } else {
            desenhaTexto(GLUT_BITMAP_HELVETICA_18, "Você venceu!", -0.7f, 0.0f);
        }
    }

    glFlush();
}

// Função para capturar o movimento do mouse no menu
void movimentoMouse(int x, int y) {
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    float mouseX = (float)x / width;
    float mouseY = (float)(height - y) / height;

    hoverIniciar = (mouseX > 0.4f && mouseX < 0.6f && mouseY > 0.5f && mouseY < 0.6f);
    hoverSair = (mouseX > 0.4f && mouseX < 0.6f && mouseY > 0.3f && mouseY < 0.4f);

    glutPostRedisplay();
}

// Função para capturar cliques no menu
void mouseMenu(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (hoverIniciar) {
            cout << "Insira a palavra secreta: ";
            cin >> palavraSecreta;
            palavraOculta = string(palavraSecreta.length(), '_');
            letrasDigitadas.clear();
            erros = 0;
            estadoAtual = GAME;
        } else if (hoverSair) {
            exit(0);
        }
    }
}

// Função para capturar entrada do teclado durante o jogo
void teclado(unsigned char key, int x, int y) {
    if (estadoAtual == GAME && letrasDigitadas.find(key) == letrasDigitadas.end()) {
        letrasDigitadas.insert(key);

        bool acerto = false;
        for (int i = 0; i < palavraSecreta.length(); i++) {
            if (palavraSecreta[i] == key) {
                palavraOculta[i] = key;
                acerto = true;
            }
        }

        if (!acerto) {
            erros++;
        }

        if (palavraOculta == palavraSecreta) {
            estadoAtual = GAME_OVER;
        } else if (erros >= maxErros) {
            estadoAtual = GAME_OVER;
        }

        glutPostRedisplay();
    } else if (estadoAtual == GAME_OVER) {
        reiniciarJogo();
    }
}

// Função para reiniciar o jogo
void reiniciarJogo() {
    cout << "Pressione qualquer tecla para reiniciar..." << endl;
    char input;
    cin >> input;

    // Reinicializa as variáveis do jogo
    palavraSecreta.clear();
    palavraOculta.clear();
    letrasDigitadas.clear();
    erros = 0;
    estadoAtual = MENU; // Volta para o menu
}

// Inicializa as configurações de exibição
void inicializa() {
    glClearColor(0.0, 0.0, 1.0, 1.0); // Fundo azul
    glColor3f(1.0, 1.0, 1.0);         // Cor de desenho branca
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

// Função principal
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Jogo da Forca");

    inicializa();

    glutDisplayFunc([]() {
        if (estadoAtual == MENU) {
            desenhaMenu();
        } else if (estadoAtual == GAME || estadoAtual == GAME_OVER) {
            desenhaJogo();
        }
    });

    glutMouseFunc(mouseMenu);
    glutPassiveMotionFunc(movimentoMouse);
    glutKeyboardFunc(teclado);

    glutMainLoop();
    return 0;
}
