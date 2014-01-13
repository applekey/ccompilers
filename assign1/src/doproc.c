#include <stdio.h>
#include <simple.h>
#include "bitvector.h"


simple_instr *
do_procedure (simple_instr *inlist, char *proc_name)
{
    /* build the flow graph */

    /* find immediate dominators */

    return inlist;
}


/*
// first pass find the targets of all instructions
while(i){
	//
	add the instruction to the labeltab

	//
	find branch instructions, mark these as ends
	next instruction mark as beginnings

	i = i->next
}

while(i){
	find all branch targets, mark tese as beginnings
}

foreach basic block{
	if end instruction is a not a branch, next block is the next instruction,
		connect these two basic blocks

	if there is a if condition, the next block as well as the next condition

		if there is a goto then 
		// add the thing to the adjacency list (src, destination

			// add an entry and exit node
	)
}
*/

