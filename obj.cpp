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

GLfloat dx = 0.0;
GLfloat translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;
GLfloat rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
GLfloat tamX = 1.0f, tamY = 1.0f, tamZ = 1.0f;
GLfloat currentColor[3] = {1.0f, 1.0f, 1.0f};
GLfloat movementSpeed = 0.1f;


// Variáveis para controle da câmera
GLfloat cameraX = 0.0f, cameraY = 1.0f, cameraZ = 5.0f;
GLfloat lookX = 0.0f, lookY = 0.0f, lookZ = 0.0f;
bool followCamera = false;
GLfloat angle = 65.0f, fAspect = 1.0f; // Inicialize angle e fAspect
GLfloat cameraOffsetY = 2.0f;
GLfloat cameraOffsetZ = 5.0f;

// Variáveis para o controle do mouse
GLint dragging = 0;
GLint draggingTranslate = 0;
GLint lastX = 0;
GLint lastY = 0;

bool wireframe = true;

void updateCameraPosition() {
    if (followCamera) {
        // Define a posição da câmera atrás do personagem, em relação à sua rotação
        cameraX = translateX - cameraOffsetZ * sin(rotateY * M_PI / 180.0f);
        cameraZ = translateZ + cameraOffsetZ * cos(rotateY * M_PI / 180.0f);
        cameraY = translateY + cameraOffsetY;  // Define a altura da câmera
    }
}


void EspecificaParametrosVisualizacao(void) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.1, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if(followCamera) {
        gluLookAt(translateX - 5.0f, translateY + 2.0f, translateZ + 5.0f, translateX, translateY, translateZ, 0.0f, 1.0f, 0.0f);
    } else {
        gluLookAt(0.0f, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat)w / (GLfloat)h;
    EspecificaParametrosVisualizacao();
}

void Teclado(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': // Move para frente
            translateX += movementSpeed * sin(rotateY * M_PI / 180.0f);
            translateZ -= movementSpeed * cos(rotateY * M_PI / 180.0f);
            break;
        case 's': // Move para trás
            translateX -= movementSpeed * sin(rotateY * M_PI / 180.0f);
            translateZ += movementSpeed * cos(rotateY * M_PI / 180.0f);
            break;
        case 'a': // Move para a esquerda
            translateX -= movementSpeed * cos(rotateY * M_PI / 180.0f);
            translateZ -= movementSpeed * sin(rotateY * M_PI / 180.0f);
            break;
        case 'd': // Move para a direita
            translateX += movementSpeed * cos(rotateY * M_PI / 180.0f);
            translateZ += movementSpeed * sin(rotateY * M_PI / 180.0f);
            break;
        case '+':
        case '=':
            tamX += 0.1f;
            tamY += 0.1f;
            tamZ += 0.1f;
            break;
        case '-':
            tamX -= 0.1f;
            tamY -= 0.1f;
            tamZ -= 0.1f;
            break;
        case 'c':
            currentColor[0] = (float)rand() / RAND_MAX;
            currentColor[1] = (float)rand() / RAND_MAX;
            currentColor[2] = (float)rand() / RAND_MAX;
            break;
        case 'q': cameraZ -= 0.1f; break;
        case 'e': cameraZ += 0.1f; break;
        case 'm':
            wireframe = !wireframe;
        break;
        case 'v':
            followCamera = !followCamera;
        break;
    }
    updateCameraPosition();
    glutPostRedisplay();
}


void special(int tecla, int x, int y) {
    switch (tecla) {
        case GLUT_KEY_UP:
            translateX += 0.1f * sin(rotateY * M_PI / 180.0f);
        translateZ += 0.1f * cos(rotateY * M_PI / 180.0f);
        break;
        case GLUT_KEY_DOWN:
            translateX -= 0.1f * sin(rotateY * M_PI / 180.0f);
        translateZ -= 0.1f * cos(rotateY * M_PI / 180.0f);
        break;
        case GLUT_KEY_RIGHT: rotateY += 5.0f; break;
        case GLUT_KEY_LEFT: rotateY -= 5.0f; break;
    }

    updateCameraPosition();
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            dragging = 1;
            lastX = x;
            lastY = y;
        } else if (state == GLUT_UP) {
            dragging = 0;
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            draggingTranslate = 1;
            lastX = x;
            lastY = y;
        } else if (state == GLUT_UP) {
            draggingTranslate = 0;
        }
    }
    if (button == GLUT_MIDDLE_BUTTON) {
        if (state == GLUT_DOWN) {
            currentColor[0] = (float)rand() / RAND_MAX;
            currentColor[1] = (float)rand() / RAND_MAX;
            currentColor[2] = (float)rand() / RAND_MAX;
            glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    if (dragging) {
        int deltaX = x - lastX;
        int deltaY = y - lastY;
        rotateY += deltaX * 0.5f;
        rotateX -= deltaY * 0.5f;
    }
    if (draggingTranslate) {
        int deltaX = x - lastX;
        int deltaY = y - lastY;
        translateX += deltaX * 0.01f;
        translateY -= deltaY * 0.01f;
    }
    lastX = x;
    lastY = y;
    glutPostRedisplay();
}

void mouseWheel(int button, int dir, int x, int y) {
    if (dir > 0) {
        tamX += 0.1f;
        tamY += 0.1f;
        tamZ += 0.1f;
    } else {
        tamX -= 0.1f;
        tamY -= 0.1f;
        tamZ -= 0.1f;
    }
    glutPostRedisplay();
}

void Inicializa(void) {
    glClearColor(1.0, 1.0, 1.0, 0.0); // Fundo branco

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);

    EspecificaParametrosVisualizacao();

    obj.Carregar("character.obj");

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

void ObjWireFrame(void) {
    glPushMatrix();
    for (unsigned int j = 0; j < obj.faces.size(); ++j) {
        glBegin(GL_LINE_LOOP);
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

void ObjSolid(void) {
    glPushMatrix();
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
    if(followCamera) {
        gluLookAt(translateX - 5.0f, translateY + 2.0f, translateZ + 5.0f, translateX, translateY, translateZ, 0.0f, 1.0f, 0.0f);
    } else {
        gluLookAt(0.0f, 50.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }

    // Desenhar o piso sem transformações
    glPushMatrix();
    obj.CarregarTextura();

    Piso(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0); //terreno fixo
    glPopMatrix();

    // Aplicar as transformações ao objeto separadamente
    glPushMatrix();
    // Aplicar as transformações de translação, rotação e escala do objeto
    glTranslatef(translateX, translateY + 1.2f, translateZ); // Transladar o objeto
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);            // Rotacionar ao redor do eixo X
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);            // Rotacionar ao redor do eixo Y
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);            // Rotacionar ao redor do eixo Z
    glScalef(tamX, tamY, tamZ);                      // Escalar o objeto
    glColor3fv(currentColor);

    // Desenhar o objeto
    if (wireframe) {
        ObjWireFrame();
    } else {
        ObjSolid();
    }
    glPopMatrix(); // Finaliza a transformação do objeto

    glutSwapBuffers();
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Computação Gráfica: Formula 1");

    Inicializa();

    glutDisplayFunc(Desenha);
    glutReshapeFunc(AlteraTamanhoJanela);
    glutKeyboardFunc(Teclado);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    //glutMouseWheelFunc(mouseWheel);

    glutMainLoop();
    return 0;
}