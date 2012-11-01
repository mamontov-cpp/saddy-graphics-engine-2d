/** \file ifacecmdoptions.h
	\author HiddenSeeker

	Tests for IFaceCmdOptions
 */
#include <QObject>
#include <QTest>
#pragma once
class IFaceCmdOptionsTest: public QObject
{
	Q_OBJECT
private slots:
	void testMainFile();
	void testConfigOnly();
	void testTests();
	void testCommon();
};




