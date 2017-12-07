#include <stdio.h>
#include <gsl/gsl_randist.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "stat.h"

#define N 15000
#define TIME_MAX 0.001
#define PI 3.14159265359

void calc_acc(double *a, double *q, double *v, double m, double k, double eta, gsl_rng *gslr);
gsl_rng * init_rng();

//==============//
// Main program //
//==============//
int main()
{
    //========================================================================//
    // Task specific - E4
    //========================================================================//
    char output_file[255];
    sprintf(output_file, "e4.dat", 0);

    double T = 297;
    double k_B = 1.38065e-23;
    double m = 30.131e-15;
    double f_0 = 3.0000e+3;
    double omega_0 = 2*PI*f_0;
    double k = m*omega_0*omega_0;

    double dt = 0.0000001;
    double q[N]; // position
    double v[N]; // velocity
    double a[N]; // acceleration
    
    double Tau_a = 48.5e-6;  // relaxation time in microseconds, case A
    double Tau_b = 147.3e-6; // case B
    double Tau = Tau_a;
    double eta = 1/Tau;
    int Tau_i = 0;
    double Tau_log[6] = {0.1*Tau, 0.5*Tau, Tau, 2*Tau, 10*Tau, 10000000*Tau}; // Values for which to log offset/vel
    
    double c_0 = exp(-eta*dt);
    double v_th = sqrt(k_B*T/m);
    //printf("k_B/m = %.5f\n", k_B/m);
    
    double x_0 = 0.1e-6; // micrometers
    double v_0 = 2.0e-3; // micrometers per microsecond
    
    double mu_q, sigma_sq_q;
    double mu_v, sigma_sq_v;

    double G_1, G_2;
    
    // Log offsets at certain timepoints
    char tauname[255];
    FILE * taufile;
    
    //========================================================================//
    // Setup
    //========================================================================//
    gsl_rng *gslr = init_rng();
    int i, j, i_log;        
    double t_max = TIME_MAX;
    int nbr_of_timesteps = t_max/dt;
    int ir = 1; // Resolution for i. Record every ir:th timestep.
    
    // Data recording
    double log_data1 [nbr_of_timesteps/ir]; // mu_q
    double log_data2 [nbr_of_timesteps/ir]; // sigma_sq_q
    double log_data3 [nbr_of_timesteps/ir]; // mu_v
    double log_data4 [nbr_of_timesteps/ir]; // sigma_sq_v
    
    double log_data5 [nbr_of_timesteps/ir]; // Trajectory 1
    double log_data6 [nbr_of_timesteps/ir]; // Trajectory 2
    double log_data7 [nbr_of_timesteps/ir]; // Trajectory 3
    double log_data8 [nbr_of_timesteps/ir]; // Trajectory 4
    double log_data9 [nbr_of_timesteps/ir]; // Trajectory 5
    
    double log_data10 [nbr_of_timesteps/ir]; // Speed 1
    double log_data11 [nbr_of_timesteps/ir];
    double log_data12 [nbr_of_timesteps/ir];
    double log_data13 [nbr_of_timesteps/ir];
    double log_data14 [nbr_of_timesteps/ir]; // Speed 5
    
    //========================================================================//
    // Verlet
    //========================================================================//
    printf("\nLog resolution: 1 per %d steps, t_max = %.3f, nbr_of_timesteps = %d\n", ir, t_max, nbr_of_timesteps);
    
    for (j = 0; j < N; j++) { // Initial conditions
        q[j] = x_0;
        v[j] = v_0;
    }
    calc_acc(a, q, v, m, k, eta, gslr);
    for (i = 1; i < nbr_of_timesteps; i++) {
        if (10*(i-1)%(nbr_of_timesteps) == 0) { // Print progress
            printf("\tt = %.2f \t\t %.3f  \n", (i-1)*dt, ((double)(i-1)/nbr_of_timesteps));
        }
          

        //======================================//
        // Verlet
        //======================================//
        for (j = 0; j < N; j++) { // v(t+dt/2)
            G_1 = gsl_ran_ugaussian(gslr);
            v[j] = 0.5*a[j]*dt + sqrt(c_0)*v[j] + v_th*sqrt(1-c_0)*G_1;
        }
        
        for (j = 0; j < N; j++) { // q(t+dt)
            q[j] += v[j]*dt;
        }

        //=====================//
        // Accelerations
        //=====================//
        calc_acc(a, q, v, m, k, eta, gslr);
        
        for (j = 0; j < N; j++) { // v(t+dt)
            G_2 = gsl_ran_ugaussian(gslr);
            v[j] = 0.5*sqrt(c_0)*a[j]*dt + sqrt(c_0)*v[j] + v_th*sqrt(1-c_0)*G_2;
        }
        
        //====================================================================//
        // Record data
        //====================================================================//
        if ((i-1)%ir == 0) { // Time to log data?
            i_log = (i-1)/ir;         
                
            // Mu and sigma squared
            mu_q = get_mean(q, N);
            sigma_sq_q = get_variance(q, mu_q, N);
            mu_v = get_mean(v, N);
            sigma_sq_v = get_variance(v, mu_v, N);
            log_data1[i_log] = mu_q;
            log_data2[i_log] = sigma_sq_q;
            log_data3[i_log] = mu_v;
            log_data4[i_log] = sigma_sq_v;
            
            // Trajectories
            log_data5[i_log] = q[0];
            log_data6[i_log] = q[(N-1)/4];
            log_data7[i_log] = q[(N-1)/2];
            log_data8[i_log] = q[3*(N-1)/4];
            log_data9[i_log] = q[N-1];
            
            // Speeds
            log_data10[i_log] = v[0];
            log_data11[i_log] = v[(N-1)/4];
            log_data12[i_log] = v[(N-1)/2];
            log_data13[i_log] = v[3*(N-1)/4];
            log_data14[i_log] = v[N-1];
            
            if (i*dt > Tau_log[Tau_i]) {
                printf("Log offset/velocity: t = %.2f*Tau\n", i*dt/Tau);
                // Output offset for all particles
                sprintf(tauname, "offset_tau%d.dat", Tau_i);
                taufile = fopen(tauname, "w");
                for (j = 0; j < N; j++) {          
                    fprintf (taufile,"%e\n", q[j]);
                }
                printf("\t Printed: %s\n", tauname);
                fclose(taufile);
                
                // Output velocity for all particles
                sprintf(tauname, "vel_tau%d.dat", Tau_i);
                
                taufile = fopen(tauname, "w");
                for (j = 0; j < N; j++) {          
                    fprintf (taufile,"%e\n", v[j]/v_th);
                }
                printf("\t Printed: %s\n", tauname);
                fclose(taufile);
                
                Tau_i++;
            }
        }
    }
    printf("\tt = %.2f \t\t %.3f  \n", i*dt, ((double)i/nbr_of_timesteps));
    
    //====================================================================//
    // Simulation complete - print data to file(s)
    //====================================================================//
    FILE * file1 = fopen(output_file, "w");
    if (file1 != NULL){
        for (i = 0; i < nbr_of_timesteps/ir; i++) { // i are log steps now!
            double t = ir*i*dt;
            
            // Print file1
            fprintf (file1,"%e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e %e\n",
                t, // Time
                log_data1[i], log_data2[i], //avg q(t), mu/sigma
                log_data3[i], log_data4[i], //avg v(t), mu/sigma
                log_data5[i], log_data6[i], log_data7[i], log_data8[i], log_data9[i], //Individual trajectories q_j(t)
                log_data10[i], log_data11[i], log_data12[i], log_data13[i], log_data14[i] //Individual velocities v_j(t)
                );
        }
        
        
        // Close file(s)
        fclose(file1);
        printf("Data printed to file: \"%s\".\n", output_file);
    } else {
        printf("File is NULL!\n");
    }
    
    printf("v_th = %e, v_th*sqrt(1-c0), c0 = %e\n", v_th, c_0, v_th*sqrt(1-c_0));
    printf("sqrt(c0) = %e, sqrt(1-c0) = %e\n", sqrt(c_0), sqrt(1-c_0));
    printf("Tau = %e, eta = %e\n", Tau, eta);

    return 0;
}

void calc_acc(double *a, double *q, double *v, double m, double k, double eta, gsl_rng *gslr) {
	int j;
	double f;
	for (j = 0; j < N; ++j)
		a[j] = -k*q[j]/m;
}

gsl_rng * init_rng() {
	const gsl_rng_type *rngT;
	gsl_rng *q;
	gsl_rng_env_setup();
	rngT = gsl_rng_default;
	q = gsl_rng_alloc(rngT);
	gsl_rng_set(q,time(NULL));
	return q;
}
