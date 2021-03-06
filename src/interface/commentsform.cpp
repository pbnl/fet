//
//
// Description: This file is part of FET
//
//
// Author: Lalescu Liviu <Please see https://lalescu.ro/liviu/ for details about contacting Liviu Lalescu (in particular, you can find here the e-mail address)>
// Copyright (C) 2005 Liviu Lalescu <https://lalescu.ro/liviu/>
//
/***************************************************************************
 *                                                                         *
 *   This program is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Affero General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
  ***************************************************************************/

#include "timetable.h"
#include "fet.h"

#include "centerwidgetonscreen.h"

#include "commentsform.h"

#include <QMessageBox>

extern Timetable gt;

extern bool simulation_running;

CommentsForm::CommentsForm(QWidget* parent): QDialog(parent)
{
	setupUi(this);
	
	//!!!Do NOT set commentsTextEdit read only
	
	okPushButton->setDefault(true);

	connect(cancelPushButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(okPushButton, SIGNAL(clicked()), this, SLOT(ok()));

	centerWidgetOnScreen(this);
	restoreFETDialogGeometry(this);
	
	commentsTextEdit->setPlainText(gt.rules.getComments());
	commentsTextEdit->selectAll();
	commentsTextEdit->setFocus();
}

CommentsForm::~CommentsForm()
{
	saveFETDialogGeometry(this);
}

void CommentsForm::ok()
{
	if(!simulation_running)
		gt.rules.setComments(commentsTextEdit->toPlainText());
	else{
		QMessageBox::information(this, tr("FET information"),
			tr("Cannot update comments during simulation."
			" Please stop simulation before this"));
		return;
	}

	this->close();
}
