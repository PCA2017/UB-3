//============================================================================
// Name        : heated_plate.cpp
// Author      : M
// Description : Calculates the heat distribution on a plate iteratively.
//               Boundary has the const value 0.0.
//               The initial Matrix can have a circle of a specified Temperature
//               between 0.0 and 127.0 in the middle.
// Arguments   : n - Dimension of the quadratic discrete heated plate
//               r - Radius of an inital circle with specified Temperature
//               H - Temperature of the initial circle between 0.0 and 127.0
//               filename - Name of the file whitch will contain the simulation results
// Example     : ./heated_plate 10 2 10 file_1
//               Creates a 10x10 plate with a initial circle of radius 2 with
//               Temperature 10.0 and saves the results in an File with name file_1
//============================================================================

#include <iostream>
#include <fstream>
#include <new>
#include <sys/time.h>

using namespace std;

void init_matrix(double ** matrix, int n, double a, double b, int r, double H);   // Initiale Matrix erzeugen
void update_matrix(int n, const double phi, double ** matrix, double ** matrix_old); // Updatet den Wert in matrix anhand Wärmeleitungsgleichung und der alten Matrix
void ergebniszeile_eintragen(double ** matrix, int n, ofstream &);   // Ergebnis der Matrix im aktuellen Zeitrschitt in Datei eintragen

int main(int argc, char **argv) {

	unsigned int i, n, r, t;  // Nur Werte > 0 möglich
	double H, a, b;

	const double phi = 6.0/25.0;

	double ** m1;
	double ** m1old; // Matrix aus dem Zeitschritt davor

	struct timeval start, stop;   // Zeitmessung mit gettimeofday
	long seconds, useconds;
	double duration;

	string filename;
	ofstream ergebnisdatei;

	n = atoi(argv[1]);
	r = atoi(argv[2]);
	H = atoi(argv[3]);
	filename = argv[4];

	ergebnisdatei.open(filename.c_str(),ios::out);

	// H auf Bereich 0.0 bis 127.0 begrenzen. (Werte in der Matrixen können dann Aufgrund der Berechnungsvorschrift den Wertebereich auch nicht verlassen.)
	if (H > 127.0)
		H = 127.0;
	else if (H < 0.0)
		H = 0.0;

	// Radius des erhitzten Kreises darf nicht den Rand der Matrix berühren
	if (r > ((n/2) -2))
		r = ((n/2) -2);

	a = (n-1)/2.0;  // Mittelpunkt Kreis in x-Richtung
	b = (n-1)/2.0;  // Mittelpunkt Kreis in y-Richtung

	// Matrix mit (nxn) Einträgen
	m1 = new double *[n];
	m1[0] = new double [n*n];
	for(i = 1; i<n; i++)
		m1[i] = m1[0] + i*n;

	// Matrix aus dem letzten Zeitschritt mit (nxn) Einträgen
	m1old = new double *[n];
	m1old[0] = new double [n*n];
	for(i = 1; i<n; i++)
		m1old[i] = m1old[0] + i*n;

	if (m1 == nullptr || m1old == nullptr)
		cout << "Error: memory could not be allocated";
	else
	{
		// Matrix initialisieren
		init_matrix(m1, n, a, b, r, H);

		ergebniszeile_eintragen(m1, n, ergebnisdatei);  // Initiale Matrix in Datei schreiben

		// Zeit stoppen
		gettimeofday(&start, NULL);

		// Innere Werte der Matrix (ohne Randwerte) iterativ updaten
		for (t = 0; t<100; t++)  // 100 Zeitschritte
		{
			update_matrix(n, phi, m1, m1old);
			ergebniszeile_eintragen(m1, n, ergebnisdatei);
		}

		// Zeit stoppen
		gettimeofday(&stop, NULL);

		// Dauer berechnen
		seconds  = stop.tv_sec  - start.tv_sec;
		useconds = stop.tv_usec - start.tv_usec;
		duration = seconds + useconds/1000000.0;  // Dauer in Sekunden

		cout << "Dauer: " << duration << " Sekunden" << endl;

		delete[] m1old[0];
		delete[] m1old;
		delete[] m1[0];
		delete[] m1;
	}

	return 0;
}

void init_matrix(double ** matrix, int n, double a, double b, int r, double H){
// Initialisiert eine (nxn) Matrix mit 0en bis auf einen Kreis mit den Werten H mit Radius r und Mittelpunkt bei (a,b)

	unsigned int r2 = r*r; // r^2

	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
		{
			// Punkte im Kreis zu H setzten
			if (((i-a)*(i-a)+(j-b)*(j-b)) < r2)
				matrix[i][j] = H;
			// Rest zu =0.0 setzten
			else
				matrix[i][j] = 0.0;  // Achtung: Wenn Kreisradius zu groß übergeben wurden, Randwerte !=0 gesetzt die sich nicht verändern! -> Bei Eingabe geprüft
		}
	}
}

void update_matrix(int n, const double phi, double ** matrix, double ** matrix_old){
// Updatet den Wert in matrix anhand Wärmeleitungsgleichung mit phi und der alten Matrix matrix_old

	// Werte aus dem letzten Zeitschritt in matrix_old zwischenspeichern
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
			matrix_old[i][j] = matrix[i][j];
	}

	// Werte des aktuellen Zeitschritts berechnen
	for (int i=1; i<(n-1); i++)
	{
		for (int j=1; j<(n-1); j++)
			matrix[i][j] = matrix_old[i][j] + phi*((-4)*matrix_old[i][j] + matrix_old[i+1][j] + matrix_old[i-1][j] + matrix_old[i][j+1] + matrix_old[i][j-1]);
	}
}

void ergebniszeile_eintragen(double ** matrix, int n, ofstream &OUT){
// Ergebniss eines Zeitschritts in OUT schreiben.

	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
			OUT << matrix[i][j] << ",";

		OUT << endl;
	}
	OUT << "#" << endl;
}
