#ifndef _LIQ_REFACTORING_H_
#define _LIQ_REFACTORING_H_

#define Refactoring 1


enum TempControlBreak//for my refactoring only [3/4/2011 yys]
{
	TCB_OK     = 0,
	TCB_Break  = 1,
	TCB_Continue = 2,
	TCB_MS_Success = 3,
	TCB_MS_Failure = 4,
};

#define PROCESS_TEMP_CONTROL_BREAK_CONTINUE_RETURN(tcb) \
	if(TCB_Break == tcb)\
	break;\
	else if(TCB_Continue == tcb)\
	continue;\
	else if(TCB_MS_Success == tcb)\
	return MS::kSuccess;\
	else if(TCB_MS_Failure == tcb)\
	return MS::kFailure;\
	else if(TCB_OK == tcb){/* go ahead */}

#define PROCESS_TEMP_CONTROL_BREAK_CONTINUE(tcb) \
	if(TCB_Break == tcb)\
	break;\
	else if(TCB_Continue == tcb)\
	continue;\
	else if(TCB_OK == tcb){/* go ahead */}

#endif//_LIQ_REFACTORING_H_


