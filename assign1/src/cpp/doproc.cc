#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "../labeltab.h"
}

#include<vector>
#include<hash_map>
// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 
void printcfg(std::vector< std::vector<int> > *cfg)
{
    std::vector< std::vector<int> > cfgraph = *cfg;
   for(int i =0;i<cfgraph.size();i++)
   {
   	  printf("block %d\n",i);
   	  for(int j =0; j<cfgraph[i].size();j++)
   	  {
        printf("%d \n",cfgraph[i][j]);
   	  }
   }
}


simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
	printf("%s\n",proc_name);
	simple_instr *i = inlist;

	// print out the entry block
	std::vector<std::vector<int> > cfList;
	int instructionIndex = 0;
	int blockIndex = 1; 
	// insert the first item onto the list
	std::vector<int> startingBlock;
	startingBlock.push_back(0);
	cfList.push_back(startingBlock);

	//create the first instruction block
	std::vector<int> newBlock;
	cfList.push_back(newBlock);
	// initilize the label tab
	//init_labeltab();
	
	bool nextStatementNewBlock = false;
	while(i){
		switch(i->opcode)
		{
			case CVT_OP:
			case CPY_OP:
			case NEG_OP:
			case NOT_OP:
			case LOAD_OP: 
			case STR_OP: 
			case MCPY_OP: 
			case LDC_OP: {
				cfList[blockIndex].push_back(instructionIndex);
				instructionIndex ++;
			}

			case JMP_OP: {
				cfList[blockIndex].push_back(instructionIndex);
				instructionIndex ++;

				std::vector<int> newBlock;
				cfList.push_back(newBlock);
				blockIndex++;
			break;
			}

			case BTRUE_OP:  // branch if true
			case BFALSE_OP: {
				cfList[blockIndex].push_back(instructionIndex);
				instructionIndex ++;

				std::vector<int> newBlock;
				cfList.push_back(newBlock);
				blockIndex++;
			break;
			}

			case CALL_OP: { // call a function
		
			break;
			}

			case MBR_OP: {
				

			break;
			}

			case LABEL_OP: {
			//fprintf(fd, "%s:\n", s->u.label.lab->name);
			// start a new block here
				cfList[blockIndex].push_back(instructionIndex);
				instructionIndex ++;

				std::vector<int> newBlock;
				cfList.push_back(newBlock);
				//enter_label(i->u.label,blockIndex);
				blockIndex++;

			break;
			}

			case RET_OP: {
			
			break;
			}

			default: {
			/* binary base instructions */
				printf("entered the default case, shouldn't be here\n");
			
			}
		}

		i = i->next;
	}
    // build flow control graph 

    // find immediate dominators    
	printcfg(&cfList);
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

