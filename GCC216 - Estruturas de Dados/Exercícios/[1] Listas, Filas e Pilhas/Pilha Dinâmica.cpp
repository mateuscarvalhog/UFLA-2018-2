#include <iostream>
using namespace std;


typedef int Dado; // para facilitar a troca de int para outro tipo
 
class Noh {
	friend class pilhav;
	private:
        Dado mDado;
        Noh* mDown;
    
    public:
        Noh(Dado data);
};

Noh::Noh(Dado data) {
    mDado = data;
    mDown = NULL;
}
 
// pilha implementada em arranjo 
class pilhav {
    private:
        int capacidade;
        int tamanho;
        Noh* posTopo;
        
    public:
        pilhav(int cap = 20);
        ~pilhav();
        void empilha(Dado valor);
        Dado desempilha();
        Dado espia(); // acessa elemento do topo, mas não retira
        void depura(); // imprime os dados do vetor da pilha sem removê-los (desempilhar)
        void info();  // imprime informações da pilha (tamanho e  posição do topo)
};

pilhav::pilhav(int cap) {
    capacidade = 20;
    tamanho = 0;
    posTopo = NULL;
}

pilhav::~pilhav() {
    while(posTopo != NULL) {
        Noh* aux = posTopo;
        posTopo = posTopo->mDown;
        delete aux;
    }
}

void pilhav::empilha(Dado data){
    if(tamanho <= capacidade) {
        Noh* novo = new Noh(data);
        if(posTopo == NULL) {
            posTopo = novo;
        } else {
            novo->mDown = posTopo;
            posTopo = novo;
        }
        tamanho++;
    } else
        cerr << "ERRO" << endl;
}

Dado pilhav::desempilha(){
	Noh* aux = posTopo;
    Dado removido = aux->mDado;
    if(posTopo == NULL) {
        cerr << "ERRO" <<endl;
        return Dado();
    } else {
        posTopo = posTopo->mDown;
        delete aux;
        tamanho--;
        return removido;
    }
}
Dado pilhav::espia(){
	return posTopo->mDado;
}

void pilhav::depura(){
	pilhav auxPilha;
    
    if(posTopo != NULL) {
        while(posTopo != NULL) {
            auxPilha.empilha(desempilha());
        }
        while(auxPilha.posTopo != NULL) {
            cout<<auxPilha.espia() << " ";
            empilha(auxPilha.desempilha());
        }
    } else {
        cerr << "ERRO" << endl;
    }
}
void pilhav::info(){
	cout << tamanho << " " << capacidade << endl;
}

int main(){
	pilhav minhaPilha;
    Dado valor;
    char opcao;
    cin >> opcao;
    do {
        if(opcao == 'a') {
            cin >> valor;
            minhaPilha.empilha(valor);
        } else if(opcao == 'b') {
            cout<<minhaPilha.desempilha();
        } else if(opcao == 'c') {
            cout<<minhaPilha.espia();
        } else if(opcao == 'd') {
            minhaPilha.depura();
        } else if(opcao == 'e') {
            minhaPilha.info();
        } else {
            cerr << "comando invalido\n";
        }
        cin >> opcao;
    } while(opcao != 'f');
    
    return 0;
}
