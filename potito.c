
void log_usuario(int id_usuario, FILE *user){
	char c[10];
	int intc=0;
	
	printf("\n-----------------------------------------\nPosts actuales:\n");
	// mostrar_posts(fopen("archivo_post.dat","rb"));
	printf("\n-----------------------------------------\n");
	
	usuario u, u_original;
	menu_user:{ 										//se utiliza el mismo formato que en el menu principal
		printf("Menu administrador:\n\t1.- Editar Perfil\n\t2.- Seguir a un usuario\n\t3.- Ver posts\n\t4.- Volver.\nSeleccionar: ");
		gets(c);
		if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0){
			printf(" Respuesta no valida\n\n");
			goto menu_user;
		}
		if (strcmp(c,"1")==0){intc=1;}
		else if(strcmp(c,"2")==0){intc=2;}
		else if(strcmp(c,"3")==0){intc=3;}
		else if(strcmp(c,"4")==0){intc=4;}
	}
	
	//AHAHA HAE UGUU NOSE QUE MIERDA ES ESTO XD
	FILE *usrs = fopen("archivo_usuario.dat","rb"); //ya esta llamado anteriormente
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	// FILE *postmp = fopen("archivo_post_temp.dat","ab");
	// FIN DE AHAHA.
	
	switch (intc){
		case 1: modificar_usuario:{ 		//editar perfil
			printf("\nMenu de edicion:\n\t1.- Editar preferencia\n\t2.- Editar biografia\n\t3.- Volver.\nSeleccionar: ");
			gets(c);
			if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0){
				printf(" Respuesta no valida\n\n");
				goto modificar_usuario;
			}
			if (strcmp(c,"1")==0){intc=1;}
			else if(strcmp(c,"2")==0){intc=2;}
			else if(strcmp(c,"3")==0){intc=3;}
			
			while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){
			printf("error al crear el archivo temporal de usuarios\n");
			}
			
			switch (intc){
			case 1:
				menu_preferencia_usuario:{
					printf("Nueva preferencia:\n 1.- Graves\n 2.- Oldfags\n 3.- Newfags\n 4.- Gores\n 5.- Happy\n Seleccionar: ");
					gets(c);
					if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
					printf(" Respuesta no valida\n\n");
					goto menu_preferencia_usuario;
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
			
				fseek(usrs, id_usuario, SEEK_SET);
				fread(&u_original, sizeof(usuario),1, usrs);
				u.id_usuario=id_usuario;
				u.id_usuario_sigue=u_original.id_usuario_sigue;
				u.fecha_creacion=u_original.fecha_creacion;
				strcpy(u.avatar,u_original.avatar);
				strcpy(u.bio, u_original.bio);
				fseek(usrstmp, id_usuario, SEEK_SET);
				if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){//escritura correcta
					remove("archivo_usuario.dat");
					rename("archivo_usuario_temp.dat","archivo_usuario.dat"); //podria verificarse si el renombramiento fue correcto... es correcto cuando es ==0
					printf("Edicion de preferencia exitosa\n");
					goto modificar_usuario;
				}
		
	
}
