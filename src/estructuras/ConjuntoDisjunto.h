#ifndef CONJUNTO_DISJUNTO_H
#define CONJUNTO_DISJUNTO_H

class ConjuntoDisjunto {
private:
    int* representante;
    int tamano;

    void copiarDesde(const ConjuntoDisjunto& otra);

public:
    ConjuntoDisjunto();
    ConjuntoDisjunto(int n);
    ConjuntoDisjunto(const ConjuntoDisjunto& otra);
    ConjuntoDisjunto& operator=(const ConjuntoDisjunto& otra);
    ~ConjuntoDisjunto();

    void reiniciar(int n);
    int encontrar(int x) const;
    bool unir(int a, int b);
    bool mismoConjunto(int a, int b) const;
    int obtenerTamano() const;
};

#endif
