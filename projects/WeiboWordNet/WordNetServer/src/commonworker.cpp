#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <map>
#include <fstream>
#include <iterator>
#include <vector>
#include <sys/time.h>
#include "strmatch.h"
#include "dictmatch.h"
#include "nx_log.h"
#include "commonworker.h"
#include "parsejson.h"
using namespace std;

static char const rcsid[] ="$commonworker:  2008/04/03 $ ";

extern global_data_t global_data;

typedef struct // 线程资源结构体，可以根据自己需求 增加
{	
	JsonTree_t *ptree;
	JsonText_t *ptext;
	char *query_type;
	char *content;
	char *res;
}WorkData_t;
///////////////////////////////////////////////////////////////
MATCHENTRY * loadMachine(const char *file)
{
	MATCHENTRY *machine_ww = NULL;
	if(!(machine_ww = strMatchInit(2)))
	{
		fprintf(stderr,"Init machine err!\n");
		return NULL;
	}
	ifstream fin;
	fin.open(file);
	if(!fin.is_open())
	{
		fprintf(stderr,"%s can not be opened!\n",file);
		return NULL;
	}
	char text[1024]="";
	char unitary[1024]="";
	char *pos = NULL;
	int tLen = 0,iret = 0;
	double labelLen=0,disturbRate=0;
	map<string,int> machineMap;
	map<string,int>::iterator it;
	while(!fin.eof())
	{
		if(fin.getline(text,1024).good()==false)
			break;
		tLen = strlen(text);
		if(tLen < 2)
			continue;
		it = machineMap.find(text);
		if(it == machineMap.end())
		{
			machineMap.insert(map<string,int>::value_type(text,1));
			Addword(text,0,machine_ww,0);
		}
	}
	fin.close();
	Prepare(0, machine_ww);
	return machine_ww;
}

int findBlackWord(char *text,int &watchNum,MATCHENTRY *machine_ww,vector<pair<string,int> > &watchWord)
{
	if(text == NULL || machine_ww == NULL || machine_ww->wordsnum == 0)
		return -1;
	FindAllUntilNull(text,0,machine_ww);
	watchNum=0;
	watchWord.clear();
	pair<string,int> pp;
	for(int i = 0; i< machine_ww->wordsnum; i++)
	{
		if (machine_ww->wordlist[i].findnum > 0)
		{
			pp.first = (char*)machine_ww->wordlist[i].word;
			pp.second = machine_ww->wordlist[i].findnum;
#ifdef DEBUG
			fprintf(stderr,"findBlackWord:%s--%d\n", pp.first.c_str(),machine_ww->wordlist[i].findnum);
#endif
			watchWord.push_back(pp);
			watchNum ++;
			machine_ww->wordlist[i].findnum = 0;
		}
	}
	return 0;
}
int CreateGlobalData(char **argv, int argc)
{
	int i;
	int ret;
	total_index_info_t total_info;
	WordNetIndexor * cIndexor = new WordNetIndexor;

	ret = cIndexor->LoadIndex("word_net","term_info.index","relate_info.index",global_data.TermIndex,global_data.TermInfo,global_data.RelateInfo,&total_info);
	
	if(ret < 0)
	{
		nx_log(NGX_LOG_ERR,"load index error, quit!");
		return -1;
	}
	nx_log(NGX_LOG_NOTICE,"load index successful !");

	return 0;
}

int ReleaseGlobalData()
{
	return 0;
}

void* CreateWorkingData()
{
	WorkData_t *workData = (WorkData_t*)calloc(sizeof(WorkData_t),1) ;
	assert(workData!=NULL);
	workData->ptree = CreateJsonTree();
	assert(workData->ptree!=NULL);
	workData->ptext = CreateJsonText(1000); 
	assert(workData->ptext != NULL);

	return workData;
}

void ReleaseWorkingData(void *working_data)
{	
	WorkData_t *workData = (WorkData_t*)working_data;
	if(workData->ptree != NULL)
	{
		FreeJsonTree(workData->ptree);
		workData->ptree = NULL;
		FreeJsonText(workData->ptext);
		workData->ptext = NULL;
	}
	free(working_data);
}

int InitWorkData(WorkData_t *workData)
{
	//workData->title=NULL;
	//workData->url = NULL;
	//workData->time = 0;
	workData->content=NULL;
	return 0;
}

int ParseJsonInput(WorkData_t *workData, BYTE *pSrcData, unsigned int srcLen)
{
	int ret;
	JsonTree_t *ptree = workData->ptree;
	PairNode_t *ppair;
	ret = ParseJson((char*)pSrcData, srcLen, ptree);
	if(ret<=0)
		return -1;

	if(ptree->rootType!=V_PAIR_ARRAY)
		return -2;

	ForEachPairNode(ptree, 0, ppair)
	{
		/*
		if((strcmp(ppair->keyStr,"title")==0))
		{
			if(ppair->v_type != V_STR)
			{
				return -1;
			}
		//	workData->title = ppair->pStr;
		}
		else if((strcmp(ppair->keyStr,"url")==0))
		{
			if(ppair->v_type != V_STR)
			{
				return -1;
			}
		//	workData->url = ppair->pStr;
		}
		else if((strcmp(ppair->keyStr,"time")==0))
		{
			if(ppair->v_type != V_VALUE_STR)
			{
				return -1;
			}
		//	workData->time = atoi(ppair->pStr);
		}
		*/
		if((strcmp(ppair->keyStr,"qt")==0))
		{
			if(ppair->v_type != V_STR)
			{
				return -1;
			}
			workData->query_type = ppair->pStr;
		}
		else if((strcmp(ppair->keyStr,"content")==0))
		{
			if(ppair->v_type != V_STR)
			{
				return -1;
			}
			workData->content = ppair->pStr;
		}
	}

	return 0;
}

int GenJsonResult(WorkData_t *workData,BYTE *pResultData, unsigned int * resultLen)
{
	workData->ptext->curLen=0;
	char buf[1000];
	buf[0]='\0';
	/*
	if(workData->title)
		AddPairValue(workData->ptext,"title",workData->title,V_STR);
	if(workData->url)
		AddPairValue(workData->ptext,"url",workData->url,V_STR);
	if(workData->time>0)
	{
		char timeStr[32];
		sprintf(timeStr,"%d",workData->time);
		AddPairValue(workData->ptext,"time",timeStr,V_VALUE_STR);
	}
	*/
	/*
	if(workData->dm_pack->ppseg_cnt >0)
	{
		for(int i = 0; ; )
		{
			snprintf(buf + strlen(buf)  ,strlen(((workData->dm_pack->ppseg)[i])->pstr) + 1,"%s",((workData->dm_pack->ppseg)[i])->pstr);
			i++;
			if (i < (int)workData->dm_pack->ppseg_cnt)
			{
				snprintf(buf + strlen(buf),3,"$$");
				continue;
			}
			else
			{
				break;
			}
		}
	}
	*/
	u_int hash_value;
	u_int term_index;
	__gnu_cxx::hash_map<u_int,u_int>::iterator hmIndex;

	hash_value=__gnu_cxx::__stl_hash_string(workData->content);
	nx_log(NGX_LOG_DEBUG,"get input begin: require [%s] ",workData->content);

	hmIndex= global_data.TermIndex.find(hash_value);

	if (hmIndex != global_data.TermIndex.end())
	{
		term_index = hmIndex->second;
		snprintf(buf,1000,"term:[%s] df [%d] relate_num [%d] ",global_data.TermInfo[term_index].termstr,global_data.TermInfo[term_index].df_now,global_data.TermInfo[term_index].relate_term_num);
		nx_log(NGX_LOG_DEBUG,"get input: term info [%s] ",buf);
		for (hmIndex=global_data.TermInfo[term_index].cooc_term.begin();(hmIndex !=global_data.TermInfo[term_index].cooc_term.end()) && (strlen(buf) < 900);hmIndex ++)
		{
			snprintf(buf+strlen(buf),100," %s:%u",global_data.TermInfo[global_data.TermIndex[hmIndex->first]].termstr,global_data.RelateInfo[hmIndex->second].num_now);
			nx_log(NGX_LOG_DEBUG,"get input: relate info [%s] ",buf);
		}

	}
	else
	{
		nx_log(NGX_LOG_DEBUG,"get input: no such term [%s] hash_value [%ud]",workData->content,hash_value);
		
	}	

	AddPairValue(workData->ptext,"res",buf,V_STR);
	memcpy(pResultData, workData->ptext->text, workData->ptext->curLen);
	resultLen[0] =  workData->ptext->curLen;

	return 0;
}

int DoWork(unsigned short command, DOCID_T docID,  BYTE *pSrcData, unsigned int srcLen, BYTE *pResultData, unsigned int * resultLen, void *working_data)
{
	int ret;
	char *text = (char *)pSrcData;
	WorkData_t *workData = (WorkData_t*)working_data;
	ret = ParseJsonInput( workData, pSrcData, srcLen);
	nx_log(NGX_LOG_ALERT,"get input json error");
	if (ret <0 || workData->content == NULL)
	{
		nx_log(NGX_LOG_ALERT,"get input json error");
		return 1;
	}
	/*
	ret = dm_search(g_dm_dict,workData->dm_pack,workData->content,strlen(workData->content),DM_OUT_FMM);
	if(ret <0)
	{
		nx_log(NGX_LOG_ALERT,"dm_search error");
		return 1;
	}	
	*/

	ret = GenJsonResult(workData,pResultData, resultLen);
	if(ret <0)
	{
		nx_log(NGX_LOG_ALERT,"GenJsonResult error");
		return 1;
	}	
	return 0;
}

