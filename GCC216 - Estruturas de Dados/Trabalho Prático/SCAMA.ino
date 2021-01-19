/* 
  Trabalho de Estrutura de Dados
  Sistema de Cadastro de Músicas Autorais 
  Copyright 2018 by Igor Antônio dos Santos Alves Mendes, Mateus Carvalho Gonçalves, Pedro Antônio de Souza
*/

#include <Keypad.h> //Biblioteca para utilização do teclado matricial
#include <LiquidCrystal.h> //Biblioteca para utilização do display LCD

const int PIN_AUTO = 11; //PINO DO AUTOFALANTE

/* ************************************************* *
 * ************************************************* *
 * ************     MUSICA (LISTA)     ************* *
 * ************************************************* *
 * ************************************************* */

//nohMusica REPRESENTA AS NOTAS DE UMA MUSICA, EH O NOH DA LISTA (Musica)
class nohMusica {
    friend class Musica;
    private:
        int nota;
        int tempo;
        nohMusica* ANTERIOR;
        nohMusica* PROXIMO;
    public:
        nohMusica(int n, int t);
};

nohMusica::nohMusica (int n, int t) : nota(n), tempo(t) {
    ANTERIOR = NULL;
    PROXIMO = NULL;
}

//LISTA QUE REPRESENTA A MUSICA
class Musica {
    public:
        Musica();
        ~Musica();
        void insereNota(int n, int t);
        void tocarMusica();
        void deletaMusica();
    private:
        nohMusica* PRIMEIRO;
        nohMusica* ULTIMO;
        int SIZE;
        int freqNotas[8] = {262,294,330,349,392,440,494,523}; //FREQUENCIAS DE ONDAS QUADRADAS REFERENTES AS NOTAS
                        //  DO  RE  MI  FA  SOL LA  SI  DO
};

Musica::Musica() {
    PRIMEIRO = NULL;
    ULTIMO = NULL;
    SIZE = 0;
}

Musica::~Musica() {
    deletaMusica();
}

void Musica::deletaMusica() {
    nohMusica* iter = this->PRIMEIRO;
    nohMusica* prox = NULL;
    
    while (iter != NULL) {
        prox = iter->PROXIMO;
        delete iter;
        iter = prox;
        SIZE--;
    }
}

void Musica::insereNota(int n, int t) {
    nohMusica* novo = new nohMusica(n, t);

    if (SIZE == 0) {
        PRIMEIRO = novo;
        ULTIMO = novo;
    }
    else {
        novo->ANTERIOR = ULTIMO;
        ULTIMO->PROXIMO = novo;
        ULTIMO = novo;
    }

    SIZE++;
}

void Musica::tocarMusica() {
    if (SIZE > 0) {
        nohMusica* atual = PRIMEIRO;
        while (atual != NULL) {
            if (atual->nota == 0) { noTone(PIN_AUTO); }
            else { tone(PIN_AUTO, freqNotas[(atual->nota)-1], atual->tempo); }
              
            delay(atual->tempo);
            
            atual = atual->PROXIMO;
        }
    }
}


/* ************************************************* *
 * ************************************************* *
 * **************     COMPOSICAO     *************** *
 * ************************************************* *
 * ************************************************* */

//NOH DA TABELA HASH (Cadastro)
class Composicao {
    friend class Cadastro;
    private:
        int codComp;
        int codMus;
        Musica musica;
    public:
        Composicao (int c, int m) : codComp(c), codMus(m) {}
};

//TABELA HASH COM TRATAMENTO DE COLISAO POR ENCADEAMENTO ABERTO
class Cadastro {
    private:
        Composicao** COMPOSICOES;
        Composicao* REMOVIDO;
        int CAPACIDADE;
        int SIZE;
        int HASH(int c, int m);
        Composicao* buscaAux(int c, int m);
    public:
        Cadastro (int cap = 100);
        ~Cadastro();
        int insereComposicao(int c, int m);
        bool excluiComposicao(int c, int m);
        bool tocarComposicao(int c, int m);
        bool insereNotaComposicao(int c, int m, int nota, int tempo);
        int busca(int c, int m);
};

Cadastro::Cadastro (int cap) {
    SIZE = 0;
    CAPACIDADE = cap;
    COMPOSICOES = new Composicao*[CAPACIDADE];
    
    for (int i=0; i<CAPACIDADE; i++) {
        COMPOSICOES[i] = NULL;
    }
}

Cadastro::~Cadastro() {
    for (int i=0; i < CAPACIDADE; i++) {
        if(COMPOSICOES[i] != REMOVIDO) {
            if(COMPOSICOES[i] != NULL) {
                COMPOSICOES[i]->musica.deletaMusica();
            }
            delete COMPOSICOES[i];
        }
    }
    delete[] COMPOSICOES;
    delete REMOVIDO;
}

int Cadastro::HASH (int codComp, int codMus) {
    const int UMPRIMO = 421;
    return ((UMPRIMO*codComp*codMus)/(codComp + codMus + UMPRIMO))%CAPACIDADE;
}

Composicao* Cadastro::buscaAux (int c, int m) {
  int h = HASH(c, m);
  Composicao* r = NULL;
  
   if (COMPOSICOES[h] != NULL and COMPOSICOES[h] != REMOVIDO) {
    if (COMPOSICOES[h]->codComp == c and COMPOSICOES[h]->codMus == m) {
      r = COMPOSICOES[h];
    }
    else {
      int cont = 0;
      bool encontrado = false;
      ++h;
      while (!encontrado and COMPOSICOES[h]!=NULL and cont<CAPACIDADE) {
        if (h==CAPACIDADE) { h = 0; }
        
        if (COMPOSICOES[h] != REMOVIDO) {
          if (COMPOSICOES[h]->codMus == m and COMPOSICOES[h]->codComp == c) {
            encontrado = true;
            r = COMPOSICOES[h];
          }
        }
        
        cont++;
        ++h;
      }
    }
   }

   return r;
}

int Cadastro::busca (int c, int m) {
    int h = HASH(c, m);
    int p=-1, cont=0;
    
    if (COMPOSICOES[h] != NULL) {
        while (COMPOSICOES[h] == REMOVIDO or ((COMPOSICOES[h] != NULL) and ((COMPOSICOES[h]->codComp != c) or (COMPOSICOES[h]->codMus != m)) and (cont < CAPACIDADE))) {
            h++;
            cont++;
            if (h == CAPACIDADE) h = 0;
        }

        if (cont < CAPACIDADE) p = h;
    }

    return p;
}

int Cadastro::insereComposicao (int c, int m) {
    if (SIZE < CAPACIDADE) {    
        int h = HASH(c, m);
        Composicao* novo = new Composicao(c, m);
        
        if (busca(c, m) != -1) { //COMPOSICAO J CADASTRADA
            return 0;
        }
        else {
            int cont = 0;
            while (COMPOSICOES[h] != NULL and COMPOSICOES[h] != REMOVIDO and cont < CAPACIDADE) {
                h++;
                if (h == CAPACIDADE) h = 0;
                cont++;
            }
            COMPOSICOES[h] = novo;

            return 1; //INSERIDO COM SUCESSO
        }
    }
    else {
        return 2; //CAPACIDADE ATINGIDA
    }

    return -1; //NAO DEVE CHEGAR AQUI
}

bool Cadastro::insereNotaComposicao (int c, int m, int nota, int tempo) {
  Composicao* ptComp = buscaAux(c, m);
  if (ptComp != NULL) {
    ptComp->musica.insereNota(nota, tempo);
    return true;
  }

  return false;
}

bool Cadastro::excluiComposicao (int c, int m) {
    int h = busca(c, m);

    if (h == -1)
        return false; //COMPOSICAO INEXISTENTE
    else {
        delete COMPOSICOES[h];
        COMPOSICOES[h] = REMOVIDO;
        return true;
    }
}

bool Cadastro::tocarComposicao (int c, int m) {
    int h = busca(c, m);

    if (h == -1)
        return false; //COMPOSICAO INEXISTENTE
    else {
        COMPOSICOES[h]->musica.tocarMusica();
        return true;
    }
}


/* ************************************************* *
 * ************************************************* *
 * ************      DISPLAY LCD       ************* *
 * ************************************************* *
 * ************************************************* */
 
LiquidCrystal lcd(A5,A4,A3,A2,A1,A0);


/* ************************************************* *
 * ************************************************* *
 * ************      TECLADO 4X4       ************* *
 * ************************************************* *
 * ************************************************* */

const byte LINHAS = 4;
const byte COLUNAS = 4;

char SIMBOLOS[LINHAS][COLUNAS] = {{'1', '2', '3', 'A'},  //A->INSERE
                                  {'4', '5', '6', 'B'},  //B->EXCLUI
                                  {'7', '8', '9', 'C'},  //C->REPRODUZ
                                  {'c', '0', 's', 'o'}}; //c->CLEAR s->SPACE o->OK

byte PINOS_LINHA[LINHAS] = {2,3,4,5}; 
byte PINOS_COLUNA[COLUNAS] = {6,7,8,9};

Keypad teclado = Keypad(makeKeymap(SIMBOLOS), PINOS_LINHA, PINOS_COLUNA, LINHAS, COLUNAS);


//FUNCAO PARA ESCREVER CENTRALIZADO NO DISPLAY
void escreveCentral(String lin0, String lin1="", int som=-1, bool pisca=true, int d=1000) {
    int n;
    

    lcd.clear();
    lcd.noCursor();
    
    n = (16-lin0.length())/2;
    lcd.setCursor(n, 0);
    lcd.print(lin0);
    
    n = (16-lin1.length())/2;
    lcd.setCursor(n, 1);
    lcd.print(lin1);

    if (pisca) {
        for(int i=0; i<5; i++) {
            delay(100);
            noTone(PIN_AUTO);
            lcd.noDisplay();
            delay(100);
            lcd.display();
            if(som==0) tone(PIN_AUTO, 2500, 100);
            else if(som==1) tone(PIN_AUTO, 80, 100);
        }
    }
    else delay(d);
}

//EXIBE TELA DE OPCAO
void telaDigiteOpcao() {
    escreveCentral("ESCOLHA UMA", "OPCAO NO TECLADO", -1, false);
}

//EXIBE TELA DE ALERTA PARA SELECAO DE OPCAO INVLIDA 
void telaInvalido() {
    escreveCentral("OPCAO", "INVALIDA", 1);
}

//LIMPA UMA LINHA DE CARACTERES DO DISPLAY
void limpaLinhaDisplay(int line) {
    lcd.setCursor(0,line);
    lcd.print("                ");
}

//EXIBE TELA PARA DAR ENTRADA NOS CODIGOS DO COMPOSITOR E DA MESICA
void telaCompMusi(int &c, int &m){
  String comp = "";
  String musi = "";
  char dig = NO_KEY;
  int contAlg = 0; //CONTADOR DE ALGARISMOS
  
  lcd.clear();
  lcd.print("Compositor:");
  lcd.setCursor(0,1); //POSICIONA CURSOR NA LINHA DE BAIXO PARA PROXIMA IMPRESSAO
  lcd.print("Musica:");
  lcd.setCursor(12,0); //REPOSICIONA CURSOR PARA ESCRITA
  lcd.cursor();
  
  while (dig != 'o') {
    dig = teclado.getKey();
    delay(10);
    if (dig) {
      if (dig=='o' and comp.toInt()==0) dig = 'c'; //LIMPA VALOR QUANDO CODIGO E INVLIDO (0)
      
      if (dig=='o' and comp=="") { //APERTOU OK SEM CODIGO INSERIDO
        dig = NO_KEY;
      }
      else if (dig!='c' and dig!='s' and dig!='o' and dig!='A' and dig!='B' and dig!='C') { //SE FOR DIGITADO UM NEMERO
        if (contAlg < 3) {
          contAlg++;
          comp += dig;
          lcd.leftToRight();
          lcd.print(dig);
        }
        else { //LIMITA O CODIGO EM 3 ALGARISMOS, REESCREVENDO O ULTIMO DIGITO
          comp[2] = dig;
          lcd.leftToRight();
          lcd.print(dig);
        }

        if (contAlg == 3) { //RETORNA 0 CURSOR PARA POSICAO DO ELTIMO DIGITO
          lcd.rightToLeft();
          lcd.print(' ');
        }
      }
      else if (dig=='c') { //LIMPA O CODIGO DIGITADO
        lcd.leftToRight();
        comp = "";
        contAlg = 0;
        lcd.setCursor(12,0);
        lcd.print("    ");
        lcd.setCursor(12,0);
      }
    }
  }
  
  lcd.setCursor(8,1);
  dig = NO_KEY;
  contAlg = 0;
  while (dig != 'o') {
    dig = teclado.getKey();
    delay(10);
    if (dig) {
      if (dig=='o' and musi.toInt()==0) dig = 'c';
      
      if (dig=='o' and musi=="") {
        dig = NO_KEY;
      }
      else if (dig!='c' and dig!='s' and dig!='o' and dig!='A' and dig!='B' and dig!='C') {
        if (contAlg < 3) {
          contAlg++;
          musi += dig;
          lcd.leftToRight();
          lcd.print(dig);
        }
        else {
          musi[2] = dig;
          lcd.leftToRight();
          lcd.print(dig);
        }

        if (contAlg == 3) {
          lcd.rightToLeft();
          lcd.print(' ');
        }
      }
      else if (dig=='c') {
        lcd.leftToRight();
        musi = "";
        contAlg = 0;
        lcd.setCursor(8,1);
        lcd.print("        ");
        lcd.setCursor(8,1);
      }
    }
  }

  c = comp.toInt();
  m = musi.toInt();
}

void telaIniciarSistema() {
    // VETORES PARA CRIACAO DOS NOVOS CARACTERES QUE FORMAM O LOGOTIPO
    byte s[4][8] = {{B00000,B00111,B01111,B11110,B11100,B11100,B11110,B01111}, 
                    {B00000,B11111,B11110,B00000,B00000,B00000,B00000,B11100},
                    {B00111,B00000,B00000,B00000,B00000,B01111,B11111,B00000},
                    {B11110,B01111,B00111,B00111,B01111,B11110,B11100,B00000}};
                    

    //CRIA NOVOS CARACTERES PARA COMPOSICAO DO LOGOTIPO
    lcd.createChar(0, s[0]);
    lcd.createChar(1, s[1]);
    lcd.createChar(2, s[2]);
    lcd.createChar(3, s[3]);
  
    lcd.begin(16,2);

    lcd.setCursor(4,0); //COMECA CENTRALIZADO
    lcd.write(byte(0));
    lcd.write(byte(1));
    lcd.setCursor(4,1);
    lcd.write(byte(2));
    lcd.write(byte(3));
    lcd.print(" SCAMA");

    //TOCA MUSICA INICIAL
    for (int i = 20; i>=0; i--) {
        tone(PIN_AUTO, i*100, 50);
        delay(100);
    }
    
    for(int i=0; i<3; i++) { //PISCA
        delay(200);
        lcd.noDisplay();
        delay(200);
        lcd.display();
    }

    delay(1200);
    lcd.clear();
}

Cadastro SCAMA(100);

void setup()
{
  pinMode(PIN_AUTO, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);

  telaIniciarSistema();
}


void loop()
{
  int c, m; // c = CODIGO DO COMPOSITOR E m = CODIGO DA MUSICA
  
  lcd.clear();
  telaDigiteOpcao();

  // ESPERA O USUARIO TECLAR PELA PRIMEIRA VEZ
  char op = NO_KEY; //OPCAO DE ESCOLHA DO USUARIO
  while (op == NO_KEY) {
    op = teclado.getKey();
  }

  // TRATA A OPCAO DIGITADA ATE QUE O USUARIO DIGITE UMA OPCAO VALIDA
  while ((op != 'A') and (op != 'B') and (op != 'C')) {
    op = NO_KEY;
    telaInvalido();
    telaDigiteOpcao();
    while (op == NO_KEY) {
      op = teclado.getKey();
    }
  }
  
  if (op == 'A') { //OPCAO DE INSERCAO
    escreveCentral("INSERIR", "COMPOSICAO", -1, false);
    
    telaCompMusi(c, m);
    
    int insere = SCAMA.insereComposicao(c, m);
    
    if (insere == 1) {   //INSERIDO COM SUCESSO       
        lcd.clear();
        lcd.print("Nota/tempo(ms):");
        lcd.setCursor(0, 1);
        lcd.cursor();
        
        char t, n; //RECEBE DIGITO A DIGITO
        String nota="", tempo=""; //CONCATENACAO DOS DIGITOS
        bool fimInsercao=false, jaInserido=false, r;


        //RECEBE NOTAS
        n = NO_KEY;
        while (!fimInsercao) {            
            n = teclado.getKey();

            if (n != NO_KEY) {
                if (n=='c' or n=='s' or n=='A' or n=='B' or n=='C' or n=='9') n = NO_KEY; //CARACTERES INVLIDOS
                else if (n=='o') {
                    if (jaInserido) {
                        fimInsercao = true;
                        escreveCentral("INSERIDO", "COM SUCESSO", 0);
                    }
                    else n = NO_KEY;
                }
                else {
                    nota = n;
                    n = NO_KEY;
                    lcd.print(nota);
                    lcd.print('/');

                    t = NO_KEY;
                    while(t!='o') {
                        t = teclado.getKey();

                        if (t != NO_KEY) {                          
                            if (t=='c') {
                                limpaLinhaDisplay(1);
                                lcd.setCursor(0,1);
                                nota="";
                                tempo="";
                                t = 'o';
                            }

                            else if (t!='s' and t!='A' and t!='B' and t!='C' and t!='o') { //SE NAO FOR CARACTER INVLIDO
                                if (t=='0' and tempo=="") t = NO_KEY; //PRIMEIRO ALGARISMO DO TEMPO NAO PODE SER ZERO
                                else {
                                    lcd.print(t);
                                    tempo += t;
                                    t = NO_KEY;
                                }
                            }

                            else if (t=='o' and tempo=="") t=NO_KEY; //NAO PODE DAR OK COM TEMPO VAZIO

                            else if (t=='o' and tempo!="") {
                                r = SCAMA.insereNotaComposicao(c, m, nota.toInt(), tempo.toInt());
                                if (r) {
                                    nota = "";
                                    tempo = "";
                                    jaInserido = true;
                                    escreveCentral("NOTA INSERIDA", "COM SUCESSO", -1, false, 700);
                                    
                                    lcd.clear();
                                    lcd.print("Nota/tempo(ms):");
                                    lcd.setCursor(0, 1);
                                    lcd.cursor();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if (insere == 2) { //CAPACIDADE ATINGIDA 
        escreveCentral("CAPACIDADE", "ATINGIDA", 1);
    }
    else if (insere == 0) { //COMPOSICAO JA CADASTRADA
        escreveCentral("COMPOSICAO", "JA CADASTRADA", 1);
    }
    else { //OCORREU ERRO
        escreveCentral("ERRO AO", "TENTAR INSERIR", 1);
    }
    
  }
  else if (op == 'B') { //OPCAO DE EXCLUSAO
    escreveCentral("EXCLUIR", "COMPOSICAO", -1, false);
    
    telaCompMusi(c, m);
    
    bool excluido = SCAMA.excluiComposicao(c,m);

    if (excluido) {
        escreveCentral("EXCLUIDA", "COM SUCESSO", 0);
    }
    else {
        escreveCentral("COMPOSICAO", "INEXISTENTE", 1);
    }
    
  }
  else if (op == 'C') {
    escreveCentral("TOCAR", "COMPOSICAO", -1, false);
    
    telaCompMusi(c, m);

    bool toca = SCAMA.tocarComposicao(c, m);
    if (!toca) {
        escreveCentral("COMPOSICAO", "INEXISTENTE", 1);
    }
  }
}
