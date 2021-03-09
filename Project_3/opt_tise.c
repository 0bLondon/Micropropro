#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "assert.h"
#include "float.h"

#define NUM_E 5

#define V(x) x
#define dPsi_p_dx(a,b,c,d) -2 * (d - V(a)) * b
#define dPsi_dX(a,b,c,d) c


struct RK_result_t {
    double psi;
    double psi_p;
} RK_result_t;

static inline void linspace(double start, double end, double n_steps, double *buff){
    double step = (end - start)/n_steps;
    buff[0] = start;
    for (int i = 1; i < n_steps; ++i){
        buff[i] = buff[i-1] + step;
    }
}

#define K1 (dx * dPsi_p_dx(x, psi, psi_p, E))
#define L1 (dx * dPsi_dX(x, psi, psi_p, E))

#define K2 (dx * dPsi_p_dx(x + .5*dx, psi+.5*L1, psi_p+.5*K1, E))
#define L2 (dx * dPsi_dX(x + .5*dx, psi+.5*L1, psi_p+.5*K1, E))

#define K3 (dx * dPsi_p_dx(x + .5*dx, psi+.5*L2, psi_p+.5*K2, E))
#define L3 (dx * dPsi_dX(x + .5*dx, psi+.5*L2, psi_p+.5*K2, E))

#define K4 (dx * dPsi_p_dx(x + dx, psi+L3, psi_p + K3, E))
#define L4 (dx * dPsi_dX(x + dx, psi+L3, psi_p + K3, E))

#define PSI_P (psi_p + (1.0 / 6.0)*(K1 + 2 * K2 + 2 * K3 + K4))
#define PSI (psi + (1.0 / 6.0) * (L1 + 2 * L2 + 2 * L3 + L4))

static inline struct RK_result_t rungeKutta(double x, double psi, double psi_p, double dx, double E) {
    struct RK_result_t rk = {.psi = PSI, .psi_p=PSI_P};
    return rk;
}

#define ROOT1 1.8558807589337636
#define ROOT2 3.2448238992006146
#define ROOT3 4.381963325045131
#define ROOT4 5.386972869457468
#define ROOT5 6.305683553878785

// no error checking because speed
int main(int argc, char* argv[]){
    int L = 10;
    int n_steps = atoi(argv[1]);
    double dx = L/(double)n_steps;
    
    // assert(L <= 10 && (dx < L && dx > 0) && (n_steps <= 349432)); // Comment out for speed

    double x[n_steps];
    linspace(0,L,n_steps,x);
    double E[NUM_E] = {ROOT1, ROOT2, ROOT3, ROOT4, ROOT5};

    for (int i = 0; i < NUM_E; ++i){
        // Set initial values
        double psi_p[n_steps];
        double psi[n_steps];
        psi[0] = 0.0f;
        psi_p[0] = 1.0f;
        struct RK_result_t ppp;
        for (int j = 1; j < n_steps; ++j){
            ppp = rungeKutta(x[j-1], psi[j-1], psi_p[j-1], dx, E[i]);
            psi[j] = ppp.psi;
            psi_p[j] = ppp.psi_p;
        }
    }
}