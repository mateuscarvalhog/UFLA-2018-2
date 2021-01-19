#include <iostream>
#include <string>
#include <stdexcept>
#include <queue>

class NohAVL; // declaraÃ§Ã£o avanÃ§ada

typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore

// DeclaraÃ§Ãµes das classes ===================================================

class NohAVL {
    friend class AVL;
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);
    private:
        NohAVL (TChave c, const TValor& v);
        ~NohAVL();
        NohAVL* ArrumarBalanceamento();
        void AtualizarAltura();
        void DesalocarFilhosRecursivo();
        int FatorBalanceamento();
        NohAVL* InserirRecursivo(NohAVL* ptNoh);
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

class AVL {
    public:
        AVL();
        ~AVL();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(TChave chave, const TValor& valor);
    private:
        NohAVL* mPtRaiz;
};

// === Classe NohAVL ==============================================================================

using namespace std;

NohAVL::NohAVL(TChave c, const TValor& v) {
    //#warning NohAVL::NohAVL nÃ£o implementado.
    mChave = c;
    mValor = v;
    mPtEsq = NULL;
    mPtDir = NULL;
    mPtPai = NULL;
    mAltura = 1;
}

NohAVL::~NohAVL() {
}

// Faz as rotaÃ§Ãµes e ajustes necessÃ¡rios inclusive do nÃ³ pai. Atualiza a altura.
// Retorna o nÃ³ que ficar na posiÃ§Ã£o dele apÃ³s os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {
    AtualizarAltura();
    int fatorBal = FatorBalanceamento();
    if(fatorBal > 1 and mPtEsq->FatorBalanceamento() >= 0){
        return RotacionarDireita();
    }
    if(fatorBal > 1 and mPtEsq->FatorBalanceamento() < 0) {
        mPtEsq = mPtEsq->RotacionarEsquerda();
        return RotacionarDireita();
    }
    if(fatorBal < -1 and mPtDir->FatorBalanceamento() <= 0) {
        return RotacionarEsquerda();
    }
    if(fatorBal < -1 and mPtDir->FatorBalanceamento() > 0) {
        mPtDir = mPtDir->RotacionarDireita();
        return RotacionarEsquerda();
    }
    
    return this;
}

// Calcula e atualiza a altura de um nÃ³.
void NohAVL::AtualizarAltura() {
    //#warning NohAVL::AtualizarAltura nÃ£o implementado.
    unsigned altEsq = 0;
    if(mPtEsq != NULL)
        altEsq = mPtEsq->mAltura;
    unsigned altDir = 0;
    if(mPtDir != NULL)
        altDir = mPtDir->mAltura;
    
    if(altEsq > altDir)
        mAltura = 1 + altEsq;
    else 
        mAltura = 1 + altDir;
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
    //#warning NohAVL::DesalocarFilhosRecursivo nÃ£o implementado.
    if(mPtEsq != NULL)
        mPtEsq->DesalocarFilhosRecursivo();
    if(mPtDir != NULL)
        mPtDir->DesalocarFilhosRecursivo();
    delete this;
}

// Calcula e retorna o fator de balanceamento do nÃ³.
int NohAVL::FatorBalanceamento() {
    unsigned altEsq = 0;
    if(mPtEsq != NULL)
        altEsq = mPtEsq->mAltura;
    unsigned altDir = 0;
    if(mPtDir != NULL)
        altDir = mPtDir->mAltura;
    
    return altEsq - altDir;
}

// Insere um nÃ³ numa subÃ¡rvore.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {
    if(ptNoh->mChave < mChave) {
        if(mPtEsq == NULL){
            mPtEsq = ptNoh;
            ptNoh->mPtPai = this;
        } else {
            mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
        }
    } else {
        if(mPtDir == NULL) {
            mPtDir = ptNoh;
            ptNoh->mPtPai = this;
        } else {
            mPtDir = mPtDir->InserirRecursivo(ptNoh);
        }
    }
    return ArrumarBalanceamento();
}

// Rotaciona a subÃ¡rvore Ã  direita. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarDireita() {
    NohAVL* aux = mPtEsq;
    mPtEsq = aux->mPtDir;
    if(aux->mPtDir != NULL)
        aux->mPtDir->mPtPai = this;
    aux->mPtPai = mPtPai;
    aux->mPtDir = this;
    mPtPai = aux;
    
    AtualizarAltura();
    aux->AtualizarAltura();
    
    return aux;
}

// Rotaciona a subÃ¡rvore Ã  esquerda. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarEsquerda() {
    NohAVL* aux = mPtDir;
    mPtDir = aux->mPtEsq;
    if(aux->mPtEsq != NULL)
        aux->mPtEsq->mPtPai = this;
    aux->mPtPai = mPtPai;
    aux->mPtEsq = this;
    mPtPai = aux;
    
    AtualizarAltura();
    aux->AtualizarAltura();
    
    return aux;
}

// Substitui um dos filhos por um novo nÃ³.
void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    //#warning NohAVL::TrocarFilho nÃ£o implementado.
    
}

// Escreve o conteÃºdo de um nÃ³ no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() {
    //#warning AVL::AVL nÃ£o implementado.
    mPtRaiz = NULL;
}

AVL::~AVL() {
    //#warning AVL::~AVL nÃ£o implementado.
    mPtRaiz->DesalocarFilhosRecursivo();
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel, na saÃ­da de dados informada.
// Usado para conferir se a estrutra da Ã¡rvore estÃ¡ correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nÃ³ especial para marcar fim de um nÃ­vel
    filhos.push(mPtRaiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        NohAVL* ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mPtEsq);
                filhos.push(ptNoh->mPtDir);
            }
        }
    }
    delete fimDeNivel;
}

// Insere um par chave/valor na Ã¡rvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
    //#warning AVL::Inserir nÃ£o implementado.
    NohAVL* novo = new NohAVL(chave, valor);
    if(mPtRaiz == NULL)
        mPtRaiz = novo;
    else
        mPtRaiz = mPtRaiz->InserirRecursivo(novo);
}

// === Programa ===================================================================================
int main() {
    AVL minhaArvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                minhaArvore.Inserir(chave, valor);
                break;
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                minhaArvore.EscreverNivelANivel(cout);
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
