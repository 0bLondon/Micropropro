#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "float.h"

#define NUM_E 5

struct RK_result_t {
    double psi;
    double psi_p;
} RK_result_t;

const int L = 10;
const double dx = .001;
const int n_steps = L/dx;

void linspace(double start, double end, double n_steps, double *buff){
    double step = (end - start)/n_steps;
    buff[0] = start;
    for (int i = 1; i < n_steps; ++i){
        buff[i] = buff[i-1] + step;
    }
}

double potential(double x){
    if (x < 0){
        return DBL_MAX;
    }
    return x;
}

double dpsi_pdx(double x, double psi, double psi_p, double E){
    (void) psi_p;
    return -2 * (E - potential(x)) * psi;
}

double dpsidx(double x, double psi, double psi_p, double E){
    (void) x; 
    (void) psi;
    (void) E;
    return psi_p;
}



struct RK_result_t rungeKutta(double x, double psi, double psi_p, double dx,
 double f1(double x, double psi, double psi_p, double E), 
 double f2(double x, double psi, double psi_p, double E) ,double E){

    double k1 = dx * f1(x, psi, psi_p, E);
    double l1 = dx * f2(x, psi, psi_p, E);

    double k2 = dx * f1(x + .5*dx, psi+.5*l1, psi_p+.5*k1, E); 
    double l2 = dx * f2(x + .5*dx, psi+.5*l1, psi_p+.5*k1, E);
    
    double k3 = dx * f1(x + .5*dx, psi+.5*l2, psi_p+.5*k2, E); 
    double l3 = dx * f2(x + .5*dx, psi+.5*l2, psi_p+.5*k2, E);
    
    double k4 = dx * f1(x + dx, psi+l3, psi_p + k3, E);
    double l4 = dx * f2(x + dx, psi+l3, psi_p + k3, E); 
  
    psi_p = psi_p + (1.0 / 6.0)*(k1 + 2 * k2 + 2 * k3 + k4);
    psi = psi + (1.0 / 6.0)*(l1 + 2 * l2 + 2 * l3 + l4);
    
    struct RK_result_t rk = {.psi = psi, .psi_p=psi_p};
    return rk;
}

int main(){
    double x[n_steps];
    linspace(0,L,n_steps,x);
    
    double root1_rg = 1.8558807589337636;
    double root2_rg = 3.2448238992006146;
    double root3_rg = 4.381963325045131;
    double root4_rg = 5.386972869457468;
    double root5_rg = 6.305683553878785;

    double E[NUM_E] = {root1_rg, root2_rg, root3_rg, root4_rg, root5_rg};

    for (int i = 0; i < NUM_E; ++i){
        // Set initial values
        double psi_p[n_steps];
        double psi[n_steps];
        psi[0] = 0.0f;
        psi_p[0] = 1.0f;
    
        struct RK_result_t ppp;
        for (int j = 1; j < n_steps; ++j){
            ppp = rungeKutta(x[j-1], psi[j-1], psi_p[j-1], dx, &dpsi_pdx, &dpsidx, E[i]);
            psi[j] = ppp.psi;
            psi_p[j] = ppp.psi_p;
        }
    }
}