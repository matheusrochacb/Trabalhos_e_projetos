#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <queue>

typedef std::string TChave; // chave da árvore
typedef float TValor; // valor da árvore

// === Declarações das classes ===================================================
class NohAVL; // declaração avançada

class AVL {
    public:
        AVL();
        ~AVL();
        TValor& Valor(TChave chave);
        void EscreverEmOrdem();
        void EscreverNivelANivel(std::ostream& saida);
        void Inserir(TChave chave, const TValor& valor);
        bool ConferirLigacoes();
        void Remover(TChave chave);
    private:
        NohAVL* mPtRaiz;
};

class NohAVL {
    friend std::ostream& operator<<(std::ostream& saida, NohAVL* ptNoh);
    friend void AVL::EscreverNivelANivel(std::ostream& saida);
    public:
        NohAVL (TChave c, const TValor& v);
        ~NohAVL();
        void DesalocarFilhosRecursivo();
        NohAVL* InserirRecursivo(NohAVL* ptNoh);
        NohAVL* MenorRecursivo();
        NohAVL* RemoverRecursivo(TChave chave);
        TValor& ValorAux(TChave chave);
    private:
        NohAVL* ArrumarBalanceamento();
        void AtualizarAltura();
        int FatorBalanceamento();
        NohAVL* Remover();
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        void SubstitutirPor(NohAVL* ptNoh);
        void Transplantar(NohAVL* ptFilho);
        void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas têm altura 1
};

// === Implementações das classes ================================================

using namespace std;

// === Classe NohAVL =============================================================

NohAVL::NohAVL(TChave c, const TValor& v)
    : mChave(c), mValor(v), mPtEsq(NULL), mPtDir(NULL), mPtPai(NULL), mAltura(1) {
}

NohAVL::~NohAVL() {
	if(mPtEsq == NULL)
		delete mPtEsq;
	if(mPtDir == NULL)
		delete mPtDir;
	if(mPtPai == NULL)
		delete mPtPai;
}

// Faz as rotações e ajustes necessários inclusive do nó pai. Atualiza a altura.
// Retorna o ponteiro para o nó que ficar na posição dele após os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento() {

	AtualizarAltura();

	int fatbal = FatorBalanceamento();

	if((fatbal >= -1) and (fatbal <= 1)){
		return this;

	} else if((fatbal > 1) and (mPtEsq->FatorBalanceamento() >= 0)){
		return RotacionarDireita();

	} else if((fatbal > 1) and (mPtEsq->FatorBalanceamento() < 0)){
		mPtEsq = mPtEsq->RotacionarEsquerda();
		return RotacionarDireita();

	} else if((fatbal < -1) and (mPtDir->FatorBalanceamento() <= 0)){
		return RotacionarEsquerda();

	} else if((fatbal < -1) and (mPtDir->FatorBalanceamento() > 0)){
		mPtDir = mPtDir->RotacionarDireita();
		return RotacionarEsquerda();
	}
	
	return this;
}

// Calcula e atualiza a altura de um nó.
void NohAVL::AtualizarAltura() {
    int AltEsq, AltDir;
   
    if(mPtEsq == NULL){
		AltEsq = 0;
	} else{
		AltEsq = mPtEsq->mAltura;
	}

	if(mPtDir == NULL){
		AltDir = 0;
	} else{
		AltDir = mPtDir->mAltura;
	}

	if(AltEsq > AltDir){
		mAltura = AltEsq + 1;
	} else{
		mAltura = AltDir + 1;
	}
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
    NohAVL* aux = this;
    if(aux != NULL){
		aux->mPtEsq->DesalocarFilhosRecursivo();
		aux->mPtDir->DesalocarFilhosRecursivo();
		delete this;
}
}

// Calcula e retorna o fator de balanceamento do nó.
int NohAVL::FatorBalanceamento() {
int AltEsq, AltDir;
   
    if(mPtEsq == NULL){
		AltEsq = 0;
	} else{
		AltEsq = mPtEsq->mAltura;
	}

	if(mPtDir == NULL){
		AltDir = 0;
	} else{
		AltDir = mPtDir->mAltura;
	}

	return (AltEsq - AltDir);
}

// Insere um nó numa subárvore. Retorna o ponteiro para o nó que ficou no lugar do que recebeu
// a mensagem.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh) {

	if(ptNoh->mChave < mChave){

		if(mPtEsq == NULL){
			mPtEsq = ptNoh;
		} else{
			mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
		}

	} else{
		if(mPtDir == NULL){
			mPtDir = ptNoh;
		}else{
			mPtDir = mPtDir->InserirRecursivo(ptNoh);
		}
}
	return ArrumarBalanceamento();
}

// Busca o nó que tem a menor chave. Retorna o ponteiro para ele.
NohAVL* NohAVL::MenorRecursivo() {
	
	NohAVL* atual = this;
	
	if(atual->mPtEsq == NULL){
		return atual;
	}else{
		mPtEsq->MenorRecursivo();
	}
}

// Remove o nó. Retorna o ponteiro para o nó que ficou no lugar dele.
// Confira com RemoverRecursivo(TChave).
NohAVL* NohAVL::Remover() {
	NohAVL* NovaRaiz;
	
	if(mPtEsq == NULL){
		NovaRaiz = mPtDir;
		Transplantar(mPtDir);

	} else if(mPtDir == NULL){
		NovaRaiz = mPtEsq;
		Transplantar(mPtEsq);

	} else{
		
		NohAVL* aRemover = this;
		NohAVL* sucessor = mPtDir->MenorRecursivo();
		NovaRaiz = sucessor;
			
		if(sucessor->mPtPai != aRemover){
			sucessor->Transplantar(sucessor->mPtDir);
			NohAVL* aux = sucessor->mPtPai;
			
			if(aux != NULL){
				
			
			// Atualiza altura do pai até o nó à direita do aRemover
			while(aux != aRemover->mPtDir){
				
				aux->AtualizarAltura();
				aux = aux-> mPtPai;
			}
			// chama arrumar balanceamento no filho à direita do aRemover
			// e seu retorno é o nó que sucessor dir aponta
			sucessor->mPtDir = aRemover->mPtDir->ArrumarBalanceamento();
			// novo sucessor direito pai se torna o sucessor
			sucessor->mPtDir->mPtPai = sucessor;
			}
		}
	
		aRemover->Transplantar(sucessor);
		sucessor->mPtEsq = aRemover->mPtEsq;
		sucessor->mPtEsq->mPtPai = sucessor;
		delete aRemover;
	}
	
	if(NovaRaiz == NULL){
		return NovaRaiz;
	} else{
		return NovaRaiz->ArrumarBalanceamento();
	}
}

// Busca recursivamente um nó com dada chave e o remove. Retorna o ponteiro para o nó que ficou
// no lugar do nó que recebeu a mensagem.
// Confira com Remover().
NohAVL* NohAVL::RemoverRecursivo(TChave chave) {
	if(mChave == chave){
		return Remover();
	} else if(chave < mChave){
		mPtEsq = mPtEsq->RemoverRecursivo(chave);
	} else{
		mPtDir = mPtDir->RemoverRecursivo(chave);
	}
	return this;
}

// Rotaciona à direita a subárvore. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarDireita() {
	NohAVL* NovaRaiz = mPtEsq;
	
	mPtEsq = NovaRaiz->mPtDir;
	
	if(NovaRaiz->mPtDir != NULL){
		NovaRaiz->mPtDir->mPtPai = this;
	}
	
	NovaRaiz->mPtPai = mPtPai;
	
	if(mPtPai != NULL){
		if(this == mPtPai->mPtEsq){
			mPtPai->mPtEsq = NovaRaiz;
		} else{
			mPtPai->mPtDir = NovaRaiz;
		}
	}
	
	NovaRaiz->mPtDir = this;
	mPtPai = NovaRaiz;
	
	this->AtualizarAltura();
	NovaRaiz->AtualizarAltura();
	
	return NovaRaiz;
}

// Rotaciona à esquerda a subárvore. Retorna a nova raiz da subárvore.
NohAVL* NohAVL::RotacionarEsquerda() {
	
	NohAVL* NovaRaiz = mPtDir;

	mPtDir = NovaRaiz->mPtEsq;

	if(NovaRaiz->mPtEsq != NULL){
		NovaRaiz->mPtEsq->mPtPai = this;
	}

	NovaRaiz->mPtPai = mPtPai;
	
	if(mPtPai != NULL){
		if(this == this->mPtPai->mPtEsq){
			mPtPai->mPtEsq = NovaRaiz;
		} else{
			mPtPai->mPtDir = NovaRaiz;
		}
	}

	mPtPai = NovaRaiz;
	NovaRaiz->mPtEsq = this;
	this->AtualizarAltura();
	NovaRaiz->AtualizarAltura();

	return NovaRaiz;
}

// Atualiza todas as ligações, para que ptNoh, fique no lugar deste nó.
// Usado na remoção com dois filhos.
/*void NohAVL::SubstitutirPor(NohAVL* ptNoh) {
    #warning NohAVL::SubstitutirPor não implementado.
}*/

// Muda as ligações do pai e do filho para desligar o nó atual.
// Usado para remoção na situação em que não existem os dois filhos.
void NohAVL::Transplantar(NohAVL* ptFilho) {
    NohAVL* antigo = this;
    
    if(antigo->mPtPai != NULL){

		if(antigo == antigo->mPtPai->mPtEsq){
			antigo->mPtPai->mPtEsq = ptFilho;
		} else{
			antigo->mPtPai->mPtDir = ptFilho;
		}
	
		if(ptFilho != NULL){
			ptFilho->mPtPai = antigo->mPtPai;
		}
	}
}

// Substitui um dos filhos por um novo nó.
/*void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    #warning NohAVL::TrocarFilho não implementado.
}*/

// Busca recursivamente uma dada chave e retorna o valor associado a ela.
// Levanta exceção se não encontrar a chave.
TValor& NohAVL::ValorAux(TChave chave) {
	cout << this->mValor << endl;
	
	if(chave == mChave){
		return this->mValor;
	} else if(chave < mChave){
		mPtEsq->ValorAux(chave);
		
	} else{
		mPtDir->ValorAux(chave);
	}
}

// Escreve o conteúdo de um nó no formato [altura:chave/valor].
// Escreve "[]" se o ponteiro recebido for NULL.
ostream& operator<<(ostream& saida, NohAVL* ptNoh) {
    if (ptNoh == NULL)
        saida << "[]";
    else
        saida << '[' << ptNoh->mAltura << ':' << ptNoh->mChave << '/' << ptNoh->mValor << ']';
    return saida;
}

// === Classe AVL =================================================================================
AVL::AVL() : mPtRaiz(NULL) {
}

AVL::~AVL() {
    if (mPtRaiz != NULL) {
        mPtRaiz->DesalocarFilhosRecursivo();
        delete mPtRaiz;
    }
}

// Retorna o valor associado a uma dada chave.
TValor& AVL::Valor(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de buscar valor numa arvore vazia.");
    
    return mPtRaiz->ValorAux(chave);
}

// Escreve o conteúdo da árvore nível a nível, na saída de dados informada.
// Usado para conferir se a estrutra da árvore está correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nó especial para marcar fim de um nível
    filhos.push(mPtRaiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty()) {
        NohAVL* ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel) {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else {
            saida << ptNoh << ' ';
            if (ptNoh != NULL) {
                filhos.push(ptNoh->mPtEsq);
                filhos.push(ptNoh->mPtDir);
            }
        }
    }
    delete fimDeNivel;
}

// Insere um par chave/valor na árvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
   
    NohAVL* novoNoh = new NohAVL(chave, valor);
   
    if(mPtRaiz == NULL){
		mPtRaiz = novoNoh;

	} else{
		mPtRaiz = mPtRaiz->InserirRecursivo(novoNoh);
	}
}

// Remove da árvore o nó que tiver uma dada chave.
void AVL::Remover(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de remover elemento de uma árvore vazia.");
    mPtRaiz = mPtRaiz->RemoverRecursivo(chave);
}

// === Programa ===================================================================================
int main() {
    AVL minhaArvore;
    char opcao;
    TChave chave;
    TValor valor;
    do {
        cin >> opcao;
        switch (opcao) {
            case 'i': // Inserir
                cin >> chave >> valor;
                minhaArvore.Inserir(chave, valor);
                break;
            case 'r': // Remover
                try {
                    cin >> chave;
                    minhaArvore.Remover(chave);
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
                break;
            case 'b': // Buscar
                try {
                    cin >> chave;
                    valor = minhaArvore.Valor(chave);
                    cout << valor << endl;
                }
                catch (runtime_error& erro) {
                    cerr << erro.what() << endl;
                }
                break;
            case 'e': // Escrever nós nível a nível
                minhaArvore.EscreverNivelANivel(cout);
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

