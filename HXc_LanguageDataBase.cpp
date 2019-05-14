
#include "HXc_LanguageDataBase.h"
#include <Sql/sch_source.h>
#include <Sql/sch_schema.h>

#include <Core/Core.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>  



using namespace Upp;




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

/* 
Module de Base utilisé dans la Création de l'IA HXc 
Ce module permet de récupérer des données relatif à la langue français dans une base de données au Format XML  
Ce module est serat une dépendance de HXc Language Analyser permettant de sortir une analyse
basic d'un texte
*/

unMot::unMot(){
	
}
unMot::unMot(Upp::String orthographe, Upp::String lemme, Upp::String type,	Upp::String genre, Upp::String nombre){
	this->orthographe = orthographe;
	this->lemme = lemme;
	this->type = type;
	this->genre = genre;
	this->nombre = nombre;
}
		
void   unMot::Dump(){
	if(orthographe.GetCount() >0){		
		Cout() << ((orthographe.GetCount()>0)?(orthographe + "\n"): "");
		Cout() << ((lemme.GetCount()>0)?(lemme + "\n"): "");
		Cout() << ((type.GetCount()>0)?(type + "\n"): "");
		Cout() << ((nombre.GetCount()>0)?(nombre + "\n"): "");
		Cout() << ((genre.GetCount()>0)?(genre + "\n"): "");
	}
	Cout() << "-----------------" <<"\n";
}
String unMot::ToXML(){
	//Dump();
	return  XmlTag("Mot")(
		XmlTag("Orthographe").Text(orthographe )+ "\n"+
		XmlTag("Lemme").Text(lemme)+ "\n"+
		XmlTag("Type").Text(type)+ "\n"+
		XmlTag("Nombre").Text(nombre)+ "\n"+
		XmlTag("Genre").Text(genre)
	);
}

void unMot::LoadXML(XmlParser& p){
	while(!p.IsTag())
		p.Skip();
	p.ReadTag();
	while(!p.End()){
		if(p.Tag("Orthographe")){
			this->orthographe =p.ReadText();
			p.PassEnd();
		}
		else if(p.Tag("Lemme")){
			this->lemme =p.ReadText();
			p.PassEnd();
		}
		else if(p.Tag("Type")){
			this->type =p.ReadText();
			p.PassEnd();
		}
		else if(p.Tag("Nombre")){
			this->nombre =p.ReadText();
			p.PassEnd();
		}
		else if(p.Tag("Genre")){
			this->genre =p.ReadText();
			p.PassEnd();
		}	
		else
			p.PassEnd();
		}
}


/*
HXc_LanguageDataBase::HXc_LanguageDataBase(){
	std::string fonc;
	Cout() << "Démarrage du module de Base de données Française..." <<"\n";
	Sleep(100);
	while(fonc.length() < 1){
		Cout() << "Choisissez le mode de fonctionnement : \"Bdd\" ou \"Xml\" \n";
		std::cin >> fonc;
		ToUpper(fonc);
		if(fonc.compare("BDD") == 0){
			this->type = DataTypeManager::Bdd;
			String path = GetExeFilePath();
			path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.db");
			if(FileExists(path)){
				sqlite3.LogErrors(true);
				if(!sqlite3.Open(path)) {
					LOG("Bdd impossible à ouvrir !");
					exit(0);
				}
				Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
			}
			else{
				Cout() << "Base de données introuvable. Veuillez données un fichier de base (Lexique3)." <<"\n";
				bool isDefault=false;
				char szFile[100] ;
				OPENFILENAME ofn ;
				ZeroMemory( &ofn , sizeof( ofn));
				ofn.lStructSize = sizeof ( ofn );
				ofn.hwndOwner = NULL  ;
				ofn.lpstrFile = szFile ;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof( szFile );
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex =1;
				ofn.lpstrFileTitle = NULL ;
				ofn.nMaxFileTitle = 0 ;
				ofn.lpstrInitialDir=NULL ;
				ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
				BOOL selected = GetOpenFileName( &ofn );
				// Now simpley display the file name 
				//	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
				if(selected){
					Cout() << "Transformation du fichier..." <<"\n";
					path =ofn.lpstrFile;
					TransformTxtToBdd(path);
					Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
				}
			}	
		}
		else if(fonc.compare("XML") == 0){
			this->type = DataTypeManager::Xml;
			String path = GetExeFilePath();
			path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.Xml");
			if(FileExists(path)){
				Cout() << "Base de données trouvée. Chargement des Mots...\n";
				FileIn in(path);
				if(!in) {
					Cout() << "Error lors de l'ouverture de la Base de données" <<"\n";
					exit(0);
				}
				int c;
				while((c = in.Get()) >= 0)
					this->leXml.Cat(c);
				LoadXml();
				Cout() <<"Chargement terminé : " <<this->nbMotsCharger() <<" mots chargés"<<"\nModule Linguistique Français prêt !\n-----------------\n";
			}
			else{
				Cout() << "Base de données introuvable. Veuillez données un fichier de base (Lexique3)." <<"\n";
				bool isDefault=false;
				char szFile[100] ;
				OPENFILENAME ofn ;
				ZeroMemory( &ofn , sizeof( ofn));
				ofn.lStructSize = sizeof ( ofn );
				ofn.hwndOwner = NULL  ;
				ofn.lpstrFile = szFile ;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof( szFile );
				ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
				ofn.nFilterIndex =1;
				ofn.lpstrFileTitle = NULL ;
				ofn.nMaxFileTitle = 0 ;
				ofn.lpstrInitialDir=NULL ;
				ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
				BOOL selected = GetOpenFileName( &ofn );
				// Now simpley display the file name 
				//	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
				if(selected){
					Cout() << "Transformation du fichier..." <<"\n";
					path =ofn.lpstrFile;
					TransformTxtToXml(path);
					Cout() <<"Transformation Reussite !\nChargement terminé : " <<this->nbMotsCharger() <<" mots chargés" << "\nModule Linguistique Français prêt !\n-----------------\n";
				}
			}
		}
		else{
			fonc = "";	
		}
	}
}
*/
HXc_LanguageDataBase::HXc_LanguageDataBase(Upp::String PathToTheData){
	if(!FileExists(PathToTheData)){
		std::string fonc;
		Cout() << "Démarrage du module de Base de données Française..." <<"\n";
		Sleep(100);
		while(fonc.length() < 1){
			Cout() << "Choisissez le mode de fonctionnement : \"Bdd\" ou \"Xml\" \n";
			std::cin >> fonc;
			ToUpper(fonc);
			if(fonc.compare("BDD") == 0){
				this->type = DataTypeManager::Bdd;
				String path = GetExeFilePath();
				path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.db");
				if(FileExists(path)){
					sqlite3.LogErrors(true);
					if(!sqlite3.Open(path)) {
						LOG("Bdd impossible à ouvrir !");
						exit(0);
					}
					Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
				}
				else{
					Cout() << "Base de données introuvable. Veuillez données un fichier de base (Lexique3)." <<"\n";
					bool isDefault=false;
					char szFile[100] ;
					OPENFILENAME ofn ;
					ZeroMemory( &ofn , sizeof( ofn));
					ofn.lStructSize = sizeof ( ofn );
					ofn.hwndOwner = NULL  ;
					ofn.lpstrFile = szFile ;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof( szFile );
					ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
					ofn.nFilterIndex =1;
					ofn.lpstrFileTitle = NULL ;
					ofn.nMaxFileTitle = 0 ;
					ofn.lpstrInitialDir=NULL ;
					ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
					BOOL selected = GetOpenFileName( &ofn );
					// Now simpley display the file name 
					//	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
					if(selected){
						Cout() << "Transformation du fichier..." <<"\n";
						path =ofn.lpstrFile;
						TransformTxtToBdd(path);
						Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
					}
				}	
			}
			else if(fonc.compare("XML") == 0){
				this->type = DataTypeManager::Xml;
				String path = GetExeFilePath();
				path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.Xml");
				if(FileExists(PathToTheData)){
					Cout() << "Base de données trouvée. Chargement des Mots...\n";
					FileIn in(PathToTheData);
					if(!in) {
						Cout() << "Error lors de l'ouverture de la Base de données" <<"\n";
						exit(0);
					}
					int c;
					while((c = in.Get()) >= 0)
						this->leXml.Cat(c);
					LoadXml();
					Cout() <<"Chargement terminé : " <<this->nbMotsCharger() <<" mots chargés"<<"\nModule Linguistique Français prêt !\n-----------------\n";
				}
				else{
					Cout() << "Base de données introuvable. Veuillez données un fichier de base (Lexique3)." <<"\n";
					bool isDefault=false;
					char szFile[100] ;
					OPENFILENAME ofn ;
					ZeroMemory( &ofn , sizeof( ofn));
					ofn.lStructSize = sizeof ( ofn );
					ofn.hwndOwner = NULL  ;
					ofn.lpstrFile = szFile ;
					ofn.lpstrFile[0] = '\0';
					ofn.nMaxFile = sizeof( szFile );
					ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
					ofn.nFilterIndex =1;
					ofn.lpstrFileTitle = NULL ;
					ofn.nMaxFileTitle = 0 ;
					ofn.lpstrInitialDir=NULL ;
					ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
					BOOL selected = GetOpenFileName( &ofn );
					// Now simpley display the file name 
					//	MessageBox ( NULL , ofn.lpstrFile , "File Name" , MB_OK);
					if(selected){
						Cout() << "Transformation du fichier..." <<"\n";
						path =ofn.lpstrFile;
						TransformTxtToXml(path);
						Cout() <<"Transformation Reussite !\nChargement terminé : " <<this->nbMotsCharger() <<" mots chargés" << "\nModule Linguistique Français prêt !\n-----------------\n";
					}
				}
			}
			else{
				fonc = "";	
			}
		}
	}else{
		std::string fonc;
		Cout() << "Démarrage du module de Base de données Française..." <<"\n";
		Sleep(100);
		while(fonc.length() < 1){
			Cout() << "Choisissez le mode de fonctionnement : \"Bdd\" ou \"Xml\" \n";
			std::cin >> fonc;
			ToUpper(fonc);
			if(fonc.compare("BDD") == 0){
				this->type = DataTypeManager::Bdd;
				sqlite3.LogErrors(true);
				if(!sqlite3.Open(PathToTheData)) {
					LOG("Bdd impossible à ouvrir !");
					exit(0);
				}
				Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
			}
			else if	(fonc.compare("XML") == 0){
				this->type = DataTypeManager::Xml;
				FileIn in(GetDataFile(PathToTheData));
				if(!in) {
					Cout() << "Error lors de l'ouverture de la Base de données" <<"\n";
					exit(0);
				}
				int c;
				while((c = in.Get()) >= 0)
					this->leXml.Cat(c);
				LoadXml();
				Cout() <<"Transformation Reussite !\nChargement terminé : " <<this->nbMotsCharger() <<" mots chargés" << "\nModule Linguistique Français prêt !\n-----------------\n";
			}
			else{
				fonc = "";	
			}
		}
	}
}
/*
HXc_LanguageDataBase::HXc_LanguageDataBase(DataTypeManager dataType , Upp::String PathToTheData){
	this->type = dataType;
	if(type == DataTypeManager::Bdd){
		if(FileExists(PathToTheData)){
			sqlite3.LogErrors(true);
			if(!sqlite3.Open(PathToTheData)) {
				LOG("Bdd impossible à ouvrir !");
				exit(0);
			}
			Cout() <<"Chargement terminé : " << nbMotsCharger() <<" mots en Bdd !"<<"\nModule Linguistique Français prêt !\n-----------------\n";
		}else{
			LOG("Fichier Introuvable...");
			exit(0);
		}
	}
	else if(type == DataTypeManager::Xml){
		if(FileExists(PathToTheData)){
			FileIn in(GetDataFile(PathToTheData));
			if(!in) {
				Cout() << "Error lors de l'ouverture de la Base de données" <<"\n";
				exit(0);
			}
			int c;
			while((c = in.Get()) >= 0)
				this->leXml.Cat(c);
			LoadXml();
			Cout() <<"Transformation Reussite !\nChargement terminé : " <<this->nbMotsCharger() <<" mots chargés" << "\nModule Linguistique Français prêt !\n-----------------\n";
		}
		else{
			LOG("Fichier Introuvable...");
			exit(0);	
		}
	}
}*/

HXc_LanguageDataBase::~HXc_LanguageDataBase(){
	
}

void HXc_LanguageDataBase::initSqlite3(){
		String path = GetExeFilePath();
		path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.db");
		sqlite3.LogErrors(true);
		if(!sqlite3.Open(path)) {
			LOG("Bdd impossible à ouvrir !");
			exit(0);
		}
		SQL = sqlite3;
		SqlSchema sch(SQLITE3);
		All_Tables(sch);
		
		SqlPerformScript(sch.Upgrade());
		SqlPerformScript(sch.Attributes());
		SqlPerformScript(sch.ConfigDrop());
		SqlPerformScript(sch.Config());
		
		sch.SaveNormal();
}

void HXc_LanguageDataBase::TransformTxtToBdd(Upp::String pathToBdd){
	if(FileExists(pathToBdd)){
		std::ifstream originFile;
		char* buffer;
		originFile.open(pathToBdd.ToStd());
		Cout()<< "Attention ! La première ligne doit être une Entête !" <<"\n";
		Cout() <<"Travail en cours..."<<"\n";
		int cpt = 0;
		std::string str; 
		
		int orthographeIterator = -1;
		int lemmeIterator = -1;
		int typeIterator = -1;
		int genreIterator = -1;
		int nombreIterator = -1;
	    while (std::getline(originFile, str))
	    { 
	          Upp::String aLine(str);
	          if(cpt == 0){
	            auto y=  Split(aLine, "\t",false);
	            FindIteratorOf(y,Nomenclature_orthographe,orthographeIterator);
				FindIteratorOf(y,Nomenclature_lemme,lemmeIterator);
				FindIteratorOf(y,Nomenclature_type,typeIterator);
				FindIteratorOf(y,Nomenclature_genre,nombreIterator);
				FindIteratorOf(y,Nomenclature_nombre,genreIterator);         
	            cpt++;
	          }else{
	           	auto y=  Split(aLine, "\t",false);
	           	lesMots.Add(unMot( this->removeAccent(y[orthographeIterator]), y[lemmeIterator], y[typeIterator],y[genreIterator], y[nombreIterator]));
	         	cpt++;
	          } 
	    }
		originFile.close();
		
		initSqlite3();
		SQL = sqlite3;
		Sql sql;
		String theRequete;
	//	Sql insert("insert into MOTS(ORTHOGRAPHE,LEMME,MTYPE,ANOMBRE,AGENRE) values(?,?,?,?,?)",sqlite3);
		theRequete << "insert into MOTS(ORTHOGRAPHE,LEMME,MTYPE,ANOMBRE,AGENRE) values";
		int cpt2 = 0;
		for(unMot &m : lesMots){
			if(cpt2>0) theRequete << ",";
			theRequete << "(\"" << m.orthographe <<"\",\"" << m.lemme << "\",\"" << m.type << "\",\"" << m.nombre << "\",\"" << m.genre << "\")";
			cpt2++;	
		}
		theRequete <<";";
		//	insert.Run(m.orthographe,m.lemme,m.type,m.nombre,m.genre);
		
		sql.Execute(theRequete);
		return;
		
	}
	exit(0);
}

Upp::String HXc_LanguageDataBase::TransformTxtToXml(Upp::String TxtDataBasePath){
	if(FileExists(TxtDataBasePath)){
			std::ifstream originFile;
			char* buffer;
			originFile.open(TxtDataBasePath.ToStd());
			Cout()<< "Warning ! First row of selected File Must be an header !" <<"\n";
			Cout() <<"Travail en cours..."<<"\n";
			int cpt = 0;
			std::string str; 
			
			int orthographeIterator = -1;
			int lemmeIterator = -1;
			int typeIterator = -1;
			int genreIterator = -1;
			int nombreIterator = -1;
		    while (std::getline(originFile, str))
		    { 
		          Upp::String aLine(str);
		          if(cpt == 0){
		            auto y=  Split(aLine, "\t",false);
		            FindIteratorOf(y,Nomenclature_orthographe,orthographeIterator);
					FindIteratorOf(y,Nomenclature_lemme,lemmeIterator);
					FindIteratorOf(y,Nomenclature_type,typeIterator);
					FindIteratorOf(y,Nomenclature_genre,nombreIterator);
					FindIteratorOf(y,Nomenclature_nombre,genreIterator);         
		            cpt++;
		          }else{
		           	auto y=  Split(aLine, "\t",false);
		           	lesMots.Add(unMot( this->removeAccent(y[orthographeIterator]), y[lemmeIterator], y[typeIterator],y[genreIterator], y[nombreIterator]));
		         	cpt++;
		          } 
		    }
			Upp::String mots;
			for(int i = 0; i < lesMots.GetCount(); i++){
			    mots<< lesMots[i].ToXML()<<"\n";
		}
		originFile.close();
		
		String path = GetExeFilePath();
		path.Replace(GetExeTitle() +".exe","HXc_LanguageDataBase.Xml");

		FileOut out(path);
		if(!out){
			Cout()<< "Echec lors de la création de la BDD XML"<<"\n";
			exit(0);
		}else{
			this->leXml = XmlHeader() + XmlTag("Mots")(mots);
			out << this->leXml;
			out.Close();
		}
		return this->leXml;
	}
	exit(0);
}

void HXc_LanguageDataBase::FindIteratorOf(Upp::Vector<Upp::String> &theLine,const Upp::String nomenclature, int &iterator){
	for(int i = 0; i < theLine.GetCount(); i++){
		if(nomenclature.Find(theLine[i])> -1){
			iterator = i;
			break;   
		}
	}
}



void HXc_LanguageDataBase::LoadXml(){
	Upp::String xmlizer;
	unMot leMot;
	XmlParser p(this->leXml);
	
	while(!p.IsTag())
		p.Skip();
	
	p.PassTag("Mots");
	while(!p.End()){
		if(p.Tag("Mot")){
			xmlizer ="";
			while(!p.IsEnd()){
				xmlizer << 	XmlTag(p.ReadTag()).Text(p.ReadText()) << "\n";
				p.PassEnd();
			}
			xmlizer = XmlTag("Mot")(xmlizer);
			XmlParser m(xmlizer);
			leMot.LoadXML(m);
			this->lesMots.Add(leMot);
			p.PassEnd();
		}
		else{
			p.Skip();	
		}
	}
}

unMot HXc_LanguageDataBase::GetAWord(Upp::String mot){
	if(type == DataTypeManager::Bdd){
		SQL = sqlite3;
		//S_MOTS row;
		Sql sql;
		sql.Execute("Select * from MOTS where ORTHOGRAPHE = '" <<  mot << "' COLLATE NOCASE");
	//	sql.Execute("Select * from MOTS where ORTHOGRAPHE = '" << String("abaissée") << "' COLLATE NOCASE");
	//	sql*Select(row).From(MOTS).Where(ToUpper(ORTHOGRAPHE) == ToUpper(mot));
		if(sql.Fetch()) return unMot(sql[1],sql[2],sql[3],sql[5],sql[4]);
		return unMot();
	}else if(type == DataTypeManager::Xml){
		String buffer = this->removeAccent(mot);
		for(int i = 0; i< lesMots.GetCount() ; i++){
			String buffer2 = this->removeAccent(lesMots[i].orthographe);
			String buffer3 = lesMots[i].orthographe;
		//	Cout() << "Mot entrée : " <<  buffer << "  comparer à : " << this->removeAccent(lesMots[i].orthographe)<< "\n";
			if (this->removeAccent(lesMots[i].orthographe).IsEqual(buffer)){
				return lesMots[i];
			}
		}
		return unMot();
	}
	return unMot();
}

int HXc_LanguageDataBase::nbMotsCharger(){
	if(type == DataTypeManager::Bdd){
		SQL = sqlite3;
		Sql sql;
		sql.Execute("Select count(ID) from MOTS;");
		if(sql.Fetch()){	
			return atoi(sql[0].ToString().ToStd().c_str()) ;
		}	 
	}
	else if(type==DataTypeManager::Xml){
		return lesMots.GetCount();
	}
	return 0;	
}

Upp::String HXc_LanguageDataBase::removeAccent(Upp::String str){
  /*  char cstr[str.ToStd().length()+ 1];
	strcpy(cstr, str.ToStd().c_str());
	/*char * p =cstr;
    while ( (*p)!=0 ) {
        const char* tr = "AAAAAAECEEEEIIIIDNOOOOOx0UUUUYPsaaaaaaeceeeeiiiiOnooooo/0uuuuypy";
        unsigned char ch = (*p);
        if ( ch >=192 ) {
            (*p) = tr[ ch-192 ];
        }
        ++p;
    }
    return Upp::String(ToUpper(cstr));*/
    return str;//ToUpperAscii(str,CHARSET_CP437);
}

/*
CONSOLE_APP_MAIN
{
	HXc_LanguageDataBase bdd("");
	std::string theWord;
	for(;;){
		Cout() << "Entrer le mot à rechercher (les accents compte !) :\n";
		std::cin >> theWord;
		theWord = ToCharset(CHARSET_DEFAULT,theWord,CHARSET_CP850); //CETTE MERDE DENCODAGE MA PRIT 3 JOURS 
																	//la console française 
																	//windows 7 est de base à
																	//l'encodage ISO_8859_CP850
																	//ce qui représente
																	//l'encodage Latin 1.
																	//C++ récupère la string et
																	//la caste en UTF-8 (de ce
																	//que j'en ai compris) ce
																	//qui faussais les données
																	//envoyées Il fallait donci
																	//après récupération dans
																	//la variable convertire la
																	//data de la variable de
																	//l'encodage par défaut
																	//(Utf 8) vers l'encodage
																	//Windows soit
																	//ISO_8859_CP850
		bdd.GetAWord(Upp::String(theWord)).Dump();
	}
	// Le code ci dessous est juste un exemple d'utilisation de la base de données relatif au language HXc
}*/