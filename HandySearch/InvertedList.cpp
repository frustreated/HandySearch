/*************************************
* Copyright(C),2015-2016,Ryan Wang
*
* File:	InvertedList.cpp
*
* Version: V1.0
*
* Brief:	This controls several local inverted lists, send requery and load signals��
* receive signals from local inverted lists, in order to do multi-thread trick.
*
* Author:	Ryan
*
* Date:	Nov. 2015
*************************************/
#include "stdafx.h"
#include "InvertedList.h"
#include "LocalInvertedList.h"
#include "LoadUI.h"


/*--------------------------
* InvertedList::InvertedList
* 	The common-used constructor.
----------------------------*/
InvertedList::InvertedList()
{
	this->hasLoaded = false;
}


/*--------------------------
* InvertedList::~InvertedList
* 	The default destructor which quit all threads and kills all 
* local inverted lists.
----------------------------*/
InvertedList::~InvertedList()
{
	for (QThread* thread : threadList)
	{
		thread->quit();
		thread->wait();
		delete thread;
	}
}


/*--------------------------
* InvertedList::setHtmlFolder
* 	Set the html loading directory.
* Parameter:
* 	const QDir & htmlFolder - The html folder.
----------------------------*/
void InvertedList::setHtmlFolder(const QDir &htmlFolder)
{
	this->htmlFolder = htmlFolder;
}


/*--------------------------
* InvertedList::load
* 	Start loading htmls and put all sub-tasks into global thread pool.
----------------------------*/
void InvertedList::load()
{
	if (hasLoaded)
		return;

	emit htmlLoadStarted();
	/* List all files */
	QStringList pathList = htmlFolder.entryList(QDir::NoDotAndDotDot | QDir::Files);
	for (QString& path : pathList)
		path.prepend(htmlFolder.absolutePath() + "/");

	const unsigned int LOCAL_HTML_NUM = 50;
	/* Traverse html folder */
	for (int i = 0; i < pathList.size(); i += LOCAL_HTML_NUM)
	{
		QThread* thread = new QThread();
		threadList.append(thread);
		LocalInvertedList* localList = new LocalInvertedList(pathList.mid(i, LOCAL_HTML_NUM));
		localList->moveToThread(thread);

		/* Create multiple sub-threads to load htmls ( every 50 files ) */
		connect(localList, &LocalInvertedList::htmlLoaded, LoadUI::getInstance(), &LoadUI::htmlLoaded);
		connect(this, &InvertedList::localLoadStart, localList, &LocalInvertedList::localLoadStart);
		connect(localList, &LocalInvertedList::localLoadFinished, this, &InvertedList::localLoadFinished);
		connect(this, &InvertedList::localQuery, localList, &LocalInvertedList::localQuery);
		connect(localList, &LocalInvertedList::localQueryResult, this, &InvertedList::localQueryResult);
		connect(thread, &QThread::finished, localList, &QObject::deleteLater);

		thread->start();
	}
	activeThreadList.append(threadList);
	emit localLoadStart();
}


/*--------------------------
* Index::getTitleList
* 	Return the list of current loaded html titles.
* Returns:	const QStringList & - The list of html titles.
----------------------------*/
const QStringList & InvertedList::getTitleList() const
{
	return titleList;
}


/*--------------------------
* InvertedList::localQueryResult
* 	Receives every result return of local inverted lists.
* Parameter:
* 	QThread * thread - The thread pointer of the local inverted list.
*	const QList<Html*> & resultList - The result of local query.
----------------------------*/
void InvertedList::localQueryResult(QThread *thread, const QList<Html*> &resultList)
{
	activeThreadList.removeOne(thread);
	this->resultList.append(resultList);
#ifdef _DEBUG
	qDebug() << "[" << thread->currentThreadId() << "]" << " Result list size: " << resultList.size();
#endif // _DEBUG
	if (activeThreadList.size() == 0)
	{
		std::sort(this->resultList.begin(), this->resultList.end(), compareHtml);
		emit queryResult(this->resultList, this->keyWordList);
	}
}


/*--------------------------
* InvertedList::localLoadFinished
* 	Receives the load finish signal from local inverted lists.
* Parameter:
* 	QThread * thread - The thread pointer of the local inverted list.
*	const QStringList & titleList - The title list of loaded htmls.
----------------------------*/
void InvertedList::localLoadFinished(QThread *thread, const QStringList &titleList)
{
	activeThreadList.removeOne(thread);
#ifdef _DEBUG
	qDebug() << "Html Loading Thread(s) remainning: " << activeThreadList.size();
#endif // _DEBUG
	this->titleList.append(titleList);
	if (activeThreadList.size() == 0)
	{
		hasLoaded = true;
		this->titleList.removeDuplicates();
		emit htmlLoadFinished();
	}
}


/*--------------------------
* InvertedList::query
* 	Query the inverted list, signal all local inverted lists to start working.
* Parameter:
* 	const QStringList & keyWordList - The key word list.
----------------------------*/
void InvertedList::query(const QStringList& keyWordList)
{
	resultList.clear();
	this->keyWordList = keyWordList;
	activeThreadList.append(threadList);
	emit localQuery(keyWordList);
}

