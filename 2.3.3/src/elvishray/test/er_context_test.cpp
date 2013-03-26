#include "eiAPI/ei.h"

void ei_context_unittest0()
{
	ei_context();

	ei_end_context();
}
//
void ei_context_unittest1()
{
	ei_end_context();

	ei_context();

	ei_end_context();
}
//
void ei_context_unittest()
{
	//ei_context_unittest0();
	ei_context_unittest1();
}
