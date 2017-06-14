/***** AUTEURS *****/
Decolle Anais 11609753
Auffret Quentin 11508216

/***** Fichiers du projet *****/
 - README.md : Ce fichier
 - BackgroundExtractor.h/.cpp, BlobExtractor.h/.cpp, Skeleton.h/.cpp et main.cpp : Fichiers sources du projet
 - CMakeLists.txt : Fichier de compilation
 - Analyse Référence.avi : Vidéo de référence pour l'extraction de fond
 - Analyse analyse.avi : Vidéo à analyser par l'application

/***** Installation et compilation *****/
L'application requiert les libraries suivantes :
 - CMake (sudo apt-get install cmake)
 - OpenCV avec les modules des contributeurs, disponible sur ce GitHub (avec la procédure d'installation) :
    https://github.com/opencv/opencv_contrib

Pour compiler, allez dans le répertoire racine du projet et tapez les commandes :
 - cmake CMakeLists.txt
 - make

Pour exécuter, tapez la commande :
 - ./Posture_Analysis