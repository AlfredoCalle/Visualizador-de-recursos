# Programa lector uso de memoria de procesos
Este repositorio contiene un programa que muestra el uso básico de memoria de un proceso. El programa tiene el siguiente uso:

```
./procesos [<periodo>] <pid>
```

donde *periodo* es el periodo de muestreo y *pid* es el *process ID* del proceso a investigar. Para compilar el programa usar el comando *make*.

## Ejemplo de uso
Por ejemplo, si existe un periodo con *pid* = 15000 entonces el programa puede ser llamado así:

```
$ ./procesos 15000
Abriendo /proc/15000/status...
Use Ctrl+C para terminar.
Proceso "test" con pid: 15000
Memoria en RAM (kB)     Stack (kB)
2032                    388
```

## Taller
El taller consiste en crear programa basado en el código de este repositorio con la siguiente funcionalidad:

```
./procesos <pid1> [<pid2> <pid3> <pid4> ... <pidN>]
```

El programa debe leer el *status* de *N* procesos y producir el siguiente resultado:

```
$ ./procesos <pid1> <pid2> <pid3> <pid4> ... <pidN>
Proceso: <pid1>
Nombre: <Name>
Memoria en RAM (kB): <VmRSS>
Memoria en Stack (kB): <VmStk>
Memoria en Data (kB): <VmData>

Proceso: <pid2>
Nombre: <Name>
Memoria en RAM (kB): <VmRSS>
Memoria en Stack (kB): <VmStk>
Memoria en Data (kB): <VmData>

Proceso: <pid3>
Nombre: <Name>
Memoria en RAM (kB): <VmRSS>
Memoria en Stack (kB): <VmStk>
Memoria en Data (kB): <VmData>
...
```

El programa debe leer los archivos */proc/pidN/status* una sola vez y mostrar los resultados en consola. Además, es mandatorio usar memoria dinámica para guardar la información en memoria de cada proceso y luego presentarlo en consola. La información de cada proceso debe estar en una estructura con el siguiente formato:

```C
struct Proceso{
    char name[MAXLINE];
    int pid;
    long VmRSS;
    long VmStk;
    long VmData;
};
```