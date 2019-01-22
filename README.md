# Adaptive rectangular decomposition simulator

A 3D sound propagation simulator.

Theory:
> Raghuvanshi, Nikunj, Rahul Narain, and Ming C. Lin. "Efficient and accurate sound propagation using adaptive rectangular decomposition." IEEE Transactions on Visualization and Computer Graphics 15.5 (2009): 789-801.
> Grote, Marcus J., and Imbo Sim. "Efficient PML for the wave equation." arXiv preprint arXiv:1001.0319 (2010).

Extended from 2D simulator.
> https://github.com/thecodeboss/AcousticSimulator

Input file example:

assets/*.txt

```
0 0 0 3 3 3  <- partition 0: x, y, z, width, height, depth
3 0 0 3 3 3  <- partition 1: x, y, z, width, height, depth
```

## Note

### FFTW installation note

> <http://www.fftw.org/install/windows.html>

- right click on the project -> properties -> C/C++ -> General -> Additional include Directories.
- right click on the project -> properties -> Linker -> General -> additional library directories.
- right click on the project -> properties -> Linker -> Input -> additional Dependencies.

### SDL installation note

> <https://www.wikihow.com/Set-Up-SDL-with-Visual-Studio-2017>

- right click on the project -> properties -> C/C++ -> General -> Additional include Directories.
- right click on the project -> properties -> Linker -> General -> additional library directories.
- right click on the project -> properties -> Linker -> Input -> additional Dependencies.

### style guide

> <https://google.github.io/styleguide/cppguide.html>

## Examples

![](ARD-simulator-190113/assets/scene-1.gif)
![](ARD-simulator-190113/assets/scene-2.gif)
![](ARD-simulator-190113/assets/scene-3.gif)