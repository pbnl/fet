/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include <QtTest>

#include "constraints/paradoxorderedifsamedayvsconsecutive.h"

#include "generate_pre.h"
#include "timetable.h"
Timetable gt; // Just to please generate_pre.cpp that yet require it. It must be not used in tests

class ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest : public QObject
{
	Q_OBJECT

public:
	ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest();

private:
	Rules &rules;
	void populate_basic_data();

private Q_SLOTS:
	void init();
	void cleanup();
	void test_redundant_prints_no_error_message();
	void test_generate_error_message();
	void test_generate_error_message_reverse_order();
	void test_generate_multiple_error_messages();
	void test_reset_properly();
	void test_added_to_generate();
};

ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest()
	: rules(gt.rules)
{
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::populate_basic_data()
{
	QStringList teachers;
	QStringList activitytags;
	QStringList studentsnames;
	QString subject("subject1");

	if (!rules.initialized)
		rules.init();
	Subject *psubject = new Subject();
	psubject->name = subject;
	rules.addSubjectFast(psubject);
	rules.addSimpleActivityFast(12345, 12345, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(23456, 23456, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.addSimpleActivityFast(34567, 34567, teachers, subject, activitytags, studentsnames, 1, 1, true, false, 10, 10);
	rules.computeInternalStructure();

	TimeConstraint * ctr = new ConstraintBasicCompulsoryTime();
	rules.addTimeConstraint(ctr);
	ConstraintBasicCompulsorySpace * sctr = new ConstraintBasicCompulsorySpace();
	rules.addSpaceConstraint(sctr);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::init()
{
	rules.init();
	populate_basic_data();
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::cleanup()
{
	rules.kill();
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_redundant_prints_no_error_message()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxOrderedIfSameDayVsConsecutive paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(result, "Paradox wrongly found");

	QCOMPARE(paradox.getErrors().size(), 0);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_generate_error_message()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxOrderedIfSameDayVsConsecutive paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_generate_error_message_reverse_order()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxOrderedIfSameDayVsConsecutive paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_generate_multiple_error_messages()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 23456, 12345);
	rules.addTimeConstraint(ctr);

	ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 23456, 34567);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 34567, 23456);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxOrderedIfSameDayVsConsecutive paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 2);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_reset_properly()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	ParadoxOrderedIfSameDayVsConsecutive paradox;
	processTimeSpaceConstraints(NULL);
	bool result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);

	result = paradox.prepare(rules);
	QVERIFY2(!result, "Paradox found");

	QCOMPARE(paradox.getErrors().size(), 1);
}

void ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest::test_added_to_generate()
{
	TimeConstraint* ctr = new ConstraintTwoActivitiesOrderedIfSameDay(100, 12345, 23456);
	rules.addTimeConstraint(ctr);
	ctr = new ConstraintTwoActivitiesConsecutive(100, 23456, 12345);
	rules.addTimeConstraint(ctr);
	rules.computeInternalStructure();

	bool success = processTimeSpaceConstraints(NULL);
	QVERIFY2(!success, "Paradox not checked in generate_pre");
}

QTEST_APPLESS_MAIN(ParadoxOrderedIfOnSameDayVsReversedConsecutiveTest)

#include "tst_paradoxorderedifonsamedayvsreversedconsecutivetest.moc"
