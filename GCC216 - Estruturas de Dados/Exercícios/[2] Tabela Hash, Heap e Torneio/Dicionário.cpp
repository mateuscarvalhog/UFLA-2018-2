#include <iostream>
#include <string>

using namespace std;

unsigned funcaoHash(string palavra) {
    return (palavra.length() % 23);
}

class Noh {
    friend class tabelaHash;
    private:
        Noh* mProx = NULL;
        string mPalavra;
        string mSignificado;
    
    public:
        Noh(string p, string s) {
            mPalavra = p;
            mSignificado = s;
        }
};

class tabelaHash {
    private:
        unsigned mCap = 23;
        Noh** mElementos;
    
    public:
        tabelaHash();
        ~tabelaHash();
        void Inserir(string p, string s);
        string Buscar(string buscado);
};

tabelaHash::tabelaHash() {
    mElementos = new Noh*[mCap];
    for(unsigned i = 0; i < mCap; i++)
        mElementos[i] = NULL;
}

tabelaHash::~tabelaHash() {
    for (unsigned i = 0; i < mCap; i++) {
        Noh* aux = mElementos[i];
        while (aux != NULL) {
            Noh* ant = aux;
            aux = aux->mProx;
            delete ant;
        }
    }
    delete[] mElementos;
}

void tabelaHash::Inserir(string p, string s) {
    unsigned pos = funcaoHash(p);
    Noh* novo = mElementos[pos];
    if(mElementos[pos] == NULL)
        mElementos[pos] = new Noh(p, s);
    else {
        while(novo->mProx != NULL)
            novo = novo->mProx;
        novo->mProx = new Noh(p, s);
    }
}

string tabelaHash::Buscar(string buscado) {
    unsigned pos = funcaoHash(buscado);
    if (mElementos[pos] != NULL) {
        Noh* aux = mElementos[pos];
        while(aux->mProx != NULL and aux->mPalavra != buscado)
            aux = aux->mProx;
        if(aux->mPalavra == buscado)
            return aux->mSignificado;
    }
    return "NULL";    
}

int main() {
    tabelaHash th;
    unsigned qtd;
    cin >> qtd;
    string palavra, significado;
    for(unsigned i = 0; i < qtd; i++) {
        cin >> palavra;
        cin.ignore();
        getline(cin, significado);
        th.Inserir(palavra, significado);
    }
    string buscado;
    cin >> buscado;
    do {
        cout << "[" << buscado << "]" << " => " << th.Buscar(buscado) << endl;
        cin >> buscado;
    } while(buscado != "-1");
    
    return 0;
}
