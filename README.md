*WARNING: This repository is now archived, it is superseded by the [collatinus](https://github.com/biblissima/collatinus) repository, where you will find the source code of both the Collatinus desktop app and the Collatinus daemon used to process requests from the web client (collatinus-web) (see [Daemon](https://github.com/biblissima/collatinus/tree/Daemon) branch)*


# Collatinus-web

Ce dépôt contient les sources de Collatinus-web (partie serveur de l'application, sous la forme d'un démon).

Collatinus-web est la version en ligne de [Collatinus](http://outils.biblissima.fr/collatinus), un logiciel de lemmatisation et d'analyse morphologique de textes latins. Il permet de :
- rechercher un lemme dans plusieurs dictionnaires de latin (Gaffiot, Calonghi, Lewis & Short, du Cange, Georges, Valbuena)
- fléchir un lemme
- scander un texte latin
- lemmatiser un texte latin (7 langues cibles)
- effectuer son analyse morphologique

Il est développé par Yves Ouvrard et Philippe Verkerk avec l'aide de Régis Robineau dans le cadre de la "Boîte à Outils" de l'Equipex [Biblissima](http://www.biblissima-condorcet.fr).

Collatinus-web sur le site Biblissima : [http://outils.biblissima.fr/collatinus-web](http://outils.biblissima.fr/collatinus-web)

## Licence

Ce programme est mis à disposition par Yves Ouvrard et Philippe Verkerk sous licence [Creative Commons Attribution-NonCommercial 4.0 International License](http://creativecommons.org/licenses/by-nc/4.0/) (CC BY-NC).

## Instructions

L'application Collatinus-web est constituée de deux dépôts distincts :
- un dépôt pour l'interface web (client) : https://github.com/biblissima/collatinus-web-ui
- un autre pour la partie serveur (démon C++) : https://github.com/biblissima/collatinus-web-daemon

La partie serveur (démon) communique avec la partie cliente à travers un socket de connexion (port 5555). Côté client, le script traitement.php se charge de transmettre les requêtes au démon via fsockopen() et de récupèrer les réponses pour l'affichage des données dans la page web.

### Installation et compilation

    // Install Qt
    sudo apt-get install make g++ qt4-qmake qt4-qtconfig qt4-dev-tools
    
    // Compile
    qmake -config release
    make
    
    // Run daemon
    ./collatinusd
