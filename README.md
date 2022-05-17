# EE6470 HW2
Gaussian Filter implemented with row buffers, TLM sockets, and a simple bus module in SystemC

How to run:
1. Log in to the Ubuntu Docker provided by EE6470
2. Download the repo to your working directory
3. Navigate to the downloaded directory
4. Compile the program
   1. Make a new directory named "build", and enter the directory
    ```bash
        mkdir build
        cd build
    ```
   2. Generate Makefiles using CMake
    ```bash
        cmake ..
    ```
   4. Compile SystemC source code
    ```bash
       make
    ```
5. Run the program
```bash
   make run
```

6. Resulting image is saved as 'build/out.bmp'


吳哲廷 學號:110061590
