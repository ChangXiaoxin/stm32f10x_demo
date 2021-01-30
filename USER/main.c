#include "task.h"
#include "timer.h"
#include "init.h"


int main(void){
	
	TopInit(); //Initialize on-chip resources and peripherals.
	
	while(1){
		TaskProcess();//Run the task process.
		
	}
	
}



