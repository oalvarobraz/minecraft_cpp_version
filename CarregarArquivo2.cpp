#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <vector>
#include <string>

using namespace std;
class CarregarArquivo
{
public:
    vector<vector<GLfloat> > vertices; //guarda os vertices
    vector<vector<GLfloat> > normais;   //guarda as normais
    vector<vector<GLfloat> > texturas;  //guarda as texturas
    vector<vector<vector<GLint> > > faces; //guarda as faces
    GLubyte floor[512][512][3];
    GLubyte steve[64][64][3];
    GLubyte sky[2000][2000][3];
    GLubyte skybox[1024][1024][3];
    GLubyte bloco[256][256][3];


    GLuint textura_id;
    GLuint textura_id_steve;
    GLuint textura_id_skybox[6];
    GLuint textura_id_blocos;
    GLuint textura_id_sky[6];

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif


    CarregarArquivo()
    {


    }

    void Carregar(char in[])
    {
        string *tPonteiro = new string;
        string &t = *tPonteiro;

        ifstream arquivo(in);
        //string t;
        if(!arquivo)
            cout << "Erro ao ler o arquivo";
        char lido;
        arquivo.get(lido); // pega o primeiro char

        while(arquivo)
        {


            if(lido == 'v') // olha se ? um v,vn ou vt
            {
                arquivo.get(lido); // usado para decidir se ? v,vn ou vt
                float aux ;
                GLfloat aux1 ;
                vector<GLfloat> vauxf;// ser? colocado na lista
                if(lido == 'n') // olha se ? normal
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o z

                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    normais.push_back(vauxf); //coloca na sua lista

                }
                if(lido == 't') // olha se ? textura
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);
                    texturas.push_back(vauxf); //coloca na sua lista

                }
                if(lido == ' ') // olha se ? vertice
                {
                    arquivo >>  aux; //pega o x
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o y
                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    arquivo >>  aux; //pega o z

                    aux1 =  aux;
                    vauxf.push_back(aux1);

                    vertices.push_back(vauxf);//coloca na sua lista

                }
            }

            if(lido == 'f')
            {
                vector<vector<GLint> > face; // a face pode ter qualquer numero de vertices...
                arquivo.get(lido);

                if(lido == ' ')
                {

                    bool barras = true;

                    //string t;
                    getline(arquivo,t);

                    int found = t.find("//");

                    if (found!=string::npos) //vertices e normal
                    {
                        barras = false;
                        vector<GLint> vert_normal;
                        GLint valor_inteiro ;
                        int valor = 0;
                        int numero = 10;
                        int posi = 0;
                        for(int i = 0 ; i < t.size(); i++)
                        {
                            char ch = t[i];
                            if(ch >= '0' && ch <= '9')
                            {
                                valor = valor*numero;
                                valor = valor + ((int)ch - (int)'0');//constroi numero

                                if(i== t.size() -1)// chegou no final da linha
                                {
                                    valor_inteiro  = valor -1;
                                    vert_normal.push_back(valor_inteiro );

                                    valor = 0;

                                    face.push_back(vert_normal);
                                    vector<GLint> aux4;
                                    vert_normal = aux4; //limpar

                                }


                            }
                            else if(ch == '/')
                            {
                                valor_inteiro  = valor -1;
                                vert_normal.push_back(valor_inteiro );
                                i++;
                                vert_normal.push_back(-1); //textura n?o existe
                                valor = 0;


                            }
                            else
                            {
                                valor_inteiro  = valor -1;
                                vert_normal.push_back(valor_inteiro );

                                valor = 0;
                                face.push_back(vert_normal);
                                vector<GLint> aux4;
                                vert_normal = aux4;

                            }




                        }

                        faces.push_back(face);


                    }
                    if(barras) //n?o tem normal
                    {

                        found = t.find("/"); //

                        if (found!=string::npos)//tem textura
                        {

                            vector<GLint> vert_textura;
                            GLint valor_inteiro ;
                            int valor = 0;
                            int numero = 10;
                            int posi = 0;

                            for(int i = 0 ; i < t.size(); i++)
                            {

                                char ch = t[i];

                                if(ch >= '0' && ch <= '9')
                                {
                                    valor = valor*numero;
                                    valor = valor + ((int)ch - (int)'0');

                                    if(i== t.size() -1)
                                    {
                                        valor_inteiro = valor -1;
                                        vert_textura.push_back(valor_inteiro);
                                        vert_textura.push_back(-1);//normal
                                        valor = 0;

                                        face.push_back(vert_textura);
                                        vector<GLint> aux4;
                                        vert_textura = aux4;

                                    }


                                }
                                else if(ch == '/')
                                {
                                    valor_inteiro = valor -1;
                                    vert_textura.push_back(valor_inteiro); //vertice



                                    valor = 0;


                                }
                                else
                                {
                                    valor_inteiro = valor -1;
                                    vert_textura.push_back(valor_inteiro);
                                    vert_textura.push_back(-1);//normal

                                    valor = 0;

                                    face.push_back(vert_textura);
                                    vector<GLint> aux4;
                                    vert_textura = aux4;

                                }




                            }

                            faces.push_back(face);


                        }
                        else // s? tem vertice
                        {
                            vector<GLint> vertice_resto;
                            GLint valor_inteiro ;
                            int valor = 0;
                            int numero = 10;
                            int posi = 0;

                            for(int i = 0 ; i < t.size(); i++)
                            {

                                char ch = t[i];

                                if(ch >= '0' && ch <= '9')
                                {
                                    valor = valor*numero;
                                    valor = valor + ((int)ch - (int)'0');

                                    if(i== t.size() -1)
                                    {
                                        valor_inteiro = valor -1;
                                        vertice_resto.push_back(valor_inteiro);//vertice
                                        vertice_resto.push_back(-1);//textura
                                        vertice_resto.push_back(-1);//normal
                                        valor = 0;

                                        face.push_back(vertice_resto);

                                        vector<GLint> aux4;
                                        vertice_resto = aux4;

                                    }


                                }
                                else
                                {
                                    valor_inteiro = valor -1;

                                    vertice_resto.push_back(valor_inteiro);//vertice
                                    vertice_resto.push_back(-1);//textura
                                    vertice_resto.push_back(-1);//normal
                                    valor = 0;

                                    face.push_back(vertice_resto);
                                    vector<GLint> aux4;
                                    vertice_resto = aux4;

                                }




                            }

                            faces.push_back(face);






                        }

                    }


                }




            }


            arquivo.get(lido);

        }


    }

    void CarregarTextura() {
        static bool texturaCarregada = false;

        if (texturaCarregada) {
            return;
        }

        try {
            ifstream arq("grass.bmp", ios::binary);
            char c;
            if(!arq) cout << "Erro ao abrir arquivo";
            for(int i = 0; i < 138; i++) c = arq.get();
            for(int i = 0; i < 512; i++)
                for (int j = 0; j < 512; j++) {
                    c = arq.get(); floor[i][j][2] = c;
                    c = arq.get(); floor[i][j][1] = c;
                    c = arq.get(); floor[i][j][0] = c;
                }
            arq.close();
            arq.clear();

            // Gerar e associar a textura no OpenGL
            glGenTextures(1, &textura_id);
            glBindTexture(GL_TEXTURE_2D, textura_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, floor);

            // Definir parâmetros de filtro de textura
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            texturaCarregada = true;
        } catch (const exception &e) {
            cout << "Erro ao ler o arquivo";
        }
    }

    void CarregarTexturaSteve() {
        static bool texturaCarregada = false;

        if (texturaCarregada) {
            return;
        }

        try {
            ifstream arq("steveBlender.bmp", ios::binary);
            char c;
            if(!arq) cout << "Erro ao abrir arquivo";

            // Ignorar cabe alho BMP
            for(int i = 0; i < 54; i++) c = arq.get();
            for(int i = 0; i < 64; i++)
                for (int j = 0; j < 64; j++) {
                    c = arq.get(); steve[i][j][2] = c;
                    c = arq.get(); steve[i][j][1] = c;
                    c = arq.get(); steve[i][j][0] = c;
                }
            arq.close();
            arq.clear();

            // Gerar e associar a textura no OpenGL

            glGenTextures(2, &textura_id_steve);
            glBindTexture(GL_TEXTURE_2D, textura_id_steve);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 64, 64, 0, GL_RGB, GL_UNSIGNED_BYTE, steve);

            // Definir par metros de filtro de textura
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            texturaCarregada = true;
        } catch (const exception &e) {
            cout << "Erro ao ler o arquivo";
        }
    }

GLuint CarregarTexturaTGA(const char* arquivo, GLint larguraFinal = 1024, GLint alturaFinal = 1024) {
    FILE* file = fopen(arquivo, "rb");
    if (!file) {
        cout << "Erro ao abrir arquivo: " << arquivo << endl;
        return 0;
    }

    GLubyte header[18]; // Cabeçalho do TGA
    fread(header, sizeof(GLubyte), 18, file);

    GLint larguraOriginal = header[12] + (header[13] << 8);
    GLint alturaOriginal = header[14] + (header[15] << 8);
    GLubyte profundidade = header[16];

    GLint formato = (profundidade == 32) ? GL_RGBA : GL_RGB;

    GLubyte* dados = new GLubyte[larguraOriginal * alturaOriginal * (profundidade / 8)];
    fread(dados, sizeof(GLubyte), larguraOriginal * alturaOriginal * (profundidade / 8), file);

    fclose(file);

    // Redimensionar a imagem para o tamanho final (1024x1024)
    GLubyte* dadosRedimensionados = new GLubyte[larguraFinal * alturaFinal * (profundidade / 8)];

    // Realiza interpolação bilinear
    for (int y = 0; y < alturaFinal; ++y) {
        for (int x = 0; x < larguraFinal; ++x) {
            // Mapeia as coordenadas da imagem de destino para a imagem original
            float xOriginal = (float)x / larguraFinal * (larguraOriginal - 1);
            float yOriginal = (float)y / alturaFinal * (alturaOriginal - 1);

            int x0 = (int)xOriginal;
            int y0 = (int)yOriginal;
            int x1 = min(x0 + 1, larguraOriginal - 1);
            int y1 = min(y0 + 1, alturaOriginal - 1);

            // Interpolação bilinear
            for (int c = 0; c < (profundidade / 8); ++c) {
                float dx = xOriginal - x0;
                float dy = yOriginal - y0;

                // Cálculo dos valores dos quatro pixels
                GLubyte p00 = dados[(y0 * larguraOriginal + x0) * (profundidade / 8) + c];
                GLubyte p10 = dados[(y0 * larguraOriginal + x1) * (profundidade / 8) + c];
                GLubyte p01 = dados[(y1 * larguraOriginal + x0) * (profundidade / 8) + c];
                GLubyte p11 = dados[(y1 * larguraOriginal + x1) * (profundidade / 8) + c];

                // Interpolação linear em x
                float interpX0 = (1 - dx) * p00 + dx * p10;
                float interpX1 = (1 - dx) * p01 + dx * p11;

                // Interpolação linear em y
                float finalColor = (1 - dy) * interpX0 + dy * interpX1;

                dadosRedimensionados[(y * larguraFinal + x) * (profundidade / 8) + c] = (GLubyte)min(max(0.0f, finalColor), 255.0f);
            }
        }
    }

    // Gerar a textura
    GLuint idTextura;
    glGenTextures(1, &idTextura);
    glBindTexture(GL_TEXTURE_2D, idTextura);

    glTexImage2D(GL_TEXTURE_2D, 0, formato, larguraFinal, alturaFinal, 0, formato, GL_UNSIGNED_BYTE, dadosRedimensionados);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    delete[] dados;
    delete[] dadosRedimensionados;

    return idTextura;
}

void CarregarTexturaSkybox() {
    static bool texturaCarregada = false;

    if (texturaCarregada) {
        return;
    }

    // Carrega as texturas da skybox, redimensionando todas para 1024x1024
        textura_id_skybox[0] = CarregarTexturaTGA("right.tga");
        textura_id_skybox[1] = CarregarTexturaTGA("left.tga");
        textura_id_skybox[2] = CarregarTexturaTGA("back.tga");
        textura_id_skybox[3] = CarregarTexturaTGA("front.tga");
        textura_id_skybox[4] = CarregarTexturaTGA("top.tga");
        textura_id_skybox[5] = CarregarTexturaTGA("bottom.tga");

        texturaCarregada = true;
    }

    void CarregarTexturaBlocos() {
        static bool texturaCarregada = false;

        if (texturaCarregada) {
            return;
        }

        try {
            ifstream arq("meteoro.bmp", ios::binary);
            char c;
            if (!arq) cout << "Erro ao abrir arquivo";

            // Pular cabeçalho BMP
            for (int i = 0; i < 54; i++) c = arq.get();
            for (int i = 0; i < 256; i++) {
                for (int j = 0; j < 256; j++) {
                    c = arq.get(); bloco[i][j][2] = c;
                    c = arq.get(); bloco[i][j][1] = c;
                    c = arq.get(); bloco[i][j][0] = c;
                }
            }
            arq.close();

            glGenTextures(3, &textura_id_blocos);
            glBindTexture(GL_TEXTURE_2D, textura_id_blocos);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, bloco);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            texturaCarregada = true;
        } catch (const exception &e) {
            cout << "Erro ao ler o arquivo de textura dos blocos";
        }
    }

    GLuint CarregarTexturaSky(const char* arquivo) {
        ifstream arq(arquivo, ios::binary);
        if (!arq) {
            cout << "Erro ao abrir arquivo: " << arquivo << endl;
            return 0;
        }

        // Ignorar cabeçalho BMP (54 bytes)
        char header[54];
        arq.read(header, 54);

        // Obtenção das dimensões da imagem a partir do cabeçalho BMP
        int largura = *(int*)&header[18];
        int altura = *(int*)&header[22];
        int tamanhoImagem = 3 * largura * altura; // 3 bytes por pixel (RGB)

        // Leitura dos dados da imagem
        GLubyte* dados = new GLubyte[tamanhoImagem];
        arq.read(reinterpret_cast<char*>(dados), tamanhoImagem);
        arq.close();

        // Gerar a textura no OpenGL
        GLuint idTextura;
        glGenTextures(1, &idTextura);
        glBindTexture(GL_TEXTURE_2D, idTextura);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, largura, altura, 0, GL_RGB, GL_UNSIGNED_BYTE, dados);

        // Definir parâmetros de filtro e modo de empacotamento
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        delete[] dados;
        return idTextura;
    }

    void CarregarTexturas() {
        static bool texturaCarregada = false;

        if (texturaCarregada) {
            return;
        }

        // Carrega as texturas da skybox, redimensionando todas para 1024x1024
        textura_id_sky[0] = CarregarTexturaSky("right.bmp");
        textura_id_sky[1] = CarregarTexturaSky("left.bmp");
        textura_id_sky[2] = CarregarTexturaSky("back.bmp");
        textura_id_sky[3] = CarregarTexturaSky("front.bmp");
        textura_id_sky[4] = CarregarTexturaSky("top.bmp");
        textura_id_sky[5] = CarregarTexturaSky("bottom.bmp");

        texturaCarregada = true;
    }
};
