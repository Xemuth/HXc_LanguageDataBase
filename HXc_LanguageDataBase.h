#ifndef _HXc_LanguageDataBase_HXc_LanguageDataBase_h_
#define _HXc_LanguageDataBase_HXc_LanguageDataBase_h_
#include <plugin/sqlite3/Sqlite3.h>
#include <Core/Core.h>
using namespace Upp;

#define SCHEMADIALECT <plugin/sqlite3/Sqlite3Schema.h>
#define MODEL <HXc_LanguageDataBase/HXc_LanguageDataBase.sch>
#include "Sql/sch_header.h"

#include <fstream>

/*
	Ce projet et son code source sont la propriété de Clément Hamon.
	Et sont strictement reservés à l'unique utilité et analyse.
	Copyright 2019 HXc_IA_project 
*/

/*
La BDD initiale pour la création de la BDD XML est http://www.lexique.org

Lexique3 ne m'appartient pas.
Copyleft © 2001 Lexique
Powered by phpWebLog
Université Savoie Mont Blanc
LPNC
RICS
CNRS 	

Présentation de Lexique : 
Lexique 3 est une base de données qui fournit pour 135 000 mots du français: 
les représentations orthographiques et phonémiques, la syllabation, la catégorie grammaticale, le genre et le nombre, les fréquences, les lemmes associés, etc.
Si vous êtes nouveau venu, commencez par télécharger et lire le manuel de Lexique.
Lexique permet d'obtenir des informations précises sur un mot ou, par exemple, de calculer diverses statistiques du français.
Ces informations sont stockées dans des tables que vous pouvez télécharger sur votre ordinateur ou interroger en ligne.
Lexique est une base de données libre à laquelle chacun est encouragé à participer. 
*/



/* Amelioration possible : 
	Inclure la phonétique du mots dans le but de pouvoir faire une IA Qui parle
*/

/*	
	Amelioration possible :
	via la programmation par reflection essayer de refactoriser la structure "unMot"
*/ 
const Upp::String Nomenclature_orthographe("1_ortho");
const Upp::String Nomenclature_lemme("3_lemme");
const Upp::String Nomenclature_type("4_cgram");
const Upp::String Nomenclature_genre("5_genre");
const Upp::String Nomenclature_nombre("6_nombre");

enum class DataTypeManager {
	Xml,
	Bdd	
};

class unMot : public Upp::Moveable<unMot>{	
	public:
		Upp::String orthographe; //Le mot récupérer
		Upp::String lemme; //unité autonome constituante du lexique d'une langue (par exemple a appartient à "AVOIR", le lemme de "a" est donc "avoir")
		Upp::String type;  //ADV VER ADJ NOM
		Upp::String genre; // m ou f selon masculin ou féminin
		Upp::String nombre; //singulier ou pluriel
		
		unMot();
		unMot(Upp::String orthographe, Upp::String lemme, Upp::String type,	Upp::String genre, Upp::String nombre);
		
		void Dump();
		Upp::String ToXML();
		void LoadXML(Upp::XmlParser& p);
		
};

class HXc_LanguageDataBase{
	private:
		DataTypeManager type;
		
		//----------Pour la gestion par XML----------------//
		Upp::Vector<unMot> lesMots;
		Upp::String leXml;
		void FindIteratorOf( Upp::Vector<Upp::String> &theLine,const Upp::String nomenclature, int &iterator);
		Upp::String removeAccent(Upp::String str);
		//----------Pour la gestion par Bdd---------------//
		void initSqlite3();
		Sqlite3Session sqlite3;

	public:
	//	HXc_LanguageDataBase() ;
		HXc_LanguageDataBase(Upp::String PathToTheData ="");
	//	HXc_LanguageDataBase(DataTypeManager dataType , Upp::String PathToTheData);
		
		~HXc_LanguageDataBase();
	
		void LoadXml();
		
		
		Upp::String TransformTxtToXml(Upp::String TxtDataBasePath);
		void TransformTxtToBdd(Upp::String pathToBdd); //Sql Lite only
		
		
		int nbMotsCharger();
		
		unMot GetAWord(Upp::String mot);
};

#endif
