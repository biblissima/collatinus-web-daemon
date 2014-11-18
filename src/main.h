/*                 main.h
 *
 *  This file is part of COLLATINVS.
 *                                                                            
 *  COLLATINVS is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *                                                                            
 *  COLLATINVS is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *                                                                            
 *  You should have received a copy of the GNU General Public License along
 *  with COLLATINVS; if not, write to the Free Software Foundation, Inc., 59
 *  Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef MAIN_H
#define MAIN_H

#include "libcollatinus.h"
#include "syntaxe.h"
#include "lewis.h"
#include <iostream>
#include <QtCore>
#include <QtNetwork>

class QTcpServer;

class Serveur : public QObject
{
	Q_OBJECT

	public:
		Serveur ();
    private slots:
	    void connexion ();
		QString lemm (QString requete, QString lang="fr");
        QString analyse_morpho (QString requete, QString lang="fr");
		QString flexion (QString requete);
		QString mesure (QString requete);
		void exec ();
	private:
	    //QUdpSocket socket;
		QTcpServer * serveur;
		QTcpSocket * soquette;
        Lexicum * lexicum;
        Syntaxe * syntaxe;
        ListeDic listeD;
	signals:
	    void donneesRecues (QString texte);
};

#endif
