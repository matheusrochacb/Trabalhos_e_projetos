#include <iostream>
using namespace std;

class Noh{
    friend class ABB;
private:
    int mValor;
    int mNivel;
    Noh* mEsq;
    Noh* mDir;
    Noh* mPai;

public:
    Noh(int valor);
    ~Noh();
    int AtualizarNivel();
    void InserirAux(int valor);
    bool RemoverAux(int valor);
    Noh* BuscarAux(int valor);
    Noh* MinimoAux();
    void PosOrdemAux(Noh* umNoh);

    
};

Noh::Noh(int valor){
    mValor = valor;
    mEsq = NULL;
    mDir = NULL;
    mPai = NULL;
    mNivel = 0;
}

Noh::~Noh(){
    if(mEsq == NULL)
        delete mEsq;
    if(mDir == NULL)
        delete mDir;
    if(mPai == NULL)
        delete mPai;
}

Noh::AtualizarNivel(){
    if(mPai == NULL){
        return 0;
    }
    return 1 + mPai->AtualizarNivel();
}

class ABB{
private:
    Noh* mRaiz;

public:
    ABB();
    ~ABB();
    void InserirABB(int valor);
    void RemoverABB(int valor);
    void PreOrdem(Noh* umNoh);
    void EmOrdem(Noh* umNoh);
    void DestruirRecursivo(Noh* umNoh); // Pos-Ordem para o destrutor
    Noh* Minimo();
    void Transplantar(Noh* antigo, Noh* novo);
};

ABB::ABB(){
    mRaiz = NULL;
}

ABB::~ABB(){
    DestruirRecursivo(mRaiz);
}

void ABB::DestruirRecursivo(Noh* umNoh){
    umNoh->PosOrdemAux(umNoh);
}

void Noh::PosOrdemAux(Noh* umNoh){
    if(umNoh != NULL){
        PosOrdemAux(umNoh->mEsq);
        PosOrdemAux(umNoh->mDir);
        delete umNoh;
    }
}

void ABB::PreOrdem(Noh* umNoh){
    if(umNoh != NULL){
        cout << umNoh->mValor << "/" << umNoh->mNivel << " ";
        PreOrdem(umNoh->mEsq);
        PreOrdem(umNoh->mDir);
    }
}

void ABB::EmOrdem(Noh* umNoh){
    if(umNoh != NULL){
        EmOrdem(umNoh->mEsq);
        cout << umNoh->mValor << "/" << umNoh->mNivel << " ";
        EmOrdem(umNoh->mDir);
    }
}

void ABB::InserirABB(int valor){
    if(mRaiz == NULL){
        Noh* novo = new Noh(valor);
        mRaiz = novo;
    } else{
        mRaiz->InserirAux(valor);
    }
}

void Noh::InserirAux(int valor){
    
    Noh* novo = new Noh(valor);
    Noh* anterior;
    Noh* atual = this;
    
    while(atual != NULL){
        anterior = atual;
        
        if(valor < atual->mValor){
            atual = atual->mEsq;
        } else{
            atual = atual->mDir;
        }
    }
    
    novo->mPai = anterior;
    if(valor < anterior->mValor){
        anterior->mEsq = novo;
    } else{
        anterior->mDir = novo;
    }
    
    novo->AtualizarNivel();
}

Noh* Noh::BuscarAux(int valor){
    Noh* aRetornar = this;
    
    while(aRetornar != NULL){
        if(aRetornar->mValor == valor){
            return aRetonar;
        }else{
            if(valor < aRetornar->mValor){
                aRetornar = aRetornar->mEsq;
            } else{
                aRetornar = aRetornar->mDir;
            }
        }
    }
    
    return aRetornar;
}

Noh* ABB::Minimo(){
    
    return mRaiz->MinimoAux();
}

Noh* Noh::MinimoAux(){
    while(mEsq != NULL){
        mEsq->MinimoAux();
    }
    
    return this;
}

void ABB::Transplantar(Noh* antigo, Noh* novo){
    if(antigo = mRaiz){
        mRaiz = novo;
    }else if(antigo = antigo->mPai->mEsq){
        antigo->mPai->mEsq = novo;
    } else{
        antigo->mPai->mDir = novo;
    }
    
    if(novo != NULL){
        novo->mPai = antigo->mPai;
    }
}

bool ABB::RemoverABB(int valor){
    if(mRaiz == NULL){
        return false;
    } else{
        bool retorno = mRaiz->RemoverAux(valor);
    }
}

bool Noh::RemoverAux(int valor){
    Noh* aRemover = BuscarAux(valor);
    
    if(aRemover == NULL){
        return false;
    } else{
        if(aRemover->mEsq == NULL){
            Transplantar(aRemover, aRemover->mDir);
        }else if{
            
        }else{
            
        }
    }
    
    
}

int main(){



    return 0;
}
