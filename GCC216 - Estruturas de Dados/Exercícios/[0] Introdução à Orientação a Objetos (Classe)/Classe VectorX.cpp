#include <iostream>
using namespace std;

typedef int Dado;

class VectorX {
    public:
        VectorX (unsigned cap);
        ~VectorX ();
        Dado Elemento(unsigned pos);
        void Alterar(unsigned pos, Dado novo);
        void Ordenar();
        void Multiplicar(unsigned mult);
        void Imprimir();
        void InserirNoFim(Dado valor);
        
    protected:
        unsigned mCap;
        unsigned mTam;
        Dado* mVet;
};

VectorX::VectorX(unsigned cap)
    : mCap(cap), mTam(), mVet (NULL) {
    if (cap > 0)
        mVet = new Dado[mCap];
}

VectorX::~VectorX() {
    delete[] mVet;
}

Dado VectorX::Elemento(unsigned pos) {
    return mVet[pos];
}

void VectorX::Alterar(unsigned pos, Dado novo) {
    mVet[pos] = novo;
}

void VectorX::Ordenar() {
    for(unsigned i = 0; i < mTam - 1; i++)
        for(unsigned j = 0; j < mTam - 1 - i; j++)
			if (mVet[j] > mVet[j + 1])
				swap(mVet[j], mVet[j + 1]);
}
void VectorX::Multiplicar(unsigned mult) {
    for(unsigned i = 0; i < mTam; i++){
        mVet[i] *= mult;
    }
}

void VectorX::Imprimir() {
    for(unsigned i=0; i<mTam; i++){
        cout << mVet[i] << " ";  
    }
}

void VectorX::InserirNoFim(Dado valor) {
    if (mTam < mCap)
        mVet[mTam] = valor;
    else {
        mCap = unsigned (mCap * 1.1) + 2;
        Dado* vetor = new Dado[mCap];
        for (unsigned i = 0; i < mCap; i++) {
            vetor[i] = mVet[i];
        }
        vetor[mTam] = valor;
        delete[] mVet;
        mVet = vetor;
    }
    mTam++;
}

int main() {
    unsigned cap;
    cin >> cap;
    
    
    VectorX vetor(cap);
    
    cout << vetor.Elemento(1) << endl;
    vetor.Alterar(0, 8);
    vetor.Ordenar();
    vetor.Multiplicar(2);
    vetor.Imprimir();
    vetor.InserirNoFim(69);
    
    return 0;
}
