#include <iostream>
#include <stdexcept>
#include <queue>

typedef std::string TValor;
typedef unsigned TChave;
enum Cor {PRETO, VERMELHO};

class Noh {
    friend class ArvoreRN;
    friend std::ostream& operator<<(std::ostream& saida, Noh* ptNoh);
    public:
        Noh(unsigned chave, TValor valor);
    private:
        Noh* Tio();
        void DesalocarRecursivo();
        TValor mValor;
        TChave mChave;
        Cor mCor;
        Noh* mEsq;
        Noh* mDir;
        Noh* mPai;
};

class ArvoreRN {
    public:
        ArvoreRN();
        ~ArvoreRN();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(unsigned chave, TValor valor);
    private:
        void ArrumarInsercao(Noh* umNoh);
        void RotacionarDireita(Noh* umNoh);
        void RotacionarEsquerda(Noh* umNoh);
        Noh* mRaiz;
};

using namespace std;

// === classe Noh ==============================================================
Noh::Noh(TChave chave, TValor valor)
    : mValor(valor), mChave(chave), mCor(VERMELHO), mEsq(NULL), mDir(NULL), mPai(NULL) {
}

// Desaloca as subÃ¡rvores e depois a si prÃ³prio.
void Noh::DesalocarRecursivo() {
    //#warning Noh::DesalocarRecursivo nÃ£o foi implementado.
    if(mEsq != NULL)
        mEsq->DesalocarRecursivo();
    if(mDir != NULL)
        mDir->DesalocarRecursivo();
    delete this;
}

// Retorna o tio do nÃ³, ou NULL se nÃ£o houver tio
Noh* Noh::Tio() {
    if(mPai->mPai != NULL) {
        if(mPai == mPai->mPai->mEsq)
            return mPai->mPai->mDir;
        else
            return mPai->mPai->mEsq;
    } else
        return NULL;
}

// === classe ArvoreRN =========================================================
ArvoreRN::ArvoreRN() : mRaiz(NULL) {
}

ArvoreRN::~ArvoreRN() {
    //#warning ArvoreRN::~ArvoreRN nÃ£o foi implementado.
    mRaiz->DesalocarRecursivo();
}

// insere uma chave/valor na Ã¡rvore,
// chama ArrumarInsercao para corrigir eventuais problemas
void ArvoreRN::Inserir(unsigned chave, TValor valor) {
    //#warning ArvoreRN::Inserir nÃ£o foi implementado.
    Noh* novo = new Noh(chave, valor);
    if(mRaiz == NULL) {
        mRaiz = novo;
        mRaiz->mCor = PRETO;
    } else {
        Noh* ant = mRaiz;
        Noh* atual = mRaiz;
        while(atual != NULL) {
            ant = atual;
            if(novo->mChave < atual->mChave)
                atual = atual->mEsq;
            else
                atual = atual->mDir;
        }
        novo->mPai = ant;
        novo->mCor = VERMELHO;
        if(ant->mChave < novo->mChave)
            ant->mDir = novo;
        else
            ant->mEsq = novo;
        
        ArrumarInsercao(novo);
    }
}

// procedimento para manter balanceamento apÃ³s inserÃ§Ã£o
void ArvoreRN::ArrumarInsercao(Noh* umNoh) {
    //#warning ArvoreRN::ArrumarInsercao nÃ£o foi implementado.
    Noh* tio;
    while(umNoh != mRaiz and umNoh->mPai->mCor == VERMELHO) {
        tio = umNoh->Tio();
        if(tio != NULL and tio->mCor == VERMELHO) {
            umNoh->mPai->mCor = PRETO;
            umNoh->mPai->mPai->mCor = VERMELHO;
            tio->mCor = PRETO;
            umNoh = umNoh->mPai->mPai;
        } else {
            if(umNoh == umNoh->mPai->mEsq and umNoh->mPai == umNoh->mPai->mPai->mEsq) {
                tio->mCor = PRETO;
                umNoh->mPai->mCor = PRETO;
                umNoh->mPai->mPai->mCor = VERMELHO;
                RotacionarDireita(umNoh->mPai->mPai);
                continue;
            }
            if(umNoh->mPai == umNoh->mPai->mPai->mEsq and umNoh == umNoh->mPai->mDir) {
                umNoh = umNoh->mPai;
                RotacionarEsquerda(umNoh);
                umNoh->mPai->mCor = PRETO;
                umNoh->mPai->mPai->mCor = VERMELHO;
                RotacionarDireita(umNoh->mPai->mPai);
                continue;
            }
            if ((umNoh->mPai == umNoh->mPai->mPai->mDir) and (umNoh == umNoh->mPai->mDir)) {
                umNoh->mPai->mCor = PRETO;
                umNoh->mPai->mPai->mCor = VERMELHO;
                RotacionarEsquerda(umNoh->mPai->mPai);
                continue;
            }
            // CASO 4 : Rotação Direita-Esquerda (RL)
            if ((umNoh->mPai = umNoh->mPai->mPai->mDir) and (umNoh == umNoh->mPai->mEsq)) {
                umNoh = umNoh->mPai;
                RotacionarDireita(umNoh);
                umNoh->mPai->mCor = PRETO;
                umNoh->mPai->mPai->mCor = VERMELHO;
                RotacionarEsquerda(umNoh->mPai->mPai);
                continue;
            }
        }
    }
    mRaiz->mCor = PRETO;
}

// rotaÃ§Ã£o Ã  esquerda, muda a raiz se necessÃ¡rio
void ArvoreRN::RotacionarEsquerda(Noh* umNoh) {
    //#warning ArvoreRN::RotacionarEsquerda nÃ£o foi implementado.
    Noh* aux = umNoh->mDir;
    umNoh->mDir = aux->mEsq;
    if(aux->mEsq != NULL)
        aux->mEsq->mPai = umNoh;
    aux->mPai = umNoh->mPai;
    if(umNoh == mRaiz)
        mRaiz = umNoh;
    else if(umNoh == umNoh->mPai->mEsq)
        umNoh->mPai->mEsq = aux;
    else
        umNoh->mPai->mDir = aux;
    aux->mEsq = umNoh;
    umNoh->mPai = aux;
}

// rotaÃ§Ã£o Ã  direita, muda a raiz se necessÃ¡rio
void ArvoreRN::RotacionarDireita(Noh* umNoh) {
    //#warning ArvoreRN::RotacionarDireita nÃ£o foi implementado.
    Noh* aux = umNoh->mEsq;
    umNoh->mEsq = aux->mDir;
    if(aux->mDir != NULL)
        aux->mDir->mPai = umNoh;
    aux->mPai = umNoh->mPai;
    if(umNoh == mRaiz)
        mRaiz = aux;
    else if(umNoh == umNoh->mPai->mEsq)
        umNoh->mPai->mEsq = aux;
    else
        umNoh->mPai->mDir = aux;
    aux->mDir = umNoh;
    umNoh->mPai = aux;
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel
void ArvoreRN::EscreverNivelANivel(ostream& saida) {
    queue<Noh*> filhos;
    filhos.push(mRaiz);
    while (not filhos.empty()) {
        unsigned nroNohsNesteNivel = unsigned(filhos.size());
        for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
            Noh* ptNoh = filhos.front();
            filhos.pop();
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mEsq);
                filhos.push(ptNoh->mDir);
            }
        }
        cout << "\n";
    }
}

// Escreve um nÃ³ (ou NIL).
ostream& operator<<(ostream& saida, Noh* ptNoh) {
    static char vetLetrasCores[] = "PV";
    if (ptNoh == NULL)
        saida << "NIL";
    else
        saida << '[' << vetLetrasCores[ptNoh->mCor] << ':' << ptNoh->mChave << '/'
              << ptNoh->mValor << ']';
    return saida;
}

int main() {
    ArvoreRN arvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                arvore.Inserir(chave, valor);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                arvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
}
