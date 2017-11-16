#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "initfcc.h"
#include "alpotential.h"

#define N 256

/* Main program */
int main()
{
    //========================================================================//
    // Task specific - H1.1
    //========================================================================//
    double  pos[N][3]; // Position
    double  f[N][3]; // Forces
    double lattice[4*N*N*N][3];
    int Nc = 4; // #primitive calls in each direction                           // Segfault sensitive - 4 seems to be about max
    double a0 = 0.1; // Lattice parameter
    double L = 1; // Length of supercell

    //========================================================================//
    // Setup
    //========================================================================//
    int i, i_log;                                                               // i - actual timestep, i_log - logging of timestep data
    double dt = 0.1;
	double t_max = 250;
	int nbr_of_timesteps = t_max/dt;
	int ir = 100; // Resolution for i. Record every ir:th timestep.             // Segfault sensitive.
        
    // Data recording
    double log_data1 [nbr_of_timesteps/ir];
    double log_data2 [nbr_of_timesteps/ir];
    double log_data3 [nbr_of_timesteps/ir];
    
    //========================================================================//
    // Verlet
    //========================================================================//
    printf("\nLog resolution: %d, t_max = %.3f \n", ir, t_max);
    for (i = 1; i < nbr_of_timesteps; i++) {
    	if (i%(nbr_of_timesteps/10) == 0) { // Print progress - 10%
    		printf("\tt = %.2f \t\t %.3f  \n", i*dt, ((double)i/nbr_of_timesteps));
    	}
    	
    	// Verlet steps
    	
    	
    	if (i%ir == 0) { // Time to log data?
    	    i_log = i/ir;
            //================================================================//
            // Log step - record data
            //================================================================//
            
            log_data1[i_log] = 1;
            log_data2[i_log] = 2;
            log_data3[i_log] = 3;
        }
    }
    printf("\tt = %.2f \t\t %.3f  \n", i*dt, ((double)i/nbr_of_timesteps));
    
    //====================================================================//
    // Simulation complete - print data to file(s)
    // fopen("filename", acc), acc \in {"r", "w", "o"} (read, write, append)
    //====================================================================//
    FILE * file1 = fopen("energy.dat", "w");
    if (file1 != NULL){
    	printf("Print to file.");

    	for (i = 0; i < nbr_of_timesteps/ir; i++) { // i are log steps now!
    	    double t = ir*i*dt;  
    	    printf("%e \n", t);
    		fprintf (file1,"%e \t %e \t %e \t %e     \n",
    			t, // Time
    		    log_data1[i], log_data2[i], log_data3[i]); // data
    	}
    	
    	
    	// Close file(s)
    	fclose(file1);
    	printf("Data printed!\n");
    } else {
    	printf("A file is NULL!\n");
    }
    
    return 1;
    
    
    /*
    int bound = 4;
    for (i = -8; i <= 7; i++) {
        int bounded = periodic_bound(i, bound);
        printf("%d\n", bounded);
    }
    printf("Looks aight.\n");
    */
    
    /*
     Code for generating a uniform random number between 0 and 1. srand should only
     be called once.
    */
    //srand(time(NULL));
    double random_value = 0;
    //random_value = (double) rand() / (double) RAND_MAX;
    printf("random_value = %.5f \n", random_value);

    /*
     Descriptions of the different functions in the files initfcc.c and
     alpotential.c are listed below.
    */

    /* 
     Function that generates a fcc lattice in units of [Å]. Nc is the number of 
     primitive cells in each direction and a0 is the lattice parameter. The
     positions of all the atoms are stored in pos which should be a matrix of the
     size N x 3, where N is the number of atoms. The first, second and third column
     correspond to the x,y and z coordinate respectively.
    */
    init_fcc(lattice, Nc, a0);
    
    /* 
     Function that calculates the potential energy in units of [eV]. pos should be
     a matrix containing the positions of all the atoms, L is the length of the 
     supercell and N is the number of atoms.
    */
    double energy;
    energy = get_energy_AL(pos, L, N);
    printf("energy = %.5f", energy);
    
    /* 
     Function that calculates the virial in units of [eV]. pos should be a matrix
     containing the positions of all the atoms, L is the length of the supercell 
     and N is the number of atoms.
    */
    //double virial;
    //virial = get_virial_AL(pos, L, N);
    //printf("virial = %.5f", virial);
    
    
    /*
     Function that calculates the forces on all atoms in units of [eV/Å]. the 
     forces are stored in f which should be a matrix of size N x 3, where N is the
     number of atoms and column 1,2 and 3 correspond to the x,y and z component of
     the force resepctively . pos should be a matrix containing the positions of 
     all the atoms, L is the length of the supercell and N is the number of atoms.
    */
    //get_forces_AL(f,pos, L, N);  

    return 0; // Prevent err printout
}
