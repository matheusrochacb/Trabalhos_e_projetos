// Árvore 2-3-4  
// (Árvore B em memória, com nós com capacidade para 2, 3 ou 4 filhos)
// Estruturas de Dados - 2018/2
// Código by Joukim

#include <iostream>

using namespace std;

// constantes para verificação
const int MAXITENS = 3;
const int MINITENS = 1;
const int MAXFILHOS = 4;

// posição do elemento do meio (o que sobe para página pai em caso de estouro
const int MEIO = 1; 

typedef float Dado;

class Noh {
friend class Arvore234;
private:
    bool folha;
    int num; // número de itens armazenadas no nó
    Dado itens[MAXITENS];
    Noh* filhos[MAXFILHOS];
    Noh* pai;
public:
    Noh();
    ~Noh();
};

Noh::Noh() {
    num = 0;
    folha = false;
    for (int i = 0; i < MAXFILHOS; i++) {
        filhos[i] = NULL;
    }
}

Noh::~Noh() {
    for (int i = 0; i < num+1; i++) {
        delete filhos[i];
    }
}

class Arvore234 {
private:
    Noh* raiz;
    void percorreEmOrdemAux(Noh* atual, int nivel);   
public:
    Arvore234() { raiz = NULL; }
    ~Arvore234() { delete raiz; }
    // código não implementado, sua tarefa é implementá-lo!
    void insere(Dado umItem);
    Noh* InserirRecursivo(Noh* umNoh, Dado umItem, Dado* itemProm);
    // percorrimento (impressão)
    void percorreEmOrdem();
    Noh* DividirNoh(Noh* umNoh, Dado* itemProm);
    void InserirNohNaoCheio(Noh* umNoh, Dado umItem); // Noh folha
    void InserirNohIntermediarioNaoCheio(Noh* umNoh,Noh* novoFilho, Dado* itemProm);
};

void Arvore234::insere(Dado umItem) {// tratar raiz
    if(raiz == NULL){
		raiz = new Noh;
		raiz->itens[0] = umItem;
		raiz->folha = true;
		raiz->num = MINITENS;
		raiz->pai = NULL;
		
	}else{
		// verificar no e folha ou nao
		Dado* itemProm = new Dado;
		Noh* novoFilho = InserirRecursivo(raiz, umItem, itemProm);
		
		// se novoFilho diferente de null, ajusta raiz
		if(novoFilho != NULL){ // mudanca na raiz
			Noh* RaizAntiga = raiz;
			raiz = new Noh;
			raiz->itens[0] = *itemProm;
			raiz->folha = false;
			raiz->num = MINITENS;
			raiz->pai = NULL;
			raiz->filhos[0] = RaizAntiga;
			RaizAntiga->pai = raiz;
			raiz->filhos[1] = novoFilho;
			novoFilho->pai = raiz;
		}
	}
}

Noh* Arvore234::InserirRecursivo(Noh* umNoh, Dado umItem, Dado* itemProm){
	// verificar se umNoh e folha
	if(umNoh->folha){ // e folha
		// verificar se vazio ou cheio
		if(umNoh->num < MAXITENS){ // noh nao cheio - insere nele
			InserirNohNaoCheio(umNoh, umItem);
			itemProm = NULL;
			return NULL;
			
		} else{// noh cheio - dividir noh
			
			Noh* novoNoh = DividirNoh(umNoh, itemProm);
			
			// inserir umItem
			
			if(umItem < umNoh->itens[MEIO]){
				InserirNohNaoCheio(umNoh, umItem);
			} else{
				InserirNohNaoCheio(novoNoh, umItem);
			}
			
			return novoNoh;
			
		}
		
	} else{ // nao e folha - chamar recursao
		// Achar no folha para inserção
		int pos = umNoh->num - 1;
		
		while(pos >= 0 and umNoh->itens[pos] > umItem){
			pos--;
		}
		
		Noh* novoFilho = InserirRecursivo(umNoh->filhos[pos + 1], umItem, itemProm);

		
		if(novoFilho != NULL){
			if(umNoh->num < MAXITENS){ // verifica se noh cheio ou vazio
				InserirNohIntermediarioNaoCheio(umNoh, novoFilho, itemProm);
				
				itemProm = NULL;
				return NULL;
			} else{
				// divide no intermediario
				Dado* itemPromovidoFilho = new Dado;
				*itemPromovidoFilho = *itemProm;
				
				Noh* novoNoh = DividirNoh(umNoh, itemProm);
				// verificar quem recebe item promovido do filho
				if(*itemPromovidoFilho < umNoh->itens[MEIO]){
					InserirNohIntermediarioNaoCheio(umNoh, novoFilho, itemPromovidoFilho);
				} else{
					InserirNohIntermediarioNaoCheio(novoNoh, novoFilho, itemPromovidoFilho);
				}
				return novoNoh;
			}
		}
	}
	return NULL;
}

Noh* Arvore234::DividirNoh(Noh* umNoh, Dado* itemProm){
	*itemProm = umNoh->itens[MEIO];
	
	Noh* novoNoh = new Noh;
	novoNoh->itens[0] = umNoh->itens[MEIO + 1];
	novoNoh->num = MINITENS;
	novoNoh->folha = umNoh->folha;
	
	umNoh->num = MINITENS;
	
	if(not umNoh->folha){ // dividir filhos
		for(int i = 0; i < MEIO + 1; i++){
			novoNoh->filhos[i] = umNoh->filhos[MEIO + 1 + i];
			umNoh->filhos[MEIO + 1 + i]->pai = novoNoh;
		}
	}
	return novoNoh;
}

void Arvore234::InserirNohIntermediarioNaoCheio(Noh* umNoh,Noh* novoFilho, Dado* itemProm){
	int pos = umNoh->num - 1;
	
	while(pos >= 0 and umNoh->itens[pos] > *itemProm){
		umNoh->itens[pos + 1] = umNoh->itens[pos];
		umNoh->filhos[pos + 2] = umNoh->filhos[pos + 1];
		pos--;
	}
	umNoh->itens[pos + 1] = *itemProm;
	umNoh->filhos[pos + 2] = novoFilho;
	novoFilho->pai = umNoh;
	
	umNoh->num++;
}

void Arvore234::InserirNohNaoCheio(Noh* umNoh, Dado umItem){
	int pos = umNoh->num - 1;
	
	while(pos >= 0 and umNoh->itens[pos] > umItem){
		umNoh->itens[pos + 1] = umNoh->itens[pos];
		pos--;
	}
	umNoh->itens[pos + 1] = umItem;
	umNoh->num++;
}

void Arvore234::percorreEmOrdem() {
    percorreEmOrdemAux(raiz,0);        
    cout<< endl;
}

void Arvore234::percorreEmOrdemAux(Noh* umNoh, int nivel) {
    int i;
    for (i = 0; i < umNoh->num; i++) {
        // se nó não é folha, imprima os dados do filho i
        // antes de imprimir o item i
        if (not umNoh->folha) {
            percorreEmOrdemAux(umNoh->filhos[i], nivel+1); 
        }
        cout << umNoh->itens[i] << '/' << nivel << ' ';
    }
 
    // Imprima os dados do último filho
    if (not umNoh->folha) {
        percorreEmOrdemAux(umNoh->filhos[i], nivel+1);   
    }
}


// =========================== Programa ================================
int main() {
    Arvore234* B = new Arvore234;
    
    char opcao;
    float valor;
    
    do {
        cin >> opcao;
        if (opcao == 'i') {
            cin >> valor;
            B->insere(valor);
        } else if(opcao == 'e') {
            B->percorreEmOrdem();
        }
    } while(opcao != 'q');
    
    
    delete B;    
    
    return 0;
}
