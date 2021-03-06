/*****************************************
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
*
* Copyright 2015 Ryan Wang, wangyuxin@hust.edu.cn
*
* This file is part of HandySearch.
* HandySearch is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* HandySearch is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with HandySearch.  If not, see <http://www.gnu.org/licenses/>.
*
* - Author: Ryan Wang
* - Contact: wangyuxin@hust.edu.cn
* - License: GNU Lesser General Public License (LGPL)
* - Blog and source code availability: http://ryanwanggit.github.io/HandySearch/
*****************************************/
#pragma once

/**
 * Class:	Html
 *
 * Brief:	This class first tries to open the file the path indicates,
 * and uses some methods to return different kinds of data.
 *
 * Date:	Oct. 2015
 */
class Html 
{
public:
	enum class WeightType
	{
		NotAssigned,
		InContent,
		InTitle,
	};
private:
	/* The html file */
	QFile file;
	/* The file name */
	QString fileName;
	/* Pure text content extracted from file content */
	QStringList textContent;
	/* Title extracted from file content*/
	QString title;
	QString brief;
	static unsigned int totalNum;
	bool analyzed;
	/* The weight member is for sorting  when displaying the results
	 * NOTE:Everytime new search begins the weight becomes meaningless
	 * and will be restored when sorting. 
	 */
	float weight;
	Html::WeightType type;
	void extractText(const QString &fileContent);
	void extractTitle(const QString &fileContent);
	bool load();
public:
	Html();
	Html(const QString &filePath);
	Html(const Html &c);
	Html::WeightType getWeightType() const;
	void setWeightType(Html::WeightType type);
	float getWeight() const;
	void setWeight(const float weight);
	void clearWeight();
	const QStringList& getText();
	const QString& getTitle();
	const QString getFilePath();
	const QString& getBrief();
	void setBrief(const QString& brief);
	static unsigned int getTotalHtmlCount();
	bool hasAnalyzed() const;
	void setAnalyzed(const bool analyzed);
	bool loadFrom(const QString& filePath);
	bool operator== (const Html& other);
	bool operator< (const Html& other);
	Html& operator= (const Html &other);
};

