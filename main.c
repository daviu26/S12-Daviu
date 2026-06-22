#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char marca[30];
    char modelo[30];
    char tipo[30]; 
    char estado[20]; 
    float precio;
    int disponible; 
} Vehiculo;

void mostrarInventario() {
    FILE *archivo = fopen("inventario.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el archivo de inventario.\n");
        return;
    }

    Vehiculo v;
    printf("\n--- INVENTARIO RUEDAS DE ORO ---\n");
    printf("ID\tMarca\t\tModelo\t\tTipo\t\tEstado\tPrecio\t\tDisponible\n");
    
    while (fscanf(archivo, "%d %s %s %s %s %f %d", &v.id, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
        printf("%d\t%s\t\t%s\t\t%s\t\t%s\t%.2f\t%s\n", 
               v.id, v.marca, v.modelo, v.tipo, v.estado, v.precio, v.disponible ? "Si" : "No");
    }
    fclose(archivo);
}

void buscarVehiculo() {
    FILE *archivo = fopen("inventario.txt", "r");
    if (archivo == NULL) {
        printf("Error: No se pudo abrir el inventario.\n");
        return;
    }

    char tipoBuscado[30], marcaBuscada[30], estadoBuscado[20];
    float presupuesto;

    printf("\n--- BUSQUEDA DE VEHICULOS ---\n");
    printf("Ingrese Tipo (ej. Camioneta): ");
    scanf("%s", tipoBuscado);
    printf("Ingrese Marca (ej. Chevrolet): ");
    scanf("%s", marcaBuscada);
    printf("Ingrese Estado (Nuevo/Usado): ");
    scanf("%s", estadoBuscado);
    printf("Ingrese Presupuesto Maximo: $");
    scanf("%f", &presupuesto);

    Vehiculo v;
    int encontrados = 0;
    
    printf("\nResultados encontrados:\n");
    while (fscanf(archivo, "%d %s %s %s %s %f %d", &v.id, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
        if (v.disponible == 1 && 
            strcasecmp(v.tipo, tipoBuscado) == 0 && 
            strcasecmp(v.marca, marcaBuscada) == 0 && 
            strcasecmp(v.estado, estadoBuscado) == 0 && 
            v.precio <= presupuesto) {
            
            printf("ID: %d | %s %s | Precio: $%.2f\n", v.id, v.marca, v.modelo, v.precio);
            encontrados++;
        }
    }
    
    if(encontrados == 0) {
        printf("No se encontraron vehiculos con esas caracteristicas.\n");
    }
    fclose(archivo);
}

void registrarVenta() {
    int idVehiculoBuscado;
    char nombreCliente[50];
    
    printf("\n--- REGISTRAR VENTA ---\n");
    printf("Nombre del cliente: ");
    scanf("%s", nombreCliente);
    printf("ID del vehiculo a comprar: ");
    scanf("%d", &idVehiculoBuscado);

    FILE *archivoIn = fopen("inventario.txt", "r");
    FILE *archivoTemp = fopen("temp.txt", "w");
    FILE *archivoVentas = fopen("ventas.txt", "a");

    if (archivoIn == NULL || archivoTemp == NULL || archivoVentas == NULL) {
        printf("Error al manejar los archivos.\n");
        return;
    }

    Vehiculo v;
    int vehiculoEncontrado = 0;

    while (fscanf(archivoIn, "%d %s %s %s %s %f %d", &v.id, v.marca, v.modelo, v.tipo, v.estado, &v.precio, &v.disponible) == 7) {
        if (v.id == idVehiculoBuscado && v.disponible == 1) {
            v.disponible = 0; 
            vehiculoEncontrado = 1;
            fprintf(archivoVentas, "Cliente: %s | ID_Vehiculo: %d | Marca: %s | Modelo: %s | Precio: %.2f\n", 
                    nombreCliente, v.id, v.marca, v.modelo, v.precio);
            printf("\n¡Venta registrada con exito para %s!\n", nombreCliente);
        }
                fprintf(archivoTemp, "%d %s %s %s %s %.2f %d\n", v.id, v.marca, v.modelo, v.tipo, v.estado, v.precio, v.disponible);
    }

    if (!vehiculoEncontrado) {
        printf("Vehiculo no encontrado o ya vendido.\n");
    }

    fclose(archivoIn);
    fclose(archivoTemp);
    fclose(archivoVentas);

    remove("inventario.txt");
    rename("temp.txt", "inventario.txt");
}

int main() {
    int opcion;
    do {
        printf("\n=== CONCESIONARIA RUEDAS DE ORO ===\n");
        printf("1. Ver inventario completo\n");
        printf("2. Buscar vehiculo por preferencias\n");
        printf("3. Registrar venta\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1: mostrarInventario(); break;
            case 2: buscarVehiculo(); break;
            case 3: registrarVenta(); break;
            case 4: printf("Saliendo del sistema...\n"); break;
            default: printf("Opcion no valida.\n");
        }
    } while(opcion != 4);

    return 0;
}