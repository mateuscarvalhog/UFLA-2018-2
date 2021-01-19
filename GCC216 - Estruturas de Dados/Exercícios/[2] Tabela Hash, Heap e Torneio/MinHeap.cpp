#include <iostream>

using namespace std;

class minHeap {
    private:
        unsigned mCapacidade;
        int* mVetor;
        unsigned FilhoDaEsquerda(int pos) { return (2 * pos) + 1; }
        unsigned FilhoDaDireita(int pos) { return (2 * pos) + 2; }
        unsigned Pai(int pos) { return (pos - 1) / 2; }
    
    public:
        minHeap(int vetor[], unsigned cap);
        ~minHeap();
        void CorrigeDescendo(unsigned pos);
        void Imprime(unsigned indice);
};

minHeap::minHeap(int vetor[], unsigned cap) {
    mCapacidade = cap;
    mVetor = new int(mCapacidade);
    for(unsigned i = 0; i < mCapacidade; i++)
        mVetor[i] = vetor[i];
    bool terminou = false;
    unsigned i = ((cap - 1) / 2) + 1;
    while (!terminou) {
        i--;
        CorrigeDescendo(i);
        Imprime(i);
        if (i == 0)
            terminou = true;
    }
}

minHeap::~minHeap() {
    delete[] mVetor;
}

void minHeap::CorrigeDescendo(unsigned pos) {
    unsigned esq = FilhoDaEsquerda(pos);
    unsigned dir = FilhoDaDireita(pos);
    unsigned menor = pos;
    if (esq < mCapacidade and mVetor[esq] < mVetor[menor])
        menor = esq;
    if (dir < mCapacidade and mVetor[dir] < mVetor[menor])
        menor = dir;
    if (menor != pos) {
        swap(mVetor[pos], mVetor[menor]);
        CorrigeDescendo(menor);
    }
}

void minHeap::Imprime(unsigned indice) {
    cout << indice << ":";
    for (unsigned i = 0; i < mCapacidade; i++)
        cout << " " << mVetor[i];
    cout << endl;
}

int main() {
    unsigned qtd;
    cin >> qtd;
    int vetor[qtd];
    for(unsigned i = 0; i < qtd; i++)
        cin >> vetor[i];
    minHeap meuHeap(vetor, qtd);
    
    return 0;
}
