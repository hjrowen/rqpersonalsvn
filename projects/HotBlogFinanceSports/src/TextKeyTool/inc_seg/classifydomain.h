/*
* Copyright (c) 2005-2008
* All rights reserved.
* 
* 文件名称：classifydomain.h
* 摘    要：分词之上的应用层
* 作    者：孙健 2006/04/03
* 版    本：1.0
* 
*/

#ifndef _IASK_NLP_Domain_H
#define _IASK_NLP_Domain_H

#include "classifier.h"
#include "ExKeywordsInterface.h"
#include <stdio.h>

#define IN
#define OUT

#define CONCEPT_MAX_LEN 20

#define MAX_VOTE_NUM 100
//#define SUN_DEBUG_VOTE 1

// ****** tagoption说明 ********** //
//unsigned short的9-16位用于分类 
//最高四位用于分类器选择，低四位用于分类文本的选区
 
//knn分类器分类类型
#define TAG_KNN_ADSENSE 0
#define TAG_KNN_NEWS 1
#define TAG_KNN_BLOG 2

#ifdef __cplusplus

class ClassifyDomain
{
public:
	virtual int InitClsResource(IWordSegment *pWordSegment, IPOSTagger *pPOSTagger, IPhraseTerm *pPhraseTerm, IKeywords *pKeywords) = 0;

	/*使用方法: 传递的参数襬ption蟀辽�3个部分
	 * 短语合并部分:AOP_PHRASETERM_XX 没有就不做短语合并
	 * 
	 * featvec 文本的特征向量串，用 seg 分割，如果用GetDocFeat得到了关键词串
	 * 则featvec置NULL, seg不起作用
	 */
	virtual int GetDocPropInfo(DocInfo *docinfo, unsigned char pagetype) = 0;
	//virtual int GetDocFeat(unsigned short *srz, unsigned int nlen, unsigned char pagetype, unsigned short Option) = 0;
};

extern "C" ClassifyDomain *CreateClassifyDomainInstance(void);
typedef ClassifyDomain* (*CREATECLASSIFYDOMAININSTANCE)(void);

extern "C" int ReleaseClassifyDomain(ClassifyDomain *);
typedef int(*RELEASECLASSIFYDOMAIN)(ClassifyDomain *);

#endif //end __cplusplus
#endif
