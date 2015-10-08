#pragma once
#include "ui_loading.h"
class LoadUI : public QDialog
{
	Q_OBJECT
private:
	QDir htmlFolder;
	QDir dictFolder;
	QTime clock;
	QTimer timer;
	QPoint origin;
	bool isPressed;
	unsigned long currentProgress;
	unsigned long maximumProgress;
public:
	LoadUI();
	~LoadUI();
public slots:
	/* UI slots */
	void loadingDots();
	/* Load slots */
	void loadStarted();
	void loadFinished();
	//Html load slots
	void htmlLoadStarted();
	void htmlLoaded(unsigned int threadID, QString path);
	void htmlLoadFinished();
	//Dictionary load slots
	void dictLoadStarted();
	void dictLoaded(int num);
	void dictLoadFinished();
	//Override event handler
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent * event);
	bool eventFilter(QObject *obj, QEvent *event);
	
signals:
	void finished();
private:
	Ui::Loading ui;
};

