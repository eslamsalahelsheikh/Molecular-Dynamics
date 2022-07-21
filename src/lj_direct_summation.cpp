
# include "../include/Molecular-Dynamics/lj_direct_summation.h"

double lj_direct_summation(Atoms &atoms, double epsilon, double sigma) {
    double potential_energy = 0.0;
    atoms.forces.setZero();
    for (int i = 0; i < atoms.nb_atoms(); i++) {
        for (int j = i + 1; j < atoms.nb_atoms(); j++) {
            Eigen::Array3d r_ij{atoms.positions.col(i) - atoms.positions.col(j)}; // r_ij
            double distance = r_ij.matrix().norm();            // Euclidean distance
            double sig_over_r_ij = sigma / distance;
            double LJ_potential = 4 * epsilon * (pow(sig_over_r_ij, 12) - pow(sig_over_r_ij, 6));  // Leonard Jones potential energy between two atoms with distance r_ij
            Forces_t F_ij = 48 * epsilon * pow(sigma, 12) / pow(r_ij, 13)
                               - 24 * epsilon * pow(sigma, 6) / pow(r_ij, 7);  // force between two atoms with distance r_ij

            // TODO: check if this is correct
            atoms.forces.col(i) += F_ij;                              // add force to atom i
            potential_energy += LJ_potential;
        }
    }
    return potential_energy;
}