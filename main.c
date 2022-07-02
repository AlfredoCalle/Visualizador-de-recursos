#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024
#define PROC_INFO_FILE "/status"

long get_memory_status(FILE *file, char *field);

/*
* Muestra el uso de memoria de un proceso
*/
int main(int argc, char **argv) {
	int periodo = 3000; //periodo de muestreo
	char *pid_str; //pid del proceso a mostrar en cadena de caracteres
	int pid; //pid del proceso a mostrar

	if (argc == 3) {
		periodo = atoi(argv[1]);
		pid_str = argv[2];
		pid = atoi(pid_str);
	}else if(argc == 2){
		pid_str = argv[1];
		pid = atoi(pid_str);
	}else{
		fprintf(stderr, "uso: %s [<periodo>] <pid>\n", argv[0]);
		return 1;
	}

	if (periodo <= 0){
		fprintf(stderr, "Periodo inválido...\n");
		return 1;
	}

	if (pid <= 0){
		fprintf(stderr, "pid inválido...\n");
		return 1;
	}

	//Nombre de archivo a abrir
	char *filename_info;

	// El nombre de archivo será alamacenado en el heap
	filename_info = malloc(strlen("/proc/") + strlen(pid_str) + strlen(PROC_INFO_FILE));

	strcpy(filename_info, "/proc/");
	strcat(filename_info, pid_str);
	strcat(filename_info, PROC_INFO_FILE);

	printf("Abriendo %s...\n", filename_info);
	
	FILE *procInfoFile;
	//Uso /proc/<pid>/status para información del proceso
	procInfoFile = fopen (filename_info,"r");

	if (procInfoFile == NULL){
		fprintf(stderr, "Error al leer información del proceso...\n");
		return 1;
	}

	//Libera reservas
	free(filename_info);

	//Deshabilita el buffer de stdout para no esperar un salto de línea
	setbuf(stdout, NULL);
	//Deshabilita el buffer del archivo para obtener datos frescos de /proc/<pid>/status
	setbuf(procInfoFile, NULL);

	char proc_name[MAXLINE];

	//Lee la primera linea de /proc/<pid>/status y extrae el nombre 
	fscanf(procInfoFile,"Name: %s", proc_name);

	printf("Use Ctrl+C para terminar.\n");
	printf("Proceso \"%s\" con pid: %d\n", proc_name, pid);
	printf("Memoria en RAM (kB)\tStack (kB)\n");

	long resident_memory = 0;
	long stack_memory = 0;
	do{
		printf("\r%ld\t\t\t%ld          ", resident_memory, stack_memory);

		//Obtiene el uso de memoria en RAM, esto está en VmRSS
		resident_memory = get_memory_status(procInfoFile, "VmRSS:");
		//Obtiene el uso del stack, esto está en VmStk
		stack_memory = get_memory_status(procInfoFile, "VmStk:");

		usleep(periodo);
	}while(resident_memory > 0);

	//Cierra el archivo
	fclose(procInfoFile);

	return 0;
}

/*
* Busca el campo "field" en el archivo "file" y retorna el valor de memoria asociado.
* Está función asume que "file" apunta al pseudo-archivo "/proc/<pid>/status" donde el formato es:
* <field>: <value>\n
* Cuando el campo es sobre uso de memoria el formato es:
* <field>: <value> kB\n
* Retorna -1 si no pudo encontrar el campo o error.
*/
long get_memory_status(FILE *file, char *field)
{
	char line[MAXLINE]; //Línea del archivo
	char field_name[MAXLINE]; //campo en la línea
	long memory = 0;

	//Se asegura que el archivo está al principio
	rewind(file);

	int n;
	while(fgets(line, MAXLINE, file) > 0){
		n = sscanf(line, "%s %ld kB", field_name, &memory);
		if(n == 2)
			if(strcmp(field_name, field) == 0)
				return memory;
	}

	return -1;
}