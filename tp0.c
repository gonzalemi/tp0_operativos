/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"

int main(void) {
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	char* ip;
	char* puerto;

	t_log* logger = iniciar_logger();
	t_config* config = leer_config();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger, "IP: %s, PUERTO: %s", ip, puerto);
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/
	//antes de continuar, tenemos que asegurarnos que el servidor esté
	//corriendo porque lo necesitaremos para lo que sigue.
	//crear conexion
	int conexion = crear_conexion(ip, puerto);

	//enviar mensaje
	enviar_mensaje("hola soy Emi", conexion);

	//recibir mensaje
	char* mensaje = recibir_mensaje(conexion);

	//loguear mensaje recibido
	log_info(logger, mensaje);
	free(mensaje);

	terminar_programa(conexion, logger, config);

	return 0;
}

t_log* iniciar_logger(void) {

	return log_create("top0.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(void) {
	return config_create("./tp0.config");
}

void terminar_programa(int conexion, t_log* logger, t_config* config) {
	//Y por ultimo, para cerrar, hay que liberar lo que utilizamos (conexion, log y config) con las funciones de las commons y del TP mencionadas en el enunciado
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);
}
