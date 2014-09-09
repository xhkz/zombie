#include <iostream>
#include <stdlib.h>
#include <omp.h>
using namespace std;

#define STEPS   1000 
#define SIZE    100
#define MOVE    0.1

#if defined(_OPENMP)
void lock(int i, bool *locks) {
    for (bool locked = false; locked == false; /*NOP*/) {
        #pragma omp critical (LockRegion)
        {
            locked = !locks[i-1] && !locks[i] && !locks[i+1];
            if (locked) {
                locks[i-1] = true;
                locks[i] = true;
                locks[i+1] = true;
            }
        }
    }
}

void unlock(int i, bool *locks) {
    #pragma omp critical (LockRegion)
    {
        locks[i-1] = false;
        locks[i] = false;
        locks[i+1] = false;
    }
}
#endif

bool **CreateMesh(int I, int J) {
    bool **Mesh = new bool *[I];
    for (int i = 0; i < I; i++) {
        Mesh[i] = new bool[J];
        for (int j = 0; j < J; j++)
            Mesh[i][j] = false;
    }
    return(Mesh);
}

void print(bool **Mesh, int t) {
    int populationTotal = 0;

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            if (Mesh[i][j] == true) populationTotal++;
        } 
    }

    cout << t << "\t" << populationTotal << std::endl;
}

void print_matrix(bool **Mesh, int t) {
    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            cout << (Mesh[i][j] == true ? "p" : "e") << ",";
        } 
        cout << std::endl;
    }
}

int main(int argc, char **argv) {
    srand48(8767134);
    bool *locks = new bool[SIZE + 2];

    for (int i = 0; i < SIZE + 2; i++)
        locks[i] = false;

    bool **MeshA = CreateMesh(SIZE + 2, SIZE + 2);
    bool **MeshB = CreateMesh (SIZE + 2, SIZE + 2);

    for (int i = 1; i <= SIZE; i++) {
        for (int j = 1; j <= SIZE; j++) {
            if (drand48() < 0.10)
                MeshA[i][j] = true;
        }
    }

    print(MeshA, 0);

    for (int n = 0; n < STEPS; n++) {
        #ifdef _OPENMP
        //#pragma omp parallel for default(none) private(locks) shared(MeshA,MeshB,n)
        #pragma omp parallel for default(none) shared(MeshA,MeshB,n,locks)
        #endif
        for (int i = 1; i <= SIZE; i++) {
            #if defined(_OPENMP)
            lock(i, locks);
            #endif
            for (int j = 1; j <= SIZE; j++) {
                if (MeshA[i][j] == true) {
                    MeshA[i][j] = false;
                    double move = drand48();
                    if (move < 1.0*MOVE && MeshA[i-1][j] == false && MeshB[i-1][j] == false && i > 1) {
                        MeshB[i-1][j] = true;
                    } else if (move < 2.0*MOVE && MeshA[i+1][j] == false && MeshB[i+1][j] == false && i < SIZE) {
                        MeshB[i+1][j] = true;
                    } else if (move < 3.0*MOVE && MeshA[i][j-1] == false && MeshB[i][j-1] == false && j > 1) {
                        MeshB[i][j-1] = true;
                    } else if (move < 4.0*MOVE && MeshA[i][j+1] == false && MeshB[i][j+1] == false && j < SIZE) {
                        MeshB[i][j+1] = true;
                    } else {
                        MeshB[i][j] = true;
                    }
                } 
            }
            #if defined(_OPENMP)
            unlock(i, locks);
            #endif
        }

        // move point out of boundary back to matrix

        swap(MeshA, MeshB);
        print(MeshA, n+1);
    }

    return 0;
}
