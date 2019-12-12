#include <iostream>
#include <queue>

typedef int TChave;

class Noh {
    friend class Arvore23;
    friend std::ostream& operator<<(std::ostream& saida, Noh* ptNoh);
    protected:
        // Métodos
        Noh(const TChave& chave, Noh* ptPai);
        ~Noh();
        void DesalocarRecursivo();
        void DividirSeNecessario(Noh** ptPtRaiz);
        void InserirLocal(const TChave& chave, Noh* ptFilho);
        void InserirRecursivo(const TChave& chave, Noh** ptPtRaiz);
        // Atributos
        TChave mChaves[3]; // 3 chaves só temporariamente
        Noh* mFilhos[4];   // 4 filhos só temporariamente
        Noh* mPtPai;
        short int mQtdChaves;
        int mID; // identificador, útil para debugar o programa
};

class Arvore23 {
    public:
        Arvore23();
        ~Arvore23();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(const TChave& chave);
    protected:
        Noh* mPtRaiz;
};

using namespace std;

// === Classe Noh ==============================================================
Noh::Noh(const TChave& chave, Noh* ptPai) {
    mChaves[0] = chave;
    mQtdChaves = 1;
    mFilhos[0] = NULL; // usamos o primeiro filho para identificar folha
    mPtPai = ptPai;
    static int contador = 1;
    mID = contador++;
}

Noh::~Noh() {
}

void Noh::DesalocarRecursivo() {
    if (mFilhos[0] != NULL)
        for (int i = 0; i <= mQtdChaves; ++i)
            mFilhos[i]->DesalocarRecursivo();
    delete this;
}

// Divide um nó se for necessário, subindo a chave do meio e repetindo o processo.
// Recebe um ponteiro para ponteiro para a raiz, de forma a permitir a mudança da raiz da árvore
// quando necessário.
void Noh::DividirSeNecessario(Noh** ptPtRaiz) {
    // Se o nó está acima do limite de chaves:
    // -> Criar novo nó com a chave da direita e seus filhos.
    // -> Se o nó tem pai, recomeçar nele; senão, criar um novo pai e atualizar a raiz.
    if(mPtPai == NULL){
		Noh* novoFilho = new Noh(mChaves[2], NULL);
		
		Noh* novaRaiz = new Noh(mChaves[1], NULL);
		
		*ptPtRaiz = novaRaiz;
		
		novoFilho->mPtPai = novaRaiz;
		
		novaRaiz->mFilhos[0] = this;
		mPtPai = novaRaiz;
		mQtdChaves = 1;
		
		novaRaiz->mFilhos[1] = novoFilho;
		
		if(mFilhos[0] != NULL){
		int meio = 1;
		
			for(int i = 0; i < meio + 1; i++){
				novoFilho->mFilhos[i] = mFilhos[meio + 1 + i];
				// corrigir o pai do no filho
				mFilhos[meio + 1 + i]->mPtPai = novoFilho;
			}
		}
		
	} else{
		TChave chavePromovida = mChaves[1];
		
		mPtPai->InserirLocal(chavePromovida, this);
	}
}

// Adiciona uma chave e um filho ao nó, na posição certa dos vetores.
void Noh::InserirLocal(const TChave& chave, Noh* ptFilho) {
    
    // Deslocar chaves e filhos à direita até abrir espaço para a nova chave e filho.
    // Atualizar a quantidade de chaves no nó.
    short int pos = mQtdChaves - 1;
    
    while(pos >= 0 and mChaves[pos] > chave){
		mChaves[pos + 1] = mChaves[pos];
		
		mFilhos[pos + 2] = mFilhos[pos + 1];
		
		pos--;
	}
	
	mChaves[pos + 1] = chave;
	
	Noh* novoFilho = new Noh(ptFilho->mChaves[2], ptFilho->mPtPai);
	
	mFilhos[pos + 2] = novoFilho;
	
	mQtdChaves++;
	ptFilho->mQtdChaves = 1;
	
	if(ptFilho->mFilhos[0] != NULL){
		int meio = 1;
		
		for(int i = 0; i < meio + 1; i++){
			novoFilho->mFilhos[i] = ptFilho->mFilhos[meio + 1 + i];
			
			ptFilho->mFilhos[meio + 1 + i]->mPtPai = novoFilho;
		}
	}
}

// Desce recursivamente até achar uma folha para inserção. Faz as arrumações necessárias.
void Noh::InserirRecursivo(const TChave& chave, Noh** ptPtRaiz) {
    
    // Se o nó é folha, inserir aqui mesmo e fazer ajustes necessários.
    if(mFilhos[0] == NULL){ // verificar se no e folha
		short int pos = mQtdChaves - 1;
		
		while((pos >= 0) and (mChaves[pos] > chave)){
			mChaves[pos + 1] = mChaves[pos];
			pos--;
		}
		mChaves[pos + 1] = chave;
		mQtdChaves++;
		
	}else{
		short int pos = mQtdChaves - 1;
		
		while((pos >= 0) and (mChaves[pos] > chave)){
			pos--;
		}
		
		mFilhos[pos + 1]->InserirRecursivo(chave, ptPtRaiz);
	}
	
	if(mQtdChaves == 3){
		DividirSeNecessario(ptPtRaiz);
	}
    // Senão, recomeçar a inserção no filho apropriado.
}

// Escreve um nó no formato [ID|chaves|pai;filhos]
ostream& operator<<(ostream& saida, Noh* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else {
        saida << '[' << ptNoh->mID << "|" << ptNoh->mChaves[0];
        for (int i = 1; i < ptNoh->mQtdChaves; ++i)
            saida << ',' << ptNoh->mChaves[i];
        int pai = (ptNoh->mPtPai == NULL) ? 0 : ptNoh->mPtPai->mID;
        saida << "|" << pai << ';';
        if (ptNoh->mFilhos[0] != NULL) {
            saida << ptNoh->mFilhos[0]->mID;
            for (int i = 1; i <= ptNoh->mQtdChaves; ++i)
                saida << ',' << ptNoh->mFilhos[i]->mID;
        }
        saida << ']';
    }
    return saida;
}

// === Classe Arvore23 =========================================================
Arvore23::Arvore23() : mPtRaiz(NULL) {
}

Arvore23::~Arvore23() {
    if (mPtRaiz != NULL)
        mPtRaiz->DesalocarRecursivo();
}

// Escreve a árvore nível a nível para facilitar a depuração.
void Arvore23::EscreverNivelANivel(ostream& saida) {
    if (mPtRaiz == NULL)
        cout << "[]\n";
    else {
        queue<Noh*> filhos;
        filhos.push(mPtRaiz);
        while (not filhos.empty()) {
            unsigned nroNohsNesteNivel = unsigned(filhos.size());
            for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
                Noh* ptNoh = filhos.front();
                filhos.pop();
                saida << ptNoh << ' ';
                if (ptNoh->mFilhos[0] != NULL)
                    for (short int i = 0; i <= ptNoh->mQtdChaves; ++i)
                        filhos.push(ptNoh->mFilhos[i]);
            }
            cout << "\n";
        }
    }
}

// Insere uma chave na árvore.
void Arvore23::Inserir(const TChave& chave) {
    if (mPtRaiz == NULL)
        mPtRaiz = new Noh(chave, NULL);
    else
        mPtRaiz->InserirRecursivo(chave, &mPtRaiz);
}

// === Programa ================================================================
int main() {
    Arvore23 arvore;
    char opcao;
    TChave chave;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave;
                arvore.Inserir(chave);
                break;
            case 'e': // Escrever nós nível a nível
                arvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "Opção inválida\n";
        }
    } while (opcao != 'f');
    return 0;
}
