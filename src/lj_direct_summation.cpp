
# include "../include/Molecular-Dynamics/lj_direct_summation.h"
# include <iostream>

double lj_direct_summation(Atoms &atoms, double epsilon, double sigma) {
    double potential_energy = 0.0;
    atoms.forces.setZero(); // set all forces to zero
    for (int i = 0; i < atoms.nb_atoms(); i++) {
        for (int j = i + 1; j < atoms.nb_atoms(); j++) {
            Eigen::Array3d r_ij =
                    atoms.positions.col(i) - atoms.positions.col(j); // calculate the distance between every two atoms
            double distance = r_ij.matrix().norm();                               // Euclidean distance
            double sig_over_r_ij = sigma / distance;                              // sigma / r_ij
            // Leonard Jones potential energy between two atoms with distance r_ij
            double LJ_potential = 4 * epsilon * (pow(sig_over_r_ij, 12) - pow(sig_over_r_ij, 6));
            // Force between two atoms with distance r_ij
            Eigen::Array3Xd F_ij = (48 * epsilon * pow(sigma, 12.0) / pow(distance, 14.0)
                                    - 24 * epsilon * pow(sigma, 6.0) / pow(distance, 8.0)) * r_ij;
            atoms.forces.col(i) += F_ij;        // adding the force to atom i
            atoms.forces.col(j) -= F_ij;        // subtracting the force to atom j
            potential_energy += LJ_potential;   // adding the potential energy to the total potential energy
        }
    }
    return potential_energy;
}

double lj_direct_summation_neighbors(Atoms &atoms, NeighborList &neighbor_list, double epsilon, double sigma) {
    // compute the shifting potential energy
    double cutoff_radius = neighbor_list.get_cutoff_radius();
    double V_rc = 4 * epsilon * (pow(sigma / cutoff_radius, 12) - pow(sigma / cutoff_radius, 6));

    double potential_energy = 0.0;
    atoms.forces.setZero(); // set all forces to zero
    for (auto [i, j]: neighbor_list) {
        if (i < j) {
            Eigen::Array3d r_ij =
                    atoms.positions.col(i) - atoms.positions.col(j); // calculate the distance between every two atoms
            double distance = r_ij.matrix().norm();                               // Euclidean distance
            double sig_over_r_ij = sigma / distance;                              // sigma / r_ij
            // Leonard Jones potential energy between two atoms with distance r_ij
            double LJ_potential = 4 * epsilon * (pow(sig_over_r_ij, 12) - pow(sig_over_r_ij, 6));
            // Force between two atoms with distance r_ij
            Eigen::Array3Xd F_ij = (48 * epsilon * pow(sigma, 12.0) / pow(distance, 14.0)
                                    - 24 * epsilon * pow(sigma, 6.0) / pow(distance, 8.0)) * r_ij;
            atoms.forces.col(i) += F_ij;        // adding the force to atom i
            atoms.forces.col(j) -= F_ij;        // subtracting the force to atom j
            potential_energy += LJ_potential -
                                V_rc;  // adding the potential energy to the total potential energy and subtracting the shifting potential energy
        }
    }
    return potential_energy;
}