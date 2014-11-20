/*                           lewis.cpp
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
 *  You should have received a copy of the GNU General Public License
 *  along with COLLATINVS; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *  
 */

#include "lewis.h"
#include <QtXmlPatterns>

/****************
 * Dictionnaire *
 ****************/

Dictionnaire::Dictionnaire (QString cfg)
{
    QFileInfo fi(cfg);
    repertoire = qApp->applicationDirPath () + "/ressources/";

    n = fi.baseName ().section ('.', 0);
    // lire le fichier de ressource cfg
    QSettings settings (repertoire + cfg, QSettings::IniFormat);
    settings.setIniCodec ("utf-8");
    settings.beginGroup ("droits");
    auteur = settings.value ("auteur").toString ();
    url = settings.value ("url").toString ();
    settings.endGroup ();
    settings.beginGroup ("fichiers");
    chData = repertoire + settings.value ("data").toString ();
    debut = settings.value ("debut").toInt ();
    echelle = settings.value ("echelle").toString ();
    if (echelle.isEmpty ()) echelle = "160";
    idxJv = repertoire + n + ".idx";
    settings.endGroup ();
    settings.beginGroup ("remplacements");
    cond_jv = settings.value ("condjv").toString ();
    ji = settings.value ("ji").toInt ();
    JI = settings.value ("JI").toInt ();
    settings.endGroup ();
    settings.beginGroup ("style");
    xsl = settings.value ("xsl").toInt ();
    settings.endGroup ();
    xml = QFileInfo (chData).suffix () == "xml";
    djvu = !xml;
    //flien = "<form method=\"post\" name=\"%1\">\n"
        //"<input type=\"hidden\" name=\"r\" id=\"r\" value=\"d%2:@%1\">\n</form>\n"
        //"<a href=\"#\" onClick=\"document['%1'].submit();return false;\">%1</a>\n";
    flien = "<li class=\"%3\">\n"
    			"<a href=\"#\" data-value=\"d%2:@%1\">%1</a>\n"
    		"</li>\n";
}

QString Dictionnaire::nom ()
{
    return n;
}


QString Dictionnaire::convert (QString source)
{
    QXmlQuery query(QXmlQuery::XSLT20);
    query.setFocus (source);
    QString fichier = repertoire + n + ".xsl";
    QFile xml2html (fichier);
    xml2html.open (QIODevice::ReadOnly|QIODevice::Text);
    query.setQuery (&xml2html);
    QString html, retour;
    QTextStream ts (&retour);
    ts.setCodec ("UTF-8");
    query.evaluateTo (&html);
    ts << html;
    return retour ;
}

QString Dictionnaire::entree_pos (qint64 pos)
{
   QFile file (chData);
   file.open (QFile::ReadOnly | QFile::Text);
   file.seek (pos);
   QTextStream ts (&file);
   ts.setCodec ("UTF-8");
   QString linea = ts.readLine ();
   file.close ();
   if (xsl)
   {
       linea = convert (linea);
       return linea;
   }
   else 
   {
       linea.replace("H1>","strong>");
       linea.prepend("<br/>\n");
       return linea;
   }
   return "Error. Nil legere potui.";
}

void Dictionnaire::vide_index ()
{
    idxDjvu.clear ();
}

void Dictionnaire::vide_ligneLiens ()
{
    ligneLiens.clear ();
}

bool Dictionnaire::lis_index_djvu ()
{
    QFile f (idxJv);
    if (!f.open (QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream ts (&f);
    while (!f.atEnd ())
    {
        idxDjvu << f.readLine ().trimmed ();
    }
    f.close ();
    return true;
}

QString Dictionnaire::pageDjvu (int p)
{
    qDebug () << "debog pageDjvu p" << p;
    QString pg;
    // variables feuilletage;
    prec = QString::number (p-1);
    suiv = QString::number (p+1);
    // liens
    QTextStream (&pg) << auteur << " <a href=\"" << url <<"\">" << url << "</a> "
        << "<ul class=\"liste-liens\">\n" 
        << ligneLiens 
        << "</ul>\n"
        // feuilletage haut
        << "<ul class=\"pager\">\n"
        << flien.arg (prec).arg(n.left (2).toLower ()).arg("previous") 
        << "<li class=\"lead\">" << p << "</li>\n"
        << flien.arg (suiv).arg(n.left (2).toLower ()).arg("next")
        << "</ul>"
        // image
        << "<img src=\"png/" << repPng << "/" << p << ".png\">"
         // feuilletage bas
        << "<ul class=\"pager\">\n"
        << flien.arg (prec).arg(n.left (2).toLower ()).arg("previous") 
        << "<li class=\"lead\">" << p << "</li>\n"
        << flien.arg (suiv).arg(n.left (2).toLower ()).arg("next")
        << "</ul>";
    qDebug () << "debog pg" << pg;
    return pg;
}

QString Dictionnaire::pageDjvu (QStringList req, int no)
{
    // seul le lemme n° noLien est affiché, les autres sont en hyperliens.
    // Si l'index chargé est le bon, inutile de le relire.
    leLem = req[no];
    bool nombre = false;
    int np = leLem.toInt (&nombre);
    // si leLem est un nombre, aller directement à la page.
    if (nombre)
    {
        return pageDjvu (np);
    }
    if (idxDjvu.isEmpty ())
    {
        lis_index_djvu ();
    }
    leLem = ramise (leLem);
    pdj = debut;
    foreach (QString l, idxDjvu)
    {
        if (QString::compare (l, leLem, Qt::CaseInsensitive) > 0)
        {
            break;
        }
        ++pdj;
    }
    ligneLiens.clear ();
    foreach (QString lien, req)
    {
         ligneLiens.append (flien.arg (lien).arg(n.left (2).toLower ()).arg(""));
    }
    return pageDjvu (pdj);
}

QString Dictionnaire::pageXml (QStringList req)
{
    QString pg; // contenu de la page de retour
    llew listeE;
    QFile * findex = NULL;
    ligneLiens.clear ();
    QStringList ante; // feuilletage
    QStringList post;
    prec.clear (); suiv.clear ();
    foreach (QString l, req)
    {
        l = l.toLower ();
        findex = new QFile (idxJv);
        if (JI) l.replace ('J', 'I');
        if (ji) l.replace ('j', 'i');
        if (findex == NULL || !findex->open(QFile::ReadOnly | QFile::Text))
        {
            prec = "error";
            suiv = "error";
            return "Error";
        }
        QString linea;
        QStringList eclats;
        QString avanDerCh, derCh, ch;
        bool trouve = false;
        int trouve1 = -10; // on commence avec trouve1 négatif puisque "a" est < l
        int p;
        while (trouve1 < 0) // on s'arrête quand on a dépassé l
        {
            linea = findex->readLine ();
            p = linea.indexOf(":");
            if (p > -1)
            {
                avanDerCh = derCh;
                derCh = ch;
                eclats = linea.split(":");
                ch = eclats[0];
                trouve  = QString::compare (ch, l, Qt::CaseInsensitive) == 0;
                trouve1 =  QString::compare (ch, l+"1", Qt::CaseInsensitive);
                // trouve1 est un entier qui est la différence entre les chaines
                if (trouve)
                {
                    prec = derCh;
                    if (eclats.size () == 3)
                    {
                        listeE.append (pairL (eclats[2].trimmed (), eclats[1].toLongLong ()));
                    }
                    else listeE.append (pairL (ch, eclats[1].toLongLong ()));
                    linea = findex->readLine ();
                    eclats = linea.split (":");
                    ch = eclats [0];
                    trouve1 = 1;
                    ante << prec;
                    post << ch; 
                }
                else if (trouve1 == 0)
                {
                    prec = derCh;
                    while (QRegExp ("^"+l+"\\d+$").exactMatch (ch.toLower ()))
                    {
                        if (eclats.size() == 3)
                        {
                            listeE.append (pairL (eclats[2].trimmed (), eclats[1].toLongLong ()));
                        }
                        else listeE.append (pairL (ch, eclats[1].toLongLong ()));
                        linea = findex->readLine ();
                        eclats = linea.split(":");
                        ch = eclats[0];
                        ante << prec;
                        post << ch;  // = suiv
                    }
                }
                else if (trouve1 > 0 || findex->atEnd ())
                {
                    // je ne suis pas sûr que ce dernier else soit nécessaire
                    QString rl = ramise (l);
                    if (rl != l)
                    {
                        QStringList lramise; 
                        lramise << rl;
                        return pageXml (lramise);
                    }
                    prec = derCh;
                    if (eclats.size() == 3)
                    {
                        listeE.append (pairL (eclats[2].trimmed (), eclats[1].toLongLong ()));
                    }
                    else listeE.append (pairL (ch, eclats[1].toLongLong ()));
                    ante << prec;
                    post << suiv; 
                    break;
                }
            }
        }
        suiv = eclats[0];
        findex->close ();
    }
    for (int i=0;i<listeE.size();i++)
    { 
        ligneLiens.append ("<li><a href=\"#"+listeE[i].first+"\">"+listeE[i].first+"</a></li>");
    }
    //ligneLiens.append ("<br/>\n");
    for (int i=0;i<listeE.size();i++)
    {
        pg.append ("\n<div id=\""+listeE[i].first+"\" class=\"entree-dico\">");
        pg.append ("<ul class=\"liste-liens\">\n" + ligneLiens + "</ul>\n");
        /// feuilletage haut
        pg.append ("<ul class=\"pager\">\n");
        pg.append (flien.arg (ante[i]).arg(n.left (2).toLower ()).arg("previous"));
        //pg.append ("&larr; &rarr;"); -> ajoutés en css via classes previous/next
        pg.append (flien.arg (post[i]).arg(n.left (2).toLower ()).arg("next"));
        pg.append ("</ul>");
        // entrées
        QString np = entree_pos (listeE[i].second);
        pg.append (np);
        pg.append ("</div>");
    }
    /* NB : styles adequats inclu dans css locale
    if (QFile::exists (repertoire + n +  ".css"))
    {
        pg.prepend ("<link rel=\"stylesheet\" href=\""+repertoire+n+".css\" type=\"text/css\" />\n");
    }*/
    pg.prepend (auteur + " <a href=\"http://"+url+"\">"+url+ "</a> ");
    /*
    // code de débogage
    QFile fdebug ("debug.html");
    fdebug.open (QFile::WriteOnly | QFile::Text);
    QTextStream fl (&fdebug);
    fl << pg; 
    fdebug.close ();
    */
    return pg;
}

QString Dictionnaire::page (QStringList req, int no)
{
    if (xml)
    {
        // qDebug () << "xml" << req << no;
        return pageXml (req);
    }
    else // djvu
    {
        liens = req;
        return pageDjvu (req, no); // passage de req pour les hyperliens
    }
}

bool Dictionnaire::estXml ()
{
   return xml;
}

QString Dictionnaire::pgPrec ()
{
    return prec;
}

QString Dictionnaire::pgSuiv ()
{
    return suiv;
}

int Dictionnaire::noPageDjvu ()
{
    return pdj;
}

QString Dictionnaire::indexJv ()
{ 
    return idxJv;
}

QStringList Dictionnaire::links () 
{
    return liens;
}

/****************
*    ListeDic   * 
*****************/

Dictionnaire * ListeDic::dictionnaire_par_nom (QString nom)
{
    QMap<QString, Dictionnaire*>::iterator retour = liste.find (nom);
    if (retour == liste.end ())
        return NULL;
    return retour.value ();
}

void ListeDic::ajoute (Dictionnaire *d)
{
    liste.insert (d->nom (), d);
}

void ListeDic::change_courant (QString nom)
{
    currens = dictionnaire_par_nom (nom);
}

Dictionnaire * ListeDic::courant ()
{
    return currens;
}

QString Dictionnaire::ramise (QString f)
{
    if (!ji)
        f = f.replace (QRegExp ("(^|[aeo]+|^in|^ad|^per)i([aeiou])"), "\\1j\\2");
    f = f.replace (QRegExp ("(^|[aeio]+|^in|^ad|^per)u([aeiou])"), "\\1v\\2");
    f = f.replace (QRegExp ("(^|[\\w]+r)u([aeiou])"), "\\1v\\2");
    return f;
}

void Dictionnaire::setRepPng (QString rp)
{
    repPng = rp;
}
