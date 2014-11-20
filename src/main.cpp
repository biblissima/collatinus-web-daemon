/*                           main.cpp
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
 *  You should have received a copy of the GNU General Public License *  along with COLLATINVS; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 */
#include <QtCore>
#include <QString>
#include <QTextStream>
#include "main.h"
// variables

Serveur::Serveur ()
{
	// lecture des données
    lexicum = new Lexicum ();
    lexicum->dicLinguam ("fr");
    syntaxe = new Syntaxe (qApp->applicationDirPath () +"/ressources/expressions.fr", lexicum);
    // fprintf (stdout, qPrintable ("lexiques chargés\n"));
    QDir chDicos (qApp->applicationDirPath () +"/ressources/");
    QStringList lcfg = chDicos.entryList
    (QStringList () << "*.cfg");
    QStringList ldic;
    foreach (QString fcfg, lcfg)
    {
        Dictionnaire * d = new Dictionnaire (fcfg);
        listeD.ajoute (d);
        ldic << d->nom ();
    }
	// initialisation et connexion du serveur
    serveur = new QTcpServer (this);
	connect (serveur, SIGNAL(newConnection()), this, SLOT (connexion ()));
	if (!serveur->listen (QHostAddress::LocalHost, 5555)) 
	{
	    std::cout << "Ne peux écouter.\n";
		return;
	}
	std::cout << "Collatinus est lancé en console. Ctrl-C pour l'arrêter.\n";
}

void Serveur::connexion ()
{
    soquette = serveur->nextPendingConnection ();
	connect (soquette, SIGNAL (readyRead ()), this, SLOT (exec ()));
}

QString Serveur::lemm (QString requete, QString lang)
{
    lexicum->dicLinguam (lang);
    QStringList lignes;
    QStringList echecs;
    while (!requete.isEmpty ())
    {
        int p = requete.indexOf (Ch::rePonct);
        QString t;
        if (p < 0)
        {
            t = requete;
            requete.clear ();
        }
        else 
        {
            t = requete.left (p+1);
            requete.remove (0, p+1);
        }
        syntaxe->creePhrase (t.simplified (), 0, t.length ());
        QStringList sl = syntaxe->lemmatisation (false);
        lignes << sl.join ("");
        echecs << syntaxe->getEchecs ();
    }
    /*
    lignes << "<br/>" << "<strong>HAEC NON RECOGNITA:</strong><br/>";
    echecs.removeDuplicates ();
    lignes << echecs;
    */
    return lignes.join ("");
}


QString Serveur::analyse_morpho (QString requete, QString lang)
{
    // éclater le texte en mots
    lexicum->dicLinguam (lang);
    QStringList eclats = requete.split (QRegExp ("\\W+"));
    QStringList retour;
    QString html;
    foreach (QString eclat, eclats)
		if (!eclat.isEmpty ())
		{
			retour << "<span class=\"forme-accroche\">" + eclat + "</span>"; // forme en accroche
			retour << "<div class=\"analyse-morpho\">";
			retour << lexicum->lemmatiseM (eclat);
			retour << "</div>";
		}
	return retour.join ("");
}

QString Serveur::flexion (QString requete)
{
   QString forme = requete.section (" ", 0, 0);
   Entree * e = NULL;
   QStringList lfl;
   QStringList lnk;
   if (forme.contains ("|"))
   {
       e = new Entree (forme);
       lnk.append (e->quantite ());
       lfl << "<div id=a\"" + e->quantite () + "\">" + lexicum->flechis (e) + "</div>\n";
       delete e;
   }
   else
   {
       ListeAnalyses analyses = lexicum->lanalyses (forme);
       if (analyses.empty ())
           return forme + " ?";
       Entree * ae = NULL;
       foreach (AnalyseMorpho * am, analyses)
       {
           Entree *e = am->entree ();
           if (e != ae)
           {
               lnk.append (e->quantite ());
               lfl << "<div id=\"" + e->quantite () + "\">" + lexicum->flechis (e) + "</div>\n";
           }
           ae = e;
       }
       lexicum->deleteAnalyses (analyses);
   }
    QString liens;
    foreach (QString l, lnk)
    {
        liens.append ("<a href=\"#"+l+"\">"+l+"</a>&nbsp;");
    }
    liens.append ("<br/n>\n");
    QString html;
    for (int i=0;i<lnk.length ();++i)
    {
        html.append ("<div id=\""+lnk[i]+"\">");
        html.append (liens);
        html.append (lfl[i]);
        html.append ("</div>\n");
    }
   return html;
}

QString Serveur::mesure (QString requete)
{
    return lexicum->scandeTxt (requete);
}

void Serveur::exec ()
{
	QByteArray octets = soquette->readAll ();
    // retour
	QString requete = QString (octets); 
	QString p;
	QString retour;
	QTextStream ts (&retour);
	p = requete.section (":", 0,0);
    requete = requete.mid (p.length ()+1);
    if (p.at (0) == 'l')
    {
        p.remove (0, 1); 
        ts << lemm (requete, p); // p = langue
    }
    else if (p.at (0) == 'a')
    {
        p.remove (0, 1); 
        ts << analyse_morpho (requete, p);
    }
    else if (p == "m")
        ts << mesure (requete);
    else if (p == "f")
        ts << flexion (requete);
    else if (p.at (0) ==  'd')
    {
        QStringList listekr; 
        // requête de dictionnaire
        // demande de page : la requête est un nombre
        bool nombre = false;
        // répertoire pour les png
        QString repPng = "";
        requete.toInt (&nombre);
        if (nombre)
        {
            listekr << requete;
        }
        else if (requete.startsWith ("@"))
        {
            requete = requete.mid (1);
            listekr << requete;
        }
        else
        {
            ListeAnalyses analyses = lexicum->lanalyses (requete);
            foreach  (AnalyseMorpho *am, analyses)
            {
                listekr << am->entree ()->canon ();
            }
            listekr.removeDuplicates ();
            if (listekr.empty ()) listekr << requete;
        }
        if (p == "dge")
        {
            listeD.change_courant ("Georges_1913");
        }
        else if (p == "dle") 
        {
            listeD.change_courant ("Lewis_and_Short_1879");
        }
        else if (p == "dga")
        {
            listeD.change_courant ("Gaffiot_1934");
            repPng = "gaffiot";
        }
        else if (p == "dca")
        {
            listeD.change_courant ("Calonghi_1898");
            repPng = "calonghi";
        }
        else if (p == "ddu")
        {
            listeD.change_courant ("du_Cange_1883-7");
        }
        else if (p == "dva")
        {
            listeD.change_courant ("Valbuena_1819");
            repPng = "valbuena";
        }
        // extraction, conversion et définition du lien
        listeD.courant ()->setRepPng (repPng);
        ts << listeD.courant ()->page (listekr);
        /*
           Calonghi_1898.cfg
           du_Cange_1883-7.cfg
           Gaffiot_1934.cfg
           Georges_1913.cfg
           Lewis_and_Short_1879.cfg
        */
    }
	soquette->write (retour.toUtf8 ());
	soquette->close ();
}

int main(int argc, char **argv)
{
    fprintf (stdout, "Collatinus 10.3\n"); 
#if (QT_VERSION < 0x050000)
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
#endif
    QCoreApplication app(argc, argv);
    Serveur s;
    app.exec ();
    return 0; 
}
