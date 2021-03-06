//
//
// Description: This file is part of FET
//
//
// Author: Liviu Lalescu <Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>
// Copyright (C) 2003 Liviu Lalescu <https://lalescu.ro/liviu/>
//
/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 ***************************************************************************/

#include "teacher.h"
#include "rules.h"

Teacher::Teacher() :
	targetNumberOfHours(0)
{
}

Teacher::~Teacher()
{
}

QString Teacher::getXmlDescription() const
{
	QString s="<Teacher>\n";
	s+="	<Name>"+protect(name)+"</Name>\n";
	s+="	<Target_Number_of_Hours>"+CustomFETString::number(targetNumberOfHours)+"</Target_Number_of_Hours>\n";
	s+="	<Qualified_Subjects>\n";
	for(const QString& sbj : qAsConst(qualifiedSubjectsList))
		s+="		<Qualified_Subject>"+protect(sbj)+"</Qualified_Subject>\n";
	s+="	</Qualified_Subjects>\n";
	s+="	<Comments>"+protect(comments)+"</Comments>\n";
	s+="</Teacher>\n";

	return s;
}

QString Teacher::getDescription() const
{
	QString s=tr("N:%1", "The name of the teacher").arg(name);
	
	if(!comments.isEmpty())
		s+=", "+tr("C: %1", "Comments").arg(comments);
	
	return s;
}

QString Teacher::getDetailedDescription() const
{
	QString s=tr("Teacher");
	s+="\n";
	s+=tr("Name=%1", "The name of the teacher").arg(name);
	s+="\n";
	
	s+=tr("Target number of hours=%1", "The target number of hours for the teacher").arg(targetNumberOfHours);
	s+="\n";

	s+=tr("Qualified subjects:", "The list of qualified subjects for a teacher");
	s+="\n";
	for(const QString& sbj : qAsConst(qualifiedSubjectsList)){
		s+=sbj;
		s+="\n";
	}

	//Has comments?
	if(!comments.isEmpty()){
		s+=tr("Comments=%1").arg(comments);
		s+="\n";
	}

	return s;
}

QString Teacher::getDetailedDescriptionWithConstraints(const Rules &r) const
{
	QString s=this->getDetailedDescription();

	s+="--------------------------------------------------\n";
	s+=tr("Time constraints directly related to this teacher:");
	s+="\n";
	for(TimeConstraintsList::const_iterator it = r.timeConstraintsList.constBegin(); it != r.timeConstraintsList.constEnd(); ++it){
		const TimeConstraint *c = *it;
		if(c->isRelatedToTeacher(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}

	s+="--------------------------------------------------\n";
	s+=tr("Space constraints directly related to this teacher:");
	s+="\n";
	for(SpaceConstraintsList::const_iterator it = r.spaceConstraintsList.constBegin(); it != r.spaceConstraintsList.constEnd(); ++it){
		const SpaceConstraint *c = *it;
		if(c->isRelatedToTeacher(this)){
			s+="\n";
			s+=c->getDetailedDescription(r);
		}
	}
	s+="--------------------------------------------------\n";

	return s;
}

bool teachersAscending(const Teacher* t1, const Teacher* t2)
{
	return t1->name.localeAwareCompare(t2->name) < 0;
}
