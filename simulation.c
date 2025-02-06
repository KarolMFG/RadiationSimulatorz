#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emscripten.h>
#include <math.h>
#include <time.h>
typedef struct {
    const char* name;
    double alpha;
    double beta;
    double gamma;
    double density;
} Material;
Material materials[] = {
    {"Lead", 0.1, 0.5, 0.5, 11.34},
    {"Concrete", 0.01, 0.1, 0.1, 2.3},
    {"Water", 0.005, 0.05, 0.05, 1.0},
    {"Aluminum", 0.02, 0.2, 0.2, 2.7}
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
        double random_val = (double)rand() / RAND_MAX;
        double distance = -log(1.0 - random_val) / mu;
        if (distance < thickness) absorbed++;
    }
    return absorbed;
}
EMSCRIPTEN_KEEPALIVE
void simulate_multiple(const char* material_name, const char* radiation_type, int num_particles, double* thicknesses, int num_thicknesses, int* results) {
    for (int i = 0; i < num_thicknesses; i++) {
        results[i] = simulate_absorption(material_name, radiation_type, num_particles, thicknesses[i]);
    }
}
EMSCRIPTEN_KEEPALIVE
void list_materials() {
    printf("Available Materials:\n");
    for (int i = 0; i < num_materials; i++) {
        printf("%s (Density: %.2f g/cm^3)\n", materials[i].name, materials[i].density);
    }
}
int main() {
    srand(time(NULL)); // Seed random generator
    list_materials();
    int absorbed = simulate_absorption("Lead", "gamma", 10000, 5.0);
    printf("Absorbed particles in 5cm Lead: %d\n", absorbed);
    return 0;
}
