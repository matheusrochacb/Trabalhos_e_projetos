#include <iostream>
#include <stdexcept>
#include <queue>

typedef std::string TValor;
typedef unsigned TChave;
enum Cor {PRETO, VERMELHO};

class Noh {
    friend class ArvoreRN;
    friend std::ostream& operator<<(std::ostream& saida, Noh* ptNoh);
    public:
        Noh(unsigned chave, TValor valor);
    private:
        Noh* Tio();
        void DesalocarRecursivo();
        TValor mValor;
        TChave mChave;
        Cor mCor;
        Noh* mEsq;
        Noh* mDir;
        Noh* mPai;
};

class ArvoreRN {
    public:
        ArvoreRN();
        ~ArvoreRN();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(unsigned chave, TValor valor);
    private:
        void ArrumarInsercao(Noh* umNoh);
        void RotacionarDireita(Noh* umNoh);
        void RotacionarEsquerda(Noh* umNoh);
        Noh* mRaiz;
};

using namespace std;

// === classe Noh ==============================================================
Noh::Noh(TChave chave, TValor valor)
    : mValor(valor), mChave(chave), mCor(VERMELHO), mEsq(NULL), mDir(NULL), mPai(NULL) {
}

// Desaloca as subárvores e depois a si próprio.
void Noh::DesalocarRecursivo() {
    Noh* atual = this;
    if(atual != NULL){
		mEsq->DesalocarRecursivo();
		mDir->DesalocarRecursivo();
		delete atual;
	}
}

// Retorna o tio do nó, ou NULL se não houver tio
Noh* Noh::Tio() {
	Noh* paiNoh = this->mPai;
	if(paiNoh == NULL){
		return NULL;
	} else if(paiNoh == paiNoh->mPai->mEsq){
		return paiNoh->mPai->mDir;
	} else{
		return paiNoh->mPai->mEsq;
	}
	
	return NULL;
}

// === classe ArvoreRN =========================================================
ArvoreRN::ArvoreRN() : mRaiz(NULL) {
}

ArvoreRN::~ArvoreRN() {
    mRaiz->DesalocarRecursivo();
}

// insere uma chave/valor na árvore,
// chama ArrumarInsercao para corrigir eventuais problemas
void ArvoreRN::Inserir(unsigned chave, TValor valor) {
    Noh* novo = new Noh(chave, valor);
    
    if(mRaiz == NULL){
		mRaiz = novo;
		
	} else{
		Noh* atual = mRaiz;
		Noh* anterior = mRaiz;
		
		while(atual != NULL){
			anterior = atual;
			
			if(chave < atual->mChave){
				atual = atual->mEsq;
			} else{
				atual = atual->mDir;
			}
		}
		if(chave < anterior->mChave){
			anterior->mEsq = novo;
		} else{
			anterior->mDir = novo;
		}
		
		novo->mPai = anterior;
	}
	
	ArrumarInsercao(novo);
	
}

// procedimento para manter balanceamento após inserção
void ArvoreRN::ArrumarInsercao(Noh* umNoh) {
   
	while((umNoh != mRaiz) and (umNoh->mPai->mCor == VERMELHO)){
		
		Noh* Tio = umNoh->Tio();
		
		if((Tio != NULL) and (Tio->mCor == VERMELHO)){
			//Caso 1
			umNoh->mPai->mCor = PRETO;
			Tio->mCor = PRETO;
			umNoh->mPai->mPai->mCor = VERMELHO;
			umNoh = umNoh->mPai->mPai;
			
		} else{ // Tio preto
			// Caso 2 - novo à direita e pai à direita do avo
			
			if(umNoh == umNoh->mPai->mDir and umNoh->mPai == umNoh->mPai->mPai->mDir){
				umNoh->mPai->mCor = PRETO;
				umNoh->mPai->mPai->mCor = VERMELHO;
				Noh* avo = umNoh->mPai->mPai;
				RotacionarEsquerda(avo);
				umNoh = avo;
				
			} else if(umNoh == umNoh->mPai->mEsq and umNoh->mPai == umNoh->mPai->mPai->mEsq){
				// Caso 3 - novo à esquerda do pai e pai à esquerda do avo
				umNoh->mPai->mCor = PRETO;
				umNoh->mPai->mPai->mCor = VERMELHO;
				Noh* avo = umNoh->mPai->mPai;
				RotacionarDireita(avo);
				umNoh = avo;
                
			} else if(umNoh == umNoh->mPai->mEsq and umNoh->mPai == umNoh->mPai->mPai->mDir){
				// Caso 4 - novo à esquerda do pai e pai à direita do avo
				umNoh->mCor = PRETO;
				umNoh->mPai->mPai->mCor = VERMELHO;
				Noh* avo = umNoh->mPai->mPai;
				RotacionarDireita(umNoh->mPai);
				RotacionarEsquerda(avo);
				umNoh = umNoh->mPai;
                
			} else{
				// Caso 5 - novo à direita do pai e pai à esquerda do avo
				umNoh->mCor = PRETO;
				umNoh->mPai->mPai->mCor = VERMELHO;
				Noh* avo = umNoh->mPai->mPai;
				RotacionarEsquerda(umNoh->mPai);
				RotacionarDireita(avo);
				umNoh = umNoh->mPai;	
                			
			}
		}
	}
	
	mRaiz->mCor = PRETO;
}


// rotação à esquerda, muda a raiz se necessário
void ArvoreRN::RotacionarEsquerda(Noh* umNoh){
    Noh* novaraiz = umNoh->mDir;
    
    umNoh->mDir = novaraiz->mEsq;
    
    if(novaraiz->mEsq != NULL){
		novaraiz->mEsq->mPai = umNoh;
	}
	
	if(mRaiz == umNoh){
		mRaiz = novaraiz;
	} else if(umNoh == umNoh->mPai->mEsq){
		umNoh->mPai->mEsq = novaraiz;
	} else{
		umNoh->mPai->mDir = novaraiz;
	}
	
	novaraiz->mPai = umNoh->mPai;
	
	novaraiz->mEsq = umNoh;
	umNoh->mPai = novaraiz;
	
	// nesse caso nao precisa atualizar as alturas do umNoh e da nova raiz
}

// rotação à direita, muda a raiz se necessário
void ArvoreRN::RotacionarDireita(Noh* umNoh) {
    Noh* novaraiz = umNoh->mEsq;
    
    umNoh->mEsq = novaraiz->mDir;
    
    if(novaraiz->mDir != NULL){
		novaraiz->mDir->mPai = umNoh;
	}
	
	if(mRaiz == umNoh){
		mRaiz = novaraiz;
	} else if(umNoh == umNoh->mPai->mEsq){
		umNoh->mPai->mEsq = novaraiz;
	} else{
		umNoh->mPai->mDir = novaraiz;
	}
	
	novaraiz->mPai = umNoh->mPai;
	
	novaraiz->mDir = umNoh;
	umNoh->mPai = novaraiz;
	
	// nesse caso nao precisa atualizar as alturas do umNoh e da nova raiz
}

// Escreve o conteúdo da árvore nível a nível
void ArvoreRN::EscreverNivelANivel(ostream& saida) {
    queue<Noh*> filhos;
    filhos.push(mRaiz);
    while (not filhos.empty()) {
        unsigned nroNohsNesteNivel = unsigned(filhos.size());
        for (unsigned i = 0; i < nroNohsNesteNivel; ++i) {
            Noh* ptNoh = filhos.front();
            filhos.pop();
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mEsq);
                filhos.push(ptNoh->mDir);
            }
        }
        cout << "\n";
    }
}

// Escreve um nó (ou NIL).
ostream& operator<<(ostream& saida, Noh* ptNoh) {
    static char vetLetrasCores[] = "PV";
    if (ptNoh == NULL)
        saida << "NIL";
    else
        saida << '[' << vetLetrasCores[ptNoh->mCor] << ':' << ptNoh->mChave << '/'
              << ptNoh->mValor << ']';
    return saida;
}

int main() {
    ArvoreRN arvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                arvore.Inserir(chave, valor);
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
