void log_usuario(int id_usuario, FILE *user){
	char c[10], bio_temp[100];
	int intc=0;
	usuario u, u_original;
	
	printf("\n-----------------------------------------\nPosts actuales:\n");
	// mostrar_posts(fopen("archivo_post.dat","rb"));
	printf("\n-----------------------------------------\n");
	
	menu_user:{ 																	// mismo formato que el menu principal
		printf("Menu usuario:\n\t1.- Editar Perfil\n\t2.- Seguir a un usuario\n\t3.- Ver un post\n\t4.- Volver.\nSeleccionar: ");
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
	
	FILE *usrs = fopen("archivo_usuario.dat","rb"); 								//ya esta llamado anteriormente
	FILE *usrstmp = fopen("archivo_usuario_temp.dat","wb");
	// FILE *postmp = fopen("archivo_post_temp.dat","ab");   
	
	switch (intc){
		case 1: modificar_usuario:{ 												//1.- editar perfil
			printf("\nMenu de edicion:\n\t1.- Editar preferencia\n\t2.- Editar biografia\n\t3.- Volver.\nSeleccionar: ");
			gets(c);
			if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0){
				printf(" Respuesta no valida\n\n");
				goto modificar_usuario;
			}}
		
			if (strcmp(c,"1")==0){intc=1;}
			else if(strcmp(c,"2")==0){intc=2;}
			else if(strcmp(c,"3")==0){intc=3;}
			
			while(copiar_archivo("archivo_usuario.dat","archivo_usuario_temp.dat")){	//crear temp
				printf("error al crear el archivo temporal de usuarios\n");
			}
			
			switch (intc){
				case 1:		//nueva preferencia
					menu_preferencia_usuario:{
						printf("Nueva preferencia:\n 1.- Graves\n 2.- Oldfags\n 3.- Newfags\n 4.- Gores\n 5.- Happy\n Seleccionar: ");
						gets(c);
						if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0 && strcmp(c,"3")!=0 && strcmp(c,"4")!=0 && strcmp(c,"5")!=0){
						printf(" Respuesta no valida\n\n");
						goto menu_preferencia_usuario;
						}
					}
					if (strcmp(c,"1")==0){intc=1;}					// no deberia ser un else if igual? total el anterior es un if...
					else if(strcmp(c,"2")==0){intc=2;}	
					else if(strcmp(c,"3")==0){intc=3;}
					else if(strcmp(c,"4")==0){intc=4;}
					else if(strcmp(c,"5")==0){intc=5;}
					
					switch (intc){ 		// set nueva preferencia
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
				
					fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
					fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
					u.id_usuario=id_usuario;										
					u.id_usuario_sigue=u_original.id_usuario_sigue;					//
					strcpy(u.fecha_creacion,u_original.fecha_creacion);						//pasar datos del original al u
					strcpy(u.avatar,u_original.avatar);								//
					strcpy(u.bio, u_original.bio);									//
					fseek(usrstmp, id_usuario, SEEK_SET);
					if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correcta
						remove("archivo_usuario.dat");								//borrar original
						rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
						printf("Edicion de preferencia exitosa\n");					//verificar despues el renombramiento, correcto ==0
						goto modificar_usuario;
					}
					break;
				
				
				case 2:		//nueva biografia
					fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
					fread(&u_original, sizeof(usuario),1, usrs);
					printf("Biografia actual:\n%s\n",u_original.bio);				// (!) que imprima la bio anterior, no se si es correcto...
					
					menu_nueva_biografia:{
						printf("ingresar biografía nueva?\n\t1.- Si\n\t2.- Cancelar\nSeleccion: ");
						gets(c);
						
						if (strcmp(c,"1")!=0 && strcmp(c,"2")!=0){
							printf(" Respuesta no valida\n\n");
							goto menu_nueva_biografia;
						}}
					if (strcmp(c,"1")==0){		// ingresar nueva
						printf("-----------------------------------------\ningrese su nueva biografia:\n");
						gets(bio_temp);
						printf("\n-----------------------------------------\n");
						
						fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
						fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
						u.id_usuario=id_usuario;										
						u.id_usuario_sigue=u_original.id_usuario_sigue;					//
						strcpy(u.fecha_creacion,u_original.fecha_creacion);				//pasar datos del original al u
						strcpy(u.avatar,u_original.avatar);								//
						u.preferencia=u_original.preferencia;									//(!) revisar si es correcto
						fseek(usrstmp, id_usuario, SEEK_SET);
						if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correct
							remove("archivo_usuario.dat");								//borrar original
							rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
							printf("Edicion de biografia exitosa\n");					//verificar despues el renombramiento, correcto ==0
							goto modificar_usuario;
						}
					}
					else if(strcmp(c,"2")==0) goto modificar_usuario;
					break;
				
				
				case 3:	goto menu_user;
						break;			
			}
			
			
		case 2:		// seguir a un usuario
			printf("\n-----------------------------------------\nUsuarios disponibles:\n");
			mostrar_usuarios(fopen("archivo_usuario.dat","rb"));
			printf("\n-----------------------------------------\n");
			ingresar_usuario_a_seguir:{
				printf("Ingrese nombre de usuario a seguir: ");
				gets(c);
				if (!strcmp(c,"0")){
					goto menu_user;
				}
				if (buscar_id_usuario(c, usrs)==-1){
					printf("no se encuentra el nombre, inténtelo nuevamente.\t(ingrese '0' para cancelar)\n");
					goto ingresar_usuario_a_seguir;
					break;
				}}
			
			fseek(usrs, id_usuario, SEEK_SET);								//ir al lugar del usuario
			fread(&u_original, sizeof(usuario),1, usrs);					// AHAHA
			u.id_usuario_sigue= buscar_id_usuario(c, usrs);
			u.id_usuario=id_usuario;										
			strcpy(u.fecha_creacion,u_original.fecha_creacion);				//pasar datos del original al u
			strcpy(u.avatar,u_original.avatar);								//
			u.preferencia=u_original.preferencia;									//(!) revisar si es correcto
			fseek(usrstmp, id_usuario, SEEK_SET);
			if (fwrite(&u, sizeof(usuario), 1, usrstmp)==1){				//escritura correct
				remove("archivo_usuario.dat");								//borrar original
				rename("archivo_usuario_temp.dat","archivo_usuario.dat"); 	//temp ahora es el original
				printf("#follow registrado\n");					//verificar despues el renombramiento, correcto ==0
				goto menu_user;
			}
			break;
		
	
		case 3: 				// ver posts
		printf("\n-----------------------------------------\nPosts actuales:\n");
		// mostrar_posts(fopen("archivo_post.dat","rb"));
		printf("\n-----------------------------------------\n");
		
		case 4: //volver
			return;			//pa que se cierre no? :s
		
	}
	}
