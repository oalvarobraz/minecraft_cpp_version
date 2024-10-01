#include <cmath>
#include <iostream>
#include "CarregarArquivo2.cpp"

using namespace std;


//////////////////////////////////////////////////////
//                                                  //
// Álvaro Braz Cunha                                //
// Diego Sanches Nere dos Santos                    //
//                                                  //
//////////////////////////////////////////////////////


CarregarArquivo obj;

// Variáveis de controle do personagem
GLfloat translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;
GLfloat rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
GLfloat tamX = 1.0f, tamY = 1.0f, tamZ = 1.0f;
GLfloat movementSpeed = 0.02f;


// Variáveis para controle da câmera
GLfloat cameraX = 0.0f, cameraY = 1.0f, cameraZ = 5.0f;
GLfloat lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
bool followCamera = false;
GLfloat angle = 65.0f, fAspect = 1.0f; // Inicialize angle e fAspect
GLfloat cameraOffsetY = 2.25f;
GLfloat cameraOffsetZ = 4.5f;
bool cameraSwitched = false;
enum CameraView {
    THIRD_PERSON,
    FIRST_PERSON,
    FIXED_VIEW
};
CameraView camera = FIXED_VIEW;

// Variáveis para o controle do mouse
GLint dragging = 0;
GLint draggingTranslate = 0;
GLint lastX = 0;
GLint lastY = 0;
float mouseSensitivity = 0.05f;

// Variáveis para o controle do teclado
bool keys[256];
float originalRotateX = 0.0f, originalRotateY = 0.0f;


void updateCameraPosition() {
    // Define a posição da câmera atrás do personagem, em relação à sua rotação
    cameraX = translateX + cameraOffsetZ * sin(rotateX * M_PI / 180.0f);
    cameraZ = translateZ - cameraOffsetZ * cos(rotateX * M_PI / 180.0f);
    cameraY = translateY + cameraOffsetY;  // Define a altura da câmera
}


void EspecificaParametrosVisualizacao(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    switch (camera) {
        case THIRD_PERSON:
            updateCameraPosition();
            gluLookAt(cameraX, cameraY + 2.0f, cameraZ, translateX, translateY, translateZ, 0.0f, 1.0f, 0.0f);
            break;
        case FIRST_PERSON:
            gluLookAt(translateX, translateY + 1.2f, translateZ, translateX + sin(rotateY * M_PI / 180.0f), translateY + 1.2f + sin(rotateX * M_PI / 180.0f), translateZ + cos(rotateY * M_PI / 180.0f), 0.0f, 1.0f, 0.0f);
            break;
        case FIXED_VIEW:
            gluLookAt(0.0f, 25.0f, -30.0f, translateX, translateY, translateZ, 0.0f, 1.0f, 0.0f);
            break;
    }
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}

void Keys(unsigned char key, int x, int y) {
    keys[key] = true; // Set key state to true when pressed
}

void KeysUp(unsigned char key, int x, int y) {
    keys[key] = false; // Set key state to false when released
}

void updateMovement() {
    GLfloat dirX = -sin(rotateX * M_PI / 180.0f);
    GLfloat dirZ = -cos(rotateX * M_PI / 180.0f);
    if (camera == FIRST_PERSON) {
        dirX = -sin(rotateY * M_PI / 180.0f);
        dirZ = -cos(rotateY * M_PI / 180.0f);
    }


    bool movingForward = keys['w'];
    bool movingBackward = keys['s'];
    bool movingLeft = keys['a'];
    bool movingRight = keys['d'];

    // Movimentação
    if (movingForward && !movingBackward) {
        translateX += movementSpeed * dirX;
        translateZ -= movementSpeed * dirZ;

        if (movingRight && !movingLeft) {
            // Diagonal para frente e direita
            translateX += movementSpeed * dirZ;
            translateZ += movementSpeed * dirX;
            if (camera != FIRST_PERSON) {
                rotateY = -45.0f;
            }
        } else if (movingLeft && !movingRight) {
            // Diagonal para frente e esquerda
            translateX -= movementSpeed * dirZ;
            translateZ -= movementSpeed * dirX;
            if (camera != FIRST_PERSON) {
                rotateY = 45.0f;
            }
        } else {
            // Apenas para frente
            if (camera != FIRST_PERSON) {
                rotateY = 0.0f;
            }
        }
    } else if (movingBackward && !movingForward) {
        translateX -= movementSpeed * dirX;
        translateZ += movementSpeed * dirZ;

        if (movingRight && !movingLeft) {
            // Diagonal para trás e direita
            translateX += movementSpeed * dirZ;
            translateZ += movementSpeed * dirX;
            if (camera != FIRST_PERSON) {
                rotateY = -135.0f;
            }
        } else if (movingLeft && !movingRight) {
            // Diagonal para trás e esquerda
            translateX -= movementSpeed * dirZ;
            translateZ -= movementSpeed * dirX;
            if (camera != FIRST_PERSON) {
                rotateY = 135.0f;
            }
        } else {
            // Apenas para trás
            if (camera != FIRST_PERSON) {
                rotateY = 180.0f;
            }
        }
    } else if (movingRight && !movingLeft) {
        // Apenas para a direita
        translateX += movementSpeed * dirZ;
        translateZ += movementSpeed * dirX;
        if (camera != FIRST_PERSON) {
            rotateY = -90.0f;
        }
    } else if (movingLeft && !movingRight) {
        // Apenas para a esquerda
        translateX -= movementSpeed * dirZ;
        translateZ -= movementSpeed * dirX;
        if (camera != FIRST_PERSON) {
             rotateY = 90.0f;
        }
    }


    if (keys['v'] && !cameraSwitched) {
        camera = static_cast<CameraView>((camera + 1)%3);
        cameraSwitched = true;
    } else if (!keys['v']) {
        cameraSwitched = false;
    }

    if (keys[27]) {
        glutSetCursor(GLUT_CURSOR_INHERIT);
    }
}

void passiveMotion(int x, int y) {
    if (camera == FIRST_PERSON) {
        int midX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        int midY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

        int deltaX = x - midX;
        int deltaY = y - midY;

        // Controle horizontal (esquerda/direita)
        rotateY -= deltaX * mouseSensitivity;

        // Controle vertical (cima/baixo), limitando o angulo para não passar dos 90 graus
        rotateX -= deltaY * mouseSensitivity;

        // Reposiciona o mouse no centro da tela
        glutWarpPointer(midX, midY);


        glutPostRedisplay();
    }
}

void desenharReticula() {
    int screenWidth = glutGet(GLUT_WINDOW_WIDTH);
    int screenHeight = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight); // Coordenadas baseadas no tamanho atual da janela
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(0.0f, 0.0f, 0.0f);

    int size = 5;

    // Desenhar a retícula no centro da tela
    glBegin(GL_LINES);
    glVertex2f(screenWidth / 2 - size, screenHeight / 2);
    glVertex2f(screenWidth / 2 + size, screenHeight / 2);
    glVertex2f(screenWidth / 2, screenHeight / 2 - size);
    glVertex2f(screenWidth / 2, screenHeight / 2 + size);
    glEnd();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void Inicializa(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0); // Fundo branco

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    GLfloat luzAmbiente[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat luzDifusa[4] = {1.0f, 0.9f, 0.7f, 1.0f};    // cor
    GLfloat luzEspecular[4] = {1.0f, 1.0f, 1.0f, 1.0f}; // brilho
    GLfloat direcaoLuz[4] = {0.0f, 1.0f, 0.0f, 0.0f};   // alto para baixo

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
    glLightfv(GL_LIGHT0, GL_POSITION, direcaoLuz);

    EspecificaParametrosVisualizacao();

    obj.Carregar("steve.obj");

}

void Piso(float escala, float altura, GLfloat translateX, GLfloat translateY, GLfloat translateZ, GLfloat rotateX, GLfloat rotateY, GLfloat rotateZ, float repeatFactor) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obj.textura_id);

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();

    // Desenhar o piso
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(100.0, 0.0, 100.0);
        glTexCoord2f(repeatFactor, 0.0); glVertex3f(-100.0, 0.0, 100.0);
        glTexCoord2f(repeatFactor, repeatFactor); glVertex3f(-100.0, 0.0, -100.0);
        glTexCoord2f(0.0, repeatFactor); glVertex3f(100.0, 0.0, -100.0);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void ObjSolid(void) {
    glPushMatrix();

    GLfloat materialAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat materialDifuso[] = {1.0f, 0.5f, 0.31f, 1.0f};
    GLfloat materialEspecular[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat brilho = 32.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDifuso);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialEspecular);
    glMaterialf(GL_FRONT, GL_SHININESS, brilho);

    for (unsigned int j = 0; j < obj.faces.size(); ++j) {
        glBegin(GL_POLYGON);
        for (unsigned int i = 0; i < obj.faces[j].size(); ++i) {
            GLfloat nor[3] = {obj.normais[obj.faces[j][i][2]][0], obj.normais[obj.faces[j][i][2]][1], obj.normais[obj.faces[j][i][2]][2]};
            glNormal3fv(nor);
            GLfloat vert[3] = {obj.vertices[obj.faces[j][i][0]][0], obj.vertices[obj.faces[j][i][0]][1], obj.vertices[obj.faces[j][i][0]][2]};
            glVertex3fv(vert);
        }
        glEnd();
    }
    glPopMatrix();
}

void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Aplicar a mesma configuração de câmera
    glLoadIdentity();
    switch (camera) {
        case THIRD_PERSON:
            updateCameraPosition();
            gluLookAt(cameraX, cameraY + 2.0f, cameraZ, translateX, translateY + 2.0f, translateZ, 0.0f, 1.0f, 0.0f);
            break;
        case FIRST_PERSON:
            gluLookAt(translateX, translateY + 1.2f, translateZ, translateX + sin(rotateY * M_PI / 180.0f), translateY + 1.2f + sin(rotateX * M_PI / 180.0f), translateZ + cos(rotateY * M_PI / 180.0f), 0.0f, 1.0f, 0.0f);
            break;
        case FIXED_VIEW:
            gluLookAt(0.0f, 25.0f, -30.0f, translateX, translateY, translateZ, 0.0f, 1.0f, 0.0f);
            break;
    }

    // Desenhar o piso sem transformações
    glPushMatrix();
    obj.CarregarTextura();

    Piso(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0); //terreno fixo
    glPopMatrix();

    // Aplicar as transformações ao objeto separadamente
    glPushMatrix();
    glTranslatef(translateX, translateY, translateZ);           // Transladar o objeto
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);            // Rotacionar ao redor do eixo X
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);            // Rotacionar ao redor do eixo Y
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);            // Rotacionar ao redor do eixo Z
    glScalef(tamX, tamY, tamZ);                                 // Escalar o objeto

    // Desenhar o objeto
    if (camera == FIRST_PERSON) {
        glScalef(0.0f, 0.0f, 0.0f);
    }
    ObjSolid();

    glPopMatrix(); // Finaliza a transformação do objeto

    if(camera == FIRST_PERSON) {
        glutSetCursor(GLUT_CURSOR_NONE);
        desenharReticula();
    }

    glutSwapBuffers();
}

void display() {
    updateMovement();
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Minecraft");

    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(Keys);
    glutKeyboardUpFunc(KeysUp);
    glutIdleFunc(display);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    //glutMouseWheelFunc(mouseWheel);
    glutPassiveMotionFunc(passiveMotion);

    glutMainLoop();
    return 0;
}