// Tabela Hash com tratamento de colisÃ£o por endereÃ§amento aberto
#include <iostream>
#include <string>
#include <exception>

typedef int TValor; // tipo do valor armazenado

class InfoHash {
    friend class Hash; // essa classe sÃ³ deve ser usada na classe Hash
    InfoHash();
    InfoHash(const std::string& chave, const TValor& valor);
    std::string mChave;
    TValor mValor;
};

class Hash {
    public:
        Hash(unsigned capacidade = 50);
        ~Hash();
        // Mostra todos as posiÃ§Ãµes de armazenamento da hash.
        void EscreverEstrutura(std::ostream& saida) const;
        // Insere uma cÃ³pia do valor. NÃ£o permite inserÃ§Ã£o de chave repetida.
        void Inserir(const std::string& chave, const TValor& valor);
        // Remove um item da hash associado com a chave dada.
        void Remover(const std::string& chave);
        // Retorna o valor associado a uma chave.
        TValor Valor(const std::string& chave) const;
    protected:
        // Retorna a posicao em que uma chave estÃ¡ armazenada na estrutura.
        unsigned Buscar(const std::string& chave) const;
        // Insere um InfoHash na estrutura. Auxiliar para Inserir(chave/valor) e Redimensionar.
        void Inserir(InfoHash* ptPar);
        // Retorna a posicao em que uma chave deveria ficar na estrutura.
        unsigned Posicao(const std::string& chave) const;
        // Altera a capacidade da estrutura.
        void Redimensionar(unsigned novaCapacidade);
        InfoHash** mVetPtDados;
        InfoHash* REMOVIDO; // ponteiro especial a ser usado para marcar posiÃ§Ãµes de elemento removido
        unsigned mCapacidade;
        unsigned mTamanho;
};

using namespace std;

InfoHash::InfoHash()
    : mChave(), mValor() {
}

InfoHash::InfoHash(const std::string& chave, const TValor& valor)
    : mChave(chave), mValor(valor) {
}

Hash::Hash(unsigned capacidade) // capacidade tem valor default
    : mVetPtDados(new InfoHash*[capacidade]), REMOVIDO(new InfoHash()), mCapacidade(capacidade), mTamanho(0) {
    // FALTA FAZER:
    // inicializar todas as posiÃ§Ãµes de armazenamento com NULL indicando posiÃ§Ã£o VAZIA
    for(unsigned i = 0; i < mCapacidade; i++)
        mVetPtDados[i] = NULL;
}

Hash::~Hash() {
    // FALTA FAZER:
    // desalocar memÃ³ria de cada item (InfoHash) armazenado
    // desalocar o ponteiro especial REMOVIDO
    // desalocar o vetor de ponteiros
    for(unsigned i = 0; i < mCapacidade; i++)
        if (mVetPtDados[i] != REMOVIDO)
            delete mVetPtDados[i];
    delete REMOVIDO;
    delete[] mVetPtDados;
}

unsigned Hash::Buscar(const std::string& chave) const {
    // Retorna a posicao em que uma chave estÃ¡ armazenada na estrutura. Protegido.
    unsigned pos = Posicao(chave);
    unsigned cont = 0;
    while(cont < mCapacidade and mVetPtDados[pos] != NULL) {
        if(mVetPtDados[pos]->mChave == chave)
            return pos;
        else {
            pos++;
            cont++;
            if(pos == mCapacidade)
                pos = 0;
        }
    }
    throw runtime_error("NAO EXISTE");
}

void Hash::EscreverEstrutura(std::ostream& saida) const {
    // Mostra todos as posiÃ§Ãµes de armazenamento da hash. Ajuda a debugar.
    for (unsigned i = 0; i < mCapacidade; ++i) {
        saida << '[' << i;
        if (mVetPtDados[i] != NULL) {
            if (mVetPtDados[i] == REMOVIDO)
                saida << "/removido";
            else
                saida << '/' << mVetPtDados[i]->mChave << '/' << mVetPtDados[i]->mValor;
        }
        saida << "] ";
    }
}

void Hash::Inserir(const string& chave, const TValor& valor) {
    // Insere uma cÃ³pia do valor. NÃ£o permite inserÃ§Ã£o de chave repetida.
    if(mTamanho == mCapacidade)
        Redimensionar(mCapacidade + 5);
    unsigned pos = Posicao(chave);
    bool inserido = false;
    while(!inserido) {
        if(mVetPtDados[pos] == REMOVIDO or mVetPtDados[pos] == NULL) {
            mVetPtDados[pos] = new InfoHash(chave, valor);
            mTamanho++;
            inserido = true;
        } else {
            if(mVetPtDados[pos]->mChave == chave)
                throw runtime_error("JA EXISTE");
            pos++;
            if(pos == mCapacidade)
                pos = 0;
        }
    }
}

void Hash::Inserir(InfoHash* ptPar) {
    // Insere um par (chave/valor) na estrutura. SupÃµe que a chave Ã© vÃ¡lida e Ãºnica.
    // SupÃµe que hÃ¡ espaÃ§o para inserÃ§Ã£o e que o ponteiro Ã© vÃ¡lido. NÃ£o altera o tamanho.
    // MÃ©todo protegido, auxiliar para Inserir(chave, valor) e Redimensionar(cap).
    unsigned pos = Posicao(ptPar->mChave);
    bool inserido = false;
    while(!inserido) {
        if(mVetPtDados[pos] == REMOVIDO or mVetPtDados[pos] == NULL) {
            mVetPtDados[pos] = ptPar;
            inserido = true;
        } else {
            pos++;
            if(pos == mCapacidade)
                pos = 0;
        }
    }
}

unsigned Hash::Posicao(const string& chave) const {
    // Retorna a posiÃ§Ã£o de armazenamento de uma chave, 0 <= posicao < mCapacidade. Protegido.
    unsigned pos = 1;
    unsigned tamanho = unsigned(chave.length());
    for (unsigned i = 0; i < tamanho; ++i)
        pos = 7 * pos + chave[i];
    return pos % mCapacidade;
}

void Hash::Redimensionar(unsigned novaCapacidade) {
    // Altera a capacidade da estrutura. Pode ser chamado mesmo que a estrutura nÃ£o esteja cheia.
    // Verifica se a novaCapacidade Ã© vÃ¡lida. MÃ©todo protegido.
    if (novaCapacidade < mTamanho){
        throw runtime_error("nova capacidade invalida");
    } else {
        InfoHash** auxVet = mVetPtDados;
        mVetPtDados = new InfoHash*[novaCapacidade];
        unsigned auxCap = mCapacidade;
        mCapacidade = novaCapacidade;
        for (unsigned i = 0; i < novaCapacidade; i++)
            mVetPtDados[i] = NULL;
        for(unsigned i = 0; i < auxCap; i++)
            Inserir(auxVet[i]);
        delete[] auxVet;
    }
}

void Hash::Remover(const std::string& chave) {
    // Remove um item da hash associado com a chave dada.
    unsigned pos = Buscar(chave);
    mVetPtDados[pos] = REMOVIDO;
    mTamanho--;
}

TValor Hash::Valor(const std::string& chave) const {
    // Retorna o valor associado a uma chave.
    unsigned pos = Buscar(chave);
    return mVetPtDados[pos]->mValor;
}

int main() {
    unsigned capacidade;
    cin >> capacidade;
    Hash tabela(capacidade);
    char operacao;
    string chave;
    TValor valor;
    do {
        try {
            cin >> operacao;
            switch (operacao) {
                case 'i': // inserir
                    cin >> chave >> valor;
                    tabela.Inserir(chave, valor);
                    break;
                case 'r': // remover
                    cin >> chave;
                    tabela.Remover(chave);
                    break;
                case 'c': // consultar
                    cin >> chave;
                    valor = tabela.Valor(chave);
                    cout << valor << endl;
                    break;
                case 'd': // debug (mostrar estrutura)
                    tabela.EscreverEstrutura(cout);
                    cout << endl;
                    break;
                case 'f': // finalizar
                    // vai testar depois
                    break;
                default:
                    cerr << "operaÃ§Ã£o invÃ¡lida\n";
            }
        }
        catch (exception& e) {
            // ignorar a mensagem que explica o erro e sÃ³ escrever ERRO.
            cout << "ERRO" << endl;
        }
    } while (operacao != 'f');
    return 0;
}
