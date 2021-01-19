#include <iostream>
#include <exception>

// controle de exceção não funciona;

using namespace std;

typedef int Dado;

class Noh {
    friend class ABB;
    private:
        Noh* mEsq;
        Noh* mDir;
        Noh* mPai;
        Dado mChave;
        
        Noh (Dado d);
};

Noh::Noh(Dado d)
    : mEsq(NULL), mDir(NULL), mPai(NULL), mChave(d) {};

class ABB {
    private:
        Noh* mRaiz;
    
    public:
        ABB() { mRaiz = NULL; }
        ~ABB();
        void InserirRecursivo(Dado chave);
        Noh* InserirRecAux(Noh* aux, Dado chave);
        void Inserir(Dado chave);
        Dado Minimo();
        Noh* MinimoAux(Noh* aux);
        Dado Maximo();
        Noh* MaximoAux(Noh*aux);
        Noh* Busca(Dado chave);
        void Transplanta(Noh* antigo, Noh* novo);
        void Remover(Dado chave);
        void PercorrerEmOrdem();
        void PercorrerEmOrdemAux(Noh* aux);
        void PercorrerPreOrdem();
        void PercorrerPreOrdemAux(Noh* aux);
        void PercorrerPosOrdem();
        void PercorrerPosOrdemAux(Noh* aux);
};

ABB::~ABB() {
    PercorrerPosOrdem();
}

void ABB::InserirRecursivo(Dado chave) {
    mRaiz = InserirRecAux(mRaiz, chave);
}

Noh* ABB::InserirRecAux(Noh* aux, Dado chave) {
    if(aux == NULL) {
        Noh* novo = new Noh(chave);
        return novo;
    } else {
        if(chave < aux->mChave) {
            aux->mEsq = InserirRecAux(aux->mEsq, chave);
            aux->mEsq->mPai = aux;
        } else {
            aux->mDir = InserirRecAux(aux->mDir, chave);
            aux->mDir->mPai = aux;
        }
    }
    return aux;
}

void ABB::Inserir(Dado chave) {
    if(mRaiz == NULL)
        mRaiz = new Noh(chave);
    else {
        Noh* novo = new Noh(chave);
        Noh* atual = mRaiz;
        Noh* anterior = mRaiz;
        while(atual != NULL) {
            if(atual->mChave > chave) {
                anterior = atual;
                atual = atual->mEsq;
            } else {
                anterior = atual;
                atual = atual->mDir;
            }
        }    
        novo->mPai = anterior;
        if(anterior->mChave > chave)
            anterior->mEsq = novo;
        else
            anterior->mDir = novo;
    }
}

Dado ABB::Minimo() {
    if(mRaiz == NULL)
        throw runtime_error("ABB VAZIA");
    else {
        Noh* nohMinimo = MinimoAux(mRaiz);
        return nohMinimo->mChave;
    }
}

Noh* ABB::MinimoAux(Noh* aux) {
    while(aux->mEsq != NULL)
        aux = aux->mEsq;
    return aux;
}

Dado ABB::Maximo() {
    if(mRaiz == NULL)
        throw runtime_error("ABB VAZIA");
    else {
        Noh* nohMaximo = MaximoAux(mRaiz);
        return nohMaximo->mChave;
    }
}

Noh* ABB::MaximoAux(Noh* aux) {
    while(aux->mDir != NULL)
        aux = aux->mDir;
    return aux;
}

Noh* ABB::Busca(Dado chave) {
    Noh* aux = mRaiz;
    while(aux != NULL) {
        if(aux->mChave == chave)
            return aux;
        else if(chave < aux->mChave)
            aux = aux->mEsq;
        else
            aux = aux->mDir;
    }
    return aux;
}

void ABB::Transplanta(Noh* antigo, Noh* novo) {
    if(mRaiz == antigo)
        mRaiz = novo;
    else if(antigo == antigo->mPai->mEsq)
        antigo->mPai->mEsq = novo;
    else
        antigo->mPai->mDir = novo;
    cout << antigo->mChave << " " << novo->mChave << " " << mRaiz->mChave <<endl;
    if(novo != NULL)
        novo->mPai = antigo->mPai;
}

void ABB::Remover(Dado chave) {
    Noh* removido = Busca(chave);
    if(removido == NULL)
        throw runtime_error("NAO ENCONTRADO");
    else {
        if(removido->mEsq == NULL)
            Transplanta(removido, removido->mDir);
        else if(removido->mDir == NULL)
            Transplanta(removido, removido->mEsq);
        else {
            Noh* sucessor = MinimoAux(removido->mDir);
            if(sucessor->mPai != removido) {
                Transplanta(sucessor, sucessor->mDir);
                sucessor->mDir = removido->mDir;
                sucessor->mDir->mPai = sucessor;
            }
            Transplanta(removido, sucessor);
            sucessor->mEsq = removido->mEsq;
            sucessor->mEsq->mPai = sucessor;
        }
    }
    delete removido;
}

void ABB::PercorrerEmOrdem() {
    PercorrerEmOrdemAux(mRaiz);
    cout << endl;
}

void ABB::PercorrerEmOrdemAux(Noh* aux) {
    if(aux != NULL) {
        PercorrerEmOrdemAux(aux->mEsq);
        cout << aux->mChave << " ";
        PercorrerEmOrdemAux(aux->mDir);
    }
}

void ABB::PercorrerPreOrdem() {
    PercorrerPreOrdemAux(mRaiz);
    cout << endl;
}

void ABB::PercorrerPreOrdemAux(Noh* aux) {
    if(aux != NULL) {
        cout << aux->mChave << " ";
        PercorrerPreOrdemAux(aux->mEsq);
        PercorrerPreOrdemAux(aux->mDir);
    }
}

void ABB::PercorrerPosOrdem() {
    PercorrerPosOrdemAux(mRaiz);
}

void ABB::PercorrerPosOrdemAux(Noh* aux) {
    if(aux != NULL) {
        PercorrerPosOrdemAux(aux->mEsq);
        PercorrerPosOrdemAux(aux->mDir);
        delete aux;
    }
}

int main() {
    ABB arvore;
    Dado chave;
    char operacao;
	cin >> operacao;
    do {
        try {
            switch (operacao) {
                case 'i': // Inserir
                    cin >> chave;
                    arvore.Inserir(chave);
                    break;
                case 'u': // Inserir Recursivo
                    cin >> chave;
                    arvore.InserirRecursivo(chave);
                    break;
                case 'm': // Minimo
                    cout << arvore.Minimo() << endl;
                    break;
                case 'n': // Maximo
                    cout << arvore.Maximo() << endl;
                    break;
                case 'r': // Remover
                    cin >> chave;
                    arvore.Remover(chave);
                    break;
                case 'o': 
                    arvore.PercorrerEmOrdem();
                    break;
                case 'p':  
                    arvore.PercorrerPreOrdem();
                    break;
                default:
                    cout << "Comando Inválido!";
            }
            cin >> operacao;
        } catch(exception& e) {
        }
    } while (operacao != 'f');
    
    return 0;
}
