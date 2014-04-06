void mostrar_post(FILE *archivo, enum tipo_post pref){ //archivo de solo lectura y la preferencia que se mostrara
	int post_mostrados=0, id, tamanio_archivo;
	post p;
	tipo t;
	if (archivo==NULL){
		printf("Actualmente no existen post creados\n");
	return;
	}
	FILE *tip = fopen("archivo_tipo.dat","rb");
	if (tip==NULL)
		printf("error al abrir el archivo_tipo");
	fseek(archivo, 0, SEEK_END);
	tamanio_archivo=ftell(archivo);
	//Busqueda de posts en todo el archivo
	for (id=0; id<=(tamanio_archivo-sizeof(post)); id+=sizeof(post)){
		fseek(archivo, id, SEEK_SET);
		fread(&p, sizeof(post), 1, archivo);
		fseek(tip, p.id_tipo, SEEK_SET);
		fread(&t, sizeof(tipo), 1, tip);
		if (t.type==pref){
			printf("Post: %s\t\t\tfecha de creacion: %s\n   leyenda: %s\n", p.imagen, p.fecha, p.leyenda);
			post_mostrados++;
		}
	}
	if (post_mostrados==0){
		printf("No existen post de esta categoria\n\n");
	}
	else{
		printf("\n");
	}
	fclose(archivo);
}
