/*
*   Created by N.Richard
*   Date de création : 17 novembre 2013
*	Date de version : 3 juin 2014
*	Version : 2.0
*/

#ifndef UTILITIES_H
#define UTILITIES_H

/*****************************************
	Gestion du temps (multiplateforme)
*****************************************/

#include <sstream>
#include <stdlib.h>
#include <math.h>
#ifdef WIN32
	#include <time.h>
	#include <sys/timeb.h>
	struct timeval {
		long long tv_sec;
		long long tv_usec;
	};
	int gettimeofday (struct timeval *tp, void *tz);
#else
	#include <sys/time.h>
#endif

#include <iostream>
#include <fstream>

namespace Utilities {


	/*****************************************
		Paramètre inutilisés (passer -Wall)
	*****************************************/

	#ifdef __GNUC__
	#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
	#else
	#  define UNUSED(x) UNUSED_ ## x
	#endif

	#ifdef __GNUC__
	#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
	#else
	#  define UNUSED_FUNCTION(x) UNUSED_ ## x
	#endif

	/****************************************
		    Moteur de nombres aléatoires
	****************************************/

	void initRandomEngine ();
	float getRand(float inter_a, float inter_b);

	/*****************************
	Transtypage
	*****************************/

	template<typename T>
	bool from_string( const std::string & Str, T & Dest )
	{
		std::istringstream iss(Str);
		return iss >> Dest != 0;
	}

	template<typename T>
	bool to_string( const T & Orig, const std::string & Str)
	{
		std::ostringstream oss(Str);
		return oss << Orig != 0;
	}

	template<typename T>
	T& from_string( const std::string & Str)
	{
		T Dest;
		std::istringstream iss(Str);
		iss >> Dest;
		return Dest;
	}

	template<typename T>
	const std::string to_string( const T & Orig)
	{
		std::ostringstream oss;
		oss << Orig;
		return oss.str();
	}

	/*****************************
		Gestion des benchmarks
	*****************************/

	#define t_start struct timeval tv1, tv2; gettimeofday(&tv1, NULL)
	#define t_end gettimeofday(&tv2, NULL)
	#define t_value ((tv2.tv_sec-tv1.tv_sec)*1e6 + tv2.tv_usec-tv1.tv_usec)
	#define t_show printf ("\nTemps de calcul : %g ms\n", t_value/1e3)

	/***********************************
		Manipulation de fichiers
	***********************************/

    bool isReadable (const std::string& filePath);
    bool isReadable (const char* filePath);

	/***********************************
	        Min/Max rapides
	***********************************/

	// Macro utiles pour effectuer simplement un maximum, minimum ou une limitation d'intervalle
	#define MIN(A, B) (((A)<(B)) ? (A) : (B))
	#define MAX(A, B) (((A)<(B)) ? (B) : (A))
	#define limitRange(value, downLimit, upLimit) min(max((value), (downLimit)), (upLimit))

	//Comparaison "sure" des nombres à virgules flottantes
	#define EPSILON 1e-12
	#define EQUAL(A,B) (fabs(A - B) < EPSILON)

	template<typename T>
	int length(const T* temp)
	{
		int count = 0;
	    for (; temp[count] != '\0'; ++count);
	    return count;
	}

    extern const char* tab;
} 

#endif //UTILITIES_H
