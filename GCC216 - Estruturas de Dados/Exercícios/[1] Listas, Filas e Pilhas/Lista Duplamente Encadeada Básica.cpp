#include <iostream>
using namespace std;

class Aluno {
    friend class Lista;
    private:
        Aluno* mPtProx;
        Aluno* mPtAnt;
        string mNome;
        unsigned mMatr;
    
    public:
        Aluno(string nome, unsigned matr);
};

Aluno::Aluno(string nome, unsigned matr) {
    mPtProx = NULL;
    mPtAnt = NULL;
    mNome = nome;
    mMatr = matr;
}

class Lista {
    private:
        Aluno* mPrim;
        Aluno* mUlt;
        int mTam;
    
    public:
        Lista();
        ~Lista();
        bool MatrExiste(unsigned matr);
        void InserirNoInicio(string n, unsigned matr);
        void Remover(unsigned matr);
        void ImprimirDireto();
        void ImprimirReverso();
};

Lista::Lista() {
    mPrim = NULL;
    mUlt = NULL;
    mTam = 0;
}

Lista::~Lista() {
    Aluno* aux = mPrim;
	while (aux != NULL) {
		Aluno* aux2 = aux;
		aux = aux->mPtProx;
        delete aux2;
		aux2 = aux;
	}
	delete aux;
	mPrim = NULL;
	mUlt = NULL;
}

bool Lista::MatrExiste (unsigned matr) {
    Aluno* aux = mPrim;
    while(aux != NULL) {
        if(aux->mMatr == matr) {
            return true;
        }
        aux = aux->mPtProx;
    }
    return false;
}

void Lista::InserirNoInicio(string nome, unsigned matr) {
    Aluno* novo = new Aluno(nome, matr);
    if(mPrim == NULL) {
        mPrim = novo;
        mUlt = novo;
    } else {
        novo->mPtProx = mPrim;
        mPrim->mPtAnt = novo;
        mPrim = novo;
    }
    mTam++;
}

void Lista::Remover(unsigned matr) {
    Aluno* aux = mPrim;
    if(MatrExiste(matr)){
        while(aux->mMatr != matr) {
            aux = aux->mPtProx;
        }
        if(aux == mPrim and aux == mUlt) {
            mPrim = NULL;
            mUlt = NULL;
        } else if(aux == mPrim) {
            mPrim = aux->mPtProx;
            mPrim->mPtAnt = NULL;
        } else if(aux == mUlt) {
            mUlt = aux->mPtAnt;
            mUlt->mPtProx = NULL;
        } else {
            aux->mPtProx->mPtAnt = aux->mPtAnt;
            aux->mPtAnt->mPtProx = aux->mPtProx;
        }
        delete aux;
        mTam--;
    } else {
        cout << "ERRO" << endl;
    }
}

void Lista::ImprimirDireto() {
    Aluno* aux = mPrim;
    while(aux != NULL) {
        cout << aux->mMatr << " " << aux->mNome << " ";
        aux = aux->mPtProx;
    }
}

void Lista::ImprimirReverso() {
    Aluno* aux = mUlt;
    while(aux != NULL) {
        cout << aux->mMatr << " " << aux->mNome << " ";
        aux = aux->mPtAnt;
    }
}

int main() {
    Lista minhaLista;
	string opcao;
	unsigned matr;
	string nome;
    do {
        cin >> opcao;
        if (opcao == "i") {
            cin >> matr >> nome;
            minhaLista.InserirNoInicio(nome, matr);
		} else if (opcao == "r") {
			cin >> matr;
			minhaLista.Remover(matr);
		} else if (opcao == "ed") {
			minhaLista.ImprimirDireto();
            cout << endl;
        } else if (opcao == "er") {
            minhaLista.ImprimirReverso();
            cout << endl;
        }
    } while (opcao != "t");
    
    return 0;
}
