#include <iostream>

using namespace std;

typedef int Dado;

class Noh {
    friend class AVL;
    private:
        Dado mChave;
        Noh* mEsq;
        Noh* mDir;
        Noh* mPai;
        unsigned mAlt;
    
    public:
        Noh(Dado d);
};

Noh::Noh(Dado d)
    :mChave(d), mEsq(NULL), mDir(NULL), mPai(NULL), mAlt(1) {}

class AVL {
    friend class Noh;
    private:
        Noh* mRaiz;
    
    public:
        AVL() { mRaiz = NULL; }
        ~AVL();
        
};

unsigned AVL::InfoAlt(Noh* aux) {
    if(aux == NULL)
        return 0;
    else
        return aux->mAlt;
}

void AtualizaAlt(Noh* aux) {
    unsigned altEsq = InfoAlt(aux->mEsq);
    unsigned altDir = InfoAlt(aux->mDir);
    aux->mAlt = 1 + Maior(altEsq, altDir);
}

unsigned AVL::Maior(unsigned a, unsigned b) {
    if(a > b)
        return a;
    else
        return b;
}

unsigned AVL::FatorBalanceamento(Noh* aux) {
    unsigned altEsq = InfoAlt(aux->mEsq);
    unsigned altDir = InfoAlt(aux->mDir);
    return altEsq - altDir;
}

Noh* AVL::RotacaoEsq(Noh* atual) {
    Noh* aux = atual->mDir;
    atual->mDir = aux->mEsq;
    if(aux->mEsq != NULL)
        aux->mEsq->mPai = atual;
    aux->mPai = atual->mPai;
    if(atual == mRaiz)
        mRaiz = aux;
    else if(atual == atual->mPai->mEsq)
        atual->mPai->mEsq = aux;
    else
        atual->mPai->mDir = aux;
    aux->mEsq = atual;
    atual->mPai = aux;
    AtualizaAlt(aux);
    AtualizaAlt(atual);
    return aux;
}

Noh* AVL::RotacaoDir(Noh* atual) {
    Noh* aux = atual->mEsq;
    atual->mEsq = aux->mDir;
    if(aux->mDir != NULL)
        aux->mDir->mPai = atual;
    aux->mPai = atual->mPai;
    if(atual == mRaiz)
        mRaiz = aux;
    else if(atual == atual->mPai->mEsq)
        atual->mPai->mEsq = aux;
    else
        atual->mPai->mDir = aux;
    aux->direito = atual;
    atual->pai = aux;
    AtualizaAlt(aux);
    AtualizaAlt(atual);
    return aux;
}

void AVL::RotacaoEsqDir(atual) {
    atual->mEsq = RotacaoEsq(atual->mEsq);
    return RotacaoDir(atual);
}

void AVL::RotacaoDirEsq(atual) {
    atual->mDir = RotacaoDir(atual->mDir);
    return RotacaoEsq(atual);
}

void AVL::InserirRecursivo(Dado chave) {
    mRaiz = InserirRecAux(mRaiz, chave);
}

Noh* AVL::InserirRecAux(Noh* aux, Dado chave) {
    if(aux == NULL) {
        Noh* novo = new Noh(chave)
        return novo;
    } else {
        if(chave < aux->mChave) {
            InserirRecAux(aux->mEsq, chave);
            aux->mEsq->mPai = aux
        } else {
            InserirRecAux(aux->mDir, chave);
            aux->mDir->mPai = aux;
        }
    }
    return ArrumarBalanceamento(aux);
}

Noh* AVL::ArrumarBalanceamento(aux) {
    AtualizaAlt(aux);
    int fatorBal = FatorBalanceamento(aux);
    if(fatorBal >= -1 and fatorBal <= 1)
        return aux;
    else {
        if(fatorBal > 1 and FatorBalanceamento(aux->mEsq) >= 0)
            return RotacaoDir(aux);
        if(fatorBal > 1 and FatorBalanceamento(aux->mEsq) < 0)
            return RotacaoEsqDir(aux);
        if(fatorBal < -1 and FatorBalanceamento(aux->mDir) <= 0)
            return RotacaoEsq(aux);
        return RotacaoDirEsq(aux);
    }
}

void AVL::RemoverRecursivo(Dado chave) {
    mRaiz = RemoverRecAux(mRaiz, chave);
}

Noh* AVL::RemoverRecAux(Noh* aux, Dado chave) {
    if(aux == NULL) {
        throw runtime_error("NAO ENCONTRAD0");
    else {
        if(chave < aux->mChave)
            aux->mEsq = RemoverRecAux(aux->mEsq, chave);
        else if(chave > aux->mChave)
            aux->mDir = RemoverRecAux(aux->mDir, chave);
        else {
            if(aux->mEsq == NULL) {
                Noh* temp = aux->mDir;
                Transplanta(aux, aux->mDir);
            } else if(aux->mDir == NULL) {
                Noh* temp = aux->mEsq;
                Transplanta(aux, aux->mEsq);
            } else {
                Noh* sucessor = Minimo(aux->mDir);
                Noh* temp = sucessor;
                if(sucessor->mPai != aux) {
                    Transplanta(sucessor, sucessor->mDir);
                    Noh* auxPai = sucessor->mPai;
                    while(auxPai != aux->mDir) {
                        AtualizaAlt(auxPai);
                        auxPai = auxPai->mPai;
                    }
                    sucessor->mDir = ArrumarBalanceamento(aux->mDir);
                    sucessor->mDir->mPai = sucessor;
                }
                Transplanta(aux, sucessor);
                sucessor->mEsq = aux->mEsq;
                sucessor->mEsq->mPai = sucessor;
            }
            delete aux;
            if(temp == NULL) {
                return temp;
            } else {
                return ArrumarBalanceamento(temp);
            }
        }
    }
}
