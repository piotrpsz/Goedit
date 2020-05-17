/********************************************************************
 * Copyright (C) 2020 Piotr Pszczolkowski
 *-------------------------------------------------------------------
 * This file is part of Goedit.
 *
 * Goedit is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Goedit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Goedit; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *-------------------------------------------------------------------
 * AUTHOR : Piotr Pszczolkowski (piotr@beesoft.pl)
 * PROJECT: Goedit
 * FILE   : Sidekick.cpp
 * DATE   : 17.05.2020
 *******************************************************************/

/*------- include files:
-------------------------------------------------------------------*/
#include <QAction>
#include "Sidekick/ProjectTab.h"
#include "Sidekick.h"

//*******************************************************************
//                            Sidekick                          CTOR
//*******************************************************************
Sidekick::Sidekick(QWidget *parent)
    : QDockWidget(parent)
    , _projectTab(new ProjectTab)
{
    setObjectName("Sidekick");
    toggleViewAction()->setIcon(QIcon(":/img/DockHorizontalIcon"));
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setWindowTitle("Sidekick");
    setWidget(_projectTab);
}
