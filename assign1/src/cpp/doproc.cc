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

// things to check, just 1 instruction/block


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

typedef struct blockDominator
{
	int blockNumber;
	vector<int> dominators;
}blockDom;

void printDominators(vector<blockDom> *listOfDoms, char * progname)
{
	int domSize = (*listOfDoms).size();
	printf("idominators %s %d\n",progname,domSize);
	for(int i =0;i<domSize;i++)
	{
		printf("block %d\n",i);
		vector<int> domin = (*listOfDoms)[i].dominators;
		printf("\tidom ");

		if(i ==0)
		{
			printf("\n");
			continue;
		}
			

		for(int j =0;j<domin.size();j++)
		{
			if(domin[j] == -99)
				continue;
			printf("%d ",domin[j]);
		}
		printf("\n");
		
	}
}


bool domSame(vector<int> * first, vector<int> * second)
{
	int sizeFirst = first->size();
	int sizeSecond = second->size();
	if(sizeFirst != sizeSecond)
	{
		return false;
	}
	else
	{
		for(int i =0;i<sizeFirst;i++)
		{
			if((*first)[i]!= (*second)[i])
				return false;
		}

		return true;
	}
}

vector<int> vecIntersection(vector<int> * first, vector<int> *second)
{
	vector<int> ret;
	int size = first->size();
	for(int i =0;i<size;i++)
	{
		int valToFind = (*first)[i];
		std::vector<int>::iterator it;
		it = find (second->begin(), second->end(), valToFind);
		if(it == second->end()) // counldn't find it go to next one
		{
			continue;
		}
		else
		{
			//add it
			ret.push_back(valToFind);
		}

	}
	return ret;
}

vector<int> vecUnion(int element, vector<int> * second)
{
	vector<int> vecu = *second;
	std::vector<int>::iterator it;
	it = find (second->begin(), second->end(), element);
	if(it == second->end()) // element does not exist add it
		vecu.push_back(element);
	
	return vecu;
}

bool vecContains(int element, std::vector<int> *list)
{
	std::vector<int>::iterator it;
	it = find (list->begin(), list->end(), element);
	if(it == list->end())
		return false;
	else
		return true;
}

vector<blockDom> calcIntermediateDomiantors(vector<blockDom> * blockdoms)
{
   vector<blockDom> intermediateDoms = * blockdoms;
   int domSize = intermediateDoms.size();  // remove it self from the dom list
   for(int i =1; i<domSize ;i++ )
   {
     intermediateDoms[i].dominators.pop_back();
   }
   for(int i =1; i<domSize ;i++ )
   {
   	  int dSize = intermediateDoms[i].dominators.size();
   	  for(int j = 0;j<dSize;j++)
   	  {
   	  	for(int k =0;k<dSize;k++)
   	  	{
   	  		if(k == j )
   	  			continue;
   	  		int t = intermediateDoms[i].dominators[k];
   	  		if(t == -99)
   	  			continue;

   	  		if(vecContains(t,&(intermediateDoms[intermediateDoms[i].dominators[j]].dominators)))
   	  		{
   	  		  //intermediateDoms[i].dominators.erase(intermediateDoms[i].dominators.begin()+k-1);
   	  		  intermediateDoms[i].dominators[k] = -99;
   	  		}
   	  	}
   	  }
   }
   return intermediateDoms;
}

vector<blockDom> calcDominators(std::vector< ctrbk > *cfg)
{
	vector<blockDom> listOfDominators;
	// get the number of blocks
	int numBlocks = cfg->size();
	// initilize each dom to contain all blocks
	blockDom entry;
	entry.blockNumber = 0;
	entry.dominators.push_back(0); // push back entry
	listOfDominators.push_back(entry);

	for(int i =1;i<numBlocks;i++) // everthing except the entry and exit
	{
		blockDom singleBlockDom;
		singleBlockDom.blockNumber = i;
		for(int j =0;j<numBlocks;j++)
		{
			singleBlockDom.dominators.push_back(j);

		}
		listOfDominators.push_back(singleBlockDom);
	}

	vector<int> oldDom;
	bool changed = true;
	while(changed == true)
	{
		changed = false;
		for(int i =1;i<numBlocks;i++)
		{
			oldDom = listOfDominators[i].dominators;
			/////
			vector<int> predecessors = (*cfg)[i].pred; // get predecessors
			// find the union of the predecessors
			int numUnions = predecessors.size();
			vector<int> currentUnion = listOfDominators[predecessors[0]].dominators
			;
			
			for(int j=1;j<numUnions;j++)
			{
				// union these things
				currentUnion = vecIntersection(&currentUnion,&listOfDominators[predecessors[j]].dominators);
				
			}
			currentUnion = vecUnion(i,&currentUnion);
			listOfDominators[i].dominators = currentUnion;

			if(!domSame(&oldDom,&currentUnion))
			{
				changed = true;
			}

		}

	}
	
	return listOfDominators;
}



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
        printf("%d ",cfgraph[i].instructions[j].index);
   	  }
   	  printf("\n");

   	  // print successors
   	  int numberSuccessors = cfgraph[i].succ.size();
   	  printf("\tsuccessors %d ",numberSuccessors);
   	  for(int j =0; j<numberSuccessors;j++)
   	  {
        printf("%d ",cfgraph[i].succ[j]);
   	  }
   	  printf("\n");

   	  // print predcessors
   	  int numberPredcessors = cfgraph[i].pred.size();
   	  printf("\tpredecessors %d ",numberPredcessors);
   	  for(int j =0; j<numberPredcessors;j++)
   	  {
        printf("%d ",cfgraph[i].pred[j]);
   	  }
   	  printf("\n");
   	  
   }
}

void mbubblesort(vector<int> *w) {
	if(w->size() ==0)
		return ;

    int temp;
    bool finished = false;
    while (!finished)    {
       finished = true;
       for (int i = 0; i < w->size()-1; i++) {
          if ((*w)[i] > (*w)[i+1]) {
             temp = (*w)[i];
             (*w)[i] = (*w)[i+1];
             (*w)[i+1] = temp;
             finished=false;
          }
        }
     }
    return ; 
} 


void sortLists(vector<ctrbk>* block)
{

	int count = block->size();
	for(int i =0;i<count;i++)
	{

		mbubblesort(&((*block)[i].succ));
		mbubblesort(&((*block)[i].pred));
		
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
				int found = false;  // find gotoblock label
				for(int iter = 0;iter<gotoBlock.size();iter++)
				{
					if(strcmp(gotoBlock[iter].name,labelName)==0)
					{
						gotoBlock[iter].blocks.push_back(blockIndex-1);	
						found = true;
						break;
					}
				}
				if(!found)
				{
						// this is the first label, add it
					cLabels newLabel;
					newLabel.name = labelName;
					newLabel.blocks.push_back(blockIndex-1);
					
					gotoBlock.push_back(newLabel);
				}

				break;
			}

			case CALL_OP: { // call a function
				instr newInst ={instructionIndex,simple_op_name(i->opcode)};
				cfList[blockIndex].instructions.push_back(newInst);
				instructionIndex ++;
				break;
			}

			case MBR_OP: {
				
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

				int ntargets = i->u.mbr.ntargets; // number of
				
				for(int n =0;n<ntargets;n++)
				{
						// also connect it to the branch jump to the old block
					char * labelName = i->u.mbr.targets[n]->name;
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
					// the branch jump will be handled in the next while loop

			
					// we were not able to find the label, thus add it to the gotolabel block

					int found = false;  // find gotoblock label
					for(int iter = 0;iter<gotoBlock.size();iter++)
					{
						if(strcmp(gotoBlock[iter].name,labelName)==0)
						{
							gotoBlock[iter].blocks.push_back(blockIndex-1);	
							found = true;
							break;
						}
					}
					if(!found)
					{
							// this is the first label, add it
						cLabels newLabel;
						newLabel.name = labelName;
						newLabel.blocks.push_back(blockIndex-1);
						
						gotoBlock.push_back(newLabel);
					}
			
				}

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
	blockIndex++;
	ctrbk endBlock;
	for(int i=0;i<endBlocks.size();i++)
	{	
		cfList[endBlocks[i]].succ.push_back(blockIndex);
		endBlock.pred.push_back(endBlocks[i]);
	}
    
	cfList.push_back(endBlock);

	sortLists(&cfList);


	

      
	printcfg(&cfList,proc_name);

	// find immediate dominators  
	vector<blockDom> dom = calcDominators(&cfList);
	//printDominators(&dom,proc_name);

	vector<blockDom> immDoms = calcIntermediateDomiantors(&dom);
	printDominators(&immDoms,proc_name);

    return inlist;
}



