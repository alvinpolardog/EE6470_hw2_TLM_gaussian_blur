# EE6470 HW2 Report
吳哲廷 學號:110061590

Link to [Github Repo](https://github.com/alvinpolardog/EE6470_hw2_TLM_gaussian_blur)
##

### Gaussian Blur with a TLM-2.0 interconnect

#### Routing

Instead of directly routing the data through FIFO channels similar to that of HW1, TLM sockets are used this time to improve
universality. TLM interconnects allow us to send information between different module all using generic payloads, thus generalizing
each transaction, and allows the transactions directed to different modules to be routed through a common bus.

In this example, the SimpleBus module takes the transaction initiated by the testbench, and route it toward either the GaussFilter or the
TlmMemory module depending on the transaction address. By looking at the destination address and comparing it to the base address and size
of each module stored in the memory map, the simplebus route the transaction toward the correct module while also translating or mapping the 
address into a local offset for the target module to use.

The SimpleBus and TlmMemory module used was directly copied from EE6470 Lab4 since they provide simple intercompatibility. The global address
system/numbering was also unchanged as there were no need for additional memory access from the testbench since the target image is identical.

##  
##  

#### GaussFilter implementation
HW2 uses the same Gaussian filter implemented in HW1. The Gaussian blur used in this implementation is a 3x3 mask:
 ```
    1, 2, 1
    2, 4, 2
    1, 2, 1
```
and a multiplier factor of 1/16 to avoid brightness oversaturation.

The row buffer used in HW1 is also implemented in this new TLM version, with the same three-row array that is constantly overwritten to store 
the pixels currently being used.

Contrary to HW1 however, my implementation discarded the triple channels for returning each color of the results. Since we are using a 4-byte pointer
for transfering information between the testbench and GaussFilter, after calculating a full row of pixel similar to in HW1, the results are converted 
into a 4 byte integer and travels through the o_result FIFO channel before being sent back to the testbench again through the data_ptr.


Original Image:

![original image](https://github.com/alvinpolardog/EE6470_hw1_gaussian_blur/blob/main/lena_std_short.bmp)

Filtered Images:

![filtered image](https://github.com/alvinpolardog/EE6470_hw1_gaussian_blur/blob/main/out.bmp)
