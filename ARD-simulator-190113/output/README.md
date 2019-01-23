**Output file will be stored in this folder.**

Output examples:

Source output:

<1,Nt> Matrix records the source samples along time.

```
0   <- sample value at time step 0
1   <- sample value at time step 1
0   <- sample value at time step 2
0   <- sample value at time step 3
    .
    .
    .

```

Response output:

<1,Nt> Matrix records the response at recording point along time.

```
0   <- response at recording point at time step 0
1   <- response at recording point at time step 1
0   <- response at recording point at time step 2
0   <- response at recording point at time step 3
    .
    .
    .

```

Field output:

<Nx * Ny * Nz, Nt> Matrix records the response in a cube field along time.

```
0 0 0 0 ... 0 0 0   <- Nx * Ny * Nz samples at time step 0.
0 0 0 0 ... 0 0 0   <- Nx * Ny * Nz samples at time step 0.
0 0 0 0 ... 0 0 0   <- Nx * Ny * Nz samples at time step 0.
0 0 0 0 ... 0 0 0   <- Nx * Ny * Nz samples at time step 0.
                    .
                    .
                    .

```