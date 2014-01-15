#include <stdio.h>
extern "C" { 
#include <simple.h>
#include "../labeltab.h"
}

#include<vector>
#include<hash_map>
#include <cstring>
using namespace __gnu_cxx;
using namespace std;


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

typedef struct gotoLabels
{
	char * name;
	int singleBlock;//used for labels
	std::vector<int> blocks;
}cLabels;

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
   	  printf("\tinstrs %d: ",cfgraph[i].instructions.size());
   	  for(int j =0; j<cfgraph[i].instructions.size();j++)
   	  {
        printf("%d, %s ",cfgraph[i].instructions[j].index,cfgraph[i].instructions[j].s);
   	  }
   	  printf("\n");

   	  // print successors
   	  int numberSuccessors = cfgraph[i].succ.size();
   	  printf("\tsuccessors %d: ",numberSuccessors);
   	  for(int j =0; j<numberSuccessors;j++)
   	  {
        printf("%d ",cfgraph[i].succ[j]);
   	  }
   	  printf("\n");

   	  // print predcessors
   	  int numberPredcessors = cfgraph[i].pred.size();
   	  printf("\tpredecessors %d: ",numberPredcessors);
   	  for(int j =0; j<numberPredcessors;j++)
   	  {
        printf("%d ",cfgraph[i].pred[j]);
   	  }
   	  printf("\n");
   	  printf("\n");
   	  
   }
}



simple_instr* do_procedure (simple_instr *inlist, char *proc_name)
{
	std::vector<cLabels> labelBlock;
	std::vector<cLabels> gotoBlock;
	std::vector<int> endBlocks;

	simple_instr *i = inlist;

	// print out the entry block
	std::vector<ctrbk> cfList;
	int instructionIndex = 0;
	int blockIndex = 1; 
	// insert the first item onto the list
	ctrbk startingBlock;
    startingBlock.succ.push_back(1);

	cfList.push_back(startingBlock);

	//create the first instruction block
	ctrbk newBlock;
	newBlock.pred.push_back(0);

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

		
			case BTRUE_OP:  // branch if true
			case BFALSE_OP: 
			case JMP_OP:{
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				// create a new block
				ctrbk newBlock;
				cfList.push_back(newBlock);
				blockIndex++;

				if(i->opcode!= JMP_OP)
				{
					//set the previous block to the next
					//set the current block to the previous
					cfList[blockIndex-1].succ.push_back(blockIndex);
					cfList[blockIndex].pred.push_back(blockIndex-1);
				}
				

				// the branch jump will be handled in the next while loop

				// also connect it to the branch jump to the old block
				char * labelName = i->u.bj.target->name;
				// check if the label exists
				for(int iter = 0;iter<labelBlock.size();iter++)
				{
					if(strcmp(labelBlock[iter].name,labelName)==0)
					{
						// connect the current block to the label block
						cfList[labelBlock[iter].singleBlock].pred.push_back(blockIndex-1);
						cfList[blockIndex-1].succ.push_back(labelBlock[iter].singleBlock);
						//connect them up
						break;
					}
				}
				// we were not able to find the label, thus add it to the gotolabel block

				for(int iter = 0;iter<gotoBlock.size();iter++)
				{
					if(strcmp(gotoBlock[iter].name,labelName)==0)
					{
						gotoBlock[iter].blocks.push_back(blockIndex-1);	
						break;
					}
				}
				// this is the first label, add it
				cLabels newLabel;
				newLabel.name = labelName;
				newLabel.blocks.push_back(blockIndex-1);
				
				gotoBlock.push_back(newLabel);

				break;
			}

			case CALL_OP: { // call a function
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;
				break;
			}

			case MBR_OP: {
				
				printf("multibr\n");
				break;
			}

			case LABEL_OP: {
			//fprintf(fd, "%s:\n", s->u.label.lab->name);
			// start a new block here
				// check 

			    char * labelName = i->u.label.lab->name;
				
			    // check if the current block has nothing, if it has nothing
			    // the previous instruction was most likely a jump instruction
			    if(cfList[blockIndex].instructions.size() != 0)
			    {
			    	ctrbk newBlock;
					cfList.push_back(newBlock);
					blockIndex++;

					//set the previous block to the next
					//set the current block to the previous
					
					cfList[blockIndex-1].succ.push_back(blockIndex);
					cfList[blockIndex].pred.push_back(blockIndex-1);
			    }

			
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				//add the label to the label vector
				cLabels newLabel;

				newLabel.name = labelName;
				newLabel.singleBlock = blockIndex;


				labelBlock.push_back(newLabel);
				// now check if there are any gotolabels
				for(int iter = 0;iter<gotoBlock.size();iter++)
				{
					if(strcmp(gotoBlock[iter].name,labelName)==0)
					{
						// go through each goto label in the vector
						for(int inner =0;inner<gotoBlock[iter].blocks.size();inner++)
						{
							int gotoIndex = gotoBlock[iter].blocks[inner];
							cfList[gotoIndex].succ.push_back(blockIndex);
							cfList[blockIndex].pred.push_back(gotoIndex);
						}
					}
				}

				break;
			}

			case RET_OP: {
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;

				endBlocks.push_back(blockIndex);
				break;
			}

			default: {
			/* binary base instructions */
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;
				break;
			}
		}

		i = i->next;
	}

	// finally add the end block
	ctrbk endBlock;
	for(int i=0;i<endBlocks.size();i++)
	{	
		endBlock.pred.push_back(endBlocks[i]);
	}
    
	cfList.push_back(endBlock);
	

    // find immediate dominators    
	printcfg(&cfList,proc_name);
    return inlist;
}



