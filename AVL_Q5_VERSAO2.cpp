#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <queue>

typedef std::string TChave; // chave da Ã¡rvore
typedef float TValor; // valor da Ã¡rvore

// === DeclaraÃ§Ãµes das classes ===================================================
class NohAVL; // declaraÃ§Ã£o avanÃ§ada

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
        TValor& Valor(TChave chave);
    private:
        NohAVL* ArrumarBalanceamento();
        void AtualizarAltura();
        int FatorBalanceamento();
        NohAVL* Remover();
        NohAVL* RotacionarEsquerda();
        NohAVL* RotacionarDireita();
        //~ void SubstitutirPor(NohAVL* ptNoh);
        void Transplantar(NohAVL* ptFilho);
        //~ void TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo);
        TChave mChave;
        TValor mValor;
        NohAVL* mPtEsq;
        NohAVL* mPtDir;
        NohAVL* mPtPai;
        unsigned int mAltura; // folhas tÃªm altura 1
};

// === ImplementaÃ§Ãµes das classes ================================================

using namespace std;

// === Classe NohAVL =============================================================

NohAVL::NohAVL(TChave c, const TValor& v)
    : mChave(c), mValor(v), mPtEsq(NULL), mPtDir(NULL), mPtPai(NULL), mAltura(1) {
}

NohAVL::~NohAVL() {
}

// Faz as rotaÃ§Ãµes e ajustes necessÃ¡rios inclusive do nÃ³ pai. Atualiza a altura.
// Retorna o ponteiro para o nÃ³ que ficar na posiÃ§Ã£o dele apÃ³s os ajustes.
NohAVL* NohAVL::ArrumarBalanceamento(){
	AtualizarAltura();

	int fatbal = FatorBalanceamento();

	if (fatbal >= -1 and fatbal <= 1)
	{
		return this;
	}else if (fatbal > 1 and mPtEsq->FatorBalanceamento() >= 0)
	{
		return RotacionarDireita();
	}else if (fatbal > 1 and mPtEsq->FatorBalanceamento() < 0)
	{
		mPtEsq = mPtEsq->RotacionarEsquerda();
		return RotacionarDireita();
	}else if (fatbal < -1 and mPtDir->FatorBalanceamento() <= 0)
	{
		return RotacionarEsquerda();
	}else if (fatbal < -1 and mPtDir->FatorBalanceamento() > 0)
	{
		mPtDir = mPtDir->RotacionarDireita();
		return RotacionarEsquerda();
	}
	
	return NULL;
}

// Calcula e atualiza a altura de um nÃ³.
void NohAVL::AtualizarAltura() {
    int AltEsq, AltDir;
   
    if (mPtEsq == NULL)
	{
		AltEsq = 0;
	}else{
		AltEsq = mPtEsq->mAltura;
	}

	if (mPtDir == NULL)
	{
		AltDir = 0;
	}else{
		AltDir = mPtDir->mAltura;
	}

	if (AltEsq > AltDir)
	{
		mAltura = AltEsq + 1;
	}else
	{
		mAltura = AltDir + 1;
	}
}

// Desaloca todos os descendentes.
void NohAVL::DesalocarFilhosRecursivo() {
NohAVL* atual = this;

	if (atual != NULL)
	{
		mPtEsq->DesalocarFilhosRecursivo();
		mPtDir->DesalocarFilhosRecursivo();
		delete this;
	}
}

// Calcula e retorna o fator de balanceamento do nÃ³.
int NohAVL::FatorBalanceamento() {
int AltEsq, AltDir;
   
    if (mPtEsq == NULL)
	{
		AltEsq = 0;
	}else{
		AltEsq = mPtEsq->mAltura;
	}

	if (mPtDir == NULL)
	{
		AltDir = 0;
	}else{
		AltDir = mPtDir->mAltura;
	}

	return (AltEsq - AltDir);
}

// Insere um nÃ³ numa subÃ¡rvore. Retorna o ponteiro para o nÃ³ que ficou no lugar do que recebeu
// a mensagem.
NohAVL* NohAVL::InserirRecursivo(NohAVL* ptNoh){

	if (ptNoh->mChave < mChave)
	{
		if (mPtEsq == NULL)
		{
			mPtEsq = ptNoh;
			ptNoh->mPtPai = this;
		}else
		{
			mPtEsq = mPtEsq->InserirRecursivo(ptNoh);
		}
	}else
	{
		if (mPtDir == NULL)
		{
			mPtDir = ptNoh;
			ptNoh->mPtPai = this;
		}else
		{
			mPtDir = mPtDir->InserirRecursivo(ptNoh);
		}
	}

	return ArrumarBalanceamento();
}

// Busca o nÃ³ que tem a menor chave. Retorna o ponteiro para ele.
NohAVL* NohAVL::MenorRecursivo() {
	if(mPtEsq != NULL)
		return mPtEsq->MenorRecursivo();
	
	return this;
}

// Remove o nÃ³. Retorna o ponteiro para o nÃ³ que ficou no lugar dele.
// Confira com RemoverRecursivo(TChave).
	NohAVL* NohAVL::Remover() {
	NohAVL* aRemover = this;
	NohAVL* novaRaiz;

	if (mPtDir == NULL and mPtEsq == NULL)
	{
		delete aRemover;
		novaRaiz = NULL;
		
	}if (mPtEsq == NULL)
	{
		novaRaiz = mPtDir;
		Transplantar(mPtDir);
		
	}if (mPtDir == NULL)
	{
		novaRaiz = mPtEsq;
		Transplantar(mPtEsq);
		
	}else
	{
		NohAVL* sucessor = mPtDir->MenorRecursivo();
		novaRaiz = sucessor;
		
		if(sucessor->mPtPai != aRemover)
		{
			sucessor->Transplantar(sucessor->mPtDir);
			
			// Na AVL tem que atualizar altura do suc pai ate o noh direita daquela a remover
			NohAVL* aux = sucessor->mPtPai;
			while(aux != aRemover->mPtDir){
				aux->AtualizarAltura();
				aux = aux->mPtPai;
		}
			sucessor->mPtDir = aRemover->mPtDir->ArrumarBalanceamento();
			sucessor->mPtDir->mPtPai = sucessor;
		}
		
		aRemover->Transplantar(sucessor);
		sucessor->mPtEsq = aRemover->mPtEsq;
		sucessor->mPtEsq->mPtPai = sucessor;
		
		delete aRemover;
	}
		

	return novaRaiz;
}

// Busca recursivamente um nÃ³ com dada chave e o remove. Retorna o ponteiro para o nÃ³ que ficou
// no lugar do nÃ³ que recebeu a mensagem.
// Confira com Remover().
NohAVL* NohAVL::RemoverRecursivo(TChave chave){
if (mChave == chave)
{
return Remover();
}if (chave < mChave)
{
mPtEsq->RemoverRecursivo(chave);
}else
{
mPtDir->RemoverRecursivo(chave);
}
return this;
}

// Rotaciona Ã  direita a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarDireita() {
NohAVL* novaRaiz = mPtEsq;

mPtEsq = novaRaiz->mPtDir;

if (novaRaiz->mPtDir != NULL)
{
novaRaiz->mPtDir->mPtPai = this;
}

if (mPtPai != NULL)
{
if (this == mPtPai->mPtEsq)
{
mPtPai->mPtEsq = novaRaiz;
}else
{
mPtPai->mPtDir = novaRaiz;
}
}



novaRaiz->mPtPai = mPtPai;
mPtPai = novaRaiz;
novaRaiz->mPtDir = this;

//ATUALIZAR ALTURA!!!
	
	this->AtualizarAltura();
	novaRaiz->AtualizarAltura();

return novaRaiz;
}

// Rotaciona Ã  esquerda a subÃ¡rvore. Retorna a nova raiz da subÃ¡rvore.
NohAVL* NohAVL::RotacionarEsquerda() {
	NohAVL* novaRaiz = mPtDir;

	mPtDir = novaRaiz->mPtEsq;

	if (novaRaiz->mPtEsq != NULL)
	{
		novaRaiz->mPtEsq->mPtPai = this;
	}

	if (mPtPai != NULL)
	{
		if (this == mPtPai->mPtEsq)
		{
			mPtPai->mPtEsq = novaRaiz;
		}else
		{
			mPtPai->mPtDir = novaRaiz;
		}
	}

	novaRaiz->mPtPai = mPtPai;
	mPtPai = novaRaiz;
	novaRaiz->mPtEsq = this;
	
	//ATUALIZAR ALTURA!!!
	
	this->AtualizarAltura();
	novaRaiz->AtualizarAltura();

	return novaRaiz;
}

// Atualiza todas as ligaÃ§Ãµes, para que ptNoh, fique no lugar deste nÃ³.
// Usado na remoÃ§Ã£o com dois filhos.
//~ void NohAVL::SubstitutirPor(NohAVL* ptNoh) {
    //~ #warning NohAVL::SubstitutirPor nÃ£o implementado.


// Muda as ligaÃ§Ãµes do pai e do filho para desligar o nÃ³ atual.
// Usado para remoÃ§Ã£o na situaÃ§Ã£o em que nÃ£o existem os dois filhos.
void NohAVL::Transplantar(NohAVL* ptFilho) {
    NohAVL* antigo = this;
    
    if (antigo->mPtPai != NULL)
	{
		if (antigo == mPtPai->mPtEsq)
		{
			mPtPai->mPtEsq = ptFilho;
		} else if (antigo == mPtPai->mPtDir)
		{
			mPtPai->mPtDir = ptFilho;
		}
	}
	
	if(ptFilho != NULL)
	{
		ptFilho->mPtPai = antigo->mPtPai;
	}
}

// Substitui um dos filhos por um novo nÃ³.
//~ void NohAVL::TrocarFilho(NohAVL* ptAntigo, NohAVL* ptNovo) {
    //~ #warning NohAVL::TrocarFilho nÃ£o implementado.


// Busca recursivamente uma dada chave e retorna o valor associado a ela.
// Levanta exceÃ§Ã£o se nÃ£o encontrar a chave.
TValor& NohAVL::Valor(TChave chave) {
	NohAVL* atual = this;
	if (atual == NULL)
	{
		throw runtime_error("chave nao encontrada");
		
	}else if (chave == mChave)
	{
		return this->mValor;
	}else if (chave < mChave)
	{
		return mPtEsq->Valor(chave);
	}else{
		return mPtDir->Valor(chave);
	}
}

// Escreve o conteÃºdo de um nÃ³ no formato [altura:chave/valor].
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
    return mPtRaiz->Valor(chave);
}

// Escreve o conteÃºdo da Ã¡rvore nÃ­vel a nÃ­vel, na saÃ­da de dados informada.
// Usado para conferir se a estrutra da Ã¡rvore estÃ¡ correta.
void AVL::EscreverNivelANivel(ostream& saida) {
    queue<NohAVL*> filhos;
    NohAVL* fimDeNivel = new NohAVL(TChave(), TValor()); // nÃ³ especial para marcar fim de um nÃ­vel
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

// Insere um par chave/valor na Ã¡rvore.
void AVL::Inserir(TChave chave, const TValor& valor) {
    NohAVL* novoNoh = new NohAVL(chave, valor);
   
    if (mPtRaiz == NULL)
	{
		mPtRaiz = novoNoh;
	}else{
		mPtRaiz = mPtRaiz->InserirRecursivo(novoNoh);
	}
}

// Remove da Ã¡rvore o nÃ³ que tiver uma dada chave.
void AVL::Remover(TChave chave) {
    if (mPtRaiz == NULL)
        throw runtime_error("Tentativa de remover elemento de uma Ã¡rvore vazia.");
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
            case 'e': // Escrever nÃ³s nÃ­vel a nÃ­vel
                minhaArvore.EscreverNivelANivel(cout);
                break;
            case 'f': // Finalizar o programa
                // vai verificar depois
                break;
            default:
                cerr << "OpÃ§Ã£o invÃ¡lida\n";
        }
    } while (opcao != 'f');
    return 0;
}
