

void log_usuario(int id_usuario, FILE *user){
	char c[10];
	int intc=0;
	
	printf("\n-----------------------------------------\nPosts actuales:\n");
	// mostrar_posts(fopen("archivo_post.dat","rb"));
	printf("\n-----------------------------------------\n");
	
	usuario u, u_original;
	menu_user:{ 										//se utiliza el mismo formato que en el menu principal
		printf("Menu administrador:\n\t1.- Editar Perfil\n\t2.- Seguir a un usuario\n\t3.- Comentar un post\n\t4.- Ver comentarios de un post\n\t5.- Volver.\nSeleccionar: ");
		gets(c);
		if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
			printf(" Respuesta no valida\n\n");
			goto menu_user;
		}
		if (strcmp(c,"1")==0){intc=1;}
		else if(strcmp(c,"2")==0){intc=2;}
		else if(strcmp(c,"3")==0){intc=3;}
		else if(strcmp(c,"4")==0){intc=4;}
		else if(strcmp(c,"5")==0){intc=5;}
	}
	
	//AHAHA HAE UGUU NOSE QUE MIERDA ES ESTO XD
	FILE *usrs = fopen("archivo_usuario.dat","rb"); //ya esta llamado anteriormente
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	// FILE *postmp = fopen("archivo_post_temp.dat","ab");
	// FIN DE AHAHA.
	
	switch (intc){
		case 1: modificar_usuario:{ 		//editar perfil

			while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){
			printf("error al crear el archivo temporal de usuarios\n");
			}
			printf("\nMenu de edicion:\n\t1.- Editar preferencia\n\t2.- Editar bio\n\t3.- Volver.\nSeleccionar: ");
			gets(c);
			if (strcmp(c,"1")==0){intc=1;}
			else if(strcmp(c,"2")==0){intc=2;}
			else if(strcmp(c,"3")==0){intc=3;}
			switch (intc){
			case 1:
				new_menu_preferencia:{
					printf("Nueva preferencia:\n 1.- Graves\n 2.- Oldfags\n 3.- Newfags\n 4.- Gores\n 5.- Happy\n Seleccionar: ");
					gets(c);
					if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
					printf(" Respuesta no valida\n\n");
					goto new_menu_preferencia;
					}
					if (strcmp(c,"1")==0){intc=1;}
					else if(strcmp(c,"2")==0){intc=2;}
					else if(strcmp(c,"3")==0){intc=3;}
					else if(strcmp(c,"4")==0){intc=4;}
					else if(strcmp(c,"5")==0){intc=5;}
					
					switch (intc){ 		// nueva preferencia
					case 1: u.preferencia = graves;
					break;
					case 2: u.preferencia = oldfag;
					break;
					case 3: u.preferencia = newfag;
					break;
					case 4: u.preferencia = gores;
					break;
					case 5: u.preferencia = happy;
					break;
					}
				
			fseek(usrstmp, 0, SEEK_END);
			u.id_usuario=ftell(usrstmp);//la nueva id esta al final del archivo anterior
			u.id_usuario_sigue=0;
			strtime=time(NULL); //fecha y hora actual
			timeinfo = localtime(&strtime);
			strftime(u.fecha_creacion, 30, "%d/%m/%y %I:%M%p", timeinfo);
			while(1){
			printf("Ingrese nombre de usuario a crear: ");
			gets(u.avatar);
			if (buscar_id_usuario(u.avatar, usrs)==-1){ //no se encuantra el nombre en uso
			break;
		
	
}
