#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "../labeltab.h"
}

#include<vector>
#include<hash_map>


typedef struct instruction
{
  int index;
  char*s;
}instr;

typedef struct controlblock
{
	std::vector<instr> instructions;
	std::vector<int> pred;
	std::vector<int> succ;

}ctrbk;

// data structures you should consider using are vector and hash_map from the STL
// refer to the following link as a starting point if you are not familiar with them: 
// http://www.sgi.com/tech/stl/Vector.html
// http://www.sgi.com/tech/stl/hash_map.html 
void printcfg(std::vector< ctrbk > *cfg, char* procedureName)
{
	std::vector< ctrbk > cfgraph = *cfg;
	printf("cfg %s %d\n",procedureName,cfgraph.size());

   for(int i =0;i<cfgraph.size();i++)
   {
   	  printf("block %d\n",i);
   	  // print instructions
   	  printf("\tinstrs %d ",cfgraph[i].instructions.size());
   	  for(int j =0; j<cfgraph[i].instructions.size();j++)
   	  {
        printf("%d, %s ",cfgraph[i].instructions[j].index,cfgraph[i].instructions[j].s);
   	  }
   	  printf("\n");

   	  // print successors
   	  printf("\tsuccessors %d ",cfgraph[i].instructions.size());
   	  for(int j =0; j<cfgraph[i].succ.size();j++)
   	  {
        printf("%d ",cfgraph[i].succ[j]);
   	  }
   	  printf("\n");

   	  // print predcessors
   	  printf("\tpredecessors %d ",cfgraph[i].instructions.size());
   	  for(int j =0; j<cfgraph[i].pred.size();j++)
   	  {
        printf("%d ",cfgraph[i].pred[j]);
   	  }
   	  
   }
}


simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
	
	simple_instr *i = inlist;

	// print out the entry block
	std::vector<ctrbk> cfList;
	int instructionIndex = 0;
	int blockIndex = 1; 
	// insert the first item onto the list
	ctrbk startingBlock;
	instr newInst = {1,"starting"};
	startingBlock.instructions.push_back(newInst);


	cfList.push_back(startingBlock);
	instructionIndex++;

	//create the first instruction block
	ctrbk newBlock;
	cfList.push_back(newBlock);
	// initilize the label tab
	
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
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;
				break;
			}

			case JMP_OP: {
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				ctrbk newBlock;
				cfList.push_back(newBlock);
				blockIndex++;

				//set the previous block to the next
				//set the current block to the previous
				cfList[blockIndex-1].succ.push_back(blockIndex);
				cfList[blockIndex].pred.push_back(blockIndex-1);

				break;
			}

			case BTRUE_OP:  // branch if true
			case BFALSE_OP: {
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				// create a new block
				ctrbk newBlock;
				cfList.push_back(newBlock);
				blockIndex++;

				//set the previous block to the next
				//set the current block to the previous
				cfList[blockIndex-1].succ.push_back(blockIndex);
				cfList[blockIndex].pred.push_back(blockIndex-1);
				break;
			}

			case CALL_OP: { // call a function
				//printf("callop\n");
				break;
			}

			case MBR_OP: {
				
				printf("multibr\n");
				break;
			}

			case LABEL_OP: {
			//fprintf(fd, "%s:\n", s->u.label.lab->name);
			// start a new block here
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				ctrbk newBlock;
				cfList.push_back(newBlock);
				//enter_label(i->u.label,blockIndex);
				blockIndex++;


				//set the previous block to the next
				//set the current block to the previous
				cfList[blockIndex-1].succ.push_back(blockIndex);
				cfList[blockIndex].pred.push_back(blockIndex-1);
				


				break;
			}

			case RET_OP: {
			
				break;
			}

			default: {
			/* binary base instructions */
				printf("entered the default case, shouldn't be here\n");
				break;
			}
		}

		i = i->next;
	}
    // build flow control graph 

    // find immediate dominators    
	printcfg(&cfList,proc_name);
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

