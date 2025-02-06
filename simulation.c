#include <stdio.h>
#include <stdlib.h>
#include <emscripten.h>
#include <math.h>
typedef struct {
    const char* name;
    double alpha;
    double beta;
    double gamma;
} Material;
Material materials[] = {
    {"Lead", 0.1, 0.5, 0.5},
    {"Concrete", 0.01, 0.1, 0.1},
    {"Water", 0.005, 0.05, 0.05},
    {"Aluminum", 0.02, 0.2, 0.2}
};
const int num_materials = 4;
EMSCRIPTEN_KEEPALIVE
int simulate_absorption(const char* material_name, const char* radiation_type, int num_particles, double thickness) {
    double mu = 0;
    
    for (int i = 0; i < num_materials; i++) {
        if (strcmp(materials[i].name, material_name) == 0) {
            if (strcmp(radiation_type, "alpha") == 0) {
                mu = materials[i].alpha;
            } else if (strcmp(radiation_type, "beta") == 0) {
                mu = materials[i].beta;
            } else if (strcmp(radiation_type, "gamma") == 0) {
                mu = materials[i].gamma;
            }
            break;
        }
    }
    if (mu == 0) return -1; // Invalid input
    int absorbed = 0;
    for (int i = 0; i < num_particles; i++) {
        double distance = -log(1.0 - (double)rand() / RAND_MAX) / mu;
        if (distance < thickness) absorbed++;
    }
    return absorbed;
}
int main() {
    int absorbed = simulate_absorption("Lead", "gamma", 10000, 5.0);
    printf("Absorbed particles: %d\n", absorbed);
    return 0;
}
