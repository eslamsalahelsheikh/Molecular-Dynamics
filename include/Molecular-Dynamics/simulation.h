#ifndef MOLECULARDYNAMICS_SIMULATION_H
#define MOLECULARDYNAMICS_SIMULATION_H

#include "../../../include/Molecular-Dynamics/energy.h"
#include "../../../include/Molecular-Dynamics/verlet.h"
#include "../../../include/Molecular-Dynamics/xyz.h"
#include "../../../include/Molecular-Dynamics/utils.h"
#include <filesystem>

class SimulationData {
public:
    SimulationData() {
        // Initialize all simulation parameters
        mass = 196.9665; // atomic mass of Gold (https://www.nuclear-power.com/gold-atomic-number-mass-density/)
        sigma = 1.0;
        epsilon = 1.0;
        total_steps = 1000;
        time_step = 0.01; // time step in fs
        cutoff_radius = 10.0;    // cutoff radius for LJ potential
        relaxation_time_multiplier = 30.0; // relaxation time = relaxation time multiplier * time_step in fs
        relaxation_time_multiplier_final_value = 1000; // after the system arrives at desired temp
        desired_temperature = 0.01; // desired temperature in K
        relaxation_steps = 100;  // 150 can be enough for 5 cutoff or 60 for 10 cutoff radius
        expermint_num = 50;
        add_energy = 30;  // 30 is equivilant to 20 temp, 7 is around 4.5
        cluster_name = "cluster_3871";
        continue_old_experiment = true;
        old_experiment_file = "/home/eslam/Desktop/Molecular-Dynamics/output/milestone_07/3871/traj_3871_990_initial.xyz";
    }
    double expermint_num; // number of experiments
    bool continue_old_experiment; // continue old experiment or not
    std::string cluster_name; // name of the cluster
    std::string old_experiment_file; // name of the old experiment file

protected:
    double mass;
    double sigma;
    double epsilon;
    double total_steps;
    double time_step;
    double cutoff_radius;
    double relaxation_time_multiplier;
    double relaxation_time_multiplier_final_value;
    double relaxation_time;     // relaxation time
    double desired_temperature; // desired temperature
    double relaxation_steps; // steps for relaxation after adding heat
    double add_energy; // energy added in each experiment
};

class Simulation : public SimulationData, public Energy {

public:
    // Constructors
    Simulation();
    Simulation(Atoms &new_atoms);
    // Destructor
    ~Simulation();
    // Methods
    void initial_loop();
    double relaxation_loop(int iteration);
    void add_heat();
private:
    // initialize members
    Atoms atoms_;
    NeighborList neighbor_list_;
    std::ofstream traj_;
};




#endif //MOLECULARDYNAMICS_SIMULATION_H
