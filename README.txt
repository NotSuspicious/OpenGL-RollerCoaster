# Roller Coaster
Recording: hw2/screenshots

Instructions: Run inside hw2
./hw2 splines/rollerCoaster.sp

## Cool Features
### Visuals
- Added a skybox
- Two rails
- Cylindrical Rails

### Physics
- The camera moves using u_new = u_old + (dt)(sqrt(2gh)/mag(dp/du)

### Organization
Organized the code into subfolders. Modified the Makefile to include the new structure.
- src - contains all the source code
- build - contains the build files
- assets - contains all the assets
- shaders - contains all the shaders
Created a gitignore, currently source controlled with GitHub

### Math.h
- Added a math.h file to contain all the math functions
Created a Vector3 class to represent 3D vectors
- Overloads Vector operations