/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#include "utils.h"

void* serializar_paquete(t_paquete* paquete, int *bytes) {
	/*El tamaño del paquete está dado por:
	 * tamaño del codigo de operacion: sizeof(int)
	 * tamaño del buffer -> size: sizeof(int)
	 * tamaño del buffer -> stream: longitud del mensaje */

	*bytes = paquete->buffer->size + 2 * sizeof(int);
	void* a_enviar = malloc(*bytes);
	int offset = 0;

	memcpy(a_enviar, &(paquete->codigo_operacion), sizeof(int));
	offset += sizeof(int);

	memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
	offset += sizeof(int);

	memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

	return a_enviar;
}

int crear_conexion(char *ip, char* puerto) {
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family,
			server_info->ai_socktype, server_info->ai_protocol);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen)
			== -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

t_buffer* crear_buffer(char* mensaje) {
	t_buffer* buffer = malloc(sizeof(t_buffer));
	int lonfigudMensaje = strlen(mensaje) + 1;

	buffer->size = lonfigudMensaje;
	buffer->stream = malloc(lonfigudMensaje);

	memcpy(buffer->stream, mensaje, lonfigudMensaje);

	return buffer;
}

t_paquete* crear_paquete(t_buffer* buffer) {
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = GAMEBOY;
	paquete->buffer = buffer;

	return paquete;
}

void enviar_mensaje(char* mensaje, int socket_cliente) {
	t_buffer* buffer = crear_buffer(mensaje);
	t_paquete* paquete = crear_paquete(buffer);
	int streamBytes = 0;

	void* a_enviar = serializar_paquete(paquete, &streamBytes);

	send(socket_cliente, a_enviar, streamBytes, MSG_WAITALL);

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(a_enviar);
}

char * toString(void* text) {
	return (char*) text;
}

char* obtenerMensaje(int socket_cliente) {
	int tamanio_a_recibir = 0;
	void* a_recibir;

	//Lo segundo que se recibe es el tamaño del buffer sizeof(int)
	recv(socket_cliente, &tamanio_a_recibir, sizeof(int), MSG_WAITALL);

	//Por ultimo, recibimos el buffer -> stream que enviamos originalmente, size + mensaje
	a_recibir = malloc(tamanio_a_recibir);
	recv(socket_cliente, a_recibir, tamanio_a_recibir, MSG_WAITALL);

	return toString(a_recibir);
}

char* recibir_mensaje(int socket_cliente) {

	char* mensaje = NULL;
	int codigo_operacion;

	//Lo primero que se recibe es el código de operacion
	recv(socket_cliente, &codigo_operacion, sizeof(int), 0);

	switch (codigo_operacion) {
	case GAMEBOY:
		mensaje = obtenerMensaje(socket_cliente);

		break;
	default:
		break;
	}

	return mensaje;
}

void liberar_conexion(int socket_cliente) {
	close(socket_cliente);
}
