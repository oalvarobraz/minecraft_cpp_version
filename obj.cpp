#include <cmath>
#include <iostream>
#include <cstdio>
#include "CarregarArquivo2.cpp"

using namespace std;


//////////////////////////////////////////////////////
//                                                  //
// ?lvaro Braz Cunha                                //
// Diego Sanches Nere dos Santos                    //
//                                                  //
//////////////////////////////////////////////////////


CarregarArquivo obj;

// Vari?veis de controle do personagem
GLfloat translateX = 0.0f, translateY = 0.0f, translateZ = -5.0f;
GLfloat rotateX = 0.0f, rotateY = 0.0f, rotateZ = 0.0f;
GLfloat tamX = 3.0f, tamY = 3.0f, tamZ = 3.0f;
GLfloat movementSpeed = 0.02f;


// Vari?veis para controle da c?mera
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

const GLfloat floorMinX = -100.0f;
const GLfloat floorMaxX = 100.0f;
const GLfloat floorMinZ = -100.0f;
const GLfloat floorMaxZ = 100.0f;


// Vari?veis para o controle do mouse
GLint dragging = 0;
GLint draggingTranslate = 0;
GLint lastX = 0;
GLint lastY = 0;
float mouseSensitivity = 0.05f;

// Vari?veis para o controle do teclado
bool keys[256];
float originalRotateX = 0.0f, originalRotateY = 0.0f;

// Vari?veis de controle de objetos
struct ObjetoCair {
    GLfloat x, y, z;
    GLfloat velocidade;
};

std::vector<ObjetoCair> objetos;
int count = 0;


// Fun??o para criar objetos caindo do c?u
void criarObjeto() {
    ObjetoCair obj;

    // Definir a probabilidade de cair pr?ximo ou longe do personagem
    float probabilidade = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    if (probabilidade < 0.5f) {
        obj.x = translateX + rand()/static_cast<float>(RAND_MAX);
        obj.z = translateZ + rand()/static_cast<float>(RAND_MAX);
    } else {
        obj.x = rand() % 200 - 100;
        obj.z = rand() % 200 - 100;
    }

    obj.y = 50.0f;
    obj.velocidade = 0.035f;
    objetos.push_back(obj);
}


void atualizarObjetos() {
    for (auto it = objetos.begin(); it != objetos.end(); ) {
        it->y -= it->velocidade;
        if (it->y <= -10.0f) {
            it = objetos.erase(it);
        } else {
            ++it;
        }
    }
}


// Fun??o para desenhar os objetos na tela
void desenharObjetos() {
    for (const auto &obj : objetos) {
        glPushMatrix();
        glTranslatef(obj.x, obj.y, obj.z);
        glColor3f(1.0f, 0.0f, 0.0f);
        glutSolidCube(10.0f);
        //glutSolidSphere(1.0f, 20, 20);
        glPopMatrix();
    }
}

void updateCameraPosition() {
    // Define a posi??o da c?mera atr?s do personagem, em rela??o ? sua rota??o
    cameraX = translateX + cameraOffsetZ * sin(rotateX * M_PI / 180.0f);
    cameraZ = translateZ - cameraOffsetZ * cos(rotateX * M_PI / 180.0f);
    cameraY = translateY + cameraOffsetY;  // Define a altura da c?mera
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
        dirX = sin(rotateY * M_PI / 180.0f);
        dirZ = -cos(rotateY * M_PI / 180.0f);
    }

    bool movingForward = keys['w'];
    bool movingBackward = keys['s'];
    bool movingLeft = keys['a'];
    bool movingRight = keys['d'];

    // Movimentação
    if (movingForward && !movingBackward) {
        GLfloat newTranslateX = translateX + movementSpeed * dirX;
        GLfloat newTranslateZ = translateZ - movementSpeed * dirZ;

        // Limitar a movimentação dentro das bordas do piso
        if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
            translateX = newTranslateX;
        }
        if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
            translateZ = newTranslateZ;
        }

        if (movingRight && !movingLeft) {
            // Diagonal para frente e direita
            newTranslateX = translateX + movementSpeed * dirZ;
            newTranslateZ = translateZ + movementSpeed * dirX;

            if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
                translateX = newTranslateX;
            }
            if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
                translateZ = newTranslateZ;
            }

            if (camera != FIRST_PERSON) {
                rotateY = -45.0f;
            }
        } else if (movingLeft && !movingRight) {
            // Diagonal para frente e esquerda
            newTranslateX = translateX - movementSpeed * dirZ;
            newTranslateZ = translateZ - movementSpeed * dirX;

            if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
                translateX = newTranslateX;
            }
            if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
                translateZ = newTranslateZ;
            }

            if (camera != FIRST_PERSON) {
                rotateY = 45.0f;
            }
        } else {
            if (camera != FIRST_PERSON) {
                rotateY = 0.0f;
            }
        }
    } else if (movingBackward && !movingForward) {
        GLfloat newTranslateX = translateX - movementSpeed * dirX;
        GLfloat newTranslateZ = translateZ + movementSpeed * dirZ;

        if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
            translateX = newTranslateX;
        }
        if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
            translateZ = newTranslateZ;
        }

        if (movingRight && !movingLeft) {
            newTranslateX = translateX + movementSpeed * dirZ;
            newTranslateZ = translateZ + movementSpeed * dirX;

            if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
                translateX = newTranslateX;
            }
            if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
                translateZ = newTranslateZ;
            }

            if (camera != FIRST_PERSON) {
                rotateY = -135.0f;
            }
        } else if (movingLeft && !movingRight) {
            newTranslateX = translateX - movementSpeed * dirZ;
            newTranslateZ = translateZ - movementSpeed * dirX;

            if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
                translateX = newTranslateX;
            }
            if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
                translateZ = newTranslateZ;
            }

            if (camera != FIRST_PERSON) {
                rotateY = 135.0f;
            }
        } else {
            if (camera != FIRST_PERSON) {
                rotateY = 180.0f;
            }
        }
    } else if (movingRight && !movingLeft) {
        GLfloat newTranslateX = translateX + movementSpeed * dirZ;
        GLfloat newTranslateZ = translateZ + movementSpeed * dirX;

        if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
            translateX = newTranslateX;
        }
        if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
            translateZ = newTranslateZ;
        }

        if (camera != FIRST_PERSON) {
            rotateY = -90.0f;
        }
    } else if (movingLeft && !movingRight) {
        GLfloat newTranslateX = translateX - movementSpeed * dirZ;
        GLfloat newTranslateZ = translateZ - movementSpeed * dirX;

        if (newTranslateX >= floorMinX && newTranslateX <= floorMaxX) {
            translateX = newTranslateX;
        }
        if (newTranslateZ >= floorMinZ && newTranslateZ <= floorMaxZ) {
            translateZ = newTranslateZ;
        }

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

        // Controle horizontal
        rotateY -= deltaX * mouseSensitivity;

        // Controle vertical
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

    // Desenhar a ret?cula no centro da tela
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

    obj.Carregar("steveBlender.obj");
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
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_steve);
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
            GLfloat tex[2] {obj.texturas[obj.faces[j][i][1]][0], obj.texturas[obj.faces[j][i][1]][1]};
            glTexCoord2fv(tex);
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void DrawSkybox(float size) {
    glDisable(GL_LIGHTING); // Desativa iluminação para não influenciar a skybox
    glEnable(GL_TEXTURE_2D); // Habilita texturas
    glColor3f(0.788f, 0.898f, 0.961f);

    // Frente
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[0]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f( size, -size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f( size,  size, -size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size,  size, -size);
    glEnd();

    // Trás
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f( size, -size,  size);
        glTexCoord2f(1.0, 0.0); glVertex3f(-size, -size,  size);
        glTexCoord2f(1.0, 1.0); glVertex3f(-size,  size,  size);
        glTexCoord2f(0.0, 1.0); glVertex3f( size,  size,  size);
    glEnd();

    // Esquerda
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size,  size);
        glTexCoord2f(1.0, 0.0); glVertex3f(-size, -size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f(-size,  size, -size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size,  size,  size);
    glEnd();

    // Direita
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[3]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f( size, -size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f( size, -size,  size);
        glTexCoord2f(1.0, 1.0); glVertex3f( size,  size,  size);
        glTexCoord2f(0.0, 1.0); glVertex3f( size,  size, -size);
    glEnd();

    // Topo
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[4]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size,  size, -size);
        glTexCoord2f(1.0, 0.0); glVertex3f( size,  size, -size);
        glTexCoord2f(1.0, 1.0); glVertex3f( size,  size,  size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size,  size,  size);
    glEnd();

    // Base
    glBindTexture(GL_TEXTURE_2D, obj.textura_id_skybox[5]);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-size, -size,  size);
        glTexCoord2f(1.0, 0.0); glVertex3f( size, -size,  size);
        glTexCoord2f(1.0, 1.0); glVertex3f( size, -size, -size);
        glTexCoord2f(0.0, 1.0); glVertex3f(-size, -size, -size);
    glEnd();

    glDisable(GL_TEXTURE_2D); // Desativa texturas
    glEnable(GL_LIGHTING); // Reativa iluminação, caso esteja utilizando
}



void Desenha(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Aplicar a mesma configura??o de c?mera
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

    // Desenhar o piso sem transforma??es
    glPushMatrix();
    obj.CarregarTextura();

    Piso(1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0); //terreno fixo
    glPopMatrix();

    // Aplicar as transforma??es ao objeto separadamente
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
    obj.CarregarTexturaSteve();

    ObjSolid();
    glPopMatrix(); // Finaliza a transforma??o do objeto

    desenharObjetos();
    if(camera == FIRST_PERSON) {
        glutSetCursor(GLUT_CURSOR_NONE);
        desenharReticula();
    }

    glPushMatrix();
    obj.CarregarTexturaSkybox();
    DrawSkybox(200.0f);
    glPopMatrix();

    glutSwapBuffers();
}

void display() {
    updateMovement();
    count++;
    if (count % 1000 == 0) {
        criarObjeto();
    }
    atualizarObjetos();
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