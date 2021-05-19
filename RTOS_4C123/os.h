// os.h
// Runs on LM4F120/TM4C123
// A very simple real time operating system with minimal features.



#ifndef __OS_H
#define __OS_H  1

// fill these depending on your clock
#define TIME_1MS  50000
#define TIME_2MS  2*TIME_1MS
#define TIME_100MS  5000000


// ******** OS_Init ************
// initialize operating system, disable interrupts until OS_Launch
// initialize OS controlled I/O: systick, 50 MHz PLL
// input:  none
// output: none
void OS_Init(void);

//******** OS_AddThread ***************
// add three foregound threads to the scheduler
// Inputs: three pointers to a void/void foreground tasks
// Outputs: 1 if successful, 0 if this thread can not be added
int OS_AddThreads(void(*task0)(void),
                 void(*task1)(void),
                 void(*task2)(void));



//******** OS_Launch ***************
// start the scheduler, enable interrupts
// Inputs: number of 20ns clock cycles for each time slice
//         (maximum of 24 bits)
// Outputs: none (does not return)
void OS_Launch(uint32_t theTimeSlice);

#endif
