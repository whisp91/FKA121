#ifndef _equilibration_h
#define _equilibration_h

/*
 * vel: velocities, dt: timestep, Tau_eq: target temperature, Tau_T: time decay constant
 * Tau: current temperature, N: #particles
 */
void equib_temp(double (*vel)[3], double dt, double Tau_eq, double Tau_T,
	double Tau, int N);

/*
 * pos: 3D positions, dt: timestep,
 * Tau: temperature, V: volume, , P: preassure, N: #particles
 */
void equib_preassure(double (*pos)[3], double dt,
	double Tau, double V, double P, int N);

/*
 * m: mass per particle, E_k: kinetic energy, N: #particles
 */
double instantaneus_temp (double m, double * E_k, int N);

/*
 * Tau: temperature, V: volume, W: virial, N: #particles
 */
void preassure (double Tau, double V, double W, int N);


#endif