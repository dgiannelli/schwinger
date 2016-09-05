#ifndef BOUNDARY_H
#define BOUNDARY_H

/*
 *   ---
 *  |  / \
 *  |   |
 * \ /  |
 *   ---
 */

#define Right(lattice, nx, ny, n) lattice[(nx+1)%n][ ( ny + (n-1-2*ny)*((nx+1)/n) ) % n ]

#define Top(lattice, nx, ny, n) lattice[nx][(ny+1)%n]

#define Left(lattice, nx, ny, n) lattice[(nx+n-1)%n][ ( ny + (n-1-2*ny)*(1-(nx+n-1)/n) ) % n ]

#define Bottom(lattice, nx, ny, n) lattice[nx][(ny+n-1)%n]

#endif
