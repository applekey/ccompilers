#include <stdio.h>
extern "C" { 
#include <simple.h>
}

#include<vector>
#include<hash_map>
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 

simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{

	simple_instr *i = inlist;

	while(i){
		switch(i->opcode)
		{
			case LOAD_OP: {
			
				break;
			}

			case STR_OP: {
			
				break;
			}

			case MCPY_OP: {
			
				break;
			}

			case LDC_OP: {
			
			break;
			}

			case JMP_OP: {
			
			break;
			}

			case BTRUE_OP:
			case BFALSE_OP: {
		
			break;
			}

			case CALL_OP: {
		
			break;
			}

			case MBR_OP: {
			
			break;
			}

			case LABEL_OP: {
			//fprintf(fd, "%s:\n", s->u.label.lab->name);
			break;
			}

			case RET_OP: {
			
			break;
			}

			case CVT_OP:
			case CPY_OP:
			case NEG_OP:
			case NOT_OP: {
			/* unary base instructions */
			
			break;
			}

			default: {
			/* binary base instructions */
			
			}
		}
		i = i->next;
	}
    // build flow control graph 

    // find immediate dominators    

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

