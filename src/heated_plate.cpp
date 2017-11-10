//============================================================================
// Name        : heated_plate.cpp
// Author      : M
// Version     :
// Copyright   :
// Description : Calculates the heat distribution on a plate iteratively.
//               Boundary has the const value 0.0
//============================================================================

#include <iostream>
#include <new>
#include <sys/time.h>

using namespace std;

//void ergebniszeile_eintragen(const particle*const, const int ntot, const double time, ofstream &);   // Ergebnis im aktuellen Zeitrschitt als Zeile in Datei eintragen.

int main(int argc, char **argv) {

	unsigned int i, j, n, r, r2, a, b, t;  // Nur Werte > 0 möglich
	double H;

	const double phi = 6.0/25.0;

	double ** m1;

	struct timeval start, stop;   // Zeitmessung mit gettimeofday
	long seconds, useconds;
	double duration;

	n = atoi(argv[1]);
	r = atoi(argv[2]);
	H = atoi(argv[3]);

	// H auf Bereich 0.0 bis 127.0 begrenzen. (Werte in der Matrixen können dann Aufgrund der Berechnungsvorschrift den Wertebereich auch nicht verlassen.)
	if (H > 127.0)
		H = 127.0;
	else if (H < 0.0)
		H = 0.0;

	// Radius des erhitzten Kreises darf nicht den Rand der Matrix berühren
	if (r > ((n/2) -2))
		r = ((n/2) -2);

	r2 = r*r; // r^2

	a = n/2;  // Mittelpunkt Kreis in x-Richtung
	b = n/2;  // Mittelpunkt Kreis in y-Richtung

	// Matrix mit (nxn) Einträgen
	m1 = new double *[n];
	m1[0] = new double [n*n];
	for(i = 1; i<n; i++)
		m1[i] = m1[0] + i*n;

	if (m1 == nullptr)
		cout << "Error: memory could not be allocated";
	else
	{
		// Matrix initialisieren
		for (i=0; i<n; i++)
		{
			for (j=0; j<n; j++)
			{
				// Punkte im Kreis zu H setzten
				if (((i-a)*(i-a)+(j-b)*(j-b)) < r2)
					m1[i][j] = H;
				// Rest inklusive Rand zu =0.0 setzten
				else
					m1[i][j] = 0.0;  // Achtung: Wenn Kreis vom Nutzer zu groß angegeben werden Randwerte gesetzt die sich nicht verändern!
			}
		}

		// Zeit stoppen
		gettimeofday(&start, NULL);

		// Innere Werte der Matrix (ohne Randwerte) iterativ updaten
		for (t = 0; t<10000; t++)  // 10000 Zeitschritte
		{
			for (i=1; i<(n-1); i++)
			{
				for (j=1; j<(n-1); j++)
				{
					m1[i][j] = m1[i][j] + phi*((-4)*m1[i][j] + m1[i+1][j] + m1[i-1][j] + m1[i][j+1] + m1[i][j-1]);
//					ergebniszeile_eintragen(1,1,1,1);
				}
			}
		}

		// Zeit stoppen
		gettimeofday(&stop, NULL);

		// Dauer berechnen
		seconds  = stop.tv_sec  - start.tv_sec;
		useconds = stop.tv_usec - start.tv_usec;
		duration = seconds + useconds/1000000.0;  // Dauer in Sekunden

		cout << "Dauer: " << duration << " Sekunden" << endl;

		delete[] m1[0];
		delete[] m1;
	}

	return 0;
}

//void ergebniszeile_eintragen(const particle *const p, const int nmax, const double time, ofstream &OUT){
//// Ergebniss eines Zeitschritts in OUT schreiben.
//
//	OUT << time << " ";                                    // Aktuelle Zeit angeben
//	for(int i=0; i<nmax; i++){                           // Jedes Partikel in dem Array p durchlaufen. Für jedes x und y Position eintragen:
//		OUT << p[i].x_pos << " " << p[i].y_pos << " ";
//	}
//	OUT << endl;                                         // Neue Zeile anlegen.
//}
