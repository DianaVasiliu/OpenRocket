int numberOfStars = 200;

// varfurile 
GLfloat Vertices[1000] = {
	// Background-ul
	0.f, 0.f, 0.f, 1.f, // stanga jos
	float(2 * width), 0.f, 0.f, 1.f, // dreapta jos
	float(2 * width), float(2 * height), 0.f, 1.f, // dreapta sus
	0.f, float(2 * height), 0.f, 1.f, // stnaga sus

	// Triunghiul de sus
	0.f, 160.f, 0.f, 1.f,
	50.f, 160.f, 0.f, 1.f,
	25.f, 185.f, 0.f, 1.f,

	// Triunghiul de jos
	0.f, 100.f, 0.f, 1.f,
	50.f, 100.f, 0.f, 1.f,
	25.f, 165.f, 0.f, 1.f,

	// Desptunghiul
	15.f, 100.f, 0.f, 1.f,  // stanga jos
	35.f, 100.f, 0.f, 1.f, // dr jos
	35.f, 175.f, 0.f, 1.f,
	15.f, 175.f, 0.f, 1.f,

	// Triunghiul din varf
	15.f, 175.f, 0.f, 1.f,
	35.f, 175.f, 0.f, 1.f,
	25.f, 210.f, 0.f, 1.f,

	// Focul portocaliu
	15.f, 100.f, 0.f, 1.f,
	35.f, 100.f, 0.f, 1.f,
	50.f, 85.f, 0.f, 1.f,
	25.f, 50.f, 0.f, 1.f,
	0.f, 85.f, 0.f, 1.f,

	// Focul galben
	15.f, 100.f, 0.f, 1.f,
	35.f, 100.f, 0.f, 1.f,
	40.f, 90.f, 0.f, 1.f,
	25.f, 80.f, 0.f, 1.f,
	10.f, 90.f, 0.f, 1.f,
};

GLfloat Colors[1000] = {
	// Background-ul
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,
	0.f, 0.f, 0.f, 0.f,

	// Triunghiul de sus
	1.0f, 0.0f, 0.0f, 0.f,
	1.0f, 0.0f, 0.0f, 0.f,
	1.0f, 0.0f, 0.0f, 0.f,

	// Triunghiul de jos
	1.0f, 0.0f, 0.0f, 0.f,
	1.0f, 0.0f, 0.0f, 0.f,
	1.0f, 0.0f, 0.0f, 0.f,

	// Dreptunghiul
	0.8f, 0.5f, 0.2f, 0.f,
	0.8f, 0.5f, 0.2f, 0.f,
	0.8f, 0.5f, 0.2f, 0.f,
	0.8f, 0.5f, 0.2f, 0.f,

	// Triunghiul din varf
	1.0f, 1.0f, 0.0f, 0.f,
	1.0f, 1.0f, 0.0f, 0.f,
	1.0f, 1.0f, 0.0f, 0.f,

	// Focul portocaliu
	1.f, 0.25f, 0.0f, 1.0f,
	1.f, 0.25f, 0.0f, 1.0f,
	1.f, 0.25f, 0.0f, 1.0f,
	1.f, 0.25f, 0.0f, 1.0f,
	1.f, 0.25f, 0.0f, 1.0f,

	// Focul galben
	1.0f, 0.8f, 0.0f, 1.0f,
	1.0f, 0.8f, 0.0f, 1.0f,
	1.0f, 0.8f, 0.0f, 1.0f,
	1.0f, 0.8f, 0.0f, 1.0f,
	1.0f, 0.8f, 0.0f, 1.0f
};

srand(time(NULL));
int i = 27;
while (i < 27 + numberOfStars) {
	Vertices[4 * i] = float(rand() % (2 * width) + 1);
	Vertices[4 * i + 1] = float(rand() % (2 * width) + 1);
	Vertices[4 * i + 2] = 0.f;
	Vertices[4 * i + 3] = 1.f;
	cout << i << " " << Vertices[4 * i] << " " << Vertices[4 * i + 1] << " " << Vertices[4 * i + 2] << " " << Vertices[4 * i + 3] << "\n";
	Colors[4 * i] = 1.f;
	Colors[4 * i + 1] = 1.f;
	Colors[4 * i + 2] = 1.f;
	Colors[4 * i + 3] = 1.f;
	cout << i << " " << Colors[4 * i] << " " << Colors[4 * i + 1] << " " << Colors[4 * i + 2] << " " << Colors[4 * i + 3] << "\n";
	i++;
}